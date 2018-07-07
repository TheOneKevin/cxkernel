/**
 * @file   pic.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-05T20:42:39-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIC1                0x20        /* IO base address for master PIC */
#define PIC2                0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND         PIC1
#define PIC1_DATA           (PIC1+1)
#define PIC2_COMMAND         PIC2
#define PIC2_DATA           (PIC2+1)

#define PIC_EOI             0x20

#define ICW1_ICW4           0x01        /* ICW4 (not) needed */
#define ICW1_SINGLE         0x02        /* Single (cascade) mode */
#define ICW1_INTERVAL4      0x04        /* Call address interval 4 (8) */
#define ICW1_LEVEL          0x08        /* Level triggered (edge) mode */
#define ICW1_INIT           0x10        /* Initialization - required! */

#define ICW4_8086           0x01        /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO           0x02        /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE      0x08        /* Buffered mode/slave */
#define ICW4_BUF_MASTER     0x0C        /* Buffered mode/master */
#define ICW4_SFNM           0x10        /* Special fully nested (not) */

// Remaps the PIC interrupts from offset 1 to offset 2. Automatically enables PIC too. If you don't know what
// you are doing, use the autoremap_PIC();
void PIC_remap(int offset1, int offset2);
// Automatically remaps the PIC interrupts to interrupt 32. Automatically enables PIC too.
void PIC_autoremap(void);
// Sends the End Of Interrupt signal
void PIC_sendEOI(uint8_t irq);
// Sends the EOI after CPU exception
void PIC_acknowledge(void);

#ifdef __cplusplus
}
#endif
