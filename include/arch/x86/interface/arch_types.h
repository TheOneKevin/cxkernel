/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   arch_types.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on October 13 2018, 5:53 PM
 */

#pragma once
#include "common.h"

__BEGIN_CDECLS

#if ARCH_TYPE == ARCH_x86_32
    typedef uint32_t arch_sz_t;
    typedef uint32_t virt_t;
    typedef uint64_t phys_t;
#elif ARCH_TYPE == ARCH_x86_64
    typedef uint64_t arch_sz_t;
    typedef uint64_t virt_t;
    typedef uint64_t phys_t;
#endif

__END_CDECLS
