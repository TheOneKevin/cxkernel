/**
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Monday, October 8th 2018, 5:12:18 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:46:25-05:00
 */

#define __MODULE__ "ARCH"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "console.h"
#include "arch/interface.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/global.h"

namespace arch {

// Reserve spaces for structs
static multiboot_info_t mbt;

/**
 * Early architecture initialization procedure. Sets up system
 * and collects information from bootloader. Mainly here to bootstrap
 * the physical and virtual memory managers.
 * @param sig Magic boot verification number.
 * @param ptr Pointer to any boot data structure(s).
 */
void early_init(int sig, void* ptr)
{
#if ARCH_TYPE == ARCH_x86_32
    x86_32::early_init();
#elif ARCH_TYPE == ARCH_x86_64
    x86_64::early_init();
#endif
}

void init(void)
{
#if ARCH_TYPE == ARCH_x86_32
    x86_32::init();
#elif ARCH_TYPE == ARCH_x86_64
    x86_64::init();
#endif
}

} // namespace arch