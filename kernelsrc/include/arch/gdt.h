/* 
 * File:   gdt.h
 * Author: Kevin
 *
 * Created on August 25, 2016, 10:27 AM
 */

#include "common.h"

#ifndef GDT_H
#define GDT_H

#ifdef __cplusplus
extern "C" {
#endif

struct gdt_entry
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

gdt_entry_t gdt_entries[5]; //null, code, data, user code, user data
gdt_ptr_t gdt_ptr;

void install_gdt();
void gdt_set_gate(int, uint32_t, uint32_t, uint8_t, uint8_t);
extern void load_gdt(uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* GDT_H */
