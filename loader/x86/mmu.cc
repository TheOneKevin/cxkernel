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
#include "arch/x86/cpu.h"
#include "arch/x86/llio.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/paging.h"
#include "include/global.h"

extern "C" void enable_xd();

namespace loader
{
    static volatile uint64_t pdpt[4] __ALIGNED(0x20);

    static inline void __tlb_flush_all(void)
    {
        // Read the value of CR3 into CR3 to flush the TLB
        write_cr3(read_cr3());
    }

    static inline void __tlb32_flush_single(uint32_t addr)
    {
        asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
    }

    // Default implementations
    void mmu::init() { }
    void mmu::map(uint64_t virt, uint64_t phys, uint64_t flags) { }
    static mmu __dummy_mmu;

    // Override virtual classes
    class mmu32_nopae : public mmu
    {
    private:
        uint32_t page_dir_addr = 0;
    public:
        void init() override
        {
            auto* pd = (uint32_t*) pmm_alloc_page();
            pd[1023] = ((uint32_t) pd) | (PDE_PR | PDE_RW);
            // Identity map the first 4 MiB
            auto* pt = (uint32_t*) pmm_alloc_page();
            for(uint32_t i = 0; i < 1024; i++) pt[i] = (i << ARCH_PAGE_SHIFT) | (PTE_PR | PTE_RW);
            pd[0] = ((uint32_t) pt) | (PDE_PR | PDE_RW);
            // Enable paging
            write_cr3((uint32_t) pd);
            write_cr4(read_cr4() | CR4_PGE);
            write_cr0(read_cr0() | CR0_PG | CR0_WP);
            OS_PRN("32-bit paging without PAE enabled!\n");
            __tlb_flush_all();
            page_dir_addr = 0xFFFFF000;
        }
        void map(uint64_t a, uint64_t b, uint64_t c) override
        {
            auto virt = (uint32_t) a;
            auto phys = (uint32_t) b;
            auto flags = (uint16_t) c;
            auto* page_dir = (uint32_t*) page_dir_addr;
            flags &= 0b100011111;
            uint32_t pdid = ARCH_GET_PD_IDX(virt);
            uint32_t ptid = ARCH_GET_PT_IDX(virt);
            uint32_t ptvd = ARCH_GET_VIRT(1023, pdid);
            OS_DBG("Map 0x%X -> 0x%X (0x%X)\n", phys, virt, flags);
            if(page_dir[pdid] == 0) // Page table does not exist yet
            {
                auto addr = (uint32_t) pmm_alloc_page(false);
                page_dir[pdid] = addr | (PDE_RW | PDE_PR);
                __tlb32_flush_single(ptvd);
                memset((void*) ptvd, 0, ARCH_PAGE_SIZE);
            }
            auto* page_tab = (uint32_t*) ptvd;
            page_tab[ptid] = ((uint32_t) phys) | flags;
            __tlb32_flush_single(virt & ARCH_PAGE_MASK);
        }
    };
    static mmu32_nopae __mmu32_nopae;

    class mmu32_pae : public mmu
    {
    public:
        void init() override
        {
            // Recursive mapping
            pdpt[0] = pmm_alloc_page();
            pdpt[1] = pmm_alloc_page();
            pdpt[2] = pmm_alloc_page();
            pdpt[3] = pmm_alloc_page();
            auto* pd = (uint64_t*) pdpt[3];
            pd[511] = (pdpt[3]) | (PDE_PR | PDE_RW);
            pd[510] = (pdpt[2]) | (PDE_PR | PDE_RW);
            pd[509] = (pdpt[1]) | (PDE_PR | PDE_RW);
            pd[508] = (pdpt[0]) | (PDE_PR | PDE_RW);
            // Map the first 4 MiB
            auto* pt1 = (uint64_t*) pmm_alloc_page();
            auto* pt2 = (uint64_t*) pmm_alloc_page();
            auto* pd1 = (uint64_t*) pdpt[0];
            for(uint64_t i = 0; i < 512; i++) pt1[i] = (i << ARCH_PAGE_SHIFT) | (PTE_PR | PTE_RW);
            for(uint64_t i = 512; i < 1024; i++) pt2[i - 512] = (i << ARCH_PAGE_SHIFT) | (PTE_PR | PTE_RW);
            pd1[0] = ((uint64_t) pt1) | (PDE_PR | PDE_RW);
            pd1[1] = ((uint64_t) pt2) | (PDE_PR | PDE_RW);
            // Enable PDPT entries
            pdpt[0] |= PDP_PR;
            pdpt[1] |= PDP_PR;
            pdpt[2] |= PDP_PR;
            pdpt[3] |= PDP_PR;
            // Enable paging
            write_cr3((uint32_t) &pdpt);
            write_cr4(read_cr4() | CR4_PAE | CR4_PGE);
            write_cr0(read_cr0() | CR0_PG | CR0_WP);
            OS_PRN("32-bit paging with PAE enabled!\n");
            if(x86_feature_test(x86_FEATURE_NX))
            {
                OS_DBG("NX/XD supported!\n");
                enable_xd();
            }
            else
                OS_DBG("NX/XD not supported!\n");
            __tlb_flush_all();
        }
        void map(uint64_t a, uint64_t b, uint64_t c) override
        {
            // Type parameters
            auto virt = (uint32_t) a;
            auto phys = (uint64_t) b;
            auto flags = (uint64_t) c;
            if(x86_feature_test(x86_FEATURE_NX))
                flags &= PTE_NX | 0b100011111ULL;
            else
                flags &= 0b100011111ULL;
            uint32_t ppid = ARCH_PAE_GET_PDPT_IDX(virt);
            uint32_t pdid = ARCH_PAE_GET_PD_IDX(virt);
            uint32_t ptid = ARCH_PAE_GET_PT_IDX(virt);
            uint64_t ptvd = ARCH_PAE_GET_VIRT(3UL, 508 + ppid, pdid);
            auto* page_dir = (uint64_t*)(0xFFFFF000);
            OS_DBG("Map 0x%lX -> 0x%X (0x%lX)\n", phys, virt, flags);
            if(page_dir[pdid] == 0) // Page table does not exist yet
            {
                auto addr = (uint32_t) pmm_alloc_page(false);
                page_dir[pdid] = addr | (PDE_RW | PDE_PR);
                __tlb32_flush_single(ptvd);
                memset((void*) ptvd, 0, ARCH_PAGE_SIZE);
            }
            auto* page_tab = (uint64_t*) ptvd;
            page_tab[ptid] = ((uint32_t) phys) | flags;
            __tlb32_flush_single(virt & ARCH_PAGE_MASK);
        }
    };
    static mmu32_pae __mmu32_pae;

    // Get the MMU
    mmu& get_mmu(void)
    {
        if(!g_load64)
        {
            if(x86_feature_test(x86_FEATURE_PAE)) return static_cast<mmu &>(__mmu32_pae);
            return static_cast<mmu &>(__mmu32_nopae);
        }
        else
        {
            return __dummy_mmu;
        }
    }
} // namespace loader
