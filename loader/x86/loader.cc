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
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/paging.h"
#include "include/global.h"

namespace loader
{
    void run_program32(elf::Context &ctx)
    {
        get_mmu().map(ARCH_VIRT_BASE + 0xB8000, 0xB8000, PTE_RW | PTE_PR);
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
                BOCHS_MAGIC_BREAK();
                memset((void*) v, 0, ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz));
                BOCHS_MAGIC_BREAK();
                memcpy((void*) v, (void*) (ctx.phdr32[i].p_offset + (uint32_t) ctx.img32), ctx.phdr32[i].p_filesz);
            }
            else
            {
                for(uint32_t j = 0; j < ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz); j += ARCH_PAGE_SIZE)
                    get_mmu().map(v + j, p + j, flags);
            }
        }
        OS_DBG("=== Loader will now exit ===\n");
        get_mmu().map(ARCH_STACK_BOTTOM - ARCH_PAGE_SIZE, pmm_alloc_page(false), PTE_PR | PTE_RW | PTE_NX);
        memset((void*)(ARCH_STACK_BOTTOM - ARCH_PAGE_SIZE), 0, ARCH_PAGE_SIZE);
        asm volatile("mov %0, %%esp" :: "r" (ARCH_STACK_BOTTOM - 0x100));
        asm volatile("xchg %bx, %bx");
        ((void (*)(loader_t)) (ctx.img32->e_entry))({g_sig, ctx, &g_mbt});
        for(;;);
    }
} // namespace loader