/**
 * @file   interrupts.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, November 26th 2017, 9:42:00 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-07-30T09:50:29-04:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

// Define each IRQ so its more organized
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#ifdef __cplusplus
extern "C" {
#endif

struct idt_entry
{
    uint16_t base_lo;             //!< The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 //!< Kernel segment selector.
    uint8_t  always0;             //!< This must always be zero.
    uint8_t  flags;               //!< More flags. See Intel developers manual.
    uint16_t base_hi;             //!< The upper 16 bits of the address to jump to.
} PACKED;
typedef struct idt_entry idt_entry_t;

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} PACKED;
typedef struct idt_ptr idt_ptr_t;

typedef void (*irq_t) (regs_t *r);

void init_idt(void);
void init_irq(void);
void idt_set_gate(int idx, uint32_t base, uint16_t sel, uint8_t flags);

void install_irqhandler(int irq, irq_t handler);
void uninstall_irqhandler(int irq);

#ifdef __cplusplus
}
#endif
