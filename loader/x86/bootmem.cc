/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   bootmem.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 22 2018, 9:58 PM
 */

#define __MODULE__ "BOOT"
#include <stdio.h>
#include <bitmap.h>
#include <string.h>
#include <math.h>
#include <panic.h>

#include "arch/x86/paging.h"
#include "core/vm.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/cpu.h"
#include "include/global.h"

// Comment out by default.
#define ALLOCATE_FIRST

extern "C" {

// External symbols
extern uint32_t _lodr_start;
extern uint32_t _lodr_end;

// Static global variables
static bitmap_t alloc_map;
static bitmap_t resrv_map;
static uint64_t avail_mem;
static uint32_t num_pages;
static phys_t _ptr;
static uint32_t __bt[8192];

using namespace loader;

void init_bootmm32()
{
    ARCH_FOREACH_MMAP(mmap, &g_mbt, 0)
    {
        if(mmap->addr == 0x100000 && !(mmap->type == MULTIBOOT_MEMORY_AVAILABLE && mmap->len > 0x4000000))
        PANIC("Not enough memory.\nNeeds at least 64 MB of continuous physical ram at 0x100000!\n");
        fprintf(
                STREAM_LOG, "  Entry address: 0x%016lX Entry length: 0x%016lX (0x%02X)\n",
                (uint64_t) mmap->addr, (uint64_t) mmap->len, mmap->type
        );
    }
    // Memory Topology
    ARCH_FOREACH_MMAP(mmap, &g_mbt, 0) MAX_MEM = MAX(MAX_MEM, mmap->addr + mmap->len);
    // Initialize and zero out the bitmap
    num_pages = (uint32_t)(ARCH_PAGE_ALIGN(MAX_MEM) / ARCH_PAGE_SIZE);
    alloc_map.length = bitmap_getlength(num_pages);
    alloc_map.bitmap = (unsigned int*)ARCH_PAGE_ALIGN(MODS_END);
    resrv_map.length = 8192;
    resrv_map.bitmap = (unsigned int*)__bt;
    memset(alloc_map.bitmap, -1, alloc_map.length * sizeof(unsigned int)); // # of bits in bitmap / 8
    // Initialize the physical memory manager
    // Mark all free areas in bitmap
    // Each free area starts aligned upwards, and ends aligned downwards (some space may be wasted)
    // The worst case scenario would be if the free region was [0x1001 -> 0x3FFF], then we would end up with a region [0x2000 -> 0x3000]
    // The most this can waste is 1 page of memory
    ARCH_FOREACH_MMAP(mmap, &g_mbt, 0)
    {
        if(!x86_feature_test(x86_FEATURE_PAE) && mmap->addr >= 0xFFFFFFFFUL)
            break;
        if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            avail_mem += mmap->len;
            uint64_t start_addr = ARCH_PAGE_ALIGN(mmap->addr) / ARCH_PAGE_SIZE;
            uint64_t end_addr = ARCH_PAGE_ALIGN_DOWN(mmap->addr + mmap->len) / ARCH_PAGE_SIZE;
            for(uint64_t i = start_addr; i < end_addr; i++)
                bitmap_clrbit(alloc_map.bitmap, i);
        }
    }

    // Mark the kernel and modules as not free
    // The kernel's end is really the end of the bitmap
    for(auto i = 0; i <= ARCH_PAGE_ALIGN((uint32_t) alloc_map.bitmap + alloc_map.length * sizeof(unsigned int)) / ARCH_PAGE_SIZE; i++)
        bitmap_setbit(alloc_map.bitmap, i), bitmap_setbit(resrv_map.bitmap, i);

    pmm_update_all();
    OS_PRN("0x%lX bytes usable, 0x%X entries, 0x%X pages indexed\n", avail_mem, alloc_map.length, num_pages);
}

void init_pps32()
{
    // Init per-page structure(round up)
    uint32_t num_struct = (num_pages*sizeof(page_t)+1)/ARCH_PAGE_SIZE;
    OS_PRN("Preparing 0x%X page structures...\n", num_struct);
    for(uint32_t i = 0; i <= num_struct; i++)
        loader::get_mmu().map(ARCH_PPS_BASE + i*ARCH_PAGE_SIZE, pmm_alloc_page(false), PTE_RW | PTE_PR);
}

// If we run out of room, we search the bitmap again to find
// a single free page frame.
bool pmm_update_all(void)
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

phys_t pmm_alloc_page(bool clear)
{
#ifdef ALLOCATE_FIRST
    if(bitmap_tstbit(alloc_map.bitmap, _ptr) || _ptr >= num_pages)
        pmm_update_all();
#else
    if(bitmap_tstbit(alloc_map.bitmap, _ptr) || _ptr <= 1)
        pmm_update_all(); // If it is not a free page, find one
#endif
    bitmap_setbit(alloc_map.bitmap, _ptr);
    OS_DBG("Alloc %s page at 0x%lX\n", clear ? "cleared" : "new", (uint64_t)(_ptr * ARCH_PAGE_SIZE));
    if(clear) memset((void*)(_ptr * ARCH_PAGE_SIZE), 0, ARCH_PAGE_SIZE);
#ifdef ALLOCATE_FIRST
    return (_ptr++) * ARCH_PAGE_SIZE;
#else
    return (_ptr--) * ARCH_PAGE_SIZE; // Return the address of the allocated page
#endif
}

void pmm_free_page(phys_t address)
{
    bitmap_clrbit(alloc_map.bitmap, ARCH_PAGE_ALIGN_DOWN(address) / ARCH_PAGE_SIZE);
    OS_DBG("Freeing page at 0x%lX\n", ARCH_PAGE_ALIGN_DOWN(address) / ARCH_PAGE_SIZE);
}

void pmm_free_page_multi(phys_t address, int pages)
{
    pages--;
    for(uint64_t a = address; a <= address + (uint64_t)(pages * ARCH_PAGE_SIZE); a++)
    {
        bitmap_clrbit(alloc_map.bitmap, ARCH_PAGE_ALIGN_DOWN(a) / ARCH_PAGE_SIZE);
        OS_DBG("Freeing page at 0x%lX\n", ARCH_PAGE_ALIGN_DOWN(a) / ARCH_PAGE_SIZE);
    }
}

} // End extern "C"
