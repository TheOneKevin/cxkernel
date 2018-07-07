/**
 * @file   multiboot_utils.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:39:14-04:00
 */

#pragma once

#include "common.h"
#include "tasking/elf.h"

#ifdef __cplusplus
extern "C" {
#endif

elf_sheader_t* mb_elf_get_section_header(char* sname);
void mb_elf_print_sections(void);

#ifdef __cplusplus
}
#endif
