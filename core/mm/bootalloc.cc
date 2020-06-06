/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   bootalloc.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on June 06 2019, 11:21 AM
 */

#define __MODULE__ "BTALC"

#include <stdio.h>
#include <panic.h>
#include <string.h>
#include <bitmap.h>
#include "arch/interface.h"
#include "core/bootalloc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define ALLOCATE_FIRST

namespace pmm
{
    static BootAllocator __internal_BootAllocator;

    PhysicalAllocator* get_bootallocator()
    {
        return static_cast<PhysicalAllocator*>(&__internal_BootAllocator);
    }

    phys_t BootAllocator::page_to_physical(uintptr_t page)
    {
        return ((bafree_t*) page) -> addr;
    }

    // If we run out of room, we search the bitmap again to find
    // a single free page frame.
    bool BootAllocator::update_all(void)
    {
    #ifdef ALLOCATE_FIRST
        _ptr = (size_t) bitmap_firstz(alloc_map);
        if(bitmap_firstz(alloc_map) != -1) return true;
    #else
        _ptr = (size_t) bitmap_lastz(alloc_map);
        if(bitmap_lastz(alloc_map) != -1) return true;
    #endif
        PANIC("No free memory on boot!");
        return false;
    }

    size_t BootAllocator::allocate_single(uintptr_t p)
    {
    #ifdef ALLOCATE_FIRST
        if(bitmap_tstbit(alloc_map -> bitmap, _ptr) || _ptr >= alloc_map -> bit_count)
            BootAllocator::update_all();
    #else
        if(bitmap_tstbit(alloc_map.bitmap, _ptr) || _ptr <= 1)
            BootAllocator::update_all(); // If it is not a free page, find one
    #endif
        bitmap_setbit(alloc_map -> bitmap, _ptr);
    #ifdef ALLOCATE_FIRST
        ((bafree_t*) p) -> addr = (_ptr++) * ARCH_PAGE_SIZE;
    #else
        ((bafree_t*) p) -> addr = (_ptr--) * ARCH_PAGE_SIZE; // Return the address of the allocated page
    #endif
        return 1;
    }

    void BootAllocator::add_arena(arena_t* arena, bitmap_t* bt)
    {
        if(bt != NULL)
        {
            OS_PRN("Loaded 0x%X bitmapped pages\n", bt->bit_count);
            alloc_map = bt;
            BootAllocator::update_all();
        }
    }

    size_t BootAllocator::free(uintptr_t st)
    {
        int pages = ((bafree_t*) st) -> pages;
        phys_t address = ((bafree_t*) st) -> addr;
        pages--;
        for(uint64_t a = address; a <= address + (uint64_t)(pages * ARCH_PAGE_SIZE); a++)
            bitmap_clrbit(alloc_map -> bitmap, ARCH_PAGE_ALIGN_DOWN(a) / ARCH_PAGE_SIZE);
        return 1;
    }

    int BootAllocator::get_type()
    {
        return PMM_TYPE_BOOT;
    }

    size_t BootAllocator::get_size()
    {
        return sizeof(bafree_t);
    }
}
