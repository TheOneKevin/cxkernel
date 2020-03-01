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

#define __MODULE__ "MMU"

#include <stdio.h>
#include <string.h>

#include "core/memory.h"

#include "arch/mmu.h"
#include "arch/x86/cpu.h"
#include "arch/x86/paging.h"
#include "arch/x86/interface/arch_interface.h"
#include "arch/x86/llio.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-value"

// Disable the annoying logging if need be
#undef OS_DBG
#define OS_DBG

#define PG_FLAG_MASK        0b100011111
#define PG_PAE_FLAG_MASK    0b100011111ULL

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
        uint32_t page_dir_addr = 0xFFFFF000;
    public:
        void init() override
        {
            OS_PRN("Registered paging without PAE\n");
        }
        void map(uint64_t a, uint64_t b, uint64_t c) override
        {
            auto virt = (uint32_t) a;
            auto phys = (uint32_t) b;
            auto flags = (uint16_t) c;
            auto* page_dir = (uint32_t*) page_dir_addr;
            flags &= PG_FLAG_MASK;
            uint32_t pdid = ARCH_GET_PD_IDX(virt);
            uint32_t ptid = ARCH_GET_PT_IDX(virt);
            uint32_t ptvd = ARCH_GET_VIRT(1023, pdid);
            OS_DBG("Map 0x%X -> 0x%X (0x%X)\n", phys, virt, flags);
            if(page_dir[pdid] == 0) // Page table does not exist yet
            {
                void* page = alloca(pmm::get_allocator().GetSize());
                pmm::get_allocator().AllocateSingle((uintptr_t) page);
                auto addr = (uint32_t) pmm::get_allocator().PageToPhysical((uintptr_t) page);
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
            OS_PRN("Registered paging with PAE\n");
        }
        void map(uint64_t a, uint64_t b, uint64_t c) override
        {
            // Type parameters
            auto virt = (uint32_t) a;
            auto phys = (uint64_t) b;
            auto flags = (uint64_t) c;
            if(x86_feature_test(x86_FEATURE_NX))
                flags &= PTE_NX | PG_PAE_FLAG_MASK;
            else
                flags &= PG_PAE_FLAG_MASK;
            uint32_t ppid = ARCH_PAE_GET_PDPT_IDX(virt);
            uint32_t pdid = ARCH_PAE_GET_PD_IDX(virt);
            uint32_t ptid = ARCH_PAE_GET_PT_IDX(virt);
            
            auto* rpage_dir = (uint64_t*) ARCH_PAE_GET_VIRT(2UL, 510, 508 + ppid);
            auto ptvd = (uint64_t) ARCH_PAE_GET_VIRT(2UL, 508 + ppid, pdid);

            OS_DBG("Map 0x%lX -> 0x%X (0x%lX)\n", phys, virt, flags);
            
            if(rpage_dir[pdid] == 0) // Page table does not exist yet
            {
                void* page = alloca(pmm::get_allocator().GetSize());
                pmm::get_allocator().AllocateSingle((uintptr_t) page);
                auto addr = (uint64_t) pmm::get_allocator().PageToPhysical((uintptr_t) page);
                rpage_dir[pdid] = addr | (PDE_RW | PDE_PR);
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
    void init_mmu()
    {
#ifdef WITH_PAE
        if(x86_feature_test(x86_FEATURE_PAE))
            static_cast<arch::Mmu&>(arch::__mmu32_pae).init();
        else
#endif
            static_cast<arch::Mmu&>(arch::__mmu32_nopae).init();
    }

    arch::Mmu& get_mmu()
    {
#ifdef WITH_PAE
        if(x86_feature_test(x86_FEATURE_PAE))
            return static_cast<arch::Mmu&>(arch::__mmu32_pae);
        else
#endif
            return static_cast<arch::Mmu&>(arch::__mmu32_nopae);
    }
}
