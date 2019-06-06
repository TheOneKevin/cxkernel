/**
 * Copyright (c) 2018 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   system.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 24 2018, 5:56 PM
 */

#pragma once

#include "common.h"
#include "arch/types.h"
#include "arch/x86/multiboot.h"

#include <bitmap.h>
#include <elf_parser.h>
#include <linked_list.h>

__BEGIN_CDECLS

typedef struct
{
    int magic;
    bitmap_t* bitmap;
    elf::Context& ctx;
    void* obj;
} loader_t;

__END_CDECLS

#ifdef __cplusplus

namespace g
{
    extern loader_t* loader;
}

#endif
