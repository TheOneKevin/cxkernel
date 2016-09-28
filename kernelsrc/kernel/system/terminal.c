/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/terminal.h"
#include "system/kprintf.h"
#include "system/tdisplay.h"

#include "drivers/acpi.h"
#include "drivers/keyboard.h"

#include "localization/scanmap.h"

char buffer[256]; // TODO: We will use malloc() in the future but for now...
int i = 0;
uint32_t j = 0;
char *size = buffer;

char *builtinCmds[] = { "help", "reboot", "shutdown" };

void fetchCommand(int id)
{
    switch(id)
    {
        case 0: kprintf("Help\n"); break;
        case 1: kprintf("Going down for reboot..."); reboot(); break;
        case 2: kprintf("Going down for shutdown..."); acpiPowerOff(); break;
        default: kprintf("Command not recognized!\n"); break;
    }
}

void interpret_cmd(uint8_t scancode)
{
    if(scancode == ENTER) //Check for enter key
    {
        buffer[i] = 0;
        //Interpret this stuff
        //console_write(buffer);
        for(j = 0; j < sizeof(size);)
        {
            if(strcmp(buffer, builtinCmds[j]) == 0)
            {
                fetchCommand(j);
                break;
            }
            if(j + 1 == sizeof(size)) { kprintf("Command not recognized!\n"); break; }
            j++;
        }
        
        memset(&buffer, 0, 256);
        i = 0;
        kprintf("0:\\>");
    }
    
    else if((scancode == BCKSPACE) && (i > 0))
    {
        i--; buffer[i] = 0;
    }
    
    else
    {
        buffer[i] = scan_to_ascii(scancode);
        i++;
    }
}

void init_terminal()
{
    //Let's register our keyboard hook now
    installKeyHandler(&interpret_cmd, 0);
    setHandlerFlag(0);
}