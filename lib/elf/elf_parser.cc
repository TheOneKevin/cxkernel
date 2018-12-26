/**
 * @file   elf_parser.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 11th 2018, 3:26:40 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:23:58-05:00
 */

#define __MODULE__ "ELF "

#include <string.h>
#include <elf.h>
#include <elf_parser.h>

namespace elf
{
    static void* read_section(const Context &ctx, int idx)
    {
        return (void*) ((uint32_t) ctx.img32 + ctx.shdr32[idx].sh_offset);
    }

    static void print_section_flags(elf32_word_t flag, char (&ret)[11])
    {
        ret[0] = CHECK_MFLG(flag, SHF_WRITE) ? 'w' : '-';
        ret[1] = CHECK_MFLG(flag, SHF_ALLOC) ? 'a' : '-';
        ret[2] = CHECK_MFLG(flag, SHF_EXECINSTR) ? 'x' : '-';
        ret[3] = CHECK_MFLG(flag, SHF_MERGE) ? 'm' : '-';
        ret[4] = CHECK_MFLG(flag, SHF_STRINGS) ? 's' : '-';
        ret[5] = CHECK_MFLG(flag, SHF_INFO_LINK) ? 'i' : '-';
        ret[6] = CHECK_MFLG(flag, SHF_LINK_ORDER) ? 'l' : '-';
        ret[7] = CHECK_MFLG(flag, SHF_OS_NONCONFORMING) ? 'o' : '-';
        ret[8] = CHECK_MFLG(flag, SHF_GROUP) ? 'g' : '-';
        ret[9] = CHECK_MFLG(flag, SHF_TLS) ? 't' : '-';
        ret[10] = 0;
    }

    static void print_program_flags(elf32_word_t flag, char(&ret)[4])
    {
        ret[0] = CHECK_MFLG(flag, PF_R) ? 'r' : '-';
        ret[1] = CHECK_MFLG(flag, PF_W) ? 'w' : '-';
        ret[2] = CHECK_MFLG(flag, PF_X) ? 'x' : '-';
        ret[3] = 0;
    }

    static void print_section_headers(const Context &ctx)
    {
        uint32_t i;
        char* sh_str;    /* section-header string-table is also a section. */
        OS_LOG("Begin section headers dump.\n");
        /* Read section-header string-table */
        fprintf(STREAM_LOG, "eh.e_shstrndx = 0x%x\n", ctx.img32->e_shstrndx);
        sh_str = static_cast<char*>(read_section(ctx, ctx.img32->e_shstrndx));
        fprintf(STREAM_LOG, "================================================================================\n");
        fprintf(STREAM_LOG, " idx offset     load-addr  size       algn flags      type       section\n");
        fprintf(STREAM_LOG, "================================================================================\n");

        char c[11] = {' '}; c[10] = 0; // Extra one for null terminator

        for(i = 0; i < ctx.img32->e_shnum; i++)
        {
            print_section_flags(ctx.shdr32[i].sh_flags, c);
            fprintf(STREAM_LOG, " %03d ", i);
            fprintf(STREAM_LOG, "0x%08x ", ctx.shdr32[i].sh_offset);
            fprintf(STREAM_LOG, "0x%08x ", ctx.shdr32[i].sh_addr);
            fprintf(STREAM_LOG, "0x%08x ", ctx.shdr32[i].sh_size);
            fprintf(STREAM_LOG, "%4d ", ctx.shdr32[i].sh_addralign);
            fprintf(STREAM_LOG, "%s ", c);
            if(ctx.shdr32[i].sh_type == SHT_NULL) fprintf(STREAM_LOG, "NULL      ");
            else if(ctx.shdr32[i].sh_type == SHT_PROGBITS) fprintf(STREAM_LOG, "PROGBITS  ");
            else if(ctx.shdr32[i].sh_type == SHT_SYMTAB) fprintf(STREAM_LOG, "SYMTAB    ");
            else if(ctx.shdr32[i].sh_type == SHT_STRTAB) fprintf(STREAM_LOG, "STRTAB    ");
            else if(ctx.shdr32[i].sh_type == SHT_RELA) fprintf(STREAM_LOG, "RELA      ");
            else if(ctx.shdr32[i].sh_type == SHT_HASH) fprintf(STREAM_LOG, "HASH      ");
            else if(ctx.shdr32[i].sh_type == SHT_DYNAMIC) fprintf(STREAM_LOG, "DYNAMIC   ");
            else if(ctx.shdr32[i].sh_type == SHT_NOTE) fprintf(STREAM_LOG, "NOTE      ");
            else if(ctx.shdr32[i].sh_type == SHT_NOBITS) fprintf(STREAM_LOG, "NOBITS    ");
            else if(ctx.shdr32[i].sh_type == SHT_REL) fprintf(STREAM_LOG, "REL       ");
            else if(ctx.shdr32[i].sh_type == SHT_SHLIB) fprintf(STREAM_LOG, "SHLIB     ");
            else if(ctx.shdr32[i].sh_type == SHT_DYNSYM) fprintf(STREAM_LOG, "DYNSYM    ");
            else if(ctx.shdr32[i].sh_type == SHT_INIT_ARRAY) fprintf(STREAM_LOG, "INIT_ARR  ");
            else if(ctx.shdr32[i].sh_type == SHT_FINI_ARRAY) fprintf(STREAM_LOG, "FINI_ARR  ");
            else if(ctx.shdr32[i].sh_type == SHT_PREINIT_ARRAY) fprintf(STREAM_LOG, "PREINIT   ");
            else if(ctx.shdr32[i].sh_type == SHT_GROUP) fprintf(STREAM_LOG, "GROUP     ");
            else if(ctx.shdr32[i].sh_type == SHT_SYMTAB_SHNDX) fprintf(STREAM_LOG, "SHNDX     ");
            else fprintf(STREAM_LOG, "????      ");
            fprintf(STREAM_LOG, "%s", (sh_str + ctx.shdr32[i].sh_name));
            fprintf(STREAM_LOG, "\n");
        }

        fprintf(STREAM_LOG, "================================================================================\n");
    }

    void print_program_headers(Context& ctx)
    {
        OS_LOG("Begin program headers dump.\n");
        fprintf(STREAM_LOG, "================================================================================\n");
        fprintf(STREAM_LOG, " idx type    offset     virt addr  phys addr  mem size   file size  algn flags\n");
        fprintf(STREAM_LOG, "================================================================================\n");
        char c[4] = {' '}; c[3] = 0; // Extra one for null terminator
        for(int i = 0; i < ctx.img32->e_phnum; i++)
        {
            print_program_flags(ctx.phdr32[i].p_flags, c);
            fprintf(STREAM_LOG, " %03d ", i);
            if(ctx.phdr32[i].p_type == PT_NULL) fprintf(STREAM_LOG, "NULL    ");
            else if(ctx.phdr32[i].p_type == PT_LOAD) fprintf(STREAM_LOG, "LOAD    ");
            else if(ctx.phdr32[i].p_type == PT_DYNAMIC) fprintf(STREAM_LOG, "DYNAMIC ");
            else if(ctx.phdr32[i].p_type == PT_INTERP) fprintf(STREAM_LOG, "INTERP  ");
            else if(ctx.phdr32[i].p_type == PT_NOTE) fprintf(STREAM_LOG, "NOTE    ");
            else if(ctx.phdr32[i].p_type == PT_SHLIB) fprintf(STREAM_LOG, "SLIB    ");
            else if(ctx.phdr32[i].p_type == PT_PHDR) fprintf(STREAM_LOG, "PHDR    ");
            else if(ctx.phdr32[i].p_type == PT_TLS) fprintf(STREAM_LOG, "TLS     ");
            else fprintf(STREAM_LOG, "????    ");
            fprintf(STREAM_LOG, "0x%08x ", ctx.phdr32[i].p_offset);
            fprintf(STREAM_LOG, "0x%08x ", ctx.phdr32[i].p_vaddr);
            fprintf(STREAM_LOG, "0x%08x ", ctx.phdr32[i].p_paddr);
            fprintf(STREAM_LOG, "0x%08x ", ctx.phdr32[i].p_memsz);
            fprintf(STREAM_LOG, "0x%08x ", ctx.phdr32[i].p_filesz);
            fprintf(STREAM_LOG, "%4d ", ctx.phdr32[i].p_align);
            fprintf(STREAM_LOG, "%s ", c);
            fprintf(STREAM_LOG, "\n");
        }
        fprintf(STREAM_LOG, "================================================================================\n");
    }

    void load_img32(elf32_ehdr_t* img, Context &ctx)
    {
        ctx.img32 = img;
        ctx.shdr32 = (elf32_shdr_t*) ((uint32_t) img + img->e_shoff);
        ctx.phdr32 = (elf32_phdr_t*) ((uint32_t) img + img->e_phoff);
        print_section_headers(ctx);
        print_program_headers(ctx);
    }

    void load_img64(elf64_ehdr_t* img, Context &ctx)
    {

    }

    void load_img(void* img, Context &ctx)
    {
        auto* hdr = static_cast<elf32_ehdr_t*>(img);
        if(isValidElf(hdr) && hdr->e_ident[EI_CLASS] == ELFCLASS32)
            load_img32(hdr, ctx);
        else if(isValidElf(hdr) && hdr->e_ident[EI_CLASS] == ELFCLASS64)
            load_img64(reinterpret_cast<elf64_ehdr_t*>(hdr), ctx);

    }

} // namespace elf