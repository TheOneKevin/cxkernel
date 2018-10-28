/**
 * @file   gdt.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T19:58:10-04:00
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
extern "C" {
#endif

void init_gdt(void);
void gdt_set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);
void set_tss_stack(uint32_t);

#ifdef __cplusplus
}
#endif
