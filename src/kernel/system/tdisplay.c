/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/tdisplay.h"

enum text_color background = COLOR_BLACK;
enum text_color foreground = COLOR_WHITE;

uint8_t x = 0; uint8_t y = 0; //Our current cursor positions
uint16_t *vram = (uint16_t *)0xB8000; //Pointer to the VGA frame buffer

void console_setbg(enum text_color bg)
{
    background = bg;
}

void console_setfg(enum text_color fg)
{
    foreground = fg;
}

uint8_t get_attrib()
{
    return (background << 4) | (foreground & 0x0F);
}

uint16_t get_entry(char c, uint8_t attrib)
{
    uint16_t c16 = c; uint16_t attrib16 = attrib;
    return c16 | attrib16 << 8;
}

//For internal use only
static void move_cursor()
{
    uint16_t loc = y * 80 + x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, loc >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, loc);      // Send the low cursor byte.
}

//Sets the cursor position to the x position and y position specified
void set_cursorpos(uint8_t xpos, uint8_t ypos)
{
    x = xpos; y = ypos; move_cursor();
}

static void scroll()
{
    // Get a space character with the default colour attributes.
    uint16_t blank = get_entry(' ', get_attrib());

    // Row 25 is the end, this means we need to scroll up
    if(y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            vram[i] = vram[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            vram[i] = blank;
        }
        // The cursor should now be on the last line.
        y = 24;
    }
}

void console_clear(enum text_color bg)
{
    background = bg;
    // Make an attribute byte for the default colours
    // We are not going to use the get_attrib() as we have a custom fg
    uint8_t attrib = get_attrib();
    uint16_t entry = get_entry(' ', attrib);

    int i;
    for (i = 0; i < 80*25; i++)
    {
        vram[i] = entry;
    }

    // Move the hardware cursor back to the start.
    x = 0;
    y = 0;
    move_cursor();
}

void console_putc(char c)
{
    //Let's get the entry we're going to write to RAM first
    uint8_t attrib = get_attrib();
    uint16_t entry = get_entry(c, attrib);
    uint16_t *location;
    // Backspace by decreasing the cursor x
    if( c == 0x08 && x)
    {
        x --;
    }
    // Tab by setting the cursor x to the nearest divisible by 8 location
    else if(c == 0x09)
    {
        x = (x+8) & ~(8-1);
    }
    // Carriage return
    else if(c == '\r')
    {
        x = 0;
    }
    // Newline
    else if(c == '\n')
    {
        x = 0; y++;
    }
    // Handle any other characters
    else if(c >= ' ')
    {
        // Get the ram location we're going to write to
        location = vram + (y * 80 + x);
        *location = entry;
        x++;
    }
    // Have we reached the end of the line? If so, add new line
    if(x >= 80)
    {
        x = 0; y++;
    }
    
    // Scroll if needed, then move the cursor by one
    scroll();
    move_cursor();
}

void console_write(char *c)
{
    int i = 0;
    while(c[i])
    {
        console_putc(c[i++]);
    }
}

void console_writeline(char *c)
{
    console_write(c); console_putc('\n');
}

// From James Molly's Tutorial
void console_write_hex(uint32_t n)
{
    int tmp;
    console_write("0x");
    char noZeroes = 1;
    
    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            console_putc(tmp - 0xA + 'a');
        }
        else
        {
            noZeroes = 0;
            console_putc(tmp + '0');
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        console_putc(tmp - 0xA + 'a');
    }
    else
    {
        console_putc( tmp + '0');
    }
}

void console_write_dec(uint32_t n)
{
    if (n == 0)
    {
        console_putc('0');
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    console_write(c2);
}

void console_print_center(char *c)
{
    size_t length = (80 - strlen(c)) / 2;
    while(length > 0)
    {
        console_putc(' ');
        length--;
    }
    console_write(c);
}