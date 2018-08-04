/*
 * File:   kernel.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
  * @date Created on 2017-08-25T16:37:39-04:00
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2017-11-09T11:53:43-05:00
*/

#include "common.h"
#include "arch/arch_interface.h"
#include "core/console.h"

#include "mm/sbrk.h"
#include "mm/page_alloc.h"

#include "list.h"
#include "tasking/task.h"

typedef struct
{
    list_t list;
    int id;
} myStruct;

void kernel_main(uint32_t sig, void* ptr)
{
    console_init();             // Initialize console (early)
    arch_early_init(sig, ptr);  // Arch early init
    arch_pmeminit();            // Set up physical memory for pmm
    sbrk_init();                // Set up sbrk for malloc to use
    pmm_init();                 // ?
    arch_init();                // Start init sequence
    arch_late_init();           // Start post init sequence (clean up, syscalls, etc...)

    arch_timer_install();
    init_tasking();
    
    for(;;);
}
