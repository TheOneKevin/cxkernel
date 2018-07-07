/**
 * @file   global.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Wednesday, March 28th 2018, 2:18:49 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:38:54-04:00
 */

#pragma once

#include "common.h"
#include "bitmap.h"
#include "arch/arch_types.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/archconf.h"
#include "tasking/symbol_table.h"

#ifdef __cplusplus
extern "C" {
#endif

uint64_t g_paging_PDPT[4] ALIGNED(0x20);
multiboot_info_t* g_mbt_struct;
symbol_table_desc_t* g_krnl_sym_table;

#ifdef __cplusplus
}
#endif
