/*
 * File:   arch_common.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 07:46:54 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-22T13:52:50-05:00
*/

#pragma once

#include "bitmap.h"
#include "tasking/symbol_table.h"
#include "arch/x86/multiboot.h"

#define __TMP1__(TYPE, v) TYPE v

#define ARCH_VIRT_BASE 0xE0000000
#define ARCH_PAGE_SIZE 0x1000
#define ARCH_PAGE_MASK (~(ARCH_PAGE_SIZE - 1))
#define ARCH_PAGE_ALIGN(addr) (((addr) + ARCH_PAGE_SIZE - 1) & ARCH_PAGE_MASK)
#define ARCH_PAGE_ALIGN_DOWN(n) (ARCH_PAGE_ALIGN(n) == n ? ARCH_PAGE_ALIGN(n) : ARCH_PAGE_ALIGN(n) - ARCH_PAGE_SIZE)
#define ARCH_PAGE_NUMBER(addr) ((addr) >> 22)
#define BOCHS_MAGIC_BREAK() asm volatile("xchg %bx, %bx")
#define ARCH_FOREACH_MMAP(var)                                                                               \
    for(__TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) (__GGMBT__ -> mmap_addr + ARCH_VIRT_BASE); \
    (unsigned int) (var) < __GGMBT__ -> mmap_addr + __GGMBT__ -> mmap_length + ARCH_VIRT_BASE;          \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))    \

#define ARCH_SYSCALL_INT_NO 0x80

#define ARCH_STACK_TOP 0xFFFFFFFF

#ifdef __cplusplus
extern "C" {
#endif

multiboot_info_t* __GGMBT__;
symbol_table_desc_t* g_krnl_sym_table;
bitmap_t* g_pmm_buddy_map;
uint32_t g_mod_end;

#ifdef __cplusplus
}
#endif
