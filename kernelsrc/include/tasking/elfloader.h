/*
 * File:   elfloader.h
 * Author: Kevin
 *
 * Created on May 6, 2017, 2:24 PM
 */

#include "system.h"

#ifndef ELFLOADER_H
#define ELFLOADER_H

#ifdef __cplusplus
extern "C" {
#endif

# define ELF_NIDENT	0xF
# define ELFMAG0	0x7F // e_ident[EI_MAG0]
# define ELFMAG1	'E'  // e_ident[EI_MAG1]
# define ELFMAG2	'L'  // e_ident[EI_MAG2]
# define ELFMAG3	'F'  // e_ident[EI_MAG3]

# define ELFDATA2LSB	(1)  // Little Endian
# define ELFCLASS32	    (1)  // 32-bit Architecture

# define EM_386		(3)  // x86 Machine Type
# define EV_CURRENT	(1)  // ELF Current Version

// Here are all our data structures
typedef struct
{
    uint8_t  e_ident[ELF_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} elf32_header_t;

typedef struct
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
    uint32_t eoh;
} elf32_sheader_t;

enum ELF_Ident
{
    EI_MAG0		= 0, // 0x7F
    EI_MAG1		= 1, // 'E'
    EI_MAG2		= 2, // 'L'
    EI_MAG3		= 3, // 'F'
    EI_CLASS	= 4, // Architecture (32/64)
    EI_DATA		= 5, // Byte Order
    EI_VERSION	= 6, // ELF Version
    EI_OSABI	= 7, // OS Specific
    EI_ABIVERSION	= 8, // OS Specific
    EI_PAD		= 9  // Padding
};

enum ELF_Types
{
    ELF_NONE		= 0, // Unkown Type
    ELF_REL		    = 1, // Relocatable File
    ELF_EXEC		= 2  // Executable File
};

#ifdef __cplusplus
}
#endif

#endif /* ELFLOADER_H */
