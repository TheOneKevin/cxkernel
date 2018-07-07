/**
 * @file   serial.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:42:46-04:00
 */

#pragma once

#include "common.h"

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#ifdef __cplusplus
extern "C" {
#endif

void init_serial(uint16_t);
int serial_received(uint16_t);
char read_serial(uint16_t);
int is_transmit_empty(uint16_t);
void write_serial(uint16_t, char);
void init_serial_console(void);

#ifdef __cplusplus
}
#endif
