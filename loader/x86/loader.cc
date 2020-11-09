/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   loader.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 02 2019, 4:58 PM
 */

#define __MODULE__ "LODR"
#include <elf_parser.h>
#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "platform/pc/vga.h"
#include "core/memory.h"
#include "arch/interface.h"
#include "include/global.h"

extern bitmap_t alloc_map; // pmm bitmap

namespace loader
{
    void run_program32(elf::Context &ctx)
    {
        for(int i = 0; i < 0x100000; i += ARCH_PAGE_SIZE)
            get_mmu().map(ARCH_GET_PHYS(i), i, PTE_RW | PTE_PR);
        
        uint32_t my_pps_hard = 0;

        //TODO: The identity map of 4 MiB may not cover the entire program header if the loader become too large.
        for(int i = 0; i < ctx.img32->e_phnum; i++)
        {
            ASSERT(ctx.phdr32[i].p_align == ARCH_PAGE_SIZE, "Program section not page aligned!");
            virt_t v = ctx.phdr32[i].p_vaddr;
            phys_t p = ctx.phdr32[i].p_offset + (phys_t) ctx.img32;
            uint64_t flags = PTE_PR;
            if(CHECK_MFLG(ctx.phdr32[i].p_flags, PF_W)) flags |= PTE_RW;
            if(!CHECK_MFLG(ctx.phdr32[i].p_flags, PF_X)) flags |= PTE_NX;
            if(ctx.phdr32[i].p_filesz != ctx.phdr32[i].p_memsz)
            {
                for(uint32_t j = 0; j < ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz); j += ARCH_PAGE_SIZE)
                    get_mmu().map(v + j, pmm_alloc_page(false), flags);
                memset((void*) v, 0, ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz));
                memcpy((void*) v, (void*) (ctx.phdr32[i].p_offset + (uint32_t) ctx.img32), ctx.phdr32[i].p_filesz);
            }
            else
            {
                for(uint32_t j = 0; j < ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz); j += ARCH_PAGE_SIZE)
                    get_mmu().map(v + j, p + j, flags);
            }

            my_pps_hard = MAX(my_pps_hard, v + ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz));
        }

        init_pps32(my_pps_hard);
        
        OS_DBG("Kernel image end: 0x%X\n", my_pps_hard);

        OS_DBG("=== Loader will now exit ===\n");
        get_mmu().map(ARCH_STACK_BOTTOM - 1*ARCH_PAGE_SIZE, pmm_alloc_page(false), PTE_PR | PTE_RW | PTE_NX);
        memset((void*)(ARCH_STACK_BOTTOM - ARCH_PAGE_SIZE), 0, ARCH_PAGE_SIZE);
        asm volatile("mov %0, %%esp" :: "r" (ARCH_STACK_BOTTOM - 0x10));
        BOCHS_MAGIC_BREAK();
        ((void (*)(loader_t)) (ctx.img32->e_entry))({g_sig, &alloc_map, ctx, my_pps_hard, &g_mbt});
        for(;;);
    }
} // namespace loader