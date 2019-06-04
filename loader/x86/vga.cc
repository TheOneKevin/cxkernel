/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   vga.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 28 2018, 3:10 PM
 *
 * Here, we're going to define some of the console_* functions.
 * Since we want a minimally functioning system here, we can replace
 * getc() and puts() with dummy functions.
 */

#include "platform/console.h"
#include "arch/x86/llio.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

extern "C" {
void console_log(char* c)
{
    while(*c)
    {
        while((inb(0x3FD) & 0x20) == 0);
        outb(0x3F8, *c++);
    }
}
void console_puts(char* c)
{ while(*c) console_emit(*c++); }
char console_getc(void)
{ return 0; }

/* Hardware text mode color constants. */
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

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_FRAME_BASE 0xB8000

static uint8_t fg_color = VGA_COLOR_WHITE;
static uint8_t bg_color = VGA_COLOR_BLACK;
static uint8_t _x = 0;
static uint8_t _y = 0;

static inline uint8_t vga_entry_color(void) { return (uint8_t) ((fg_color & 0x0F) | (bg_color << 4)); }
static inline uint16_t vga_entry(unsigned char c) { return (uint16_t) c | ((uint16_t) vga_entry_color() << 8); }

void console_init(void)
{
    _x = 0, _y = 0;
    uint16_t* terminal_buffer = (uint16_t*) VGA_FRAME_BASE;
    for(size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for(size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ');
        }
    }

    // Serial
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x80);
    outb(0x3F8, 0x03);
    outb(0x3F9, 0x00);
    outb(0x3FB, 0x03);
    outb(0x3FA, 0xC7);
    outb(0x3FC, 0x0B);
}
EXPORT_CTOR(console_init, 102);

void console_upd(void)
{
    uint8_t position = (uint8_t) ((_y * VGA_WIDTH) + _x);
    outb(0x3D4, 0x0F);            // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, position);        // Send the low cursor byte.
    outb(0x3D4, 0x0E);            // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, position >> 8);   // Send the high cursor byte.
}

void console_emit(const char c)
{
    uint16_t entry = vga_entry(c);
    uint16_t* terminal_buffer = (uint16_t*) VGA_FRAME_BASE;
    if(c == '\b' && _x) _x--;
    else if(c == '\t') _x = (uint8_t) ((_x + 8) & ~(8 - 1));
    else if(c == '\r') _x = 0;
    else if(c == '\n') _x = 0, _y++;
    else if(c >= ' ') terminal_buffer[_y * VGA_WIDTH + _x++] = entry;
    if(_x >= VGA_WIDTH)
        _x = 0, _y++;
    console_upd();
}
}
