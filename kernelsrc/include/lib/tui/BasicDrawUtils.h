/**
 * @file   BasicDrawUtils.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:44:13-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c);
void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c1, enum vga_color c2);

#ifdef __cplusplus
}
#endif
