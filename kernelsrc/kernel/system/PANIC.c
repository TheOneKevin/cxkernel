/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#define Hardcore 0

#include "system/PANIC.h"
#include "display/tdisplay.h"
#include "system/kprintf.h"

void panic(const char *message)
{
    // We encountered a massive problem and have to stop.
    kprintf("Debug buffer: %X", (uint32_t)debugBuffer);
    asm volatile("cli"); // Disable interrupts.
    console_write("*****"); console_write(message); console_write("*****\n"); //TODO: Stack trace
    console_write("Kernel panic can be fixed (usually) by a restart. CPU is going to halt now...");
    // Halt by going into an infinite loop.
    halt();
}

void assert(bool assertStatus, const char *message)
{
    if(!assertStatus)
    {
        kprintf("Debug buffer: %X \n", (uint32_t)debugBuffer);
        kprintf("Assertation failed! %s", message);
#if Hardcore
        halt();
#endif
    }
}