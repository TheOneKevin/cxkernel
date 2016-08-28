/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arch/exceptions.h"
#include "system/pit.h"
#include "system/irq.h"
#include "system/tdisplay.h"

uint32_t tick = 0;

static void timer_callback(regs_t *regs)
{
   tick++;
   console_write("Tick: ");
   console_write_dec(tick);
   console_write("\n");
}

void init_timer(uint32_t frequency)
{
   // IRQ0 is the PIT IRQ
   install_handler(IRQ0, &timer_callback); // We set the timer as a pointer and then register it

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);
} 