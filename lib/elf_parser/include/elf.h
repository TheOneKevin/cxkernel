/**
 * @file   elf.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Sunday, November 11th 2018, 12:46:45 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T16:45:41-05:00
 *
 * See http://www.sco.com/developers/gabi/latest/contents.html for System-V ABI Specs.
 * If anyone wants to comment the structures fully based on the specifications, feel free to PR.
 * I gave up halfway commenting the structures :(
 * 
 */

#pragma once
#include "common.h"

typedef uint32_t elf32_addr_t;
typedef uint32_t elf32_off_t;
typedef uint16_t elf32_half_t;
typedef uint32_t elf32_word_t;
typedef uint32_t elf32_sword_t;

typedef uint64_t elf64_addr_t;
typedef uint64_t elf64_off_t;
typedef uint16_t elf64_half_t;
typedef uint32_t elf64_word_t;
typedef uint32_t elf64_sword_t;
typedef uint64_t elf64_xword_t;
typedef uint64_t elf64_sxword_t;

enum ELF_Ident
{
    EI_MAG0         = 0, //!< 0x7F
    EI_MAG1         = 1, //!< 'E'
    EI_MAG2         = 2, //!< 'L'
    EI_MAG3         = 3, //!< 'F'
    EI_CLASS        = 4, //!< Architecture (32/64)
    EI_DATA         = 5, //!< Byte Order
    EI_VERSION      = 6, //!< ELF Version
    EI_OSABI        = 7, //!< OS Specific
    EI_ABIVERSION   = 8, //!< OS Specific
    EI_PAD          = 9  //!< Padding
};

enum ELF_Types
{
    ELF_NONE        = 0, //!< Unkown Type
    ELF_REL         = 1, //!< Relocatable File
    ELF_EXEC        = 2  //!< Executable File
};

#define EI_NIDENT   16  //!< Size of e_ident[]

typedef struct {
#define ELFMAG0     0x7f    //!< e_ident[EI_MAG0]
#define ELFMAG1     'E'     //!< e_ident[EI_MAG1]
#define ELFMAG2     'L'     //!< e_ident[EI_MAG2]
#define ELFMAG3     'F'     //!< e_ident[EI_MAG3]
#define ELFCLASSNONE 0      //!< Invalid class
#define ELFCLASS32   1      //!< 32-bit objects
#define ELFCLASS64   2      //!< 64-bit objects
#define ELFDATANONE  0      //!< Invalid data encoding
#define ELFDATA2LSB  1      //!< Reserved; will be assigned to new encodings as necessary.
#define ELFDATA2MSB  2      //!< Reserved; will be assigned to new encodings as necessary.
    unsigned char   e_ident[EI_NIDENT];
#define ET_NONE     0       //!< No file type
#define ET_REL      1       //!< Relocatable file
#define ET_EXEC     2       //!< Executable file
#define ET_DYN      3       //!< Shared object file
#define ET_CORE     4       //!< Core file
#define ET_LOOS     0xfe00  //!< Operating system-specific
#define ET_HIOS     0xfeff  //!< Operating system-specific
#define ET_LOPROC   0xff00  //!< Processor-specific
#define ET_HIPROC   0xffff  //!< Processor-specific
    elf32_half_t    e_type;     //!< This member identifies the object file type.
    elf32_half_t    e_machine;  //!< This member's value specifies the required architecture for an individual file.
#define EV_NONE     0           //!< Invalid version
#define EV_CURRENT  1           //!< Current version
    elf32_word_t    e_version;  //!< Identifies the object file version.
    elf32_addr_t    e_entry;    //!< Virtual address of program entry
    elf32_off_t     e_phoff;    //!< Program header offset in bytes
    elf32_off_t     e_shoff;    //!< Section header offset in bytes
    elf32_word_t    e_flags;    //!< Some flags
    elf32_half_t    e_ehsize;   //! ELF header's size in bytes.
    elf32_half_t    e_phentsize;
    elf32_half_t    e_phnum;
    elf32_half_t    e_shentsize;
    elf32_half_t    e_shnum;
    elf32_half_t    e_shstrndx;
} elf32_ehdr_t;

typedef struct {
#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC  2
#define PT_INTERP   3
#define PT_NOTE     4
#define PT_SHLIB    5
#define PT_PHDR     6
#define PT_TLS      7
#define PT_LOOS     0x60000000
#define PT_HIOS     0x6fffffff
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7fffffff
    elf32_word_t  p_type;
    elf32_off_t   p_offset;
    elf32_addr_t  p_vaddr;
    elf32_addr_t  p_paddr;
    elf32_word_t  p_filesz;
    elf32_word_t  p_memsz;
#define PF_X    0x1 //!< Execute
#define PF_W    0x2 //!< Write
#define PF_R    0x4 //!< Read
    elf32_word_t  p_flags;
    elf32_word_t  p_align;
} elf32_phdr_t;

typedef struct {
    elf32_word_t  sh_name;
#define SHT_NULL            0
#define SHT_PROGBITS        1
#define SHT_SYMTAB          2
#define SHT_STRTAB          3
#define SHT_RELA            4
#define SHT_HASH            5
#define SHT_DYNAMIC         6
#define SHT_NOTE            7
#define SHT_NOBITS          8
#define SHT_REL             9
#define SHT_SHLIB           10
#define SHT_DYNSYM          11
#define SHT_INIT_ARRAY      14
#define SHT_FINI_ARRAY      15
#define SHT_PREINIT_ARRAY   16
#define SHT_GROUP           17
#define SHT_SYMTAB_SHNDX    18
    elf32_word_t  sh_type;
#define SHF_WRITE           0x1
#define SHF_ALLOC           0x2
#define SHF_EXECINSTR       0x4
#define SHF_MERGE           0x10
#define SHF_STRINGS         0x20
#define SHF_INFO_LINK       0x40
#define SHF_LINK_ORDER      0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP           0x200
#define SHF_TLS             0x400
#define SHF_COMPRESSED      0x800
#define SHF_MASKOS          0x0ff00000
#define SHF_MASKPROC        0xf0000000
    elf32_word_t  sh_flags;
    elf32_addr_t  sh_addr;
    elf32_off_t   sh_offset;
    elf32_word_t  sh_size;
    elf32_word_t  sh_link;
    elf32_word_t  sh_info;
    elf32_word_t  sh_addralign;
    elf32_word_t  sh_entsize;
} elf32_shdr_t;

typedef struct {
//! The section data is compressed with the ZLIB algoritm.
//! Additional documentation for ZLIB may be found at http://zlib.net.
#define ELFCOMPRESS_ZLIB    1
    elf32_word_t  ch_type;        //!< This member specifies the compression algorithm.
    elf32_word_t  ch_size;        //!< This member provides the size in bytes of the uncompressed data. See sh_size.
    elf32_word_t  ch_addralign;   //!< Specifies the required alignment for the uncompressed data. See sh_addralign.
} elf32_chdr_t;

typedef struct {
    elf32_word_t    st_name;
    elf32_addr_t    st_value;
    elf32_word_t    st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    elf32_half_t    st_shndx;
} elf32_sym_t;

typedef struct {
    unsigned char     e_ident[EI_NIDENT];
    elf64_half_t      e_type;
    elf64_half_t      e_machine;
    elf64_word_t      e_version;
    elf64_addr_t      e_entry;
    elf64_off_t       e_phoff;
    elf64_off_t       e_shoff;
    elf64_word_t      e_flags;
    elf64_half_t      e_ehsize;
    elf64_half_t      e_phentsize;
    elf64_half_t      e_phnum;
    elf64_half_t      e_shentsize;
    elf64_half_t      e_shnum;
    elf64_half_t      e_shstrndx;
} elf64_ehdr_t;

typedef struct {
    elf64_word_t  p_type;
    elf64_word_t  p_flags;
    elf64_off_t   p_offset;
    elf64_addr_t  p_vaddr;
    elf64_addr_t  p_paddr;
    elf64_xword_t p_filesz;
    elf64_xword_t p_memsz;
    elf64_xword_t p_align;
} elef64_phdr_t;

typedef struct {
    elf64_word_t  sh_name;
    elf64_word_t  sh_type;
    elf64_xword_t sh_flags;
    elf64_addr_t  sh_addr;
    elf64_off_t   sh_offset;
    elf64_xword_t sh_size;
    elf64_word_t  sh_link;
    elf64_word_t  sh_info;
    elf64_xword_t sh_addralign;
    elf64_xword_t sh_entsize;
} elf64_shdr_t;

typedef struct {
    elf64_word_t  ch_type;
    elf64_word_t  ch_reserved;
    elf64_xword_t ch_size;
    elf64_xword_t ch_addralign;
} elf64_chdr_t;

typedef struct {
    elf64_word_t    st_name;
    unsigned char   st_info;
    unsigned char   st_other;
    elf64_half_t    st_shndx;
    elf64_addr_t    st_value;
    elf64_xword_t   st_size;
} elf64_sym_t;
