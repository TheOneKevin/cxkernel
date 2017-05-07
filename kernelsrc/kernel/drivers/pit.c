/*
 * The system PIT timer
 */

#include "arch/exceptions.h"
#include "drivers/pit.h"
#include "display/tdisplay.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

uint32_t tick = 0;
uint32_t max_tick = 0;

/*static void timer_callback(regs_t *r)
{
   tick++;
   console_write("Tick: ");
   console_write_dec(tick);
   console_write("\n");
}*/

void init_timer(uint32_t frequency, irq_t handler)
{
   // IRQ0 is the PIT IRQ
   install_handler(IRQ0, handler); // We set the timer as a pointer and then register it

   // The value we send to the PIT is the value to divide it's input clock by, to get our required frequency.
   // Important to note is that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = (3579545L / 3) / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
}

static void delay_tick(regs_t *r)
{
    tick++;
    if(tick >= max_tick)
    {
        uninstall_handler(IRQ0);
    }
}

void delay(uint32_t ms)
{
    tick = 0;
    max_tick = ms;
    init_timer(100, &delay_tick);
}
