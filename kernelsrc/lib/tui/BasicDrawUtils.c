/*
 * File:   BasicDrawUtils.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 08:12:18 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-02-26T14:02:41-05:00
*/

#include "arch/x86/vga.h"
#include "arch/x86/arch_common.h"
#include "lib/tui/BasicDrawUtils.h"

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t) c | (uint16_t) color << 8;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER);
    terminal_buffer[index] = vga_entry(c, color);
}

void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c)
{
    uint16_t entry = vga_entry(' ', vga_entry_color(c, c));
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER);
    uint8_t x = x1, y = y1;
    for(y = y1; y < y2; y++)
    {
        x = x1;
        uint16_t index = y * VGA_WIDTH + x;
        for(x = x1; x < x2; x++)
        {
            terminal_buffer[index] = entry;
            index++;
        }
    }
}

void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, enum vga_color c1, enum vga_color c2)
{
    uint16_t index1 = y1 * VGA_WIDTH + x1;
    uint16_t index2 = y1 * VGA_WIDTH + x2;
    uint16_t entry1 = vga_entry(179, vga_entry_color(c1, c2));
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER);
    for(uint8_t y = y1 + 1; y < y2; y++)
    {
        index1 += VGA_WIDTH;
        index2 += VGA_WIDTH;
        terminal_buffer[index1] = entry1;
        terminal_buffer[index2] = entry1;
    }

    index1 = y1 * VGA_WIDTH + x1;
    index2 = y2 * VGA_WIDTH + x1;
    entry1 = vga_entry(196, vga_entry_color(c1, c2));

    for(uint8_t x = x1; x < x2; x++)
    {
        index1 ++;
        index2 ++;
        terminal_buffer[index1] = entry1;
        terminal_buffer[index2] = entry1;
    }

    terminal_putentryat(218, vga_entry_color(c1, c2), x1, y1);
    terminal_putentryat(192, vga_entry_color(c1, c2), x1, y2);
    terminal_putentryat(191, vga_entry_color(c1, c2), x2, y1);
    terminal_putentryat(217, vga_entry_color(c1, c2), x2, y2);
}
