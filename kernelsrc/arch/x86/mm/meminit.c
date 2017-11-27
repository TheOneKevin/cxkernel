/*
 * File:   meminit.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 04:37:25 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T16:37:20-05:00
*/

#include "bitmap.h"
#include "arch/x86/multiboot.h"
#include "panic.h"
#include "lib/printk.h"
#include "lib/string.h"
#include "mm/mmtypes.h"
#include "arch/x86/paging.h"
#include "arch/x86/arch_common.h"
#include "arch/x86/interface/meminit.h"
#include "arch/x86/bootmm.h"

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

    // Prepare mappings
    uint32_t* kernel_pt = &_kernel_table3;
    memset(kernel_pt, 0, sizeof(uint32_t) * 1024);
    uint32_t* page_dir = &_kernel_dir;
    if(page_dir[ARCH_GET_PD_NUMBER(ARCH_PAGE_ALIGN(g_mod_end))] != 0 && ARCH_GET_PD_NUMBER(ARCH_PAGE_ALIGN(g_mod_end)) + 1 < 1024)
        page_dir[ARCH_GET_PD_NUMBER(ARCH_PAGE_ALIGN(g_mod_end)) + 1] = ((uint32_t) kernel_pt - ARCH_VIRT_BASE) + 0x3;
    else if(page_dir[ARCH_GET_PD_NUMBER(ARCH_PAGE_ALIGN(g_mod_end))] == 0)
        page_dir[ARCH_GET_PD_NUMBER(ARCH_PAGE_ALIGN(g_mod_end))] = ((uint32_t) kernel_pt - ARCH_VIRT_BASE) + 0x3;
    else
        PANIC("pmman failed to initialize.");

    // Initialize and zero out the buddy bitmap
    pmm_buddy_map.length = (max_mem / (sizeof(unsigned int) * 8));
    pmm_buddy_map.bitmap = (unsigned int *) ARCH_PAGE_ALIGN(g_mod_end);
    memset(pmm_buddy_map.bitmap, 0xFFFFFFFF, pmm_buddy_map.length * sizeof(unsigned int));
    kprintf("pmman: %X entries, %X pages indexed\n", pmm_buddy_map.length, max_mem);
    g_pmm_buddy_map = &pmm_buddy_map;

    bootmm_init_memory_regions();
}
