/*
 * File:   interrupts.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 30-Jul-2017 04:32:35 PM
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 13-Aug-2017 05:18:28 PM
*/

#pragma once

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

struct idt_entry
{
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t sel;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));
typedef struct idt_ptr idt_ptr_t;

typedef struct registers
{
    uint32_t gs, fs, es, ds;                            // Pushed the segs last
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by 'pusha'
    uint32_t int_no, err_code;                          // Our 'push byte #' and ecodes do this
    uint32_t eip, cs, eflags, useresp, ss;              // Pushed by the processor automatically
} regs_t;

typedef void (*irq_t) (regs_t *r);

void install_idt();
void idt_set_gate(uint8_t,uint32_t,uint16_t,uint8_t);

void init_irq();
void install_irqhandler(uint8_t irq, irq_t handler);
void uninstall_irqhandler(uint8_t irq);

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

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
}
#endif
