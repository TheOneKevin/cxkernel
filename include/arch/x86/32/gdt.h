/**
 * Copyright (c) 2019 The cxkernel authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 *
 * @file   gdt.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on November 26 2017, 9:42 PM
 */

#pragma once

#include "common.h"

typedef struct
{
   uint16_t limit_low;           //!< The lower 16 bits of the limit.
   uint16_t base_low;            //!< The lower 16 bits of the base.
   uint8_t  base_middle;         //!< The next 8 bits of the base.
   uint8_t  access;              //!< Access byte, usually to determine what ring this segment can be used in
   uint8_t  granularity;         //!< Flags (Gr, Sz, 0, 0), see Intel Developers Manual
   uint8_t  base_high;           //!< The last 8 bits of the base.
} __PACKED gdt_entry_t;

typedef struct
{
    uint32_t TSS_PREV;
    uint32_t ESP0, SS0,
             ESP1, SS1,
             ESP2, SS2;
    uint32_t CR3, EIP, EFLAGS;
    uint32_t EAX, ECX, EDX, EBX,
             ESP, EBP, ESI, EDI;
    uint32_t ES, CS, SS,
             DS, FS, GS;
    uint32_t LDT;

    uint16_t TRAP;
    uint16_t IO_MAP_BASE;
} __PACKED tss_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __PACKED gdt_ptr_t;

#ifdef __cplusplus

namespace x86_32::gdt
{
    void init(void);
    void set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);
    void set_tss_stack(uint32_t);
}

#endif
