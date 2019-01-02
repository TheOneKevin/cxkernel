/**
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Tuesday, November 13th 2018, 8:39:05 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:48:04-05:00
 */

#include "arch/interface.h"
#include "arch/x86/32/gdt.h"
#include "arch/x86/32/idt.h"

namespace x86_32 {

void early_init()
{
    gdt::init();
    idt::init();
}

void init()
{

}

}
