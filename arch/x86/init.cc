/**
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 5:12:18 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T16:43:47-04:00
 */

#define __MODULE__ "ARCH "

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "console.h"
#include "arch/arch_interface.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/global.h"

namespace Arch {

// Reserve spaces for structs
static multiboot_info_t mbt;

/**
 * Early architecture initialization procedure. Sets up system
 * and collects information from bootloader. Mainly here to bootstrap
 * the physical and virtual memory managers.
 * @param sig Magic boot verification number.
 * @param ptr Pointer to any boot data structure(s).
 */
void EarlyInit(int sig, void* ptr)
{
    // Save the multiboot info pointer in a global variable
    memcpy(&mbt, ptr, sizeof(multiboot_info_t));
    g_mbt_struct = &mbt;
    // Multiboot sanity check
    OS_PRN("%-66s", "Checking multiboot integrity... ");
    fflush(STREAM_OUT);
    ASSERT_HARD(sig == MULTIBOOT_BOOTLOADER_MAGIC, "Magic number is invalid.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 0), "No memory information provided. Kernel cannot continue.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 3) && mbt.mods_addr != 0 && mbt.mods_count > 0, "No module(s) loaded. Check GRUB config file.");
    ASSERT_HARD(!((CHECK_FLAG(mbt.flags, 4) && CHECK_FLAG(mbt.flags, 5))), "Flags 4 and 5 are mutually exclusive.");
    ASSERT_HARD(CHECK_FLAG(mbt.flags, 6), "Memory map not loaded. Kernel cannot continue execution.");
    fprintf(STREAM_OUT, "DONE!\n");


}

void Init(void)
{

}

}