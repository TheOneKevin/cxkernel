/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   interrupts.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on January 03 2019, 3:20 PM
 */

#pragma once
#include "common.h"

#if ARCH_TYPE == ARCH_x86_32
    #include "arch/x86/32/idt.h"
#elif ARCH_TYPE == ARCH_x86_64
    #include "arch/x86/64/idt.h"
#endif
