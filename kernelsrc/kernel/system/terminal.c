/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/terminal.h"

char buffer[256]; // TODO: We will use malloc() in the future but for now...
int i = 0;

void interpret_cmd(uint8_t scancode)
{
    buffer[i] = scan_to_ascii(scancode);
    i++;
    if(scancode == 0x1C) //Check for enter key
    {
        //Interpret this stuff
        console_write(buffer);
        memset(&buffer, 0, 256);
        i = 0;
    }
}

void init_terminal()
{
    //Let's register our keyboard hook now
    installKeyHandler(&interpret_cmd, 0);
    setHandlerFlag(0);
}