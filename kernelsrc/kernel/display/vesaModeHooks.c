/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "display/tdisplay.h"
#include "display/vesaModeHooks.h"

#include "drivers/vesa.h"

screeninfo_t screen;
vscreen_t vhscreen;
font_t currentfont;

vHookA _scroll;
vHookA _updc;
vHookB _putc;
vHookC _clear;

uint32_t colours [16] = {
    0x00000000,
    0x000000FF,
    0x0000FF00,
    0x0000FFFF,
    0x00FF0000,
    0x00FF00FF,
    0x00550000,
    0x00C0C0C0,
    0x00909090,
    0x000055AA,
    0x0000FF55,
    0x0000FFAA,
    0x00FF5555,
    0x00FF55FF,
    0x00FFFF00,
    0x00FFFFFF
};

static void iupdatecur()
{
    //Nothing to put here :P
}

static void iscroll()
{
    
}

void iputcraw2(const char c, bool isKey)
{
    if(c == '\b' && screen._x)
    {
        screen._x --;
    }
    else if(c == '\t')
    {
        screen._x = (screen._x + 8) & ~(8 - 1);
    }
    else if(c == '\r')
    {
        screen._x = 0;
    }
    else if(c == '\n')
    {
        screen._x = 0; screen._y ++;
    }
    else if(c >= ' ')
    {
        printChar(screen._x, screen._y, colours[screen.foreground], colours[screen.background], false, c);
        screen._x ++;
    }
    
    if(screen._x >= currentfont.charX)
    {
        screen._x = 0; screen._y ++;
    }
    
    if(!isKey)
    {
        screen._lx = screen._x; screen._ly = screen._y;
    }
    iscroll();
    iupdatecur();
}

void iconsclear2(enum text_color c)
{
    clearScreen(colours[c]);
}

void _iinitVesaConsole()
{
    // These 4 hooks are the important ones. If you want to add more
    // hooks for new video modes, you only need to implement the 4 following functions
    _putc   = &iputcraw2; // Put a character on screen
    _updc   = &iupdatecur; // Update the cursor to the current x, y coordinates
    _clear  = &iconsclear2; // Clear the screen to a colour
    _scroll = &iscroll; // Scroll the screen when needed
    // Setup font
    currentfont.characterWidth = 8; //Change to your preference
    currentfont.characterHeight = 8;
    currentfont.charX = vhscreen.width / currentfont.characterWidth;
    currentfont.charY = vhscreen.height / currentfont.characterHeight;
}