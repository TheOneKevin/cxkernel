/**
 * @file   platform.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Saturday, October 13th 2018, 6:18:56 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-13T21:04:52-05:00
 */

#include <string.h>
#include "platform/interrupts.h"
#include "platform/console.h"
#include "platform.h"
#include "platform/pc/vga.h"
#include "platform/pc/pic.h"
#include "platform/pc/serial.h"

#if ARCH_TYPE == ARCH_x86_32
#include "arch/x86/32/idt.h"
#elif ARCH_TYPE == ARCH_x86_64
#include "arch/x86/64/idt.h"
#endif

namespace pc
{
    namespace console
    {
        class ConsoleImpl : public platform::Console
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
        static ConsoleImpl __internal_vga_cons;
    }  // namespace console

    namespace interrupts
    {
        static irq_handler_t handlers[255]; // Should be enough
        class IrqImpl : public platform::Irq
        {
        public:
            void install_handler(int n, irq_handler_t handler) override
            {
                handlers[n] = handler;
            }

            void remove_handler(int n) override
            {
                handlers[n] = nullptr;
            }

            irq_handler_t get_handler(int n) override
            {
                return handlers[n];
            }
        };
        static IrqImpl __internal_irq;
    }  // namespace interrupts
}  // namespace pc

namespace platform
{
    using namespace pc;
    Console &get_console(void)
    {
        return static_cast<Console &>(console::__internal_vga_cons);
    }

    void early_init(void)
    {
        pic::remap(IRQ0, IRQ0 + 8);
        pic::mask(0xFF, 0xFF);
        serial::init(COM1);
        serial::init(COM2);
        memset(interrupts::handlers, 0, sizeof(interrupts::handlers));
    }

    void init(void)
    {

    }

    Irq& get_irq()
    {
        return static_cast<Irq &>(interrupts::__internal_irq);
    }
} // namespace platform