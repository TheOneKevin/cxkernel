/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   pic.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 */

#pragma once

#include "common.h"

namespace pc::pic
{
    /**
     * Remaps the PIC interrupts from offset 1 to offset 2. Automatically enables PIC too. If you don't know what
     * you are doing, use the autoremap_PIC();
     * @param offset1 Offset for PIC1
     * @param offset2 Offset for PIC2
     */
    void remap(uint8_t offset1, uint8_t offset2);

    //! Masks interrupts (set mask1 and mask2 to 0xFF to disable the 8259 PIC)
    void mask(uint8_t mask1, uint8_t mask2);

    //! Sends the End Of Interrupt signal
    void send_eoi(uint8_t irq);

    //! Sends the EOI after CPU exception
    void ack(void);

    //! \deprecated { Automatically remaps the PIC interrupts to interrupt 32. Automatically enables PIC too. }
    DEPRECATED("Use pic_remap(0x20, 0x28) instead")
    void autoremap(void);
} // namespace pc::pic
