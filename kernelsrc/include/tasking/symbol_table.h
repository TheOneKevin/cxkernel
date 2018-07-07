/**
 * @file   symbol_table.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:45:41-04:00
 */

#pragma once

#include "common.h"
#include "tasking/elf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    bool         present;
    int          num_sym;
    elf_sym_t*   symbols;
    char*        string_table_addr;
} symbol_table_desc_t;

int load_symbol_table(elf_sheader_t* symbol_table, elf_sheader_t* string_table, symbol_table_desc_t* symbol_table_desc);
char* get_symbol_name(unsigned long address, symbol_table_desc_t* symbol_table_desc);
unsigned int get_symbol_addr(unsigned long address, symbol_table_desc_t* symbol_table_desc);

#ifdef __cplusplus
}
#endif
