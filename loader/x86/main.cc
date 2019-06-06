/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   main.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 27 2018, 11:30 PM
 *
 * This is the main loader for our kernel. The loader should use
 * as little code as possible. Any source files used from the kernel
 * must be added to sources.mk, as to deter heavy reliance on kernel code.
 * Standard libraries and i/o should be enough. As the codebase is small
 * and isolated, we don't need to worry too much about header files
 * and good extensibility and such. We can just manually extern in symbols :)
 *
 */

#define __MODULE__ "LODR"

// Internal includes
#include "include/global.h"
// Shared common files (nothing kernel related)
#include "common.h"
#include "system.h"
#include "arch/x86/multiboot.h"
// Standard library
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <panic.h>
#include <math.h>
// Elf parser library
#include <elf.h>
#include <elf_parser.h>
#include <math.h>
// Arch-specific stuff
#include "arch/x86/cpu.h"
#include "arch/x86/arch_utils.h"
#include "arch/x86/interface/arch_types.h"

#pragma GCC diagnostic ignored "-Wunused-variable"

// Random ctor shit
using ctor_func = void (*)();
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NO_OPTIMIZE __NOINLINE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

using namespace loader;

// Globals
multiboot_info_t loader::g_mbt;
int loader::g_sig;
virt_t loader::MODS_END;
phys_t loader::MAX_MEM;
bool loader::g_load64 = false;

// File globals
static multiboot_module_t *cxkrnl32, *cxkrnl64;
static elf::Context ctx;

void init32()
{
    g_load64 = false;
    initgdt32();
    init_bootmm32();
    elf::load_img(reinterpret_cast<void*>(cxkrnl32 -> mod_start), ctx);
    ASSERT_HARD(ctx.strtab32 != nullptr, "Malformed kernel (no strtab).");
    ASSERT_HARD(ctx.symtab32 != nullptr, "Malformed kernel (no symtab).");
    loader::get_mmu().init();
    init_pps32();
    loader::run_program32(ctx);
    for(;;);
}

void init64()
{
    g_load64 = true;
}

extern "C" void main(int sig, multiboot_info_t* ptr)
{
    // Before anything bad happens
    memcpy(&g_mbt, ptr, sizeof(multiboot_info_t));
    g_sig = sig;
    // Construct everything before we do anything
    for(ctor_func* func_arr = &_ctors_start; func_arr != &_ctors_end; func_arr++) (*func_arr)();

    // Check to ensure our structure is intact
    OS_PRN("%-66s", "Checking multiboot integrity... ");
    fflush(STREAM_OUT);
    ASSERT_HARD(g_sig == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
    ASSERT_HARD(!!CHECK_FLAG(g_mbt.flags, 0), "No memory information provided. Loader cannot continue.");
    ASSERT_HARD(CHECK_FLAG(g_mbt.flags, 3) && g_mbt.mods_addr != 0 && g_mbt.mods_count > 0, "No module(s) loaded. Check GRUB config file.");
    ASSERT_HARD(!((CHECK_FLAG(g_mbt.flags, 4) && CHECK_FLAG(g_mbt.flags, 5))), "Flags 4 and 5 are mutually exclusive.");
    ASSERT_HARD(!!CHECK_FLAG(g_mbt.flags, 6), "Memory map not loaded. Loader cannot continue execution.");
    fprintf(STREAM_OUT, "DONE!\n");

    // Find kernel(s)
    bool foundKrnl32 = false, foundKrnl64 = false;

    { // Begin local scope
        int i = 0;
        multiboot_module_t* mod;
        OS_PRN("%d %s\n", g_mbt.mods_count, g_mbt.mods_count == 1 ? "module loaded." : "modules loaded.");
        for(mod = (multiboot_module_t*) g_mbt.mods_addr; (uint32_t) mod < g_mbt.mods_addr + g_mbt.mods_count * sizeof(multiboot_module_t); mod++)
        {
            bool is32 = !memcmp((void*) mod->cmdline, "cxkrnl32", 8);
            bool is64 = !memcmp((void*) mod->cmdline, "cxkrnl64", 8);
            MODS_END = MAX(MODS_END, mod->mod_end);
            OS_DBG("%02d Location: 0x%08X to 0x%08X (%s)\n", ++i, mod->mod_start, mod->mod_end, is32 || is64 ? "kernel" : "module");
            if(is32)
            {
                if(!foundKrnl32)
                {
                    foundKrnl32 = true, cxkrnl32 = mod;
                    continue;
                }
                printf("Warning: Duplicate cxkrnl32 modules found. Ignoring.\n");
            }
            else if(is64)
            {
                if(!foundKrnl64)
                {
                    foundKrnl64 = true, cxkrnl64 = mod;
                    continue;
                }
                printf("Warning: Duplicate cxkrnl64 modules found. Ignoring.\n");
            } // End if
        } // End for
    } // End scope

    // Check if the modules are found
    ASSERT_HARD(foundKrnl32 || foundKrnl64, "Error: No kernels loaded.");
    ASSERT(foundKrnl64 || !x86_feature_test(x86_FEATURE_LONGMODE), "Warning: CPU supports x86_64 but no 64 bit kernel found");
    ASSERT(!foundKrnl64 || x86_feature_test(x86_FEATURE_LONGMODE), "Warning: No 64 bit kernel found but CPU supports x86_64");

    // Execute kernel
    // first check if the user has forced a command line argument,
    // then execute based on the CPU specifications
    if (CHECK_FLAG(g_mbt.flags, 2))
    {
        printf("Passed cmdline = [%s]\n", (char *) g_mbt.cmdline);
        if(!memcmp((void *) g_mbt.cmdline, "exec32", 6))
        {
            ASSERT_HARD(foundKrnl32, "Force 32 bit execute error: 32 bit kernel not found, aborting...");
            init32();
        }
        else if(!memcmp((void *) g_mbt.cmdline, "exec64", 6))
        {
            ASSERT_HARD(foundKrnl64, "Force 64 bit execute error: 64 bit kernel not found, aborting...");
            ASSERT_HARD(x86_feature_test(x86_FEATURE_LONGMODE), "Error: CPU does not support long mode.");
            init64();
        }
        else
        {
            fprintf(STREAM_ERR, "Unknown flag: [%s]\n", (char *) g_mbt.cmdline);
            PANIC("We cannot continue (x.x)");
        }
    }
    else
    {
        if(x86_feature_test(x86_FEATURE_LONGMODE) && foundKrnl64) init64();
        else if(foundKrnl32) init32();
        else PANIC("Error: No compatible kernel found.");
    }

    // Hang forever
    for(;;);
}
