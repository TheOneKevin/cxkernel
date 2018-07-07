/*
 * File:   meminit.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * @date Created on 04-Aug-2017 04:37:25 PM
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-05-23T13:53:26-04:00
*/

#define __MODULE__ "_PMAN"

#include "bitmap.h"
#include "arch/x86/multiboot.h"
#include "panic.h"
#include "lib/printk.h"
#include "lib/string.h"
#include "mm/page_alloc.h"

#include "arch/arch_interface.h"

#include "arch/x86/cpu.h"
#include "arch/x86/paging.h"
#include "arch/x86/arch_common.h"
#include "arch/x86/global.h"

extern uint32_t _kernel_dir;
extern uint32_t _kernel_table3;

static bitmap_t pmm_buddy_map;

void arch_pmeminit(void)
{
    // Get the largest physical memory address
    uint64_t max_mem = 0;
    ARCH_FOREACH_MMAP(mmap)
    {
        if(max_mem < (mmap -> addr + mmap -> len))
            max_mem = mmap -> addr + mmap -> len;
    }
    // Find the length in terms of pages
    max_mem /= ARCH_PAGE_SIZE;

    // Initialize and zero out the buddy bitmap
    pmm_buddy_map.length = (max_mem / (sizeof(unsigned int) * 8)) + 1; // Plus one in case integer div truncates number
    pmm_buddy_map.bitmap = (unsigned int *) (uint32_t) ARCH_PAGE_ALIGN(g_memory_map.MOD_END);
    memset(pmm_buddy_map.bitmap, 0xFFFFFFFF, pmm_buddy_map.length * sizeof(unsigned int)); // # of bits in bitmap / 8
    OS_PRN("%X entries, %X pages indexed\n", pmm_buddy_map.length, max_mem);
    g_pmm_buddy_map = &pmm_buddy_map;

    // Initialize the physical memory manager
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
    for(unsigned int i = 0x100; i <= ARCH_PAGE_ALIGN((uint32_t) g_pmm_buddy_map -> bitmap + g_pmm_buddy_map -> length * sizeof(unsigned int) - ARCH_VIRT_BASE) / ARCH_PAGE_SIZE; i++)
        bitmap_setbit(g_pmm_buddy_map -> bitmap, i);

    // Allocate from the first highest free address first
    pmm_update_all();
    //kprintf("%X %X\n", g_mod_end, _ptr * ARCH_PAGE_SIZE);

    // Map and initialize the page tables
    init_paging();
}
