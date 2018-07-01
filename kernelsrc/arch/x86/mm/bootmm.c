/*
 * File:   bootmm.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-31T16:30:36-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-05-23T13:41:43-04:00
*/

#define __MODULE__ "_MMAN"

#include "list.h"
#include "panic.h"
#include "bitmap.h"
#include "assert.h"
#include "lib/printk.h"
#include "mm/page_alloc.h"

#include "arch/x86/global.h"
#include "arch/x86/cpu.h"
#include "arch/x86/bootmm.h"
#include "arch/x86/paging.h"
#include "arch/x86/arch_common.h"

#include "arch/atomic.h"

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
    for(unsigned int i = 0x100; i <= ARCH_PAGE_ALIGN((uint32_t) g_pmm_buddy_map -> bitmap + g_pmm_buddy_map -> length * sizeof(unsigned int) - ARCH_VIRT_BASE) / ARCH_PAGE_SIZE; i++)
        bitmap_setbit(g_pmm_buddy_map -> bitmap, i);

    // Allocate from the first highest free address first
    pmm_update_all();
    //kprintf("%X %X\n", g_mod_end, _ptr * ARCH_PAGE_SIZE);
}

void bootmm_init_memory_structs(int max_pages)
{
    //kprintf("%d\n", sizeof(mem_map_t));
    OS_PRN("%-66s", "Building mmap structs...");
    g_mmap = (mem_map_t *)(0x100000); // mmap array will start at 1MiB and grow upwards
    int num_entries = (max_pages * sizeof(mem_map_t)) / (ARCH_PAGE_SIZE) + 1;
    //kprintf("%X\n", num_entries);

    for(unsigned int i = 0; i < (unsigned) num_entries; i ++)
    {
        phys_t addr = pmm_alloc_page();
        arch_map_page(0x100000 + i * ARCH_PAGE_SIZE, addr, PTE_RW);
    }
    ARCH_FOREACH_MMAP(mmap)
    {
        if(mmap -> type != MULTIBOOT_MEMORY_BADRAM)
        {
            list_head_t* prev = NULL;
            mem_map_t* e_prev = NULL;
            for(uint32_t i = ARCH_PAGE_ALIGN(mmap -> addr) / ARCH_PAGE_SIZE; i < ARCH_PAGE_ALIGN(mmap -> addr + mmap -> len) / ARCH_PAGE_SIZE - 1; i++)
            {
                g_mmap[i] = (mem_map_t) {
                    .list      = (list_head_t) NEW_LIST_HEAD(),
                    .ref_count = (atomic_t) ATOMIC_INIT(0),
                    .idx       = i,
                    .flags     = mmap -> type,
                    .priv      = 0UL,
                    .virt      = NULL
                };

                g_mmap[i].list.prev = prev;
                g_mmap[i].list.next = NULL;
                if(e_prev != NULL) e_prev -> list.next = &(g_mmap[i].list);
                e_prev = &(g_mmap[i]);
                prev = &(g_mmap[i].list);
            }
        }
    }

    for(int i = 0; i <= max_pages; i++)
    {

    }

    fprintf(STREAM_OUT, "DONE!\n");
}
