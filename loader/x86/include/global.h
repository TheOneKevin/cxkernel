/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   global.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 24 2018, 6:12 PM
 */

#pragma once

#include <elf_parser.h>
#include "common.h"
#include "system.h"
#include "arch/x86/arch_utils.h"

// Careful with the ARCH_VIRT_BASE shenanigans
#define FOREACH_MMAP(var, mbt) \
    for(__9042352479_TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) ((mbt) -> mmap_addr); \
    (unsigned int) (var) < (mbt) -> mmap_addr + (mbt) -> mmap_length; \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))

__BEGIN_CDECLS

// These are declared in main.cc
extern multiboot_info_t g_mbt;
extern bool g_load64;
extern virt_t MODS_END;
extern phys_t MAX_MEM;

// This initializes the bitmap (declared in bootmem.cc)
void init_bootmm32();
bool pmm_update_all(void);
phys_t pmm_alloc_page(bool clear = true);
void pmm_free_page(phys_t address);
void pmm_free_page_multi(phys_t address, int pages);

// GDT stuff
void initgdt32();

__END_CDECLS

namespace loader
{
    class Mmu
    {
    public:
        virtual void init();
        virtual void map(uint64_t virt, uint64_t phys, uint64_t flags);
    };
    Mmu& get_mmu(void);
    void run_program32(elf::Context& ctx);
} // namespace loader
