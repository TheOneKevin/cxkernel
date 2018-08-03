/**
 * @file   arch_common.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-25T16:09:53-04:00
 */

#pragma once

#include "arch/arch_interface.h"
#include "arch/arch_paging.h"
#include "arch/arch_types.h"
#include "arch/atomic.h"
#include "arch/debug.h"

#include "arch/x86/archconf.h"
#include "arch/x86/multiboot.h"

#define __9042352479_TMP1__(TYPE, v) TYPE v
#define BOCHS_MAGIC_BREAK() { kprintf("[BCHSB] line: %d at %s\n", __LINE__, __FILE__); asm volatile("xchg %bx, %bx"); }
#define ARCH_FOREACH_MMAP(var) \
    for(__9042352479_TMP1__(multiboot_memory_map_t*, var) = (multiboot_memory_map_t *) (g_mbt_struct -> mmap_addr + ARCH_VIRT_BASE); \
    (unsigned int) (var) < g_mbt_struct -> mmap_addr + g_mbt_struct -> mmap_length + ARCH_VIRT_BASE; \
    var = (multiboot_memory_map_t *) ((unsigned int) (var) + (var) -> size + sizeof((var) -> size)))
#define ARCH_VIRT_PHYS(addr) ((addr) + ARCH_VIRT_BASE)
#define ARCH_PAGE_NUMBER(addr) ((addr) >> 22)
#define IRQ_OFF  asm volatile ("cli");
#define IRQ_RES  asm volatile ("sti");
#define HALT     asm volatile ("hlt");
