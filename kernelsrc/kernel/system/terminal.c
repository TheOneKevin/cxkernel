/*
 * This is our internal terminal application (AKA backup)
 * Usually, this wouldn't be invoked, but in the case of the actual terminal crashing
 * we can re-initialize the terminal via this one :)
 */

#include "multiboot.h"
#include "lib/exp_system.h"

#include "system/terminal.h"
#include "system/kprintf.h"
#include "system/time.h"
#include "system/cpuid.h"

#include "display/tdisplay.h"

#include "memory/kheap.h"

#include "drivers/acpi.h"
#include "drivers/keyboard.h"
#include "drivers/vesa.h"
#include "drivers/initrd.h"

#include "localization/scanmap.h"

#include "fs/vfs.h"
#include "fs/fs.h"

#include "gdb/gdb.h"

#pragma GCC diagnostic ignored "-Wdiv-by-zero"

char *builtinCmds[] = {
    "help", "reboot", "shutdown", "mmap", "debug", "cpuid", "mm", "clear", "game",
    "memtest", "testint", "vbeinfo", "ls", "echo", "open", "gdb", "time"
};
uint32_t size = 17; //Size of array

//External variables
screeninfo_t screen;
uint32_t* vcache;
vscreen_t vhscreen;
multiboot_info_t* mbt;

//char* cd; //Current directory

/* =====================================================================================================
 * The functions below are for built-in commands
 * ===================================================================================================== */

const char* helpMessage =
    "List of available commands======================================================\n"
    "help:   Displays this message         ls:       Lists all mount points and files\n"
    "reboot: Reboots the PC                shutdown: Warm shuts down the computer\n"
    "clear:  Clears the screen             mmap:     Displays memory map\n"
    "cpuid:  Displays CPU information      mm:       Display memory usage information\n"
    "game:   Hehe. Find out yourself       memtest:  Tests heap allocation\n"
    "echo [text]: Echos the text           open [file]: Open file from path\n"
    "gdb <timeout>: Starts GDB serial      time <-a>: Retrieves the RTC time\n"
    "testint: Tests the interrupt system by dividing by 0. Crash alert!\n"
    "vbeinfo: Gives information on current vbe screen (if applicable)\n\n"
    "\nFormat: command [arguments] <optional arguments>\n";
void help ()
{
    kprintf ("%s", helpMessage);
}

void mmap ()
{
    kprintf (" Memory map address: %X \n", mbt->mem_upper);
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mbt->mmap_addr + KRNLBASE;

    while ((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
        // Print out the data sizes in GB, MB, KB and then B
        kprintf (" Length of section: %s", convertToUnit (mmap->len));
        kprintf (" Start address: %X (%X) \n", (uint32_t)mmap->addr, (uint32_t)mmap->type);
    }
}

void debug ()
{
    bprintinfo (); kprintf ("Debug buffer: %X\n", (uint32_t)debugBuffer);
}

void cpuid ()
{
    cpu_detect ();
    if (_CORES == 0)
        kprintf ("CPU Cores: 1\n");
    else
        kprintf ("CPU Cores: %u\n", _CORES);
}

void mm ()
{
    KHEAPBLOCKBM *b = kheap->fblock;
    size_t size = b->size;
    uint32_t usage = b->used;

    bprintwarn ();
    kprintf ("Note, all the values are inaccurate (no float support yet)!\n");
    bprintinfo (); kprintf ("Size of heap: %s\n", convertToUnit ((uint32_t)size));
    bprintinfo (); kprintf ("Amount used: %s\n", convertToUnit (usage));
    bprintinfo (); kprintf ("Percent used: %u%\n", (usage * 100) / size);

    kprintf ("\n"); //Every command has to follow with a newline
}

void memtest ()
{
    kprintf ("This is where we test whether our heap is working or not: \n");
    kprintf ("This should output \"0xBADBEEF\" on the first line and do nothing on the second \n");
    uint32_t *p = (uint32_t*)kmalloc (kheap, sizeof(uint32_t));
    *p = 0xBADBEEF; kprintf ("%X\n", *p);
    kfree (kheap, p);
    char* undef = 0; kfree (kheap, undef); //This should (not) throw error (anymore)
}

void divByZero ()
{
    uint32_t p = 9 / 0; kprintf ("%u", p);
}

void vbeInfo ()
{
    bprintinfo (); kprintf ("Screen resolution: %ux%u\n", vhscreen.width, vhscreen.height);
    bprintinfo (); kprintf ("Pitch: %u\n", vhscreen.pitch);
    bprintinfo (); kprintf ("BPP: %u\n", vhscreen.bpp);
    bprintinfo (); kprintf ("V-Cache: %X V-Actual: %X", vcache, vhscreen.framebuffer);
    kprintf ("\n");
}

void ls ()
{

}

void echo (char* args)
{
    kprintf ("%s\n", args + 5); //"echo " = 5 chars
}

void open (char* args)
{
    if (*(args + 5) == 0)
    {
        kprintf ("Args invalid! Proper usage: open [path]\n"); return;
    }
}

void printTime (char* args)
{
    cli ();
    read_rtc ();
    sti ();

    char* h = iotoa (hour);
    char* m = iotoa (minute);
    char* s = iotoa (second);
    char* mo = iotoa (month);
    char* da = iotoa (day);

    char* hr = pad (h, '0', 2, true);
    kfree (kheap, h);
    char* mi = pad (m, '0', 2, true);
    kfree (kheap, m);
    char* se = pad (s, '0', 2, true);
    kfree (kheap, s);
    char* mon = pad (mo, '0', 2, true);
    kfree (kheap, mo);
    char* daa = pad (da, '0', 2, true);
    kfree (kheap, da);

    if (strcmp (args + 5, "-a") == 0)
    {
        if ((hour > 12) && (hour < 24))
        {
            char* foo = iotoa (hour - 12);
            char* foobar = pad (foo, '0', 2, true);
            kfree (kheap, foo);
            kprintf ("%s:%s:%s PM %s/%s/%u\nHH:MM:SS    MM/DD/YYYY\n", foobar, mi, se, mon, daa, year);
            kfree (kheap, foobar);
        }
        else if (hour <= 12)
            kprintf ("%s:%s:%s AM %s/%s/%u\nHH:MM:SS    MM/DD/YYYY\n", hr, mi, se, mon, daa, year);
        else if ((hour == 24) || (hour == 0)) // I don't know which
            kprintf ("12:%s:%s AM %s/%s/%u\nHH:MM:SS    MM/DD/YYYY\n", mi, se, mon, daa, year);
    }

    else
        kprintf ("%s:%s:%s %s/%s/%u\nHH:MM:SS MM/DD/YYYY\n", hr, mi, se, mon, daa, year);

    cli ();
    kprintf ("\nEpoch: %u\n", getEpoch () & 0x00000000FFFFFFFF);
    sti ();

    kfree (kheap, hr);
    kfree (kheap, mi);
    kfree (kheap, se);
    kfree (kheap, daa);
    kfree (kheap, mon);
}

void startDebugger (char* args)
{
    if (atoio (args + 4) == 0)
        initDbg (5000);
    else
        initDbg (atoio (args + 4));
}

/* =====================================================================================================
 * Other stuff (Interpret command, etc)
 * ===================================================================================================== */

void fetchCommand (int id, char* buffer)
{
    switch (id)
    {
        case 0: kprintf ("[Help Message]\n"); help (); break;
        case 1: kprintf ("Going down for reboot..."); reboot (); break;
        case 2: kprintf ("Going down for shutdown..."); acpiPowerOff (); break;
        case 3: mmap (); break;
        case 4: debug (); break;
        case 5: cpuid (); break;
        case 6: mm (); break;
        case 7: console_clear (); screen._x = 0; screen._y = 0; break;
        case 8: kprintf ("Command not recognized   :P\n"); break;
        case 9: memtest (); break;
        case 10: divByZero (); break;
        case 11: vbeInfo (); break;
        case 12: ls (); break;
        case 13: echo (buffer); break;
        case 14: open (buffer); break;
        case 15: startDebugger (buffer); break;
        case 16: printTime (buffer);  break;
        default: console_setfg (COLOR_RED); kprintf ("Command not recognized!\n"); console_setfg (COLOR_WHITE); break;
    }
}

void init_terminal (multiboot_info_t* multi)
{
    mbt = multi;
    char*    text = (char*) kmalloc(kheap, 1024 * sizeof(char));
    uint16_t idx = 0;
    memset(text, 0, 1024 * sizeof(char));

    console_setfg (COLOR_LIGHT_GREY);
    kprintf ("\n/:\\> ");
    console_setfg (COLOR_WHITE);

    while(1)
    {
        char c = keyboard_getc();

        if(c == '\n' || idx > 1022)
        {
            text[idx] = 0;
            kprintf("\n");
            idx = 0;
            while(text[idx] != 0) // Prepare the text for argument parsing
            {
                if(text[idx] == ' ')
                {
                    text[idx] = 0;
                    break;
                }

                idx++;
            }

            for(uint32_t i = 0; i < size; i++) //Here, we extract the command id from a preset list of commands
            {
                if(strcmp(text, builtinCmds[i]) == 0)
                {
                    fetchCommand(i, text);
                    break;
                }
                if(i + 1 == size){ console_setfg(COLOR_RED); kprintf("Command not recognized!\n"); console_setfg(COLOR_WHITE); break; }
            }

            memset(text, 0, 1024 * sizeof(char));
            idx = 0;
            console_setfg (COLOR_LIGHT_GREY);
            kprintf ("/:\\> ");
            console_setfg (COLOR_WHITE);
        }
        else if(c != 0)
        {
            if(c == '\b' && idx > 0)
            {
                kprintf("\b \b");
                idx--;
                text[idx] = 0;
            }
            else if(c != '\b')
            {
                kprintf("%c", c);
                text[idx] = c;
                idx++;
            }
        }
    }
}
