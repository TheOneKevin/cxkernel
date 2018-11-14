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

namespace elf
{
    struct Context
    {
        elf32_ehdr_t* ehdr;
        elf32_shdr_t* shdr;
        elf32_phdr_t* phdr;
    };

    void load_img32(elf32_ehdr_t* img, Context& ctx);
    void load_img64(elf64_ehdr_t* img, Context& ctx);
    void load_img(void* img, Context& ctx);
}

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
