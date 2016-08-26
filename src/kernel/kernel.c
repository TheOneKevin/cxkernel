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
#include "system/keyboard.h"

void kernel_main()
{
    console_clear(COLOR_BLACK);
    //console_write("Hello world!");
    install_gdt();
    install_idt();
    load_isr();
    autoremap_PIC();
    register_irq();
    register_keyboard();
    asm volatile("sti");
    //init_timer(50);
    //asm volatile("int $0x3");
    for(;;); // Needed for interrupts to work properly... apparently
}