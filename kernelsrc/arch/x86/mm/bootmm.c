/*
 * File:   bootmm.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-31T16:30:36-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T20:49:23-05:00
*/

#include "panic.h"
#include "bitmap.h"
#include "assert.h"
#include "lib/printk.h"
#include "arch/x86/bootmm.h"
#include "arch/x86/arch_common.h"

static unsigned int _ptr;

// If we run out of room, we search the bitmap again to find
// a single free page frame.
static void updatePtr(void)
{
    bool flag = false;
    for(size_t i = g_pmm_buddy_map -> length; i > 0; i--)
    {
        if(g_pmm_buddy_map -> bitmap[i - 1] != 0xFFFFFFFF)
        {
            for(_ptr = 32; _ptr > 0; _ptr--)
                if(!bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr - 1 + (i - 1) * sizeof(unsigned int) * 8))
                    break;
            _ptr += (i - 1) * sizeof(unsigned int) * 8;
            flag = true;
            break;
        }
    }

    if(flag)
        return;
    else
        PANIC("No memory left on init!");
}

void* bootmm_alloc_page(void)
{
    unsigned int foo = 0;
    if(bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr)) // If it is not a free page, find one
        updatePtr();
    foo = _ptr;
    if(_ptr > 0) _ptr--;
    return (void *)(foo * ARCH_PAGE_SIZE);
}

void bootmm_free_page(unsigned int address)
{
    bitmap_clrbit(g_pmm_buddy_map -> bitmap, address / ARCH_PAGE_SIZE);
}

void bootmm_init_memory_regions(void)
{
    // Mark all free areas in bitmap
    // Each free area starts aligned upwards, and ends aligned downwards (some space may be wasted)
    // The worst case scenario would be if the free region was [0x1001 -> 0x3FFF], then we would end up with a region [0x2000 -> 0x3000]
    // The most this can waste is 1 page of memory
    ARCH_FOREACH_MMAP(mmap)
    {
        if(mmap -> type == MULTIBOOT_MEMORY_AVAILABLE)
            for(unsigned int i = ARCH_PAGE_ALIGN(mmap -> addr) / ARCH_PAGE_SIZE; i < ARCH_PAGE_ALIGN_DOWN(mmap -> addr + mmap -> len) / ARCH_PAGE_SIZE - 1; i++)
                bitmap_setbit(g_pmm_buddy_map -> bitmap, i);
    }

    // Allocate from the first highest free address first
    updatePtr();

    kprintf("%X %X\n", _ptr, (unsigned int) g_pmm_buddy_map -> bitmap);
}
