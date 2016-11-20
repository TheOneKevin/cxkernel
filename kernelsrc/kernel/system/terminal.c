/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "system/terminal.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"

#include "drivers/acpi.h"
#include "drivers/keyboard.h"
#include "drivers/cpuid.h"

#include "localization/scanmap.h"
#include "multiboot.h"

char buffer[256]; // TODO: We will use malloc() in the future but for now...
int i = 0;
uint32_t j = 0;
multiboot_info_t* mbt;

char *builtinCmds[] = { "help", "reboot", "shutdown", "mmap", "debug", "cpuid" };
uint32_t size = 6; //Size of array

// The functions below are for built-in commands

void help()
{
    
}

void mmap()
{
    kprintf(" Memory map address: %X \n", mbt -> mem_upper);
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbt -> mmap_addr;
    
    while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        // Print out the data sizes in GB, MB, KB and then B 
        if((uint32_t)mmap -> len  >= 1073741824)   { kprintf(" Length of section: %u GB", (uint32_t)mmap -> len / 1073741824); }
        else if((uint32_t)mmap -> len  >= 1048576) { kprintf(" Length of section: %u MB", (uint32_t)mmap -> len / 1048576); }
        else if((uint32_t)mmap -> len  >= 1024)    { kprintf(" Length of section: %u KB", (uint32_t)mmap -> len / 1024); }
        else { kprintf(" Length of section: %u B ", (uint32_t)mmap -> len); }
        kprintf(" Start address: %X (%X) \n", (uint32_t)mmap -> addr, (uint32_t)mmap -> type);
    }
}

void debug()
{
    kprintf("Debug buffer: %X\n", (uint32_t)debugBuffer);
}

void cpuid()
{
    cpu_detect();
    if(_CORES == 0)
        kprintf("CPU Cores: 1\n\n");
    else
        kprintf("CPU Cores: %u\n\n", _CORES);
}

// Other stuff

void fetchCommand(int id)
{
    switch(id)
    {
        case 0: kprintf("Help\n"); break;
        case 1: kprintf("Going down for reboot..."); reboot(); break;
        case 2: kprintf("Going down for shutdown..."); acpiPowerOff(); break;
        case 3: mmap(); break;
        case 4: debug(); break;
        case 5: cpuid(); break;
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
        for(j = 0; j < size; j++) //Here, we extract the command id from a preset list of commands
        {
            if(strcmp(buffer, builtinCmds[j]) == 0)
            {
                fetchCommand(j);
                break;
            }
            if(j + 1 == size){ kprintf("Command not recognized!\n"); break; }
        }
        
        memset(&buffer, 0, 256);
        i = 0;
        kprintf("0:\\>");
    }
    
    else if((scancode == BCKSPACE) && (i > 0))
    {
        i--; buffer[i] = 0;
    }
    
    else if(scancode != BCKSPACE) //Make sure backspace doesn't exceed the buffer (and underflow the array)
    {
        buffer[i] = scan_to_ascii(scancode);
        i++;
    }
}

void init_terminal(multiboot_info_t* multi)
{
    mbt = multi;
    //Let's register our keyboard hook now
    installKeyHandler(&interpret_cmd, 0);
    setHandlerFlag(0);
}
