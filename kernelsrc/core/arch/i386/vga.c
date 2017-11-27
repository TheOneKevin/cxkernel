/*
 * Filename: console.c
 * Author:   Kevin Dai
 * Email:    kevindai02@outlook.com
 *
 * Created on 29-Jul-2017 05:22:49 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 07:32:28 PM
*/

#include "lib/llio.h"
#include "lib/string.h"
#include "arch/i386/common.h"
#include "arch/i386/sys/vga.h"

static inline uint8_t vga_entry_color()
{
    return (_ttys[_current_tty_id] -> console -> foreground & 0x0F) | (_ttys[_current_tty_id] -> console -> background << 4);
}

static inline uint16_t vga_entry(unsigned char c)
{
    return (uint16_t) c | ((uint16_t) vga_entry_color() << 8);
}

static void move_cursor()
{
    uint16_t position = (_ttys[_current_tty_id] -> console -> _y * _ttys[_current_tty_id] -> console -> width) + _ttys[_current_tty_id] -> console -> _x;
    outb(0x3D4, 0x0F);            // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, position);        // Send the low cursor byte.
    outb(0x3D4, 0x0E);            // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, position >> 8);   // Send the high cursor byte.
}

static void terminal_scroll()
{
    // We only want to calculate this once
    uint16_t blank = vga_entry(' ');
    console_t* cons = _ttys[_current_tty_id] -> console;
    uint16_t* terminal_buffer = (uint16_t *) cons -> VRAM; // We need to cast this.
    // Row 25 is the end, this means we need to scroll up
    if (cons -> _y >= cons -> height)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0; i < (cons -> height - 1) * cons -> width; i++)
            terminal_buffer[i] = terminal_buffer[i + cons -> width];

        // The last line should now be blanked out
        for (i = (cons -> height - 1) * cons -> width; i < cons -> height * cons -> width; i++)
            terminal_buffer[i] = blank;

        // The cursor should now be on the last line.
        cons -> _y = cons -> height - 1;
        cons -> _x = 0;
    }
}

static void terminal_clear(uint8_t bg)
{
    // We only want to calculate this once
    uint16_t entry = vga_entry(' ');
    _ttys[_current_tty_id] -> console -> background = bg;
    uint16_t* terminal_buffer = (uint16_t *) _ttys[_current_tty_id] -> console -> VRAM; // Cast the void pointer

    for (int i = 0; i < _ttys[_current_tty_id] -> console -> width * _ttys[_current_tty_id] -> console -> height; i++)
        terminal_buffer[i] = entry;

    // Move the hardware cursor back to the start.
    _ttys[_current_tty_id] -> console -> _x = 0;
    _ttys[_current_tty_id] -> console -> _y = 0;
    move_cursor();
}

static void terminal_putc(const char c)
{
    // Let's get the entry we're going to write to VRAM first
    uint16_t entry = vga_entry(c);
    console_t* cons = _ttys[_current_tty_id] -> console;
    uint16_t* terminal_buffer = (uint16_t *) cons -> VRAM;
    // Backspace by decreasing the cursor x
    if (c == '\b' && cons -> _x)
        cons -> _x--;
    // Tab by setting the cursor x to the nearest divisible by 8 location
    else if (c == '\t')
        cons -> _x = (cons -> _x + 8) & ~(8 - 1);
    // Carriage return
    else if (c == '\r')
        cons -> _x = 0;
    // Newline
    else if (c == '\n')
    {
        cons -> _x = 0; cons -> _y++;
    }
    // Handle any other characters
    else if (c >= ' ')
    {
        // Get the ram location we're going to write to
        terminal_buffer[cons -> _y * cons -> width + cons -> _x] = entry;
        cons -> _x++;
    }
    // Have we reached the end of the line? If so, add new line
    if (cons -> _x >= cons -> width)
    {
        cons -> _x = 0; cons -> _y++;
    }

    // Scroll if needed, then move the cursor by one
    terminal_scroll();
    move_cursor();
}

void terminal_initialize(console_t* cons)
{
    cons -> width = VGA_WIDTH;
    cons -> height = VGA_HEIGHT;
    cons -> _x = 0;
    cons -> _y = 0;
    cons -> background = VGA_COLOR_BLACK;
    cons -> foreground = VGA_COLOR_WHITE;
    cons -> scroll = terminal_scroll;
    cons -> updc = move_cursor;
    cons -> putc = terminal_putc;
    cons -> clear = terminal_clear;
    cons -> isInTextmode = true;
    cons -> VRAM = (void *) (VIRT_BASE + VGA_FRAMEBUFFER);
    cons -> framebuffer = 0;

    uint16_t* terminal_buffer = (uint16_t *) cons -> VRAM;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ');
        }
    }

    move_cursor();
}
