/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   arch_interface.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 13 2018, 6:07 PM
 */

#pragma once

#if ARCH_TYPE == ARCH_x86_32
    #include "arch/x86/32/arch_defs.h"
    #include "arch/x86/32/paging.h"
#elif ARCH_TYPE == ARCH_x86_64
    #include "arch/x86/64/arch_defs.h"
    #include "arch/x86/32/paging.h"
#endif
#define ARCH_SYSCALL_INT_NO 0x2E

#define ENABLE_INTERRUPTS  asm volatile ("sti");
#define DISABLE_INTERRUPTS asm volatile ("cli");
#define HALT_CPU           asm volatile ("hlt");
#define DEBUG_EXIT         asm volatile ("outw %1, %0"::"dN"(0x604),"a"(0x0|0x2000));

#ifdef __cplusplus

namespace x86_32
{
    void early_init(loader_t args);
    void init();
    void init_mmu();
    arch::Mmu& get_mmu();
}

namespace x86_64
{
    void early_init(loader_t args);
    void init();
}

#endif
