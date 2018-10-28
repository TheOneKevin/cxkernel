/**
 * @file   interrupts.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 27th 2018, 7:45:20 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T20:02:50-04:00
 */

#pragma once
#include "common.h"

#if ARCH_TYPE == ARCH_x86_32
    #include "arch/x86/32/idt.h"
#elif ARCH_TYPE == ARCH_x86_64
    #include "arch/x86/64/idt.h"
#endif
