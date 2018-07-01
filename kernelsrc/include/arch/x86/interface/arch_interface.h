/*
 * File:   arch_interface.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T17:04:35-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-04-01T17:55:00-04:00
*/

#pragma once

#include "common.h"
#include "bitmap.h"
#include "core/console.h"
#include "arch/x86/meminit.h"
#include "arch/arch_types.h"

#define ARCH_VIRT_BASE 0xE0000000
#define ARCH_SYSCALL_INT_NO 0x80
#define ARCH_STACK_TOP 0xFF800000

#ifdef __cplusplus
extern "C" {
#endif

struct MEMORY_MAP g_memory_map;
// Global bitmap used for the buddy page allocator
bitmap_t* g_pmm_buddy_map;

console_t* arch_get_console(void);
void arch_early_init(uint32_t magic, void* ptr);
void arch_init(void);
void arch_late_init(void);

#ifdef __cplusplus
}
#endif
