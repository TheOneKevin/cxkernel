/**
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:18:56 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T08:52:18-04:00
 */

#include "console.h"
#include "platform/platform.h"
#include "platform/pc/vga.h"

class internal_vga_cons_t : public Console
{
public:
    virtual void putc(char c) { pc_terminal_putc(c); }
    virtual void clear(void)  { pc_terminal_clear(0); }
    virtual char getc(void)   { return Console::getc(); }
};
static internal_vga_cons_t __internal_vga_cons;



Console& Platform::GetConsole(void)
{
    return static_cast<Console&>(__internal_vga_cons);
}

void Platform::Init(void)
{

}
