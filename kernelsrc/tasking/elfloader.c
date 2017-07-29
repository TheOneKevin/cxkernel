#include "tasking/elfloader.h"
#include "system/kprintf.h"

bool elf_check_header(elf32_header_t* head)
{
    if(!head) return false;
    if(head -> e_ident[EI_MAG0] != ELFMAG0)
    {
        kprintf("ELF Header EI_MAG0 incorrect.\n");
        return false;
    }
    if(head -> e_ident[EI_MAG1] != ELFMAG1)
    {
        kprintf("ELF Header EI_MAG1 incorrect.\n");
        return false;
    }
    if(head -> e_ident[EI_MAG2] != ELFMAG2)
    {
        kprintf("ELF Header EI_MAG2 incorrect.\n");
        return false;
    }
    if(head -> e_ident[EI_MAG3] != ELFMAG3)
    {
        kprintf("ELF Header EI_MAG3 incorrect.\n");
        return false;
    }
    return true;
}

bool elf_check_supported(elf32_header_t *head)
{
    if(!elf_check_header(head))
    {
        kprintf("Invalid ELF File.\n");
        return false;
    }
    if(head -> e_ident[EI_CLASS] != ELFCLASS32)
    {
        kprintf("Unsupported ELF File Class.\n");
        return false;
    }
    if(head -> e_ident[EI_DATA] != ELFDATA2LSB)
    {
        kprintf("Unsupported ELF File byte order.\n");
        return false;
    }
    if(head -> e_machine != EM_386)
    {
        kprintf("Unsupported ELF File target.\n");
        return false;
    }
    if(head -> e_ident[EI_VERSION] != EV_CURRENT)
    {
        kprintf("Unsupported ELF File version.\n");
        return false;
    }
    if(head -> e_type != ELF_REL && head -> e_type != ELF_EXEC)
    {
        kprintf("Unsupported ELF File type: %X.\n", head -> e_type);
        return false;
    }
    return true;
}

void *elf_load_file(void *file)
{
    elf32_header_t *hdr = (elf32_header_t *)file;

    if(!elf_check_supported(hdr))
    {
        kprintf("ELF File cannot be loaded.\n");
        return NULL;
    }

    switch(hdr -> e_type)
    {
        case ELF_EXEC:
            kprintf("Loaded elf executable file at %X with %u sections @ %X\n", hdr -> e_entry, hdr -> e_shnum, hdr -> e_shoff);
            elf32_sheader_t* shstr = (elf32_sheader_t *) (hdr -> e_shoff + (uintptr_t) file + hdr -> e_shstrndx * 0x28);

            for(uint32_t i = 0; i < hdr -> e_shnum; i ++)
            {
                elf32_sheader_t* shdr = (elf32_sheader_t *) (hdr -> e_shoff + (uintptr_t) file + i * 0x28);

                if(((char *)(shstr -> sh_offset + (uintptr_t) file + shdr -> sh_name))[0] == 0)
                    kprintf("[.null %X] ", shdr -> sh_offset);
                else
                    kprintf("[%s %X]", (void *)(shstr -> sh_offset + (uintptr_t) file + shdr -> sh_name), shdr -> sh_offset);
            }

            kprintf("\n");

            elf32_sheader_t* shdr1 = (elf32_sheader_t *) (hdr -> e_shoff + (uintptr_t) file + 1 * 0x28);
            void (*foo)(void) = (uintptr_t) file + shdr1 -> sh_offset;
            foo();

            return NULL;
        case ELF_REL:
            kprintf("Loaded elf relocatable file.\n");
            return NULL;
        case ELF_NONE:
            kprintf("Loaded elf other file.\n");
            return NULL;
    }

    kprintf("ELF File cannot be loaded.\n");
    return NULL;
}
