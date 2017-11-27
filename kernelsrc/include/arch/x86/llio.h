/*
 * File:   llio.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 10:54:04 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:40:01-05:00
*/

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void io_wait(void);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t read_cr0(void);
uint32_t read_cr1(void);
uint32_t read_cr2(void);
uint32_t read_cr3(void);
uint32_t read_cr4(void);

#ifdef __cplusplus
}
#endif
