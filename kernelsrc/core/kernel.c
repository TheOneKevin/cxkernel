/*
 * File:   kernel.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-25T16:37:39-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-09T11:53:43-05:00
*/

#include "arch/arch_interface.h"
#include "common.h"
#include "core/console.h"

void kernel_main(uint32_t sig, void* ptr)
{
    console_init();             // Initialize console (early)
    arch_early_init(sig, ptr);  // Arch early init
    arch_pmeminit();            // Set up physical memory for pmm

    arch_init();

    arch_late_init();

    for(;;);
}
