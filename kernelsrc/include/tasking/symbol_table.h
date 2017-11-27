/*
 * File:   symbol_table.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:09:00 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T11:09:08-05:00
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
