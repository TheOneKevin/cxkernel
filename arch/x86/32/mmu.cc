/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   mmu.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on March 13 2019, 4:03 PM
 */

#pragma GCC diagnostic ignored "-Wunused-parameter"
#define __MODULE__ "MMU"

#include <stdio.h>
#include <string.h>

#include "arch/mmu.h"
#include "arch/x86/cpu.h"
#include "arch/x86/paging.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/llio.h"

static inline void __tlb_flush_all(void)
{
    // Read the value of CR3 into CR3 to flush the TLB
    write_cr3(read_cr3());
}

static inline void __tlb32_flush_single(uint32_t addr)
{
    asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
}

namespace arch
{
    // Default implementations
    void Mmu::init() { }
    void Mmu::map(uint64_t virt, uint64_t phys, uint64_t flags) { }
    static Mmu __dummy_mmu;

    // Override virtual classes
    class Mmu32NopaeImpl : public Mmu
    {
    private:
        uint32_t page_dir_addr = 0;
    public:
        void init() override
        {

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
                auto addr = 0;//(uint32_t) pmm_alloc_page(false);
                page_dir[pdid] = addr | (PDE_RW | PDE_PR);
                __tlb32_flush_single(ptvd);
                memset((void*) ptvd, 0, ARCH_PAGE_SIZE);
            }
            auto* page_tab = (uint32_t*) ptvd;
            page_tab[ptid] = ((uint32_t) phys) | flags;
            __tlb32_flush_single(virt & ARCH_PAGE_MASK);
        }
    };
    static Mmu32NopaeImpl __mmu32_nopae;

    class Mmu32PaeImpl : public Mmu
    {
    public:
        void init() override
        {

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
            uint32_t ptvd = ARCH_PAE_GET_VIRT(3UL, 508 + ppid, pdid);
            auto* page_dir = (uint64_t*)(0xFFFFF000);
            OS_DBG("Map 0x%lX -> 0x%X (0x%lX)\n", phys, virt, flags);
            if(page_dir[pdid] == 0) // Page table does not exist yet
            {
                auto addr = 0;//(uint32_t) pmm_alloc_page(false);
                page_dir[pdid] = addr | (PDE_RW | PDE_PR);
                __tlb32_flush_single(ptvd);
                memset((void*) ptvd, 0, ARCH_PAGE_SIZE);
            }
            auto* page_tab = (uint64_t*) ptvd;
            page_tab[ptid] = ((uint32_t) phys) | flags;
            __tlb32_flush_single(virt & ARCH_PAGE_MASK);
        }
    };
    static Mmu32PaeImpl __mmu32_pae;
}  // namespace arch

namespace x86_32
{
    arch::Mmu& get_mmu()
    {
        if(x86_feature_test(x86_FEATURE_PAE)) return static_cast<arch::Mmu &>(arch::__mmu32_pae);
        return static_cast<arch::Mmu &>(arch::__mmu32_nopae);
    }
}