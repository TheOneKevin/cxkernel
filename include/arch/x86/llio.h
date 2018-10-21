/**
 * @file   llio.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 5:44:15 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T08:47:44-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void io_wait(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);

arch_sz_t read_cr0(void);
arch_sz_t read_cr2(void);
arch_sz_t read_cr3(void);
arch_sz_t read_cr4(void);

void write_cr0(arch_sz_t);
void write_cr2(arch_sz_t);
void write_cr3(arch_sz_t);
void write_cr4(arch_sz_t);

#ifdef __cplusplus
}
#endif
