/*
 * File:   arch_common.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 07:46:54 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-03T20:36:06-04:00
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