/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   interface.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 *
 * All platforms must implement this file and override any
 * defines as needed.
 * \sa include/arch/dummy/interface/arch_interface.h
 */

#pragma once

#include "common.h"
#include STRINGIFY(arch/TARGET_FOLDER/interface/arch_interface.h)

#ifndef ARCH_PAGE_SIZE
    #define ARCH_PAGE_SIZE
#endif
#ifndef ARCH_SYSCALL_INT_NO
    #define ARCH_SYSCALL_INT_NO
#endif
#ifndef ARCH_PAGE_ALIGN
    #define ARCH_PAGE_ALIGN
#endif
#ifndef ARCH_PAGE_ALIGN_DOWN
    #define ARCH_PAGE_ALIGN_DOWN
#endif
#ifndef ENABLE_INTERRUPTS
    #define ARCH_PAGE_ALIGN_DOWN
#endif
#ifndef DISABLE_INTERRUPTS
    #define DISABLE_INTERRUPTS
#endif

#ifdef __cplusplus

namespace arch
{
    void early_init(int, void*);
    void init(void);
}

#endif
