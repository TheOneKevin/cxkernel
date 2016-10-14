/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arch/idt.h"
#include "system/pic.h"
#include "display/tdisplay.h"

void PIC_remap(int offset1, int offset2)
{
    unsigned char a1, a2;
    
    a1 = inb(PIC1_DATA);                        // save masks
    a2 = inb(PIC2_DATA);
 
    outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();
 
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
 
    outb(PIC1_DATA, a1);   // restore saved masks.
    outb(PIC2_DATA, a2);
}
 
void PIC_sendEOI(unsigned char irq)
{
    if(irq >= 8)
        outb(PIC2_COMMAND,PIC_EOI);
    outb(PIC1_COMMAND,PIC_EOI);
}

void autoremap_PIC()
{
    // Remap the irq table.
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
    bprintok(); console_writeline("Initialized and remapped PIC");
}