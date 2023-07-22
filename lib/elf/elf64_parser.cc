#include "elf/elf.h"
#include <ebl/string.h>

using N = elf::Context;

char* read_section64(elf64_ehdr_t const* img, elf64_shdr_t const* shdr, int idx) {
    return (char*)((uintptr_t) img + shdr[idx].sh_offset);
}

char* N::Context64::get_symbol_name(elf64_addr_t addr) {
    elf64_sym_t* sym = nullptr;
    auto* tab = (elf64_sym_t*)(symtab->sh_offset+(uintptr_t) img);
    elf64_addr_t val = 0;
    for(unsigned long i = 0; i < symtab->sh_size / sizeof(elf64_sym_t); i++) {
        if(tab[i].st_value > val && tab[i].st_value <= addr) {
            sym = &tab[i];
            val = tab[i].st_value;
        }
    }
    if(!sym) return nullptr;
    char* sh_str = (char*)(strtab->sh_offset+(uintptr_t) img);
    return &sh_str[sym->st_name];
}

elf64_addr_t N::Context64::get_symbol_addr(elf64_addr_t addr) {
    auto* tab = (elf64_sym_t*)(symtab->sh_offset+(uintptr_t) img);
    elf64_addr_t val = 0;
    for(unsigned long i = 0; i < symtab->sh_size / sizeof(elf64_sym_t); i++)
        if(tab[i].st_value > val && tab[i].st_value <= addr)
            val = tab[i].st_value;
    return val;
}

elf64_shdr_t* N::Context64::find_section(const char* name) {
    using namespace ebl;
    char* sh_str = read_section64(img, shdr, img->e_shstrndx);
    for(elf64_half_t i = 0; i < img->e_shnum; i++) {
        if((name[0] == 0) && (strcmp(".null", name) == 0))
            return &shdr[i];
        if(strcmp(sh_str + shdr[i].sh_name, name) == 0)
            return &shdr[i];
    }
    return nullptr;
}

status_t N::Context64::load(elf64_ehdr_t *hdr) {
    img = hdr;
    shdr = (elf64_shdr_t*)((uintptr_t) hdr + hdr->e_shoff);
    phdr = (elf64_phdr_t*)((uintptr_t) hdr + hdr->e_phoff);
    symtab = find_section(".symtab");
    strtab = find_section(".strtab");
    return E::SUCCESS;
}
