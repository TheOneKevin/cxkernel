#include "display/tdisplay.h"
#include "display/vesaModeHooks.h"

#include "drivers/vesa.h"

screeninfo_t screen;
vscreen_t vhscreen;
font_t currentfont;
uint32_t* vcache;

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
    0x002BEDFF,
    0x00FF5555,
    0x00FF55FF,
    0x00FFFF00,
    0x00FFFFFF
};

static void iupdatecur()
{
    //Nothing to put here yet :P
}

uint32_t getAddrOffset(uint32_t x, uint32_t y)
{
    return vhscreen.width * y + x;
}

static void iscroll()
{
    if(screen._y > currentfont.charY)
    {
        //Shift everything up by one
        for(uint32_t y = 0; y <= vhscreen.height; y++)
        {
            for(uint32_t x = 0; x <= vhscreen.width; x++)
                setPixel(x, y, *(getAddrOffset(x, y + currentfont.characterHeight) + vcache));
        }

        for(uint32_t y = vhscreen.height; y >= vhscreen.height - currentfont.characterHeight; y--)
        {
            for(uint32_t x = 0; x <= vhscreen.width; x++)
                setPixel(x, y, colours[screen.background]);
        }

        // The cursor should now be on the last line.
        screen._y = currentfont.charY;
    }
}

void iputcraw2(const char c)
{
    //Detect which key is pressed, and then take appropriate action
    if(c == '\b')
    {
        if(screen._x > 0)
            screen._x --;
        else if(screen._x <= 0) //If we hit the end of a line, backspace up 1 line
        {
            screen._y --;
            screen._x = currentfont.charX - 1;
        }

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

    if(screen._x > currentfont.charX - 1)
    {
        screen._x = 0; screen._y ++;
    }

    iscroll(); //Scroll if needed
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
    currentfont.characterWidth = queryWidth(); //Change based on font
    currentfont.characterHeight = queryHeight();
    currentfont.charX = (vhscreen.width / currentfont.characterWidth) - 1; //1 character is our buffer from the screen edges
    currentfont.charY = (vhscreen.height / currentfont.characterHeight) - 1;
}
