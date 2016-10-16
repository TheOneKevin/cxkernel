/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "display/tdisplay.h"
#include "display/textModeHooks.h"
#include "memory/kheap.h"

uint32_t debugPtr; bool doBootLog;

screeninfo_t screen; //Might add more than one console!

// Our stuff
vHookA _scroll;
vHookA _updc;
vHookB _putc;
vHookC _clear;

void console_init()
{
    // Setup screen variables
    screen._x = 0; screen._y = 0;
    screen.foreground = COLOR_WHITE;
    screen.background = COLOR_BLACK;
    console_clear(COLOR_BLACK);
    bprintok(); console_write("Console display initialized : "); console_write_hex((uint32_t)debugBuffer); console_putc('\n');
}

void console_setbg(enum text_color bg)
{
    screen.background = bg;
}

void console_setfg(enum text_color fg)
{
    screen.foreground = fg;
}

void set_cursorpos(uint8_t xpos, uint8_t ypos)
{
    screen._x = xpos; screen._y = ypos;
    _updc();
}

void console_clear(enum text_color bg)
{
    _clear(bg);
}

void console_putc(const char c)
{
    _putc(c, false);
    //Write our entire boot sequence into RAM
    if(debugPtr < 4096 && doBootLog) { debugBuffer[debugPtr] = c; debugPtr++; } //Failsafe!
}

void console_putck(const char c)
{
    _putc(c, true);
}

void console_write(const char *c)
{
    int i = 0;
    while(c[i])
    {
        console_putc(c[i++]);
    }
}

void console_writeline(const char *c)
{
    console_write(c); console_putc('\n');
}

// From James Molloy's Tutorial :P
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

void console_print_center(const char *c)
{
    size_t length = (80 - strlen(c)) / 2;
    while(length > 0)
    {
        console_putc(' ');
        length--;
    }
    console_write(c);
}

// Just for boot because I'm lazy ;)
void bprintok()
{
    console_putc('[');
    console_setfg(COLOR_LIGHT_GREEN);
    console_write("  OK  ");
    console_setfg(COLOR_WHITE);
    console_write("]   ");
}

void bprinterr()
{
    console_putc('[');
    console_setfg(COLOR_RED);
    console_write(" ERRD ");
    console_setfg(COLOR_WHITE);
    console_write("]   ");
}

void bprintinfo()
{
    console_putc('[');
    console_setfg(COLOR_LIGHT_CYAN);
    console_write(" INFO ");
    console_setfg(COLOR_WHITE);
    console_write("]   ");
}

void bprintwarn()
{
    console_putc('[');
    console_setfg(COLOR_YELLOW);
    console_write(" WARN ");
    console_setfg(COLOR_WHITE);
    console_write("]   ");
}