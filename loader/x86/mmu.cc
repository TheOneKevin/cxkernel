/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   mmu.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 26 2018, 9:41 AM
 */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define __MODULE__ "MMU"
#include <string.h>
#include <stdio.h>
#include <elf_parser.h>
#include <assert.h>
#include "arch/x86/llio.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/paging.h"
#include "include/global.h"

namespace loader
{
    static inline void __tlb_flush_all(void)
    {
        // Read the value of CR3 into CR3 to flush the TLB
        write_cr3(read_cr3());
    }

    static inline void __tlb32_flush_single(uint32_t addr)
    {
        asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
    }

    static void map_page32_nopae(uint32_t virt, uint64_t phys, uint32_t dir, uint16_t flags)
    {
        int pdid = ARCH_GET_PD_IDX(virt);
        int ptid = ARCH_GET_PT_IDX(virt);
        uint32_t* page_dir = (uint32_t*) dir;
        OS_LOG("Map 0x%lX -> 0x%X (0x%X)\n", phys, virt, flags);
        if(page_dir[pdid] == 0) // Page table does not exist yet
        {
            uint32_t addr = (uint32_t) pmm_alloc_page(false);
            page_dir[pdid] = addr | (PDE_RW | PDE_PR);
            __tlb32_flush_single(ARCH_GET_VIRT(1023, pdid));
            memset((void*) ARCH_GET_VIRT(1023, pdid), 0, ARCH_PAGE_SIZE);
        }
        uint32_t* page_tab = (uint32_t*) ARCH_GET_VIRT(1023, pdid);
        page_tab[ptid] = ((uint32_t) phys) | (flags & 0xFFF);
        __tlb32_flush_single(virt & ARCH_PAGE_MASK);
    }

    static void load_mmu32_nopae()
    {
        uint32_t* pd = (uint32_t*) pmm_alloc_page();
        pd[1023] = ((uint32_t) pd) | (PDE_PR | PDE_RW);
        // Identity map the first 8 MiB
        uint32_t* pt = (uint32_t*) pmm_alloc_page();
        for(uint32_t i = 0; i < 1024; i++) pt[i] = (i << ARCH_PAGE_SHIFT) | (PTE_PR | PTE_RW);
        pd[0] = ((uint32_t) pt) | (PDE_PR | PDE_RW);
        // Enable paging
        write_cr3((uint32_t) pd);
        write_cr4(read_cr4() | CR4_PGE);
        write_cr0(read_cr0() | CR0_PG | CR0_WP);
        OS_PRN("Paging enabled!\n");
    }

    static void load_mmu32_pae()
    {

    }

    // Default implementations
    void mmu::init() { }
    void mmu::map(uint64_t virt, uint64_t phys, uint16_t flags) { }

    // Override virtual classes
    class mmu32_nopae : public mmu
    {
    private:
        uint32_t page_dir = 0;
    public:
        void init() override
        {
            load_mmu32_nopae();
            page_dir = 0xFFFFF000;
        }
        void map(uint64_t virt, uint64_t phys, uint16_t flags) override
        {
            map_page32_nopae((uint32_t) virt, phys, page_dir, flags);
        }
    };
    static mmu32_nopae __mmu32_nopae;

    // Get the MMU
    mmu& get_mmu(void)
    {
        return static_cast<mmu&>(__mmu32_nopae);
    }

    void map_program32(elf::Context& ctx)
    {
        get_mmu().map(ARCH_VIRT_BASE + 0xB8000, 0xB8000, PTE_RW | PTE_PR);
        //TODO: The identity map of 4 MiB may not cover the entire program header if the loader become too large.
        for(int i = 0; i < ctx.img32->e_phnum; i++)
        {
            ASSERT(ctx.phdr32[i].p_align == ARCH_PAGE_SIZE, "Program section not page aligned!");
            virt_t v = ctx.phdr32[i].p_vaddr;
            phys_t p = ctx.phdr32[i].p_offset + (phys_t) ctx.img32;
            uint8_t flags = PTE_PR | PTE_RW;
            //if(CHECK_MFLG(ctx.phdr32[i].p_flags, PF_W)) flags |= PTE_RW;
            if(ctx.phdr32[i].p_filesz != ctx.phdr32[i].p_memsz)
            {
                for(uint32_t j = 0; j < ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz); j += ARCH_PAGE_SIZE)
                    get_mmu().map(v + j, pmm_alloc_page(false), flags);
                memset((void*) v, 0, ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz));
                memcpy((void*) v, (void*)(ctx.phdr32[i].p_offset + (uint32_t) ctx.img32), ctx.phdr32[i].p_filesz);
            }
            else
            {
                for(uint32_t j = 0; j < ARCH_PAGE_ALIGN(ctx.phdr32[i].p_memsz); j += ARCH_PAGE_SIZE)
                    get_mmu().map(v + j, p + j, flags);
            }
        }
        OS_LOG("=== Loader will now exit ===\n");
        ((void (*)(void))(ctx.img32->e_entry))(/*sig, ptr*/);
        for(;;);
    }
}  // namespace loader
