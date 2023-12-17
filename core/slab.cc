#include "assert.h"
#include "core/mutex.h"
#include "core/slab.h"
#include <ebl/string.h>
#include <ebl/guard.h>
#include <ebl/new.h>

using namespace kmem;
using namespace kmem_private;

// The slabcache descriptor to allocate slabcaches (For bootstrapping)
static slabcache slabcache_cache_node {
    "slabcache cache",
    align_up(sizeof(slabcache), default_order),
    default_order
};
static slabcache* const slabcache_cache = &slabcache_cache_node;
core::Mutex cache_list_lock{};
slabcache_list_head cache_list GUARDED_BY(cache_list_lock) {};

static slabcache*   find_best_cache(unsigned int size);
static slab*        find_slab_from_obj(void* obj);
static slab*        slab_create(slabcache* cache);
static slabcache*   cache_create(const char* name, unsigned int size, uint16_t order);
static void*        cache_alloc(slabcache* cache);
static void         cache_free(slab* node, objctl* obj);
static void         cache_destroy(slabcache* cache);
static void         cache_reap(slabcache* cache);

//===----------------------------------------------------------------------===//

slabcache* find_best_cache(unsigned int size) REQUIRES(cache_list_lock) {
    for(auto cache : cache_list) {
        // FIXME: Invent a better way to find best-fit cache (instead of exact match)
        if(cache->slab_objsize == size)
            return cache;
    }
    return nullptr;
}

slab* find_slab_from_obj(void* obj) {
    return (slab*) arch::page_align_down((vaddr_t) obj);
}

/**
 * @brief Set up a new page as a slab and requires cache->lock to be held
 * 
 * @param cache The parent slabcache
 * @return The slab's list node struct
 */
slab* slab_create(slabcache* cache) REQUIRES(cache->lock) {

    // Page Layout:
    // [struct slab_node] [color padding] [objctl] ... [objctl] [end padding]
    // ^                ^
    // This struct size is aligned to 2^order bytes

    void* page = arch::grow_heap(1);
    if(!page) return nullptr;

    // Calculate the color padding
    const auto slab_node_sz = align_up(sizeof(slab), cache->order);
    const auto free_space = arch::page_size - slab_node_sz;
    const auto padding = free_space - cache->slab_nobjects*cache->slab_objsize;
    const auto max_color = padding / (1 << cache->order);
    const auto color = (cache->slab_last_color + 1) % max_color;
    const auto color_padding = color * (1 << cache->order);

    // Update the color tracking
    cache->slab_last_color = color;

    // Initialize the objctl structs
    vaddr_t ptr = (vaddr_t) page + slab_node_sz + color_padding;
    for(unsigned int i = 0; i < cache->slab_nobjects; i++) {
        objctl* obj = (objctl*) ptr;
        ptr += cache->slab_objsize;
        if(cache->slab_nobjects == i+1)
            obj->next_free_obj = nullptr;
        else
            obj->next_free_obj = (objctl*) ptr;
    }

    // Initialize the slab struct
    auto* node = static_cast<slab*>(page);
    new (&node) slab { 
        cache, (objctl*) ptr, (void*) ptr
    };

    return node;
}

slabcache* cache_create(const char* name, unsigned int size, uint16_t order) {
    auto* node = (slabcache*) cache_alloc(slabcache_cache);
    if(!node) return nullptr;
    new (&node) slabcache { size, order };
    // Copy over the name
    ebl::memcpy(&node->name, name, cache_name_max);
    node->name[cache_name_max-1] = '\0';
    // Add the new cache to the cache list
    {
        ebl::Guard guard_{&cache_list_lock};
        cache_list.push_back_unsafe(node);
    }
    return node;
}

void* cache_alloc(slabcache* cache) {
    ebl::Guard guard_cache_{&cache->lock};
    slab* node = nullptr;
    if(cache->partiallist.empty()) {
        // No partial slabs, grab one from the free list
        node = cache->freelist.pop_front_unsafe();
        cache->partiallist.push_back_unsafe(node);
    } else if(cache->freelist.empty()) {
        // No free slabs, create a new one
        node = slab_create(cache);
        if(!node) return nullptr;
        cache->partiallist.push_back_unsafe(node);
    } else {
        // Grab the first partial slab
        node = cache->partiallist.pop_front_unsafe();
    }
    
    // Grab the first free object from the slab
    ebl::Guard guard_slab_{&node->lock};
    if(node->inuse + 1 == cache->slab_nobjects)
        cache->fulllist.push_back_unsafe(node);
    node->inuse++;
    node->last_alloc_time = 0;
    node->objctl = node->objctl->next_free_obj;

    return (void*) node->objctl;
}

void cache_free(slab* slab, objctl* obj) {
    ebl::Guard guard1_{&slab->parent->lock};
    ebl::Guard guard2_{&slab->lock};
    if(slab->inuse == slab->parent->slab_nobjects) {
        // Slab was full, move to the partial list
        slab->parent->fulllist.remove_unsafe(slab);
        slab->parent->partiallist.push_back_unsafe(slab);
    } else if(slab->inuse == 1) {
        // Slab will be empty, move to the free list
        slab->parent->partiallist.remove_unsafe(slab);
        slab->parent->freelist.push_back_unsafe(slab);
    } else {
        // Slab is still partially full, do nothing
    }
    slab->inuse--;
    obj->next_free_obj = slab->objctl;
    slab->objctl = obj;
}

void cache_destroy(slabcache *cache) {
    (void) cache;
    (void) cache_reap;
}

void cache_reap(slabcache *cache) {
    (void) cache;
    (void) cache_destroy;
}

//===----------------------------------------------------------------------===//

void kmem::bootstrap() NO_THREAD_SAFETY_ANALYSIS {
    // Init cache for struct slabcache
    cache_list.push_back_unsafe(&slabcache_cache_node);
}

void* kmem::alloc(unsigned int size) {
    size = align_up(size, default_order);
    struct slabcache* cache = nullptr;
    {
        ebl::Guard guard_{&cache_list_lock};
        cache = find_best_cache(size);
    }
    if(cache == nullptr) {
        cache = cache_create("", size, default_order);
    }
    return cache_alloc(cache);
}

void kmem::free(void* obj) {
    assert(obj != nullptr, "Tried to free nullptr");
    slab* node = find_slab_from_obj(obj);
    assert(
        arch::is_heap_address((vaddr_t) node),
        "Object is not in the heap");
    slabcache* cache = node->parent;
    assert(
        align_up((vaddr_t) obj, cache->order) == (vaddr_t) obj,
        "Object is not within the expected alignment"
    );
    assert(
        (vaddr_t) obj >= (vaddr_t) node->objects,
        "Object is not within the expected slab range"
    );
    cache_free(node, (objctl*) obj);
}
