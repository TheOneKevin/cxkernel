/*
 * Keyboard driver handles key presses
 */

#include "drivers/keyboard.h"
#include "arch/x86/exceptions.h"
#include "arch/x86/irq.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"
#include "localization/en_scan.h"
#include "memory/kheap.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

bool CAPSF = false; // CAPS lock flag
bool NUMLF = false; // NUM lock flag
bool SCRLF = false; // Scroll lock flag

bool SFLAG = false; // Shift flag
bool ALTFL = false; // Alt flag
bool CTRLF = false; // Ctrl flag

uint8_t  lastKey  = 0;
uint8_t* keyCache = 0;
uint16_t keyLoc   = 0;

uint8_t scan_to_ascii(uint8_t key)
{
    switch(key)
    {
        //First evaluate the things we can return
        case KEY_ENTER:    return '\n';
        case KEY_DELETE:   return '\b';
        case KEY_BCKSPACE: return '\b';
        case KEY_SPACE:    return ' ';
        case KEY_TAB:      return '\t';

        default: break;
    }

    // Here, we evaluate the different combinations of shift and CAPS flag statues
    // and decide which characters we should return. If its shift and caps, we return the modified special characters i.e., !@$:"|}{
    // but we don't return modified letter i.e., qwertyuiop, however when shift is on, we return modified for everything i.e., ASD{}!)(*)
    // and finally, when CAPS is on, we return only the modified letters i.e., QWERTYUIOP
    // We return the values stored inside a char array and find the key scan -> ASCII map inside an enum table. The table works in ranges
    // see the include/localization/ folder
    if(SFLAG && CAPSF) //If both shift and CAPS are on
    {
        //Get the modified (shift) character
        if(key == DASH)        return _mdash[0];
        else if(key == EQUALS) return _mequals[0];
        else if(key == SCH) return _mforward[0];
        else if(key >= TC_S && key <= TC_E) return _mtop_chars[key - TC_S];
        else if(key >= MC_S && key <= MC_E) return _mmiddle_chars[key - MC_S];
        else if(key >= BC_S && key <= BC_E) return _mbottom_chars[key - BC_S];
        else if(key >= FIRST_NUMBER && key <= LAST_NUMBER)                 return _mnum[key - FIRST_NUMBER];
        //However we don't want the caps characters
        else if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)       return _top_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW) return _middle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW) return _bottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }
    else if(SFLAG) //If only shift is on
    {
        //Get the modified (shift) characters
        if(key == DASH)        return _mdash[0];
        else if(key == EQUALS) return _mequals[0];
        else if(key == SCH) return _mforward[0];
        else if(key >= TC_S && key <= TC_E) return _mtop_chars[key - TC_S];
        else if(key >= MC_S && key <= MC_E) return _mmiddle_chars[key - MC_S];
        else if(key >= BC_S && key <= BC_E) return _mbottom_chars[key - BC_S];
        else if(key >= FIRST_NUMBER && key <= LAST_NUMBER)                 return _mnum[key - FIRST_NUMBER];
        else if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)       return _mtop_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW) return _mmiddle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW) return _mbottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }
    else if(CAPSF) //If only CAPS is on
    {
        //Get the unmodified (shift) character
        if(key == DASH)        return _dash[0];
        else if(key == EQUALS) return _equals[0];
        else if(key == SCH) return _forward[0];
        else if(key >= TC_S && key <= TC_E) return _top_chars[key - TC_S];
        else if(key >= MC_S && key <= MC_E) return _middle_chars[key - MC_S];
        else if(key >= BC_S && key <= BC_E) return _bottom_chars[key - BC_S];
        else if(key >= FIRST_NUMBER && key <= LAST_NUMBER)                 return _num[key - FIRST_NUMBER];
        //But we want caps characters
        else if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)       return _mtop_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW) return _mmiddle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW) return _mbottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }
    else
    {
        //Get the unmodified (shift) character
        if(key == DASH)        return _dash[0];
        else if(key == EQUALS) return _equals[0];
        else if(key == SCH) return _forward[0];
        else if(key >= TC_S && key <= TC_E) return _top_chars[key - TC_S];
        else if(key >= MC_S && key <= MC_E) return _middle_chars[key - MC_S];
        else if(key >= BC_S && key <= BC_E) return _bottom_chars[key - BC_S];
        else if(key >= FIRST_NUMBER && key <= LAST_NUMBER)                 return _num[key - FIRST_NUMBER];
        else if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)       return _top_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW) return _middle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW) return _bottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }

    return 0;
}

void keyboard_flush_cache()
{
    memset(keyCache, 0, 256 * sizeof(char));
    keyLoc = 0;
}

//Read keyboard encoder buffer
uint8_t keyboard_read_encbuf()
{
    return inb(0x60);
}

uint8_t keyboard_read_status()
{
    return inb(0x64);
}

//Send keyboard command to encode buffer
void keyboard_send_enc_cmd(uint8_t cmd)
{
    // Wait for buffer to be clear
    while(true)
    {
        if((keyboard_read_status() & 0b00000010) == 0)
            break;
    }
    // Send the command
    outb(0x60, cmd);
}

void keyboard_send_cmd(uint8_t cmd)
{
    // Wait for buffer to be clear
    while(true)
    {
        if((keyboard_read_status() & 0b00000010) == 0)
            break;
    }
    // Send the command
    outb(0x64, cmd);
}

void keyboard_set_leds(bool num, bool caps, bool scroll)
{
    uint8_t data = 0;

    if(scroll)
        data &= 0b001;
    if(num)
        data &= 0b010;
    if(caps)
        data &= 0b100;

    outb(0x60, 0xED);
    outb(0x60, data);
}

static char c = 0;
char keyboard_getc()
{
    c = 0;
    if(keyLoc == 0) return c;
    c = *keyCache;
	keyLoc --;
    for(int i = 0; i < 256; i++)
        keyCache[i] = keyCache[i+1];
    return c;
}

// Handles the keyboard interrupt
void keyboard_handler(regs_t *r)
{
    // Get key scan code
    uint8_t scancode = keyboard_read_encbuf();

    switch(scancode)
    {
        //These are toggled
        case KEY_CAPS:   CAPSF = CAPSF ? false : true; keyboard_set_leds(NUMLF, CAPSF, SCRLF); break;
        case KEY_NUM:    NUMLF = NUMLF ? false : true; keyboard_set_leds(NUMLF, CAPSF, SCRLF); break;
        case KEY_SCROLL: SCRLF = SCRLF ? false : true; keyboard_set_leds(NUMLF, CAPSF, SCRLF); break;
        //These are press + hold
        case KEY_LSHIFT: SFLAG = true; break;
        case KEY_LALT:   ALTFL = true; break;
        case KEY_LCTRL:  CTRLF = true; break;
        //Do the release cases
        case KEY_LSHIFT + 0x80: SFLAG = false; break;
        case KEY_LALT   + 0x80: ALTFL = false; break;
        case KEY_LCTRL  + 0x80: CTRLF = false; break;
        default: break;
    }

    if(keyLoc > 256) // If we overflow the buffer then...
        keyboard_flush_cache();

    if(scan_to_ascii(scancode) != 0)
    {
        if(CTRLF)
        {
            bool s = SFLAG, c = CAPSF;
            keyCache[keyLoc++] = '^';
            if(keyLoc > 256)
                keyboard_flush_cache();
            SFLAG = false; CAPSF = true;
            keyCache[keyLoc++] = scan_to_ascii(scancode);
            SFLAG = s; CAPSF = c;
        }
        else
            keyCache[keyLoc++] = scan_to_ascii(scancode);
    }
}

void register_keyboard()
{
    install_handler(IRQ1, &keyboard_handler);
    keyboard_set_leds(false, false, false);
    keyCache = (uint8_t*)kmalloc(256 * sizeof(char));
    keyboard_flush_cache();
    bprintok(); console_writeline("Registered keyboard handler");
}
