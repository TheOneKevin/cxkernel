/* This is our internal terminal application (AKA backup)
 * Usually, this wouldn't be invoked, but in the case of the actual terminal crashing
 * we can re-initialize the terminal via this one :)
*/

#include "multiboot.h"
#include "exp_common.h"

#include "system/terminal.h"
#include "system/kprintf.h"

#include "display/tdisplay.h"

#include "memory/kheap.h"

#include "drivers/acpi.h"
#include "drivers/keyboard.h"
#include "drivers/cpuid.h"
#include "drivers/vesa.h"

#include "localization/scanmap.h"
#include "fs/initrd.h"
#include "fs/vfs.h"
#include "system/time.h"

#pragma GCC diagnostic ignored "-Wdiv-by-zero"

char buffer[256];
//Some counters
int i = 0;
uint32_t j = 0;

char *builtinCmds[] = {
    "help", "reboot", "shutdown", "mmap", "debug", "cpuid", "mm", "clear", "game",
    "memtest", "testint", "vbeinfo", "ls", "echo", "open"
};
uint32_t size = 15; //Size of array

//External variables
screeninfo_t screen;
uint32_t* vcache;
vscreen_t vhscreen;
multiboot_info_t* mbt;
KHEAPBM *kheap;
//char* cd; //Current directory

/* =====================================================================================================
 * The functions below are for built-in commands
 * ===================================================================================================== */

const char* helpMessage =
"List of available commands======================================================\n"
"help:   Displays this message       ls:       Lists all mount points and files\n"
"reboot: Reboots the PC              shutdown: Warm shuts down the computer\n"
"clear:  Clears the screen           mmap:     Displays memory map\n"
"cpuid:  Displays CPU information    mm:       Display memory usage information\n"
"game:   Hehe. Find out yourself     memtest:  Tests heap allocation\n"
"echo: Echos the argument input      open: Open file from path\n"
"testint: Tests the interrupt system by dividing by 0. Crash alert!\n"
"vbeinfo: Gives information on current vbe screen (if applicable)\n";

void help()
{
    kprintf("%s", helpMessage);
}

void mmap()
{
    kprintf(" Memory map address: %X \n", mbt -> mem_upper);
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbt -> mmap_addr + KRNLBASE;

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
    bprintinfo(); kprintf("Debug buffer: %X\n", (uint32_t)debugBuffer);
}

void cpuid()
{
    cpu_detect();
    if(_CORES == 0)
        kprintf("CPU Cores: 1\n");
    else
        kprintf("CPU Cores: %u\n", _CORES);
}

void mm()
{
    KHEAPBLOCKBM *b = kheap -> fblock;
    size_t size = b -> size;
    uint32_t usage = b -> used;
    bprintwarn();
    kprintf("Note, all the values are inaccurate (no float support yet)!\n");
    bprintinfo(); kprintf("Size of heap: %s\n", convertToUnit((uint32_t)size));
    bprintinfo(); kprintf("Amount used: %s\n", convertToUnit(usage));
    bprintinfo(); kprintf("Percent used: %u%\n", (usage * 100) / size);

    kprintf("\n"); //Every command has to follow with a newline
}

void memtest()
{
    kprintf("This is where we test whether our heap is working or not: \n");
    kprintf("This should output \"0xBADBEEF\" on the first line and do nothing on the second \n");
    uint32_t *p = (uint32_t *)kmalloc(kheap, sizeof(uint32_t));
    *p = 0xBADBEEF; kprintf("%X\n", *p);
    kfree(kheap, p);
    char* undef = 0; kfree(kheap, undef); //This should (not) throw error (anymore)
}

void divByZero() { uint32_t p = 9 / 0; kprintf("%u", p); }

void vbeInfo()
{
    bprintinfo(); kprintf("Screen resolution: %ux%u\n", vhscreen.width, vhscreen.height);
    bprintinfo(); kprintf("Pitch: %u\n", vhscreen.pitch);
    bprintinfo(); kprintf("BPP: %u\n", vhscreen.bpp);
    bprintinfo(); kprintf("V-Cache: %X V-Actual: %X", vcache, vhscreen.framebuffer);
    kprintf("\n");
}

void ls()
{
    kprintf("Devices currently mounted\n");
    vfs_list_mount();
    kprintf("Files and directories on root\n");
    vfs_ls("/");
    fstat_t* stat = (fstat_t*)kmalloc(kheap, sizeof(fstat_t));
    fsnode_t* file = vfs_openfile("/initrd/Kudzu.txt");
    vfs_stat(file, stat);
    uint32_t* buffer = (uint32_t*)kmalloc(kheap, stat -> st_size);
    vfs_read("/initrd/Kudzu.txt", buffer, Read);
    kprintf("Contents of /initrd/Kudzu.txt: %s\n", buffer);
    kfree(kheap, buffer); kfree(kheap, file);
}

void echo(char* args)
{
    kprintf("%s\n", args + 5); //"echo " = 5 chars
}

void open(char* args)
{
    if(*(args + 5) == 0) { kprintf("Args invalid! Proper usage: open [path]\n"); return; }
    if(!vfs_exists(args + 5))
    {
        kprintf("File not found!\n");
        return;
    }

    fstat_t* stat = (fstat_t*)kmalloc(kheap, sizeof(fstat_t));
    fsnode_t* file = vfs_openfile(args + 5);
    vfs_stat(file, stat);
    uint32_t* fsbuffer = (uint32_t*)kmalloc(kheap, stat -> st_size);
    vfs_read(args + 5, fsbuffer, Read); //"open " = 5 chars
    console_setfg(COLOR_LIGHT_BLUE); kprintf("Contents of %s:\n", args + 5);
    kprintf("Size: %s\n", convertToUnit(stat -> st_size)); console_setfg(COLOR_WHITE);
    kprintf("%s", fsbuffer);
    kfree(kheap, fsbuffer); kfree(kheap, stat);
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
        case 9: memtest(); break;
        case 10: divByZero(); break;
        case 11: vbeInfo(); break;
        case 12: ls(); break;
        case 13: echo(buffer); break;
        case 14: open(buffer); break;
        default: console_setfg(COLOR_RED); kprintf("Command not recognized!\n"); console_setfg(COLOR_WHITE); break;
    }
}

void interpret_cmd(uint8_t scancode)
{
    if(scancode == ENTER) //Check for enter key
    {
        buffer[i] = 0;
        uint32_t n = 0;
        while(buffer[n] != ' ')
            n++;
        buffer[n] = 0;
        //Interpret this stuff
        //console_write(buffer);
        for(j = 0; j < size; j++) //Here, we extract the command id from a preset list of commands
        {
            if(strcmp(buffer, builtinCmds[j]) == 0)
            {
                fetchCommand(j);
                break;
            }
            if(j + 1 == size){ console_setfg(COLOR_RED); kprintf("Command not recognized!\n"); console_setfg(COLOR_WHITE); break; }
        }

        memset(&buffer, 0, 256);
        i = 0;

        kprintf("/:\\>");
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

    kprintf("\n/:\\>");
}
