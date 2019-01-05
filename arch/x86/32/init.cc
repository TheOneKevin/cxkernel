/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 13 2018, 8:39 PM
 */
#include <stdio.h>
#include "platform/interrupts.h"
#include "arch/interface.h"
#include "arch/x86/32/gdt.h"
#include "arch/x86/32/idt.h"

extern "C" uint32_t isr_stub_table;

namespace x86_32
{
    void early_init()
    {
        gdt::init();
        idt::init();
        for(int i = 0; i < 256; i++)
            idt::set_gate(i, (&isr_stub_table)[i], 0x08, 0x8E);
    }

    void init()
    {
        irq_install_handler(0, [](void* c) {
            auto* r = (regs_t*)c;
            printf("Caught exception %X!\n", r->int_no);
            for(;;);
        });
    }
} // namespace x86_32
