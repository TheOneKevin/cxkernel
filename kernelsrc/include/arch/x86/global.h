/*
 * File:   global.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2018-03-28T14:18:49-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-28T14:34:52-04:00
*/

#pragma once

#include "common.h"
#include "bitmap.h"
#include "mm/mmtypes.h"
#include "arch/arch_types.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/archconf.h"
#include "tasking/symbol_table.h"

#ifdef __cplusplus
extern "C" {
#endif

mem_map_t* g_mmap;
uint64_t g_paging_PDPT[4] ALIGNED(0x20);
multiboot_info_t* g_mbt_struct;
symbol_table_desc_t* g_krnl_sym_table;

#ifdef __cplusplus
}
#endif
