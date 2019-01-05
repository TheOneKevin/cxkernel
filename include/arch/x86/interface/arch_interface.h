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

#define ARCH_PAGE_SHIFT     12
#define ARCH_PAGE_SIZE      (1 << (ARCH_PAGE_SHIFT))
#define ARCH_SYSCALL_INT_NO 0x2E

#ifndef __ASSEMBLER__
// Everything
#include "common.h"
#include "arch/types.h"

#define ARCH_PAGE_MASK (~(ARCH_PAGE_SIZE - 1))
//#define ARCH_PAGE_MASK_LL ((~((unsigned long long)ARCH_PAGE_SIZE - 1ULL)))
#define ARCH_PAGE_ALIGN(addr) (((addr) + (__typeof__ (addr))(ARCH_PAGE_SIZE - 1)) & (__typeof__ (addr))ARCH_PAGE_MASK)
#define ARCH_PAGE_ALIGN_DOWN(addr) ((addr) & (__typeof__ (addr))ARCH_PAGE_MASK)

/*#define __perform_x86_syscall(n, r0, r1, r2, r3, r4, r5) \
    asm volatile("mov %1, %%ebp; int %0" :: "i" (ARCH_SYSCALL_INT_NO), "r" (r5), \
        "a" (n), "b" (r0), "c" (r1), "d" (r2), "S" (r3), "D" (r4))*/
#define __perform_x86_syscall(n, r0, r1, r2, r3, r4, r5) \
    asm volatile("int %0" :: "i" (ARCH_SYSCALL_INT_NO), "a" (n), "b" (r0), "c" (r1), "d" (r2), "S" (r3), "D" (r4))
#define ARCH_SYSCALL0(n) ARCH_SYSCALL1(n, 0)
#define ARCH_SYSCALL1(n, r0) ARCH_SYSCALL2(n, r0, 0)
#define ARCH_SYSCALL2(n, r0, r1) ARCH_SYSCALL3(n, r0, r1, 0)
#define ARCH_SYSCALL3(n, r0, r1, r2) ARCH_SYSCALL4(n, r0, r1, r2, 0)
#define ARCH_SYSCALL4(n, r0, r1, r2, r3) ARCH_SYSCALL5(n, r0, r1, r2, r3, 0)
#define ARCH_SYSCALL5(n, r0, r1, r2, r3, r4) __perform_x86_syscall(n, r0, r1, r2, r3, r4, 0)
#define ARCH_SYSCALL6(n, r0, r1, r2, r3, r4, r5) \
    _Pragma("GCC warning \"'ARCH_SYSCALL6' macro is deprecated.\"") \
    __perform_x86_syscall(n, r0, r1, r2, r3, r4, r5)

#define ENABLE_INTERRUPTS  asm volatile ("sti");
#define DISABLE_INTERRUPTS asm volatile ("cli");

#ifdef __cplusplus

namespace x86_32
{
    void early_init();
    void init();
}

namespace x86_64
{
    void early_init();
    void init();
}

#endif

#endif // __ASSEMBLER__
