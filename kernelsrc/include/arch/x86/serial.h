/*
 * File:   serial.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-27T13:41:38-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:48:16-05:00
*/

#pragma once

#include "common.h"

#define COM1 0x3F8

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
