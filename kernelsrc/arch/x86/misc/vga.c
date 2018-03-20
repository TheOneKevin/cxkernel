/*
 * File:   vga.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 05:22:49 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2018-02-26T14:06:41-05:00
*/

#include "arch/x86/llio.h"
#include "lib/string.h"
#include "core/console.h"
#include "arch/x86/arch_common.h"
#include "arch/x86/vga.h"

static uint8_t fg_color = VGA_COLOR_WHITE;
static uint8_t bg_color = VGA_COLOR_BLACK;
static uint8_t _x = 0;
static uint8_t _y = 0;

static inline uint8_t vga_entry_color(void)
{
    return (fg_color & 0x0F) | (bg_color << 4);
}

static inline uint16_t vga_entry(unsigned char c)
{
    return (uint16_t) c | ((uint16_t) vga_entry_color() << 8);
}

static void move_cursor(void)
{
    uint16_t position = (_y * VGA_WIDTH) + _x;
    outb(0x3D4, 0x0F);            // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, position);        // Send the low cursor byte.
    outb(0x3D4, 0x0E);            // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, position >> 8);   // Send the high cursor byte.
}

static void terminal_scroll(void)
{
    // We only want to calculate this once
    uint16_t blank = vga_entry(' ');
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER); // We need to cast this.
    // Row 25 is the end, this means we need to scroll up
    if (_y >= VGA_HEIGHT)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++)
            terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];

        // The last line should now be blanked out
        for (i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
            terminal_buffer[i] = blank;

        // The cursor should now be on the last line.
        _y = VGA_HEIGHT - 1;
        _x = 0;
    }
}

static void terminal_clear(uint8_t bg)
{
    // We only want to calculate this once
    uint16_t entry = vga_entry(' ');
    bg_color = bg;
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER); // Cast the void pointer

    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        terminal_buffer[i] = entry;

    // Move the hardware cursor back to the start.
    _y = 0;
    _y = 0;
    move_cursor();
}

static void terminal_putc(const char c)
{
    // Let's get the entry we're going to write to VRAM first
    uint16_t entry = vga_entry(c);
    uint16_t* terminal_buffer = (uint16_t *) ARCH_VIRT_PHYS(VGA_FRAMEBUFFER);
    // Backspace by decreasing the cursor x
    if (c == '\b' && _x)
        _x--;
    // Tab by setting the cursor x to the nearest divisible by 8 location
    else if (c == '\t')
        _x = (_x + 8) & ~(8 - 1);
    // Carriage return
    else if (c == '\r')
        _x = 0;
    // Newline
    else if (c == '\n')
    {
        _x = 0; _y++;
    }
    // Handle any other characters
    else if (c >= ' ')
    {
        // Get the ram location we're going to write to
        terminal_buffer[_y * VGA_WIDTH + _x] = entry;
        _x++;
    }
    // Have we reached the end of the line? If so, add new line
    if (_x >= VGA_WIDTH)
    {
        _x = 0; _y++;
    }

    // Scroll if needed, then move the cursor by one
    terminal_scroll();
    move_cursor();
}

console_t __internal_vga_cons =
{
    .putc  = terminal_putc,
    .clear = terminal_clear,
    .getc  = NULL
};
