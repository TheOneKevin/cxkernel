/**
 * @file   main.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 27th 2018, 11:30:14 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-28T15:49:09-04:00
 * 
 * This is the main loader for our kernel. The loader should use
 * as little code as possible. Any source files used from the kernel
 * must be added to sources.mk, as to deter heavy reliance on kernel code.
 * Standard libraries and i/o should be enough. As the codebase is small
 * and isolated, we don't need to worry too much about header files
 * and good extensibility and such. We can just manually extern in symbols :)
 */

// Shared common files (nothing kernel related)
#include "common.h"
#include "arch/x86/multiboot.h"
#include <stdio.h>

// Random ctor shit
typedef void(*ctor_func)(void);
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NO_OPTIMIZE __NOINLINE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

// Externs
extern void console_init(void);         // vga.c
extern void init_early_handles(void);   // printk.c

void main(int sig, multiboot_info_t* ptr)
{
    // Should we use ctors instead?
    init_early_handles();
    console_init();
    printf("Hello, World!\n:)\n");
    for(;;);
}
