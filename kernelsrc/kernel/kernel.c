/* "Welcome to Operating System development; the great frontier." - The OSDev Wiki
 * Hello, and welcome to my code. This is my home, and I will be your personal guide throughout this land full of perils and stuff.
 * You will find me like this at the top of many files, and besides many lines of code, serving as a reminder, tour guide of other
 * sorts of personal. Firstly, I would like to invite you to the kernel. This is where everything begins. Enjoy the scenery
 */

/* 
 * File:   kernel.c
 * Author: Kevin Dai <- That's me!
 * 
 * Created on August 24, 2016, 4:48 PM
*/

//All the (local) kernel options
#define DEBUGMSG     0   // Enable to see messages
#define GRUB_2       0   // Are we using GRUB2?
#define SAVAGEMODE   1   // Savage mode for errors and warnings ;)
//All dem tests!
#define TEST_HEAP    0   // Test the heap
//Enable only one
#define TEST_NOPAGE  0   // Enable to test non existant fault
#define TEST_NOPAGE2 0   // Enable to test protection fault
#define CPU_EXCEP    0   // Test CPU exceptions

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
#include "system/PANIC.h"

#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/exceptions.h"

#include "memory/paging.h"
#include "memory/kheap.h"
#include "memory/pmm.h"

#include "drivers/keyboard.h"
#include "drivers/acpi.h"
#include "drivers/cpuid.h"

// http://wiki.osdev.org/ <- GODSEND. Contains almost all the info I used to create LiquiDOS
// http://wiki.osdev.org/What_order_should_I_make_things_in <- Read.

//Memory infornmation
uint64_t _length;
uint64_t _addr;
KHEAPBM *kheap;
extern uint32_t end;
bool doBootLog;

void getMemDisplay(multiboot_info_t* mbt)
{
    #if DEBUGMSG
    kprintf("Memory map address: %X \n", mbt -> mem_upper);
    #endif
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbt -> mmap_addr;
    
    while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        #if DEBUGMSG
        kprintf(" Entry length: %X Entry address %X (%X) \n", (uint32_t)mmap -> len, (uint32_t)mmap -> addr, (uint32_t)mmap -> type);
        #endif
        if((mmap -> len >= _length) && (mmap -> type == 0x1)) //We want the largest chunk of free space
        {
            _length = mmap -> len; _addr = mmap -> addr; //Then store it in these variables
        }
    }
    // Print out the data sizes in GB, MB, KB and then B 
    if(_length >= 1073741824) { uint32_t tmp = _length / 1073741824; bprintinfo(); kprintf("Length of memory: %u GB Address: %X \n", tmp, _addr); }
    else if(_length >= 1048576) { uint32_t tmp = _length / 1048576; bprintinfo(); kprintf("Length of memory: %u MB Address: %X \n", tmp, _addr); }
    else if(_length >= 1024) { uint32_t tmp = _length / 1024; bprintinfo(); kprintf("Length of memory: %u KB Address: %X \n", tmp, _addr); }
    else { kprintf("Length of memory: %u B Address: %X \n", _length, _addr); }
    //Memory check!
    if(_length < 1024 * 1024 * 8)
    {
        bprinterr(); kprintf("Not enough RAM! Recommended 16 MB of RAM!\n");
        #if SAVAGEMODE
        PANIC("More RAM needed!");
        #endif
    }
    else if(_length > 1024 * 1024 * 16) { bprintok(); kprintf("Sufficient RAM installed :)\n"); }
    else { bprintwarn(); kprintf("OS might run out of RAM! Recommended 16 MB of RAM!\n"); }
}

void kernel_main(multiboot_info_t* mbt)
{
    //TODO: Add some GUI initialization of the console, like a VESA/VBE console of sorts
    doBootLog = true; //Begin boot sequence
    console_init();
    
    //kprintf("Magic number: %X\n", magic);
    cpu_detect();
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
    //Initialize our kernel heap
    k_heapBMInit(kheap);
    k_heapBMAddBlock(kheap, (uintptr_t)&end, 0x400000, 16);
    bprintinfo(); kprintf("Kernel heap starts: %X Kernel heap ends: %X\n", &end, &end + kheap -> fblock -> size);
    //Enable out physical and virtual memory managers
    initPmm();
    paging_init();
    //Enable our terminal
    init_terminal();
    //Print OS OK text
    bprintok(); console_write("OS ready!\n");
    doBootLog = false; //End of boot sequence
    // Enable interrupts
    asm volatile("sti");
    
    #if TEST_NOPAGE
    // Test page fault :)
    uint32_t* _ptr = (uint32_t*) (0xFFF); //Should cause a nonexistant fault
    #endif
    
    #if TEST_NOPAGE2
    uint32_t* _ptr = (uint32_t*) 0xA0000000; //Should cause a protection fault
    #endif
    
    #if TEST_NOPAGE || TEST_NOPAGE2
    uint32_t foo = *_ptr;
    console_write_dec(foo);
    #endif
    
    #if CPU_EXCEP
    console_write_dec(3/0); //Test if interrupts work
    #endif
    
    #if TEST_HEAP
    char* ptr;
    ptr = (char*)k_heapBMAlloc(kheap, 4);
    *ptr = 4;
    kprintf("%X\n%X", ptr, *ptr); //Should display 0x4 on the second line
    k_heapBMFree(kheap, ptr);
    #endif
    
    kprintf("\n0:\\>");
    halt(); // Needed for interrupts to work properly - Prevents the kernel from exiting early
}