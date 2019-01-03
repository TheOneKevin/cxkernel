/**
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:18:56 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T21:04:52-05:00
 */

#include "platform/console.h"
#include "platform.h"
#include "platform/pc/vga.h"
#include "platform/pc/pic.h"
#include "platform/pc/serial.h"

#include "arch/x86/interrupts.h"
using namespace pc;

namespace platform
{
    class internal_vga_cons_t : public Console
    {
    public:
        void log(char c) override
        {
            serial::write(COM1, c);
        }
        void putc(char c) override
        {
            vga::putc(c);
        }
        void clear(void) override
        {
            vga::clear(0);
        }
        char getc(void) override
        {
            return Console::getc();
        }
    };
    static internal_vga_cons_t __internal_vga_cons;

    Console &get_console(void)
    {
        return static_cast<Console &>(__internal_vga_cons);
    }

    void early_init(void)
    {
        pic::remap(IRQ0, IRQ0 + 8);
        pic::mask(0xFF, 0xFF);
        serial::init(COM1);
        serial::init(COM2);
    }

    void init(void)
    {

    }
} // namespace platform