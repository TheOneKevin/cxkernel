/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/terminal.h"

char buffer[256]; // TODO: We will use malloc() in the future but for now...
int i = 0, j = 0;

char *builtinCmds[] = { "help" };

void interpret_cmd(uint8_t scancode)
{
    buffer[i] = scan_to_ascii(scancode);
    i++;
    if(scancode == 0x1C) //Check for enter key
    {
        buffer[i-1] = 0;
        //Interpret this stuff
        //console_write(buffer);
        for(j = 0; j < 1;)
        {
            if(strcmp(buffer, builtinCmds[j]) == 0)
            {
                fetchCommand(j);
                break;
            }
            if(j + 1 == 1) { kprintf("Non existant!\n"); break; }
            j++;
        }
        
        memset(&buffer, 0, 256);
        i = 0;
        kprintf("0:\\>");
    }
}

void fetchCommand(int id)
{
    switch(id)
    {
        case 0: kprintf("Help!\n"); break;
        default: kprintf("Non existant!\n"); break;
    }
}

void init_terminal()
{
    //Let's register our keyboard hook now
    installKeyHandler(&interpret_cmd, 0);
    setHandlerFlag(0);
}