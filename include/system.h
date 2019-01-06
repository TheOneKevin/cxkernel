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
#include <elf_parser.h>

__BEGIN_CDECLS

typedef struct
{
    virt_t CX_SLAB_BASE;
    virt_t CX_SLAB_BREAK;
    virt_t CX_IMAGE_BASE;
    virt_t CX_IMAGE_END;
    virt_t CX_PFN_DATABASE_BEGIN;
    virt_t CX_PFN_DATABASE_END;
    virt_t CX_STACK_LIMIT;
    virt_t CX_STACK_BOTTOM;
    virt_t CX_PAGE_TABLES_BEGIN;
    virt_t CX_PAGE_TABLES_END;
} sysinfo_t;

typedef struct
{
    int magic;
    elf::Context& ctx;
    void* obj;
} loader_t;

__END_CDECLS
