/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arch/exceptions.h"
#include "system/irq.h"
#include "system/tdisplay.h"

#include "drivers/keyboard.h"
#include "localization/en_scan.h"

bool SFLAG = false; // Shift flag
bool CAPSF = false; // CAPS flag

uint8_t scan_to_ascii(uint8_t key)
{
    if(SFLAG || CAPSF)
    {
        //Get the modified (shift) characters
        if(!CAPSF) //However, we don't want the CAPS lock triggering special SHIFT characters
        {
            if(key >= FIRST_NUMBER && key <= LAST_NUMBER)
                return _mnum[key - FIRST_NUMBER];
            else if(key == DASH)
                return _mdash[0];
            else if(key == EQUALS)
                return _mequals[0];
        }
        if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)
            return _mtop_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW)
            return _mmiddle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW)
            return _mbottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }
    else
    {
        //Get the unmodified (shift) characters
        if(key >= FIRST_NUMBER && key <= LAST_NUMBER)
            return _num[key - FIRST_NUMBER];
        else if(key == DASH)
            return _dash[0];
        else if(key == EQUALS)
            return _equals[0];
        else if(key >= FIRST_KEY_TOP_ROW && key <= LAST_KEY_TOP_ROW)
            return _top_row[key - FIRST_KEY_TOP_ROW];
        else if(key >= FIRST_KEY_MIDDLE_ROW && key <= LAST_KEY_MIDDLE_ROW)
            return _middle_row[key - FIRST_KEY_MIDDLE_ROW];
        else if(key >= FIRST_KEY_BOTTOM_ROW && key <= LAST_KEY_BOTTOM_ROW)
            return _bottom_row[key - FIRST_KEY_BOTTOM_ROW];
    }
    
    //Get the special keys, i.e., enter, backspace, and keys that cannot be *modified*
    if(key == ENTER) return '\n';
    else if(key == BCKSPACE) return '\b';
    else if(key == SPACE) return ' ';
    else if(key == CAPS){ if(CAPSF){ CAPSF = false; } else { CAPSF = true; } return 0; } //Toggle our CAPS flag
    
    return 0;
}

/* Handles the keyboard interrupt */
void keyboard_handler(regs_t *r)
{
    uint8_t scancode;
    // Get key scan code
    scancode = inb(0x60);
    if(scancode == (LSHIFT + 0x80)) //Shift key is released, reset the caps
        SFLAG = false;
    if(scancode == LSHIFT) //See if shift is pressed and held
        SFLAG = true;
    if(scan_to_ascii(scancode) != 0)
        console_putc(scan_to_ascii(scancode));
}

void register_keyboard()
{
    install_handler(33, &keyboard_handler);
}