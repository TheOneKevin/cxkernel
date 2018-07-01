/*
 * File:   meminit.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 04:37:25 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-05-23T13:53:26-04:00
*/

#define __MODULE__ "_PMAN"

#include "bitmap.h"
#include "arch/x86/multiboot.h"
#include "panic.h"
#include "lib/printk.h"
#include "lib/string.h"
#include "mm/mmtypes.h"
#include "mm/sbrk.h"

#include "arch/x86/paging.h"
#include "arch/x86/arch_common.h"
#include "arch/x86/meminit.h"
#include "arch/x86/bootmm.h"
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
    bootmm_init_memory_regions();

    // Map and initialize the page tables
    init_paging();

    // Initialize the mmap structs
    bootmm_init_memory_structs((int) max_mem);
}
