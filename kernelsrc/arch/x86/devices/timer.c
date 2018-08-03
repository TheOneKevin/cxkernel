/**
 * File:   timer.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 * 
 * @date Created on Sunday, July 22nd 2018, 6:57:01 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-22T22:24:38-04:00
 */

#include "tasking/task.h"
#include "arch/x86/pic.h"
#include "arch/x86/llio.h"
#include "arch/x86/interrupts.h"
#include "arch/arch_interface.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define PIT_CH0     0x40
#define PIT_CH1     0x41
#define PIT_CH2     0x42
#define PIT_CTRL    0x43
#define PIT_SET_DIV 0x36
#define PIT_SCALE   1193182
#define TIMER_IRQ   0
#define TIMER_FREQ  100

static uint64_t timer_ticks = 0;
static uint64_t timer_real_ticks = 0;

void timer_set_phase(int hz)
{
    int div = PIT_SCALE / hz;    // Approx divisor from freq
    outb(PIT_CTRL, PIT_SET_DIV); // Tell PIT we want to change freq div
    outb(PIT_CH0, div & 0xFF);        // Set lo 8 bits
    outb(PIT_CH0, (div >> 8) & 0xFF); // Set hi 8 bits
}

void timer_handler(regs_t* r)
{
    if(++timer_real_ticks == 100)
    {
        timer_ticks++;
        timer_real_ticks = 0;
    }

    PIC_sendEOI(TIMER_IRQ);
    PIC_acknowledge();

    switch_task(true);
}

void arch_timer_install(void)
{
    install_irqhandler(TIMER_IRQ, timer_handler);
    timer_set_phase(100);
}