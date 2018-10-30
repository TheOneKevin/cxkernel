/**
 * @file   main.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 27th 2018, 11:30:14 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-29T19:30:43-04:00
 * 
 * This is the main loader for our kernel. The loader should use
 * as little code as possible. Any source files used from the kernel
 * must be added to sources.mk, as to deter heavy reliance on kernel code.
 * Standard libraries and i/o should be enough. As the codebase is small
 * and isolated, we don't need to worry too much about header files
 * and good extensibility and such. We can just manually extern in symbols :)
 */

#define __MODULE__ "LODR"

// Shared common files (nothing kernel related)
#include "common.h"
#include "arch/x86/multiboot.h"
// Standard library
#include <stdio.h>
#include <string.h>
#include <assert.h>

// CPU Topology
#include "arch/x86/cpu.h"

// Random ctor shit
typedef void(*ctor_func)(void);
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NO_OPTIMIZE __NOINLINE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

// Reserve spaces for structs
static multiboot_info_t mbt;

void main(int sig, multiboot_info_t* ptr)
{
    // Before anything bad happens
    memcpy(&mbt, ptr, sizeof(multiboot_info_t));
    // Construct everything before we do anything
    for(ctor_func* func_arr = &_ctors_start; func_arr != &_ctors_end; func_arr++) (*func_arr)();
    OS_PRN("%-66s", "Checking multiboot integrity... ");
    fflush(STREAM_OUT);
    ASSERT_HARD(sig == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 0), "No memory information provided. Loader cannot continue.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 3) && mbt.mods_addr != 0 && mbt.mods_count > 0, "No module(s) loaded. Check GRUB config file.");
    ASSERT_HARD(!((CHECK_FLAG(mbt.flags, 4) && CHECK_FLAG(mbt.flags, 5))), "Flags 4 and 5 are mutually exclusive.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 6), "Memory map not loaded. Loader cannot continue execution.");
    fprintf(STREAM_OUT, "DONE!\n");
    // Find kernel(s)
    bool foundKrnl32 = false, foundKrnl64 = false;
    multiboot_module_t *cxkrnl32, *cxkrnl64;
    for(multiboot_module_t* mod = (multiboot_module_t *) mbt.mods_addr; (uint32_t) mod < mbt.mods_addr + mbt.mods_count * sizeof(multiboot_module_t); mod++)
    {
        printf("Found module [%X, %X] (%s)\n", mod -> mod_start, mod -> mod_end, (char *) mod -> cmdline);
        if(strcmp((char *) mod -> cmdline, "cxkrnl32") == 0)
        {
            if(foundKrnl32)
                printf("Warning: Duplicate cxkrnl32 modules found. Ignoring.\n");
            else
                foundKrnl32 = true, cxkrnl32 = mod;
        }
        else if(strcmp((char *) mod -> cmdline, "cxkrnl64") == 0)
        {
            if(foundKrnl64)
                printf("Warning: Duplicate cxkrnl64 modules found. Ignoring.\n");
            else
                foundKrnl64 = true, cxkrnl64 = mod;
        }
    }
    // Check if the modules are found
    ASSERT_HARD(foundKrnl32, "Error: 32 bit kernel not found, aborting...");
    ASSERT(foundKrnl64 && cpu_check_feat(CPU_FEAT_EDX_PAE), "Warning: 64 bit kernel not found");
    // Execute kernel
    if (CHECK_FLAG(mbt.flags, 2))
    {
        printf ("Passed cmdline = [%s]\n", (char *) mbt.cmdline);
    }
    else
    {

    }
    for(;;);
}
