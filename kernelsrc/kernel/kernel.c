/*
 * "Welcome to Operating System development; the great frontier." - The OSDev Wiki
 * Hello, and welcome to my code. This is my home, and I will be your personal guide throughout this land full of perils and stuff.
 * You will find me like this at the top of many files, and besides many lines of code, serving as a reminder, tour guide of other
 * sorts of personal. Firstly, I would like to invite you to the kernel. This is where everything begins. Enjoy the scenery
 */

/*
 * File:   kernel.c
 * Author: Kevin Dai < That's me! >
 *
 * Created on August 24, 2016, 4:48 PM < This date will forever live on >
*/

//All the (local) kernel options
#define DEBUGMSG     0   // Enable to see messages
#define GRUB_2       0   // Are we using the GRUB multiboot2 header?
#define VESA         1   // Use VESA to print? NO
#define SAVAGEMODE   1   // Savage mode for errors and warnings ;)
//All dem tests!
#define TEST_HEAP    0   // Test the heap
//Enable only one
#define TEST_NOPAGE  0   // Enable to test non existant fault
#define TEST_NOPAGE2 0   // Enable to test protection fault
#define CPU_EXCEP    0   // Test CPU exceptions

#if GRUB_2
    #include "multiboot2.h"
#endif
#include "multiboot.h"

#include "common.h"

#include "display/tdisplay.h"
#include "display/vesaModeHooks.h"
#include "display/textModeHooks.h"

#include "system/pic.h"
#include "system/irq.h"
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
#include "drivers/vesa.h"
#include "drivers/pit.h"

#include "fs/initrd.h"
#include "fs/vfs.h"

// http://wiki.osdev.org/ <- GODSEND. Contains almost all the info I used to create LiquiDOS
// http://wiki.osdev.org/What_order_should_I_make_things_in <- Read.
//Memory information
uint64_t _length;
uint64_t _addr;
KHEAPBM *kheap;
KHEAPBM tkh;
extern uint32_t _kernel_end;
//Our switch to real mode function
bool doBootLog;
// Store multiboot pointer address in case it corrupts
multiboot_info_t* mbt;

uint32_t* vcache;
vscreen_t vhscreen;

uint32_t* startheap;

uint32_t initrd_location;
uint32_t initrd_end;

#if VESA
    extern uint8_t enablevesa();
#endif

void getMemDisplay(multiboot_info_t* mbt)
{
    #if DEBUGMSG
    kprintf("Memory map address: %X \n", mbt -> mem_upper);
    #endif
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) (mbt -> mmap_addr + KRNLBASE);

    while((uint32_t)mmap < mbt->mmap_addr + mbt->mmap_length + KRNLBASE)
    {
        mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
        #if DEBUGMSG
        kprintf(" Entry length: %X Entry address %X (%X) \n", (uint32_t)mmap -> len, (uint32_t)mmap -> addr, (uint32_t)mmap -> type);
        #endif
        if((mmap -> len >= _length) && (mmap -> type == 0x1)) //We want the largest chunk of free space
        {
            _length = mmap -> len; _addr = mmap -> addr + KRNLBASE; //Then store it in these variables
        }
    }
    // Print out the data sizes in GB, MB, KB and then . Why the HELL I used base 10 is completely out of the question. I was probably crazy.
    if(_length >= 1073741824) { uint32_t tmp = _length / 1073741824; bprintinfo(); kprintf("Length of memory: %u GB Address: %X \n", tmp, _addr); }
    else if(_length >= 1048576) { uint32_t tmp = _length / 1048576; bprintinfo(); kprintf("Length of memory: %u MB Address: %X \n", tmp, _addr); }
    else if(_length >= 1024) { uint32_t tmp = _length / 1024; bprintinfo(); kprintf("Length of memory: %u KB Address: %X \n", tmp, _addr); }
    else { kprintf("Length of memory: %u B Address: %X \n", _length, _addr); }
    //Memory check!
    const int EPSILON = 1024;
    if(_length + EPSILON < 1024 * 1024 * 64 || _length - EPSILON < 1024 * 1024 * 64)
    {
        bprinterr(); kprintf("Not enough RAM! Recommended 16 MB of RAM!\n");
        #if SAVAGEMODE
        PANIC("More RAM needed!");
        #endif
    }
    else if(_length + EPSILON > 1024 * 1024 * 64 || _length - EPSILON > 1024 * 1024 * 64) { bprintok(); kprintf("Sufficient RAM installed :)\n"); }
    else { bprintwarn(); kprintf("OS might run out of RAM! Recommended 128 MB of RAM!\n"); }
}

void initVbe()
{
    uint16_t* s;
    enablevesa(); // Call our assembly function
    asm volatile("mov %%eax, %0" : "=r" (s)); //We know the return code info pointer is in the EAX register]
    if(*s == 1) //If the return code status is 1
    {
        setVScreen(*(s + 1), *(s + 2), *(s + 3), *(s + 5), *(s + 4), *(uint32_t *)(s + 6));
        _iinitVesaConsole();
        vhscreen.enabled = true;
    }

    else
    {
        return;
    }
}

void kernel_main(multiboot_info_t* multi)
{
    vhscreen.enabled = false;
    mbt = multi; //Store the multiboot header in case we accidently corrupt it
    //mbt = 0xC0010000;
    startheap = (uint32_t*) 0x400000; //0x100 as buffer
    _iinitNormalConsole(); //Install regular VGA-text hooks
    doBootLog = true; //Begin boot sequence

    // Clear out an area for memory stuff
    memset((uint32_t*)FRAMEBSE, 0, 0x800000);
    kheap = &tkh;
    //Initialize our kernel heap
    k_heapBMInit(kheap);
    k_heapBMAddBlock(kheap, (uintptr_t)0xFFC00000, 0x400000, 16); //3 MiB not 4 for a good reason

    #if VESA
        // We need to clear off an area for the VESA tables to sit in
        memset((void *)(0x7C00 + KRNLBASE), 0, 0x100);
        initVbe();

        //Enable our physical memory managers
        pmm_init();
        pmm_addBlock((uintptr_t)FRAMEBSE, 0x400, 1);
        paging_init();
        console_init();
        //asm volatile("cli"); We know that enablevesa() does that for us
    #endif

    #if !VESA
        console_init();

        //Enable our physical memory managers
        pmm_init();
        pmm_addBlock((uintptr_t)FRAMEBSE, 0x400, 1);
        paging_init();
    #endif

    //Check if initial ramdisk exists or not
    if(mbt -> mods_addr != 0 && mbt -> mods_count > 0)
    {
        // Get initrd addresses
        initrd_location = *((uint32_t*)(mbt->mods_addr + KRNLBASE));
        initrd_end = *((uint32_t*)(mbt->mods_addr + 4 + KRNLBASE));
        // Convert the addresses into linear
        initrd_location = (initrd_location - pageAlign((uint32_t)&_kernel_end - KRNLBASE) - 0x1000) + INRDBASE;
        initrd_end = (initrd_end - pageAlign((uint32_t)&_kernel_end - KRNLBASE) - 0x1000) + INRDBASE;
    }

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

    //Initialize ACPI and enable it
    initAcpi();
    acpiEnable();

    //Get memory information
    getMemDisplay(mbt);
    bprintinfo(); kprintf("Kernel heap starts: %X Kernel size: %X\n", &_kernel_end, kheap -> fblock -> size);

    vfs_init();
    //Enable our initrd
    if(mbt -> mods_count > 0)
        initInitrd(initrd_location, initrd_end);
    else
        PANIC("Initial ramdisk not properly loaded!");

    //Print OS OK text
    bprintok(); console_write("OS ready!\n");
    doBootLog = false; //End of boot sequence
    // Enable interrupts
    asm volatile("sti");

    //Enable our terminal if real terminal doesn't exist
    //if(!vfs_exists("/initrd/terminal.elf"))
        init_terminal(mbt);
    //else
    //{
        //asm volatile("cli"); // Disable interrupts
        //kprintf("Real terminal not implemented yet!");
    //}

    halt(); // Needed for interrupts to work properly - Prevents the kernel from exiting early
    kprintf("\n Uh, oh, kernel has reached the end... halting!");
    print_dalek(); //When we screwed over all the things, just call the lord of destruction
}

void testInt()
{

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
    ptr = (char*)kmalloc(kheap, 4);
    *ptr = 4;
    //kprintf("%X\n%X", ptr, *ptr); //Should display 0x4 on the second line
    k_heapBMFree(kheap, ptr);

    //If there's a nonexistent pointer
    char* _ppap = 0; kfree(kheap, _ppap);
    #endif
}
