#pragma once

#include "sysv_elf_abi.h"
#include <ebl/status.h>

namespace elf {

    class Context {
    public:
        static status_t load(Context& ctx, void* img);
        bool is_64bits() const { return is_64bits_; }
    
    private:
        Context() {}
    
    private:
        struct Context32 {
        public:
            status_t load(elf32_ehdr_t* hdr);
        private:
            elf32_ehdr_t* img;
            elf32_shdr_t* shdr;
            elf32_phdr_t* phdr;
            elf32_shdr_t* symtab;
            elf32_shdr_t* strtab;
        };

        struct Context64 {
        public:
            status_t load(elf64_ehdr_t* hdr);
            elf64_shdr_t* find_section(const char* name);
            elf64_addr_t get_symbol_addr(elf64_addr_t addr);
            char* get_symbol_name(elf64_addr_t addr);
        private:
            elf64_ehdr_t* img;
            elf64_shdr_t* shdr;
            elf64_phdr_t* phdr;
            elf64_shdr_t* symtab;
            elf64_shdr_t* strtab;
        };

    private:
        union {
            Context64 impl64_;
            Context32 impl32_;
        };
        bool is_64bits_;
    };

} // namespace elf
