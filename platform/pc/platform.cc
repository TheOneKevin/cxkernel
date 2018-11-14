/**
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:18:56 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T20:26:45-05:00
 */

#include "console.h"
#include "platform.h"
#include "platform/pc/vga.h"
#include "platform/pc/pic.h"
#include "platform/pc/serial.h"

#include "arch/x86/interrupts.h"

using namespace pc;

class internal_vga_cons_t : public Console
{
public:
    virtual void putc(char c) { pc_terminal_putc(c); }
    virtual void clear(void)  { pc_terminal_clear(0); }
    virtual char getc(void)   { return Console::getc(); }
};
static internal_vga_cons_t __internal_vga_cons;

Console& platform::get_console(void)
{
    return static_cast<Console&>(__internal_vga_cons);
}

void platform::early_init(void)
{
    pic::remap(IRQ0, IRQ0 + 8);
    pic::mask(0xFF, 0xFF);
    serial::init(COM1);
    serial::init(COM2);
}

void platform::init(void)
{

}
