/**
 * @file   elf_parser.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 11th 2018, 3:26:40 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:23:58-05:00
 */

#include <elf.h>
#include <elf_parser.h>

namespace elf {

void load_img32(elf32_ehdr_t* img, Context& ctx)
{
    
}

void load_img64(elf64_ehdr_t* img, Context& ctx)
{

}

void load_img(void* img, Context& ctx)
{
    elf32_ehdr_t* hdr = (elf32_ehdr_t*) img;
    if(isValidElf(hdr) && hdr -> e_ident[EI_CLASS] == ELFCLASS32)
        load_img32(hdr, ctx);
    else if(isValidElf(hdr) && hdr -> e_ident[EI_CLASS] == ELFCLASS64)
        load_img64((elf64_ehdr_t*) hdr, ctx);
}

}