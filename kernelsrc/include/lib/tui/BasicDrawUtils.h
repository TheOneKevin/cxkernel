/*
 * File:   BasicDrawUtils.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 08:13:38 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 30-Jul-2017 08:14:05 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c);
void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c1, enum vga_color c2);

#ifdef __cplusplus
}
#endif

// C++ code goes here
