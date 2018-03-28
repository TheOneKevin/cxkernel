/*
 * File:   arch_common.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 07:46:54 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-26T13:53:21-04:00
*/

#pragma once

#include "bitmap.h"
#include "tasking/symbol_table.h"
#include "arch/x86/multiboot.h"
#include "arch/arch_types.h"
#include "arch/x86/archconf.h"

#define __9042352479_TMP1__(TYPE, v) TYPE v

#define ARCH_VIRT_BASE 0xE0000000
#define ARCH_PAGE_SIZE 0x1000
#define ARCH_PAGE_MASK (~(ARCH_PAGE_SIZE - 1))
#define ARCH_PAGE_ALIGN(addr) (((addr) + ARCH_PAGE_SIZE - 1) & ARCH_PAGE_MASK)
#define ARCH_PAGE_ALIGN_DOWN(n) (ARCH_PAGE_ALIGN(n) == n ? ARCH_PAGE_ALIGN(n) : ARCH_PAGE_ALIGN(n) - ARCH_PAGE_SIZE)
#define ARCH_PAGE_NUMBER(addr) ((addr) >> 22)
#define BOCHS_MAGIC_BREAK() { kprintf("[BOCHS BREAK] line: %d at %s\n", __LINE__, __FILE__); asm volatile("xchg %bx, %bx"); }
#define ARCH_FOREACH_MMAP(var)                                                                              \
    for(__9042352479_TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) (__GGMBT__ -> mmap_addr + ARCH_VIRT_BASE); \
    (unsigned int) (var) < __GGMBT__ -> mmap_addr + __GGMBT__ -> mmap_length + ARCH_VIRT_BASE;              \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))
#define ARCH_VIRT_PHYS(addr) ((addr) + ARCH_VIRT_BASE)

#define ARCH_SYSCALL_INT_NO 0x80
#define ARCH_STACK_TOP 0xFF800000

#ifdef __cplusplus
extern "C" {
#endif

multiboot_info_t* __GGMBT__;
symbol_table_desc_t* g_krnl_sym_table;
bitmap_t* g_pmm_buddy_map;
uint32_t g_mod_end;

struct MEMORY_MAP
{
    uint32_t KRN_MMAP;
    uint32_t KRN_MMAP_END;
    uint32_t KRN_PRIV_BEGIN;
    uint32_t KRN_PRIV_END;
    uint32_t KRN_BEGIN;
    uint32_t KRN_END;
    uint32_t KRN_BRK_START;
    uint32_t KRN_BRK;
    uint32_t KRN_SLIM;
    uint32_t KRN_STACK_SIZE;
    uint32_t KRN_STACK_BEGIN;
    uint32_t KRN_PAGE_TABLES_BEGIN;
    uint32_t KRN_PAGE_TABLES_END;
};

struct MEMORY_MAP MEM_MAP;

#ifdef __cplusplus
}
#endif
