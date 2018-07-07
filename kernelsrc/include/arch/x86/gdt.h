/**
 * @file   gdt.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-06T16:47:04-04:00
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gdt_entry
{
   uint16_t limit_low;           //!< The lower 16 bits of the limit.
   uint16_t base_low;            //!< The lower 16 bits of the base.
   uint8_t  base_middle;         //!< The next 8 bits of the base.
   uint8_t  access;              //!< Access byte, usually to determine what ring this segment can be used in
   uint8_t  granularity;         //!< Flags (Gr, Sz, 0, 0), see Intel Developers Manual
   uint8_t  base_high;           //!< The last 8 bits of the base.
} PACKED;
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} PACKED;
typedef struct gdt_ptr gdt_ptr_t;

void install_gdt(void);
void gdt_set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);

#ifdef __cplusplus
}
#endif
