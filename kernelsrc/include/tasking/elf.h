/*
 * File:   elf.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 12-Aug-2017 11:25 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-23T19:10:44-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef uint16_t ELF_HALF_t;
typedef uint32_t ELF_OFF_t;
typedef uint32_t ELF_ADDR_t;
typedef uint32_t ELF_WORD_t;
typedef uint32_t ELF_SWORD_t;

#define ELF_NIDENT     0xF
#define ELFMAG0        0x7F // e_ident[EI_MAG0]
#define ELFMAG1        'E'  // e_ident[EI_MAG1]
#define ELFMAG2        'L'  // e_ident[EI_MAG2]
#define ELFMAG3        'F'  // e_ident[EI_MAG3]

#define ELFDATA2LSB    (1)  // Little Endian
#define ELFCLASS32     (1)  // 32-bit Architecture

#define EM_386         (3)  // x86 Machine Type
#define EV_CURRENT     (1)  // ELF Current Version

// Here are all our data structures
typedef struct
{
    uint8_t  e_ident[ELF_NIDENT];
    ELF_HALF_t e_type;
    ELF_HALF_t e_machine;
    ELF_WORD_t e_version;
    ELF_ADDR_t e_entry;
    ELF_OFF_t  e_phoff;
    ELF_OFF_t  e_shoff;
    ELF_WORD_t e_flags;
    ELF_HALF_t e_ehsize;
    ELF_HALF_t e_phentsize;
    ELF_HALF_t e_phnum;
    ELF_HALF_t e_shentsize;
    ELF_HALF_t e_shnum;
    ELF_HALF_t e_shstrndx;
} elf_header_t;

typedef struct
{
    ELF_WORD_t sh_name;
    ELF_WORD_t sh_type;
    ELF_WORD_t sh_flags;
    ELF_ADDR_t sh_addr;
    ELF_OFF_t  sh_offset;
    ELF_WORD_t sh_size;
    ELF_WORD_t sh_link;
    ELF_WORD_t sh_info;
    ELF_WORD_t sh_addralign;
    ELF_WORD_t sh_entsize;
} elf_sheader_t;

typedef struct
{
    ELF_WORD_t st_name;
    ELF_ADDR_t st_value;
    ELF_WORD_t st_size;
    uint8_t    st_info;
    uint8_t    st_other;
    ELF_HALF_t sh_shndx;
} elf_sym_t;

enum ELF_Ident
{
    EI_MAG0         = 0, // 0x7F
    EI_MAG1         = 1, // 'E'
    EI_MAG2         = 2, // 'L'
    EI_MAG3         = 3, // 'F'
    EI_CLASS        = 4, // Architecture (32/64)
    EI_DATA         = 5, // Byte Order
    EI_VERSION      = 6, // ELF Version
    EI_OSABI        = 7, // OS Specific
    EI_ABIVERSION   = 8, // OS Specific
    EI_PAD          = 9  // Padding
};

enum ELF_Types
{
    ELF_NONE        = 0, // Unkown Type
    ELF_REL         = 1, // Relocatable File
    ELF_EXEC        = 2  // Executable File
};

#ifdef __cplusplus
    }
#endif
