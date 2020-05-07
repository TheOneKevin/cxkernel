/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   global.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 14 2018, 9:14 AM
 */

#pragma once

#include "common.h"
#include "system.h"
#include "arch/x86/multiboot.h"

#ifdef __cplusplus

namespace x86::g
{
    extern elf::Context ctx;
    extern multiboot_info_t* mbt;
    extern virt_t mods_end;
    extern phys_t max_mem;
    extern loader_t* loader;
}

#endif
