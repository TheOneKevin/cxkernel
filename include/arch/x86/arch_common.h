/**
 * @file   arch_common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, October 14th 2018, 9:06:54 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:47:55-05:00
 */

#pragma once

#define ARCH_VIRT_BASE 0xE0000000
#define ARCH_STACK_TOP 0xFF800000

#ifndef __ASSEMBLER__
#include "common.h"
#include "arch/x86/multiboot.h"

#define __9042352479_TMP1__(TYPE, v) TYPE v
#define BOCHS_MAGIC_BREAK() { kprintf("[BCHSB] line: %d at %s\n", __LINE__, __FILE__); asm volatile("xchg %bx, %bx"); }
#define ARCH_FOREACH_MMAP(var) \
    for(__9042352479_TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) (g_mbt_struct -> mmap_addr + ARCH_VIRT_BASE); \
    (unsigned int) (var) < g_mbt_struct -> mmap_addr + g_mbt_struct -> mmap_length + ARCH_VIRT_BASE; \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))
#define ARCH_VIRT_PHYS(addr) ((addr) + ARCH_VIRT_BASE)
#define ARCH_PAGE_NUMBER(addr) ((addr) >> 22)
#define HALT asm volatile ("hlt");

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__