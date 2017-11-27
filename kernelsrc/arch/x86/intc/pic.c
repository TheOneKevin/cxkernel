/*
 * File:   pic.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 05:17:59 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:44:29-05:00
 */

#include "arch/x86/pic.h"
#include "arch/x86/llio.h"

void PIC_remap(int offset1, int offset2)
{
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA);                        // Save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);  // Starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);                   // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2);                   // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);                         // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                         // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1); // Restore saved masks.
    outb(PIC2_DATA, a2);
}

inline void PIC_sendEOI(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

inline void PIC_acknowledge(void)
{
    outb(PIC1_COMMAND, PIC_EOI);
}

inline void PIC_autoremap(void)
{
    // Remap the IRQ table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}
