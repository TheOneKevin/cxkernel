/*
 * Filename: multiboo_utils.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 12-Aug-2017 11:19:39 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 03:11:06 PM
*/

#include "multiboot.h"
#include "lib/assert.h"
#include "lib/printk.h"
#include "lib/string.h"
#include "tasking/elf.h"
#include "arch/i386/common.h"

elf_sheader_t* mb_elf_get_section_header(char* sname)
{
    multiboot_info_t* mbt = __GGMBT__;

    multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbt -> u.elf_sec);
    uint32_t addr  = multiboot_elf_sec -> addr;
    uint32_t num   = multiboot_elf_sec -> num;
    uint32_t shndx = multiboot_elf_sec -> shndx;
    elf_sheader_t* shstr = (elf_sheader_t *) (addr + shndx * sizeof(elf_sheader_t));
    for(uint32_t i = 0; i < num; i++)
    {
        elf_sheader_t* shdr = (elf_sheader_t *) (addr + i * sizeof(elf_sheader_t));
        char* name = (char *) (shstr -> sh_addr + VIRT_BASE + shdr -> sh_name);
        if((name[0] == 0) && (strcmp(".null", sname) == 0))
            return shdr;
        else if(strcmp(name, sname) == 0)
            return shdr;
    }

    return 0;
}

void mb_elf_print_sections()
{
    multiboot_info_t* mbt = __GGMBT__;

    multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbt -> u.elf_sec);
    uint32_t addr  = multiboot_elf_sec -> addr;
    uint32_t num   = multiboot_elf_sec -> num;
    uint32_t shndx = multiboot_elf_sec -> shndx;
    elf_sheader_t* shstr = (elf_sheader_t *) (addr + shndx * sizeof(elf_sheader_t));
    for(uint32_t i = 0; i < num; i++)
    {
        elf_sheader_t* shdr = (elf_sheader_t *) (addr + i * sizeof(elf_sheader_t));
        if(((char *) (shstr -> sh_addr + VIRT_BASE + shdr -> sh_name))[0] == 0)
            kprintf("[.null]\n");
        else
            kprintf("[%s]\n", (void *) (shstr -> sh_addr + VIRT_BASE + shdr -> sh_name));
    }
}
