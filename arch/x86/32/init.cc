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
#include "arch/interface.h"
#include "arch/x86/32/gdt.h"
#include "arch/x86/32/idt.h"

namespace x86_32
{
    void early_init()
    {
        gdt::init();
        idt::init();
    }

    void init()
    {

    }
} // namespace x86_32
