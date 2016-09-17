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

//All the (local) kernel options
#define DEBUGMSG     1   // Enable to see messages
#define TEST_NOPAGE  0   // Enable to test non existant fault
#define TEST_NOPAGE2 0   // Enable to test non existant fault
#define CPU_EXCEP    0   // Test CPU exceptions
#define GRUB_2       0

#include "multiboot.h"

#if GRUB_2
#include "multiboot2.h"
#endif

#include "common.h"

#include "system/tdisplay.h"
#include "system/pic.h"
#include "system/irq.h"
#include "system/pit.h"
#include "system/kprintf.h"
#include "system/terminal.h"

#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/exceptions.h"

#include "memory/paging.h"

#include "drivers/keyboard.h"
#include "drivers/acpi.h"

// http://wiki.osdev.org/ <- GODSEND. Contains almost all the info I used to create LiquiDOS
// http://wiki.osdev.org/What_order_should_I_make_things_in <- Read.

// Here for debugging purposes :)
void getMemDisplay(multiboot_info_t* mbt)
{
    #if DEBUGMSG
    kprintf("Memory map address: %X \n", mbt -> mem_upper);
    #endif
    multiboot_memory_map_t *mmap = mbt -> mmap_addr;
    while(mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        #if DEBUGMSG
        kprintf(" Entry length: %X Entry address %X (%X) \n", (uint32_t)mmap -> len, (uint32_t)mmap -> addr, (uint32_t)mmap -> type);
        #endif
        if((mmap -> len >= _length) && (mmap -> type == 0x1)) //We want the largest chunk of free space
        {
            _length = mmap -> len; _addr = mmap -> addr;
        }
    }
    #if DEBUGMSG
    // Print out the data sizes in GB, MB, KB and then B 
    if(_length >= 1073741824) { uint32_t tmp = _length / 1073741824; kprintf("Length of memory: %u GB Address: %X \n", tmp, _addr); }
    else if(_length >= 1048576) { uint32_t tmp = _length / 1048576; kprintf("Length of memory: %u MB Address: %X \n", tmp, _addr); }
    else if(_length >= 1024) { uint32_t tmp = _length / 1024; kprintf("Length of memory: %u KB Address: %X \n", tmp, _addr); }
    else { kprintf("Length of memory: %u B Address: %X \n", _length, _addr); }
    #endif
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
    getMemDisplay(mbt);
    //Then setup paging based on the information
    setup_paging();
    
    init_terminal();
    
    // Enable interrupts
    bprintok(); console_write("OS ready!\n");
    
    asm volatile("sti");
    #if TEST_NOPAGE
    // Test page fault :)
    uint32_t* ptr = (uint32_t*) _addr + _length; //Should cause a nonexistant fault
    #endif
    
    #if TEST_NOPAGE2
    uint32_t* ptr = (uint32_t*) 0xA0000000; //Should cause a protection fault
    #endif
    
    #if TEST_NOPAGE || TEST_NOPAGE2
    uint32_t foo = *ptr;
    console_write_dec(foo);
    #endif
    
    #if CPU_EXCEP
    console_write_dec(3/0); //Test if interrupts work
    #endif
    kprintf("\n0:\\>");
    halt(); // Needed for interrupts to work properly - Prevents the kernel from exiting early
}