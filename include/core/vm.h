/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   vm.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 06 2019, 12:19 AM
 */

#pragma once

#include "common.h"
#include "system.h"
#include <linked_list.h>

#define CX_ARENA_PRESENT            (1 << 0)
#define CX_ARENA_KERNEL             (1 << 1)

#define CX_ARENA_TYPE_NORMAL        0
#define CX_ARENA_TYPE_DMA32         1

#define CX_PAGE_OCCUPIED            (1 << 0)

#define CX_PERM_READ    			(1 << 0)
#define CX_PERM_WRITE   			(1 << 1)
#define CX_PERM_EXECUTE 			(1 << 2)
#define CX_PERM_MAP					(1 << 3)
#define CX_PERM_CLONE				(1 << 4)
#define CX_PERM_REASSIGN			(1 << 5)
#define CX_PERM_GET_PROPERY			(1 << 6)
#define CX_PERM_SET_PROPERY			(1 << 7)
#define CX_PERM_LOCK				(1 << 8)
#define CX_PERM_SPECIFIC			(1 << 9)
#define CX_PERM_SPECIFIC_OVERWRITE	(1 << 10)

// If we wish to use listed allocators and modify page lists
#define PMM_TYPE_LIST  1
#define PMM_TYPE_BOOT  2
#define PMM_TYPE_OTHER 3

__BEGIN_CDECLS

typedef struct page             page_t;
typedef struct pmm_arena        pmm_arena_t;
typedef struct region           vmm_region_t;
typedef struct pmm_freeblock    pmm_freeblock_t;

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

struct pmm_freeblock
{
    list_node_t node;
    phys_t base;
    size_t size;
};

struct region
{
    list_node_t node;
    uint32_t flags;
    size_t size;
    unsigned int ref;
    list_node_t pages;
    list_node_t mappings;
    struct region* parent;
};

/*void pmm_add_arena(pmm_arena_t*, bitmap_t*);
size_t pmm_alloc_pages(size_t, list_head_t* pages);
size_t pmm_free(list_head_t* pages);
phys_t pmm_get_phys(page_t* page);*/

__END_CDECLS

#ifdef __cplusplus

namespace pmm
{
    class PhysicalAllocator
    {
    public:
        virtual void AddArena(pmm_arena_t* arena, bitmap_t* bt = NULL);
        virtual phys_t PageToPhysical(uintptr_t page);        
        virtual size_t Allocate(size_t, uintptr_t pages);
        virtual size_t AllocateSingle(uintptr_t pages);
        virtual size_t AllocateContiguous(size_t, uintptr_t pages);
        virtual size_t Free(uintptr_t pages);
        virtual int GetType();
        virtual size_t GetSize();
    };

    void set_allocator(PhysicalAllocator*);
    PhysicalAllocator& get_allocator();
    void add_arena(pmm_arena_t* arena, bitmap_t* bt);
    size_t alloc_pages(size_t cnt, list_head_t** pages);
    size_t alloc_pages_contig(size_t cnt, list_head_t** pages);
    size_t free(list_head_t** pages);
    phys_t get_phys(page_t* page);
}

#endif
