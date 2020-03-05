/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   slab.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 06 2019, 5:50 PM
 */

#define __MODULE__ "[SLAB]"

#include <math.h>
#include <string.h>
#include <assert.h>
#include <linked_list.h>

#include "arch/interface.h"
#include "core/memory.h"

#define SLAB_MAX_SIZE 256

#define IS_OBJECT_IN_SLAB(c, s, o) ((virt_t)(s)->objects >= (virt_t)(o) && (virt_t)(o) <= (virt_t)(s)->objects + ((c)->slab_size * (c)->slab_objects))

namespace kmem
{
    static slabcache_t root_cache;

    static void init_cache(slabcache_t* cache, size_t size)
    {
        INIT_LLIST(&cache -> list_free);
        INIT_LLIST(&cache -> list_full);
        INIT_LLIST(&cache -> list_partial);
        INIT_LLIST(&cache -> node);
        cache -> order = 0;

        cache -> slab_size = size;
        cache -> slab_objects = 0;

        // Calculate the number of objects that can fit inside one slab
        // TODO: Improve algorithm for calculation cache order

        int free = ARCH_PAGE_SIZE;
        // Minimum number of pages to fit object (object might be > 1 page)
        while(free - (int)(size + sizeof(kmem_objctl_t) + sizeof(slab_t)) < 0)
            free *= 2, cache -> order++;
        free -= sizeof(slab_t);
        while(free - (int)(size + sizeof(kmem_objctl_t)) >= 0)
            free -= size + sizeof(kmem_objctl_t), cache -> slab_objects++;

        // Note that in Linux:
        // "The offset is in units of BYTES_PER_WORD unless SLAB_HWCACHE_ALIGN is set, in which case
        // it is aligned to blocks of L1_CACHE_BYTES for alignment to the L1 hardware cache."
        // and
        // "Coloring leads to moving some of the free area of the slab from the end to the beginning."
        // Therefore we may wish to partition the remaining space into chunks the size of the L1 cache.

        cache -> colour_next = 0;
        cache -> colour_num = free / 64; // TODO: Change...
    }

    static slabcache_t* get_cache(size_t size)
    {
        // Find best fit slab
        slabcache_t* cache = &root_cache;
        slabcache_t* best = NULL;
        size_t delta = SLAB_MAX_SIZE;
        while(cache -> node.next != NULL)
        {
            if(cache -> slab_size >= size && cache -> slab_size - size <= delta)
                delta = cache -> slab_size - size, best = cache;
            cache = LIST_ENTRY(cache -> node.next, slabcache_t, node);
        }
        if(cache -> slab_size >= size && cache -> slab_size - size <= delta)
            delta = cache -> slab_size - size, best = cache;
        return best;
    }

    static slabcache_t* find_cache_kmalloc(const void* buf)
    {
        list_head_t* cur = &root_cache.node;
        slab_t* ptr = NULL;

        while(cur != NULL)
        {
            slabcache_t* cache = LIST_ENTRY(cur, slabcache_t, node);
            if(strstr(cache -> name, "ubxela") != NULL)
            {
                foreach_llist_entry(ptr, node, cache -> list_full.next)
                {
                    if(IS_OBJECT_IN_SLAB(cache, ptr, buf))
                        return cache;
                }
                foreach_llist_entry(ptr, node, cache -> list_partial.next)
                {
                    if(IS_OBJECT_IN_SLAB(cache, ptr, buf))
                        return cache;
                }
            }
            cur = cache -> node.next;
        }
        return NULL;
    }

    void init()
    {
        init_cache(&root_cache, sizeof(slabcache_t));
        new_cache(sizeof(pmm_freeblock_t));
    }

    slabcache_t* new_cache(size_t size)
    {
        slabcache_t* cache = (slabcache_t*) cache_alloc(sizeof(slabcache_t));
        if(cache == NULL) return NULL;
        init_cache(cache, size);
        list_append(&root_cache.node, &cache -> node);
        return cache;
    }

    void* cache_alloc(size_t size)
    {
        if(pmm::get_allocator().GetType() != PMM_TYPE_LIST)
            return NULL;

        slabcache_t* cache = get_cache(size);
        if(cache == NULL) return NULL;
        slab_t* slab = NULL;

        // Do we need more slabs?
        if(list_count(&cache -> list_free) == 0 && list_count(&cache -> list_partial) == 0)
        {
            /**
             *  Layout of a single slab:
             * +--------+-----------------+---------------+
             * | slab_t | kmem_objctl_t[] | color padding | objects...
             * +--------+-----------------+---------------+
             * ^ void* page
             * Note: bufctl[i] = i+1
             * bufctl [i] [i+1] [i+2] ...
             *         +---^ +---^ etc...
             * Which means that when we free, we can consult bufctl to give us the next
             * free object. See https://www.kernel.org/doc/gorman/html/understand/understand011.html
             */

            list_node_t* page = NULL;
            pmm::get_allocator().AllocateContiguous(1 << (cache -> order), (uintptr_t) &page);
            // TODO: Make more portable
            slab = (slab_t*) (pmm::get_phys(LIST_ENTRY(page, page_t, node)) + ARCH_KMALLOC_BASE);
            INIT_LLIST(&slab -> node);

            // Fill in structure
            slab -> parent = cache;
            slab -> colour_offset = cache -> colour_next * 64; // TODO: Change
            slab -> free = 0;
            slab -> inuse = 0;
            slab -> objects = (void*)((virt_t) slab + sizeof(slab_t) + cache -> slab_objects * sizeof(kmem_objctl_t) + slab -> colour_offset);
            cache -> colour_next = (cache -> colour_next + 1) % (cache -> colour_num + 1);

            // Fill in bufctl
            kmem_objctl_t* bufctl = (kmem_objctl_t*)((virt_t) slab + sizeof(slab_t));
            void* obj = slab -> objects;
            memset((void*) bufctl, 0, sizeof(kmem_objctl_t) * cache -> slab_objects);
            for (unsigned int i = 0; i < cache -> slab_objects; i++)
            {
                // TODO: CTOR
                obj = (void*)((virt_t) obj + cache -> slab_size);
                bufctl[i] = i+1;
            }
        }

        // Partial first, then free
        else if(list_count(&cache -> list_partial))
        {
            slab = LIST_ENTRY(cache -> list_partial.next, slab_t, node);
            list_remove(&slab -> node);
        }
        else if(list_count(&cache -> list_free))
        {
            slab = LIST_ENTRY(cache -> list_free.next, slab_t, node);
            list_remove(&slab -> node);
        }

        ASSERT_HARD(slab != NULL, "Fatal error! Slab cannot be empty.");

        void* ret = (void*)((virt_t) slab -> objects + slab -> free * cache -> slab_size);
        kmem_objctl_t* bufctl = (kmem_objctl_t*)((virt_t) slab + sizeof(slab_t));
        slab -> free = bufctl[slab -> free];
        slab -> inuse++;

        // Adjust state
        if(slab -> inuse == cache -> slab_objects)
            list_append(&cache -> list_full, &slab -> node);
        else if(slab -> inuse == 0)
            list_append(&cache -> list_free, &slab -> node);
        else
            list_append(&cache -> list_partial, &slab -> node);

        return ret;
    }

    void cache_free(slabcache_t* cache, const void* obj)
    {
        if(cache == NULL) return; // TODO: Error
        slab_t* slab = NULL;
        bool isObjInFull = true;

        // Search full list
        {
            slab_t* ptr = NULL;
            foreach_llist_entry(ptr, node, cache -> list_full.next)
            {
                if(IS_OBJECT_IN_SLAB(cache, ptr, obj))
                {
                    slab = ptr;
                    break;
                }
            }
        }

        // Search partial list
        if(slab == NULL)
        {
            slab_t* ptr = NULL;
            isObjInFull = false;
            foreach_llist_entry(ptr, node, cache -> list_partial.next)
            {
                if(IS_OBJECT_IN_SLAB(cache, ptr, obj))
                {
                    slab = ptr;
                    break;
                }
            }
        }

        if(slab == NULL) return; // TODO: Error

        // Adjust slabs
        unsigned int id = ((unsigned int) obj - (unsigned int) slab -> objects) / cache -> slab_size;
        kmem_objctl_t* bufctl = (kmem_objctl_t*)((virt_t) slab + sizeof(slab_t));
        bufctl[id] = slab -> free;
        slab -> free = id;
        slab -> inuse--;

        // We should guarantee at least 2 objects per slab
        if(isObjInFull)
        {
            // Full slabs must become partial
            list_remove(&slab -> node);
            list_append(&cache -> list_partial, &slab -> node);
        }
        else
        {
            // Partial slabs can only be partial or free
            if(slab -> inuse == 0)
            {
                list_remove(&slab -> node);
                list_append(&cache -> list_free, &slab -> node);
            }
        }
    }

    void *kmalloc(size_t size)
    {
	    size_t j = (size_t) next_pow2l((uint64_t) size);
        slabcache_t* best = get_cache(j);
        if(best == NULL || best -> slab_size != j)
        {
            best = new_cache(j);
            strcpy(best -> name, "ubxela\0");
        }
	    void* alloc = cache_alloc(j);
	    return alloc;
    }

    void kfree(void* obj)
    {
        slabcache_t* cache = find_cache_kmalloc(obj);
        if(cache == NULL) return;
        cache_free(cache, obj);
        // TODO: Shrink cache if needed
    }
}