/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   arch_utils.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 14 2018, 9:06 AM
 */

#pragma once

#if ARCH_TYPE == ARCH_x86_32
    #include "arch/x86/32/arch_defs.h"
#elif ARCH_TYPE == ARCH_x86_64
    #include "arch/x86/64/arch_defs.h"
#endif

#ifndef __ASSEMBLER__
#include "common.h"
#include "arch/x86/multiboot.h"

#define __9042352479_TMP1__(TYPE, v) TYPE v
#define BOCHS_MAGIC_BREAK() { printf("[BCHSB] line: %d at %s\n", __LINE__, __FILENAME__); asm volatile("xchg %bx, %bx"); }
#define ARCH_FOREACH_MMAP(var, mbt, off) \
    for(__9042352479_TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) ((mbt) -> mmap_addr + (off)); \
    (unsigned int) (var) < (mbt) -> mmap_addr + (mbt) -> mmap_length + (off); \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))
#define ARCH_VIRT_PHYS(addr) ((addr) + ARCH_VIRT_BASE)
#define ARCH_PAGE_NUMBER(addr) ((addr) >> ARCH_PAGE_SHIFT)
#define HALT asm volatile ("hlt");

__BEGIN_CDECLS

__END_CDECLS
#endif // __ASSEMBLER__