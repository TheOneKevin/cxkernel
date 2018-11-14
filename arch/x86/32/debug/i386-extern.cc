/**
 * @file   i386-extern.c
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, May 27th 2018, 9:21:35 am
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:37:15-05:00
 */

#include "platform/pc/serial.h"
#include "arch/x86/interrupts.h"

using namespace pc;

/* write a single character */
extern "C" void putDebugChar(char c)
{
    serial::write(COM2, c);
}

/* read and return a single char */
extern "C" int getDebugChar()
{
    char c = serial::read(COM2);
    return c;
}

/* assign an exception handler */
extern "C" void exceptionHandler(int a, unsigned int b)
{
    x86::idt::set_gate(a, b, 0x08, 0x8E);
}

extern "C" void initGdbSerial()
{
    serial::init(COM2);
}