/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   pic.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on July 30 2017, 5:17 PM
 */

#include "arch/x86/llio.h"
#include "platform/pc/pic.h"

#define PIC1_COMMAND    0x20        //!< IO base address for master PIC
#define PIC2_COMMAND    0xA0        //!< IO base address for slave PIC
#define PIC1_DATA       (PIC1_COMMAND+1)
#define PIC2_DATA       (PIC2_COMMAND+1)

#define PIC_EOI             0x20

#define ICW1_ICW4           0x01        //!< ICW4 (not) needed
#define ICW1_SINGLE         0x02        //!< Single (cascade) mode
#define ICW1_INTERVAL4      0x04        //!< Call address interval 4 (8)
#define ICW1_LEVEL          0x08        //!< Level triggered (edge) mode
#define ICW1_INIT           0x10        //!< Initialization - required!
#define ICW4_8086           0x01        //!< 8086/88 (MCS-80/85) mode
#define ICW4_AUTO           0x02        //!< Auto (normal) EOI
#define ICW4_BUF_SLAVE      0x08        //!< Buffered mode/slave
#define ICW4_BUF_MASTER     0x0C        //!< Buffered mode/master
#define ICW4_SFNM           0x10        //!< Special fully nested (not)

using namespace pc;
namespace pc::pic
{
    void remap(uint8_t offset1, uint8_t offset2)
    {
        uint8_t a1, a2;
        a1 = inb(PIC1_DATA);                        // Save masks
        a2 = inb(PIC2_DATA);

        outb(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4);  // Starts the initialization sequence (in cascade mode)
        outb(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);
        outb(PIC1_DATA, offset1);                   // ICW2: Master PIC vector offset
        outb(PIC2_DATA, offset2);                   // ICW2: Slave PIC vector offset
        outb(PIC1_DATA, 4);                         // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
        outb(PIC2_DATA, 2);                         // ICW3: tell Slave PIC its cascade identity (0000 0010)

        outb(PIC1_DATA, ICW4_8086);
        outb(PIC2_DATA, ICW4_8086);

        outb(PIC1_DATA, a1); // Restore saved masks.
        outb(PIC2_DATA, a2);
    }

    void mask(uint8_t mask1, uint8_t mask2)
    {
        outb(PIC1_DATA, mask1);
        outb(PIC2_DATA, mask2);
    }

    void send_eoi(uint8_t irq)
    {
        if(irq >= 8)
            outb(PIC2_COMMAND, PIC_EOI);
        outb(PIC1_COMMAND, PIC_EOI);
    }

    void ack(void)
    {
        outb(PIC1_COMMAND, PIC_EOI);
    }

    void autoremap(void)
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
} // namespace pc::pic