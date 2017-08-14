/*
 * File:   symbol_table.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:09:11 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 04:31:56 PM
*/

#pragma once

#include "system.h"
#include "tasking/elf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    bool         present;
    uint32_t     num_sym;
    elf_sym_t*   symbols;
    char*        string_table_addr;
} symbol_table_desc_t;

int load_symbol_table(elf_sheader_t* symbol_table, elf_sheader_t* string_table, symbol_table_desc_t* symbol_table_desc);
char* get_symbol_name(uint32_t address, symbol_table_desc_t* symbol_table_desc);
uint32_t get_symbol_addr(uint32_t address, symbol_table_desc_t* symbol_table_desc);

#ifdef __cplusplus
}
#endif
