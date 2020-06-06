/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   memory.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on July 16 2019, 10:35 AM
 */

#pragma once

#include "common.h"
#include "system.h"
#include <linked_list.h>

#include "core/pmm.h"
#include "core/vmm.h"

__BEGIN_CDECLS

typedef struct kmem_slab        slab_t;
typedef struct kmem_slabcache   slabcache_t;

typedef struct page             page_t;
typedef struct pmm_arena        arena_t;

typedef struct vm_aspace aspace_t;
typedef struct vm_region region_t;

typedef unsigned int kmem_objctl_t;

struct page
{
    list_node_t node;
    uint32_t flags;
    uint32_t ref;
};

struct pmm_arena
{
    list_node_t node;       //!< Node to link all arenas together
    uint16_t flags;         //!< Arena flags
    phys_t base;            //!< Base physical address
    size_t size;            //!< Size of arena in pages
    size_t free;            //!< Number of pages free
    int priority;           //!< Lower priority = more likely to be allocated from
    page_t* pages;          //!< Ordered array of all physical pages
    list_node_t free_list;  //!< Unordered linked list of all free pages
};

struct vm_aspace
{
    virt_t base;
    size_t size;
    list_head_t free_list;
};

struct vm_region
{
    list_node_t node;
    uint32_t flags;
    size_t size;
    unsigned int ref;
    list_node_t pages;
    list_node_t mappings;
    struct region* parent;
};

struct kmem_slab
{
    list_node_t node;
    void* objects;
    kmem_objctl_t free;
    unsigned int colour_offset;
    unsigned int inuse;
    slabcache_t* parent;
};

struct kmem_slabcache
{
    list_head_t list_free;
    list_head_t list_partial;
    list_head_t list_full;

    char name[20];

    unsigned int order;         //!< Order of cache
    unsigned int slab_size;     //!< Size of one object
    unsigned int slab_objects;  //!< Number of objects per slab

    unsigned int colour_next;   //!< Next assigned colour (for next slab)
    unsigned int colour_num;    //!< Number of colours. Note that # colours = size of CPU cache / 2^order / n-way associativity.

    list_node_t node;           //!< Links all caches together
};

__END_CDECLS

namespace kmem
{
    void init();
    slabcache_t* new_cache(size_t);
    void* cache_alloc(size_t);
    void cache_free(slabcache_t*, const void*);
    void *kmalloc(size_t);
    void kfree(void*);
}

namespace pmm
{
    class PhysicalAllocator
    {
    public:
        virtual void add_arena(arena_t* arena, bitmap_t* bt = NULL);
        virtual phys_t page_to_physical(uintptr_t page);        
        virtual size_t allocate(size_t, uintptr_t pages);
        virtual size_t allocate_single(uintptr_t pages);
        virtual size_t allocate_contiguous(size_t, uintptr_t pages);
        virtual size_t free(uintptr_t pages);
        virtual int get_type();
        virtual size_t get_size();
    };

    void set_allocator(PhysicalAllocator*);
    PhysicalAllocator& get_allocator();
    void add_arena(arena_t* arena, bitmap_t* bt);
    size_t alloc_pages(size_t cnt, list_head_t** pages);
    size_t alloc_pages_contig(size_t cnt, list_head_t** pages);
    size_t free(list_head_t** pages);
    phys_t get_phys(page_t* page);
}
