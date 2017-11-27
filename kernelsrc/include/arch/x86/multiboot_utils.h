/*
 * File:   multiboot_utils.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 13-Aug-2017 03:10:44 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:46:22-05:00
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
