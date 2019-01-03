/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   types.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 06 2017, 10:28 AM
 *
 * \sa include/arch/x86/interface/arch_types.h
 * \sa include/arch/raspi/interface/arch_types.h
 * \sa include/arch/dummy/interface/arch_types.h
 */

#pragma once

#include "common.h"
#include STRINGIFY(arch/TARGET_FOLDER/interface/arch_types.h)

__BEGIN_CDECLS

struct MEMORY_MAP
{
    virt_t KRN_MMAP;
    virt_t KRN_MMAP_END;
    virt_t KRN_PRIV_BEGIN;
    virt_t KRN_PRIV_END;
    virt_t KRN_BEGIN;
    virt_t KRN_END;
    virt_t KRN_BRK;
    virt_t KRN_SLIM;
    uint32_t KRN_STACK_SIZE;
    virt_t KRN_STACK_BEGIN;
    virt_t KRN_PAGE_TABLES_BEGIN;

    phys_t MOD_END;
};

__END_CDECLS
