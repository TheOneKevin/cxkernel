/*
 * File:   common.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 04-Aug-2017 07:46:54 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 07:29:29 PM
*/

#pragma once

#include "multiboot.h"
#include "sys/symbol_table.h"

#define VIRT_BASE 0xE0000000
#define PAGE_SIZE 0x1000
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & (PAGE_MASK))

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

#define SYSCALL_INT_NO 0x80

#ifdef __cplusplus
extern "C" {
#endif

multiboot_info_t* __GGMBT__;
symbol_table_desc_t* krnl_sym_table;

#ifdef __cplusplus
}
#endif
