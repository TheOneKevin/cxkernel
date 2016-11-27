/* This is our internal terminal application (AKA backup)
 * Usually, this wouldn't be invoked, but in the case of the actual terminal crashing
 * we can re-initialize the terminal via this one :)
*/

#include "system/terminal.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"

#include "memory/kheap.h"

#include "drivers/acpi.h"
#include "drivers/keyboard.h"
#include "drivers/cpuid.h"

#include "localization/scanmap.h"
#include "multiboot.h"
#include "drivers/vesa.h"

char buffer[256];
//Some counters
int i = 0;
uint32_t j = 0;

char *builtinCmds[] = {
    "help", "reboot", "shutdown", "mmap", "debug", "cpuid", "mm", "clear", "game"
};
uint32_t size = 9; //Size of array

//External variables
screeninfo_t screen;
uint32_t* vcache;
vscreen_t vhscreen;
multiboot_info_t* mbt;
KHEAPBM *kheap;

//This is HIGHLY experimental iotoa (int to string more specifically)
char* iotoa(uint32_t n)
{
    if (n == 0)
    {
        char* c2 = (char*)kmalloc(kheap, 32 * sizeof(char));
        *c2 = "0";
        return c2;
    }

    int acc = n;
    char* c = (char*)kmalloc(kheap, 32);
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char* c2 = (char*)kmalloc(kheap, 32 * sizeof(char));
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    kfree(kheap, c);
    return c2;
}

char* convertToUnit(uint32_t input)
{
    if(input  >= 1073741824)
    {
        char* tmp = iotoa((uint32_t)input / 1073741824);
        char* res = strcat(tmp, " GB");
        kfree(kheap, tmp); //Gettin' down and dirty with memory management
        return res;
    }
    
    else if(input  >= 1048576)
    {
        char* tmp = iotoa((uint32_t)input / 1048576);
        char* res = strcat(tmp, " MB");
        kfree(kheap, tmp);
        return res;
    }
    
    else if(input  >= 1024)
    {
        char* tmp = iotoa((uint32_t)input / 1024);
        char* res = strcat(tmp, " KB");
        kfree(kheap, tmp);
        return res;
    }
    
    else
    {
        char* tmp = iotoa((uint32_t)input);
        char* res = strcat(tmp, " B");
        kfree(kheap, tmp);
        return res;
    }
}

/* =====================================================================================================
 * The functions below are for built-in commands
 * ===================================================================================================== */

void help()
{
    kprintf("List of available commands: \nhelp, reboot, shutdown, mmap, debug, cpuid, mm, clear, game\n");
}

void mmap()
{
    kprintf(" Memory map address: %X \n", mbt -> mem_upper);
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbt -> mmap_addr;
    
    while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
        // Print out the data sizes in GB, MB, KB and then B 
        kprintf(" Length of section: %s", convertToUnit(mmap -> len));
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

void mm()
{
    KHEAPBLOCKBM *b = kheap -> fblock;
    size_t size = b -> size;
    uint32_t usage = b -> used;
    kprintf("Note, all the values are inaccurate (no float support yet)!\n");
    kprintf("Size of heap: %s | ", convertToUnit((uint32_t)size));
    kprintf("Amount used: %s | ", convertToUnit(usage));
    kprintf("Percent used: %u% ", (usage * 100) / size);
    
    kprintf("\n");
    
    kprintf("V-Cache: %X V-Actual: %X", vcache, vhscreen.framebuffer);
    
    kprintf("\n"); //Every command has to follow with a newline
}

/* =====================================================================================================
 * Other stuff (Interpret command, etc)
 * ===================================================================================================== */

void fetchCommand(int id)
{
    switch(id)
    {
        case 0: kprintf("[Help Message]\n"); help(); break;
        case 1: kprintf("Going down for reboot..."); reboot(); break;
        case 2: kprintf("Going down for shutdown..."); acpiPowerOff(); break;
        case 3: mmap(); break;
        case 4: debug(); break;
        case 5: cpuid(); break;
        case 6: mm(); break;
        case 7: console_clear(); screen._x = 0; screen._y = 0; break;
        case 8: kprintf("Command not recognized   :P\n"); break;
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
    kprintf("\n0:\\>");
}
