/**
 * @file   elf_parser.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, November 12th 2018, 9:42:07 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:23:26-05:00
 */

#pragma once
#include "common.h"
#include <elf.h>

#ifdef __cplusplus
namespace elf
{
    struct Context
    {
        union
        {
            elf32_ehdr_t* img32;
            elf64_ehdr_t* img64;
        };
        union
        {
            elf32_shdr_t* shdr32;
            elf32_shdr_t* shdr64;
        };
        union
        {
            elf32_phdr_t* phdr32;
            elf64_phdr_t* phdr64;
        };
        union
        {
            elf32_shdr_t* symtab32;
            elf32_shdr_t* symtab64;
        };
        union
        {
            elf32_shdr_t* strtab32;
            elf32_shdr_t* strtab64;
        };
    };

    char* get_symbol_name32(uint32_t addr, Context& ctx);
    uint32_t get_symbol_addr32(uint32_t addr, Context& ctx);
    elf32_shdr_t* find_section32(const char* name, Context& ctx);
    void load_img32(elf32_ehdr_t* img, Context& ctx);
    void load_img64(elf64_ehdr_t* img, Context& ctx);
    void load_img(void* img, Context& ctx);
} // namespace elf
#endif

__BEGIN_CDECLS

/**
 * Determines whether a given elf header is valid.
 * @param h     Pointer to the elf header
 * @return true If the given header is valid 
 */
static inline bool isValidElf(elf32_ehdr_t* h)
{
    return (h -> e_ident[EI_MAG0] == ELFMAG0) && (h -> e_ident[EI_MAG1] == ELFMAG1) &&
           (h -> e_ident[EI_MAG2] == ELFMAG2) && (h -> e_ident[EI_MAG3] == ELFMAG3);
}

__END_CDECLS
