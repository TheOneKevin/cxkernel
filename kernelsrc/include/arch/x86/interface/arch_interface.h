/**
 * @file
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-22T21:10:13-04:00
 */

#pragma once

#include "common.h"
#include "bitmap.h"
#include "core/console.h"
#include "arch/arch_types.h"

#define ARCH_VIRT_BASE 0xE0000000
#define ARCH_SYSCALL_INT_NO 0x2E
#define ARCH_STACK_TOP 0xFF800000

#ifdef __cplusplus
extern "C" {
#endif

//! Global variable containing crucial memory addresses
struct MEMORY_MAP g_memory_map;
//! Global bitmap used for the buddy page allocator
bitmap_t* g_pmm_buddy_map;
//! Gets console to print to screen
console_t* arch_get_console(void);
/**
 * Called by the kernel right after @ref arch_get_console
 * @param magic Passed by kernel (magic number)
 * @param ptr   Passed by kernel, any boot time parameters pushed on to stack
 */
void arch_early_init(uint32_t magic, void* ptr);
void arch_init(void);
void arch_late_init(void);
void arch_pmeminit(void);
void arch_timer_install(void);

#ifdef __cplusplus
}
#endif
