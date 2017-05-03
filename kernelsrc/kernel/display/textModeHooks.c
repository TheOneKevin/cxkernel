// Fancy shit

#include "display/tdisplay.h"
#include "display/textModeHooks.h"

uint16_t *vram = (uint16_t *)0xC03FF000; //Pointer to the VGA frame buffer
screeninfo_t screen;

vHookA _scroll;
vHookA _updc;
vHookB _putc;
vHookC _clear;

uint8_t get_attrib()
{
    return (screen.background << 4) | (screen.foreground & 0x0F);
}

uint16_t get_entry(char c, uint8_t attrib)
{
    uint16_t c16 = c; uint16_t attrib16 = attrib;
    return c16 | attrib16 << 8;
}

//For internal use only
static void iupdatecur()
{
    uint16_t loc = screen._y * 80 + screen._x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, loc >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, loc);      // Send the low cursor byte.
}

static void iscroll()
{
    // Get a space character with the default colour attributes.
    uint16_t blank = get_entry(' ', get_attrib());

    // Row 25 is the end, this means we need to scroll up
    if(screen._y >= 25)
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
        screen._y = 24;
    }
}

void iconsclear1(enum text_color bg)
{
    screen.background = bg;
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
    screen._x = 0;
    screen._y = 0;
    iupdatecur();
}

void iputcraw1(const char c, bool isKey)
{
    //Let's get the entry we're going to write to RAM first
    uint8_t attrib = get_attrib();
    uint16_t entry = get_entry(c, attrib);
    uint16_t *location;
    // Backspace by decreasing the cursor x
    if( c == '\b' && screen._x)
    {
        screen._x --;
    }
    // Tab by setting the cursor x to the nearest divisible by 8 location
    else if(c == '\t')
    {
        screen._x = (screen._x + 8) & ~(8 - 1);
    }
    // Carriage return
    else if(c == '\r')
    {
        screen._x = 0;
    }
    // Newline
    else if(c == '\n')
    {
        screen._x = 0; screen._y++;
    }
    // Handle any other characters
    else if(c >= ' ')
    {
        // Get the ram location we're going to write to
        location = vram + (screen._y * 80 + screen._x);
        *location = entry;
        screen._x++;
    }
    // Have we reached the end of the line? If so, add new line
    if(screen._x >= 80)
    {
        screen._x = 0; screen._y++;
    }
    if(!isKey)
    {
        screen._lx = screen._x; screen._ly = screen._y; //Make sure keyboard cannot backspace printed text
    }
    // Scroll if needed, then move the cursor by one
    iscroll();
    iupdatecur();
}

void _iinitNormalConsole()
{
    // These 4 hooks are the important ones. If you want to add more
    // hooks for new video modes, you only need to implement the 4 following functions
    _putc   = &iputcraw1; // Put a character on screen
    _updc   = &iupdatecur; // Update the cursor to the current x, y coordinates
    _clear  = &iconsclear1; // Clear the screen to a colour
    _scroll = &iscroll; // Scroll the screen when needed
}
