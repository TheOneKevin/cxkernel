/*
 * File:   llio.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 10:54:04 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-03-19T14:31:10-04:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void io_wait(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);

uint32_t read_cr0(void);
uint32_t read_cr2(void);
uint32_t read_cr3(void);
uint32_t read_cr4(void);

void write_cr0(uint32_t);
void write_cr2(uint32_t);
void write_cr3(uint32_t);
void write_cr4(uint32_t);

#ifdef __cplusplus
}
#endif
