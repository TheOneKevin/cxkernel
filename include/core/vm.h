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

__BEGIN_CDECLS

typedef struct page
{
    list_node_t node;
    uint32_t index;
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

typedef struct region vmm_region_t;

void pmm_add_arena(pmm_arena_t*);
size_t pmm_alloc(size_t cnt, list_node_t* pages);
size_t pmm_free(list_node_t* pages);

__END_CDECLS
