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

#include "multiboot.h"
//#include "multiboot2.h"

#include "common.h"
#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/exceptions.h"
#include "system/tdisplay.h"
#include "system/pic.h"
#include "system/irq.h"
#include "system/pit.h"
#include "memory/paging.h"
#include "system/kprintf.h"

#include "drivers/keyboard.h"
#include "drivers/acpi.h"

// http://wiki.osdev.org/ <- GODSEND. Contains almost all the info I used to create LiquiDOS
// http://wiki.osdev.org/What_order_should_I_make_things_in <- Read.

// Here for debugging purposes :)
void getMemDisplay(multiboot_info_t* mbt)
{
    console_write(" Memory map address:");
    console_write_hex(mbt -> mem_upper); console_putc('\n');
    
    multiboot_memory_map_t *mmap = mbt -> mmap_addr;
    while(mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        console_write("  Entry length:");
        console_write_hex(mmap -> len);
        console_write(" Entry address:");
        console_write_hex(mmap -> addr);
        console_write("    Entry size:");
        console_write_hex(mmap -> size);
        console_putc(' '); console_write_hex(mmap -> type);
        
        if((mmap -> len >= _length) && (mmap -> type == 0x1)) //We want the largest chunk of free space
        {
            _length = mmap -> len; _addr = mmap -> addr;
        }
        console_write("\n");
    }
    console_write_hex(_length); console_write_hex(_addr);
    console_write("\n");
}

void getMmap(multiboot_info_t* mbt)
{
    multiboot_memory_map_t *mmap = mbt -> mmap_addr;
    while(mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        if((mmap -> len >= _length) && (mmap -> type == 0x1)) //We want the largest chunk of free space
        {
            _length = mmap -> len; _addr = mmap -> addr;
        }
    }
}

void kernel_main(multiboot_info_t* mbt, unsigned int magic)
{
    console_init();
    // First, we install our GDT and IDT, then we fill the IDT with CPU exceptions
    // We then prepare the PIC for usage, and register our 15 PIC interrupts
    install_gdt();
    install_idt();
    load_isr();
    autoremap_PIC();
    register_irq();
    // Register and start our built-in keyboard driver
    register_keyboard();
    //init_timer(50); //Test if multiple interrupts work :)
    //Initialize ACPI and enable it
    initAcpi();
    acpiEnable();
    //Get memory information
    //getMmap(mbt); //For silent startup
    getMemDisplay(mbt);
    //Then setup paging based on the information
    setup_paging();
    
    asm volatile("sti");
    // Test page fault :)
    //uint32_t* ptr = (uint32_t*) _addr + _length; //Should cause a nonexistant fault
    //uint32_t* ptr = (uint32_t*) 0xA0000000; //Should cause a protection fault
    //uint32_t foo = *ptr;
    //console_write_dec(foo);
    //console_write_dec(3/0); //Test if interrupts work
    halt(); // Needed for interrupts to work properly - Prevents the kernel from exiting early
}