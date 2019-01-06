/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   init.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 8 2018, 5:12 PM
 */

#define __MODULE__ "ARCH"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "platform/console.h"
#include "arch/interface.h"
#include "arch/x86/multiboot.h"
#include "arch/x86/global.h"

namespace arch
{
    /**
     * Early architecture initialization procedure. Sets up system
     * and collects information from bootloader. Mainly here to bootstrap
     * the physical and virtual memory managers.
     * @param sig Magic boot verification number.
     * @param ptr Pointer to any boot data structure(s).
     */
    void early_init(loader_t args)
    {
#if ARCH_TYPE == ARCH_x86_32
        x86_32::early_init(args);
#elif ARCH_TYPE == ARCH_x86_64
        x86_64::early_init(args);
#endif
    }

    void init(void)
    {
#if ARCH_TYPE == ARCH_x86_32
        x86_32::init();
#elif ARCH_TYPE == ARCH_x86_64
        x86_64::init();
#endif
    }
} // namespace arch