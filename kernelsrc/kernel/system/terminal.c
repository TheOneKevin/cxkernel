/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/terminal.h"

static int status;
char buffer[256]; // TODO: We will use malloc() in the future but for now...

void init_terminal()
{
    status = 1;
    do
    {
        interpret_cmd();
    } while(status);
}

void interpret_cmd()
{
    int i = 0;
    while(getLastScan() != '\n')
    {
        if(getLastScan() != 0)
        {
            buffer[i] = getLastScan();
            i++;
            flush_cache();
        }
    }
    buffer[i+1] = 0;
    
    //Interpret this stuff
    console_write(buffer);
    if(strcmp(buffer, "help") == 1){ console_write("hoy!"); }
    else { console_write("hoy2!"); }
}