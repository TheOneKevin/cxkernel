/**
 * @file   arch_interface.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 6:07:19 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-19T16:08:04-04:00
 */

#pragma once

#define ARCH_PAGE_SIZE      0x1000
#define ARCH_SYSCALL_INT_NO 0x2E

#ifndef __ASSEMBLER__
#include "common.h"
// Everything
#include "arch/arch_types.h"

#define ARCH_PAGE_MASK (~(ARCH_PAGE_SIZE - 1))
#define ARCH_PAGE_MASK_LL ((~((unsigned long long)ARCH_PAGE_SIZE - 1ULL)))
#define ARCH_PAGE_ALIGN(addr) (((addr) + ARCH_PAGE_SIZE - 1) & ARCH_PAGE_MASK)
#define ARCH_PAGE_ALIGN_DOWN(n) (ARCH_PAGE_ALIGN(n) == n ? ARCH_PAGE_ALIGN(n) : ARCH_PAGE_ALIGN(n) - ARCH_PAGE_SIZE)

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

#endif // __ASSEMBLER__
