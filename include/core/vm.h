/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   vm.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 06 2019, 12:19 AM
 */
/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   vm.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 15 2018, 7:23 PM
 */

#pragma once

#include "common.h"
#include "system.h"
#include <linked_list.h>

#define CX_ARENA_PRESENT            (1 << 0)

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
#define PMM_TYPE_OTHER 2

__BEGIN_CDECLS

typedef struct page
{
    list_node_t node;
    uint32_t flags;
    uint32_t ref;
} page_t;

typedef struct pmm_arena
{
    list_node_t node;
    uint32_t flags;
    phys_t base;
    size_t size;
    size_t free;
    int priority;
    page_t* pages;
    list_node_t free_list;
} pmm_arena_t;

typedef struct region
{
    list_node_t node;
    uint32_t flags;
    size_t size;
    unsigned int ref;
    list_node_t pages;
    list_node_t mappings;
    struct region* parent;
} vmm_region_t;

typedef virt_t allocpage_t; // TODO: Review all allocpage_t uses

void pmm_add_arena(pmm_arena_t*, bitmap_t*);
size_t pmm_alloc_pages(size_t, allocpage_t pages);
size_t pmm_free(allocpage_t pages);

__END_CDECLS

#ifdef __cplusplus

namespace pmm
{
    class PhysicalAllocator
    {
    public:
        virtual void AddArena(pmm_arena_t* arena, bitmap_t* bt = NULL);
        virtual size_t Allocate(size_t, allocpage_t pages);
        virtual size_t AllocateSingle(allocpage_t pages);
        virtual size_t Free(allocpage_t pages);
        virtual int GetType();
    };
    void SetPhysicalAllocator(PhysicalAllocator*);
    PhysicalAllocator& GetPhysicalAllocator();
}

#endif
