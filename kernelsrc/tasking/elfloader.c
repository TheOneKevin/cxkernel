#include "tasking/elfloader.h"
#include "system/kprintf.h"

bool elf_check_header(ELF32_Header* head)
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

bool elf_check_supported(ELF32_Header *head)
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
		kprintf("Unsupported ELF File type.\n");
		return false;
	}
	return true;
}
