/**
 * @file   serial.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 27th 2018, 11:46:27 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T11:49:40-04:00
 */
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

void pc_serial_init(uint16_t);
char pc_read_serial(uint16_t);
void pc_write_serial(uint16_t, char);

#ifdef __cplusplus
}
#endif
