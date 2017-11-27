/*
 * File:   symbol_table.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 12-Aug-2017 10:22:22 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T11:18:06-05:00
*/

#include "tasking/symbol_table.h"

int load_symbol_table(elf_sheader_t* symbol_table, elf_sheader_t* string_table, symbol_table_desc_t* symbol_table_desc)
{
    if(symbol_table == 0 || string_table == 0)
    {
        symbol_table_desc -> present = false;
        return -1;
    }

    else
    {
        symbol_table_desc -> present = true;
        symbol_table_desc -> num_sym = symbol_table -> sh_size / sizeof(elf_sym_t);
        symbol_table_desc -> symbols = (elf_sym_t *) (symbol_table -> sh_addr);
        symbol_table_desc -> string_table_addr = (char *) (string_table -> sh_addr);
        return 0;
    }
}

char* get_symbol_name(unsigned long address, symbol_table_desc_t* symbol_table_desc)
{
    elf_sym_t* symbol = 0;
    ELF_ADDR_t symbol_val = 0;

    if(symbol_table_desc -> present)
    {
        for(int i = 0; i < symbol_table_desc -> num_sym; i++)
        {
            elf_sym_t* temp = symbol_table_desc -> symbols + i;
            if(temp -> st_value > symbol_val && temp -> st_value <= address)
            {
                symbol = temp;
                symbol_val = temp -> st_value;
            }
        }

        ELF_WORD_t string_idx = symbol -> st_name;
        char* name = symbol_table_desc -> string_table_addr + string_idx;
        return name;
    }

    return 0;
}

unsigned int get_symbol_addr(unsigned long address, symbol_table_desc_t* symbol_table_desc)
{
    ELF_ADDR_t symbol_val = 0;

    if(symbol_table_desc -> present)
    {
        for(int i = 0; i < symbol_table_desc -> num_sym; i++)
        {
            elf_sym_t* temp = symbol_table_desc -> symbols + i;
            if(temp -> st_value > symbol_val && temp -> st_value <= address)
                symbol_val = temp -> st_value;
        }

        return symbol_val;
    }

    return 0;
}
