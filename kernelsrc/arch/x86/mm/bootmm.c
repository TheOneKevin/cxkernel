/*
 * File:   bootmm.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-31T16:30:36-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-27T22:36:05-04:00
*/

#define __MODULE__ "BOOTMM"

#include "panic.h"
#include "bitmap.h"
#include "assert.h"
#include "lib/printk.h"
#include "arch/x86/cpu.h"
#include "arch/x86/bootmm.h"
#include "arch/x86/arch_common.h"

static phys_t _ptr;
static phys_t _ctr;

// If we run out of room, we search the bitmap again to find
// a single free page frame.
static bool updatePtr(void)
{
    for(size_t i = g_pmm_buddy_map -> length; i > 0; i--)
    {
        if(g_pmm_buddy_map -> bitmap[i - 1] != 0xFFFFFFFF)
        {
            for(_ptr = 32; _ptr > 0; _ptr--)
                if(!bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr - 1 + (i - 1) * sizeof(unsigned int) * 8))
                    break;
            _ptr += (i - 1) * sizeof(unsigned int) * 8;
            return true;
        }
    }

    PANIC("No memory left on init!\n");
    return false;
}

phys_t bootmm_alloc_page(void)
{
    phys_t foo = 0;
    if(bitmap_tstbit(g_pmm_buddy_map -> bitmap, _ptr)) // If it is not a free page, find one
        updatePtr();
    foo = _ptr;
    if(_ptr > 0) _ptr--;
    bitmap_setbit(g_pmm_buddy_map -> bitmap, foo);
    //OS_PRN("Allocated 1 page at 0x%X\n", foo * ARCH_PAGE_SIZE);
    return foo * ARCH_PAGE_SIZE;
}

void bootmm_free_page(phys_t address)
{
    bitmap_clrbit(g_pmm_buddy_map -> bitmap, ARCH_PAGE_ALIGN_DOWN(address) / ARCH_PAGE_SIZE);
}

void bootmm_init_memory_regions(void)
{
    // Mark all free areas in bitmap
    // Each free area starts aligned upwards, and ends aligned downwards (some space may be wasted)
    // The worst case scenario would be if the free region was [0x1001 -> 0x3FFF], then we would end up with a region [0x2000 -> 0x3000]
    // The most this can waste is 1 page of memory
    ARCH_FOREACH_MMAP(mmap)
    {
        if(!cpu_check_feat(CPU_FEAT_EDX_PAE, CPU_FEATSET_REGS_EDX) && mmap -> addr >= 0xFFFFFFFFUL)
            break;
        if(mmap -> type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            //kprintf("[%lX %lX]\n", ARCH_PAGE_ALIGN(mmap -> addr) / ARCH_PAGE_SIZE, ARCH_PAGE_ALIGN(mmap -> addr + mmap -> len) / ARCH_PAGE_SIZE - 1);
            for(uint32_t i = ARCH_PAGE_ALIGN(mmap -> addr) / ARCH_PAGE_SIZE; i < ARCH_PAGE_ALIGN(mmap -> addr + mmap -> len) / ARCH_PAGE_SIZE - 1; i++)
                bitmap_clrbit(g_pmm_buddy_map -> bitmap, i);
        }
    }

    // Mark the kernel and modules as not free
    // The kernel's end is really the end of the bitmap
    for(int i = 0x100; i <= ARCH_PAGE_ALIGN((uint32_t) g_pmm_buddy_map -> bitmap + g_pmm_buddy_map -> length * sizeof(unsigned int) - ARCH_VIRT_BASE) / ARCH_PAGE_SIZE; i++)
        bitmap_setbit(g_pmm_buddy_map -> bitmap, i);

    // Allocate from the first highest free address first
    updatePtr();
    //kprintf("%X %X\n", g_mod_end, _ptr * ARCH_PAGE_SIZE);
}
