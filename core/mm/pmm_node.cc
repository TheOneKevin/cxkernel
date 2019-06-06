/*
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   pmm_node.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 05 2019, 11:30 PM
 */

#define __MODULE__ "PMM"

#include <stdlib.h>
#include <linked_list.h>
#include <panic.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "arch/x86/arch_utils.h"
#include "system.h"
#include "core/vm.h"
#include "arch/interface.h"
#include "core/pmm_node.h"

#define PAGE_IS_IN_ARENA(p, a) \
        (((virt_t)(p) >= (virt_t)(a)->pages) && (virt_t)(p) < (virt_t)(a)->pages+(a)->size/ARCH_PAGE_SIZE*sizeof(page_t))

namespace pmm
{
    static PmmNode __internal_PmmNodeAllocator;

    void PmmNode::AddArena(pmm_arena_t* arena, bitmap_t* bt)
    {
        INIT_LLIST(&arena->node);
        INIT_LLIST(&arena->free_list);
        arena->free = 0;
        pmm_arena_t* an = NULL;
        if(arena_list.next != NULL)
        {
            foreach_llist_entry(an, node, arena_list.next)
            {
                if(an->priority > arena->priority)
                    break;
            }
        }
        if(an == NULL)
            list_append(list_end(&arena_list), &arena->node);
        else
            list_insert(&an->node, &arena->node);
        // Prepare page array
        memset(arena->pages, 0, arena->size*sizeof(page_t));
        list_node_t* prev = &arena->free_list;
        for(size_t i = 0; i < arena->size; i++)
        {
            INIT_LLIST(&arena->pages[i].node);
            list_append(prev, &arena->pages[i].node);
            prev = &arena->pages[i].node;
            arena->free++;
        }
        // Go through bitmap
        if(bt != NULL)
        {
            for(size_t i = 0; i < bt -> bit_count; i++)
            {
                if(bitmap_tstbit(bt -> bitmap, i))
                    list_remove(&arena->pages[i].node);
            }
        }
        OS_PRN("Loaded 0x%X page_t entries\n", list_count(arena->free_list.next));
    }

    size_t PmmNode::Allocate(size_t cnt, allocpage_t p)
    {
        list_node_t* pages = (list_node_t*) p;
        size_t ret = 0;
        if(cnt == 0) return 0;
        pmm_arena_t* arena;
        foreach_llist_entry(arena, node, arena_list.next)
        {
            while(ret < cnt)
            {
                page_t* pg = LIST_ENTRY(
                    list_remove(list_end(&arena->free_list)),
                    page_t, node
                );
                if(!pg) return ret;
                pg->flags |= CX_PAGE_OCCUPIED;
                arena->free--;
                list_append(list_end(pages), &pg->node);
                ret++;
            }
        }
        return ret;
    }

    size_t PmmNode::Free(allocpage_t p)
    {
        list_node_t* pages = (list_node_t*) p;
        size_t ret = 0;
        while(pages->next != NULL)
        {
            page_t* pg = LIST_ENTRY(list_remove(list_end(pages)), TYPEOF(page_t), node);
            pmm_arena_t* arena;
            foreach_llist_entry(arena, node, arena_list.next)
            {
                if(PAGE_IS_IN_ARENA(pg, arena))
                {
                    pg->flags &= ~CX_PAGE_OCCUPIED;
                    list_append(&arena->free_list, &pg->node);
                    arena->free++;
                    ret++;
                    break;
                }
            }
        }
        return ret;
    }

    PhysicalAllocator* GetPmmNodeAllocator()
    {
        return static_cast<PhysicalAllocator*>(&__internal_PmmNodeAllocator);
    }
} // namespace
