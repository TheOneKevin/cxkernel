/*
 * File:   vga.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 05:22:40 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-08-26T14:32:36-04:00
*/

#pragma once

#include "common.h"
#include "core/console.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_FRAMEBUFFER 0xB8000

#ifdef __cplusplus
extern "C" {
#endif

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

void terminal_initialize(console_t* cons);

#ifdef __cplusplus
}
#endif
