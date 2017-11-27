/*
 * File:   pic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 05:19:00 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 05:34:57 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIC1	    	0x20		/* IO base address for master PIC */
#define PIC2	    	0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA   	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define PIC_EOI         0x20

#define ICW1_ICW4	    0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */

#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM    	0x10		/* Special fully nested (not) */

// Remaps the PIC interrupts from offset 1 to offset 2. Automatically enables PIC too. If you don't know what
// you are doing, use the autoremap_PIC();
void PIC_remap(int offset1, int offset2);
// Automatically remaps the PIC interrupts to interrupt 32. Automatically enables PIC too.
void PIC_autoremap();
// Sends the End Of Interrupt signal
void PIC_sendEOI(uint8_t irq);
// Sends the EOI after CPU exception
void PIC_acknowledge();

#ifdef __cplusplus
}
#endif
