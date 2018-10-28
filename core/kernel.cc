/**
 * @file   kernel.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, October 7th 2018, 4:12:12 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T22:57:54-04:00
 */

#include <stdio.h>
#include <icxxabi.h>

#include "platform.h"
#include "arch/arch_interface.h"

// Random ctor shit
typedef void(*ctor_func)(void);
extern ctor_func _ctors_start;
extern ctor_func _ctors_end;
__NO_OPTIMIZE __NOINLINE void dummy_ctor(void) { } EXPORT_CTOR(dummy_ctor);

extern "C" void kernel_main(int sig, void* ptr)
{
    // Execute ctors, these really only initializes printf
    for(ctor_func* func_arr = &_ctors_start; func_arr != &_ctors_end; func_arr++)
        (*func_arr)();

    Platform::GetConsole().clear();
    Arch::EarlyInit(sig, ptr);
    Platform::Init();
    Arch::Init();
    
    for(;;);
}
