/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   kernel.c
 * Author: Kevin Dai
 * 
 * Created on August 24, 2016, 4:48 PM
*/

#include "common.h"
#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/exceptions.h"
#include "system/tdisplay.h"
#include "system/pic.h"
#include "system/irq.h"
#include "system/pit.h"

#include "drivers/keyboard.h"

// http://wiki.osdev.org/ <- GODSEND. Contains almost all the info I used to create LiquiDOS
// http://wiki.osdev.org/What_order_should_I_make_things_in <- Read.

void kernel_main()
{
    console_clear(COLOR_BLACK);
    // First, we install our GDT and IDT, then we fill the IDT with CPU exceptions
    // We then prepare the PIC for usage, and register our 15 PIC interrupts
    install_gdt();
    install_idt();
    load_isr();
    autoremap_PIC();
    //register_irq();
    // Start our built-in keyboard driver
    register_keyboard();
    asm volatile("sti");
    asm volatile("int $0x0");
    for(;;); // Needed for interrupts to work properly... apparently
}