/**
 * @file   pic.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T20:15:36-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Remaps the PIC interrupts from offset 1 to offset 2. Automatically enables PIC too. If you don't know what
 * you are doing, use the autoremap_PIC();
 * @param offset1 Offset for PIC1
 * @param offset2 Offset for PIC2
 */
void pic_remap(uint8_t offset1, uint8_t offset2);
//! Masks interrupts (set mask1 and mask2 to 0xFF to disable the 8259 PIC)
void pic_mask(uint8_t mask1, uint8_t mask2);
//! Sends the End Of Interrupt signal
void pic_sendEOI(uint8_t irq);
//! Sends the EOI after CPU exception
void pic_acknowledge(void);
//! \deprecated { Automatically remaps the PIC interrupts to interrupt 32. Automatically enables PIC too. }
DEPRECATED("Use pic_remap(0x20, 0x28) instead")
void pic_autoremap(void);

#ifdef __cplusplus
}
#endif
