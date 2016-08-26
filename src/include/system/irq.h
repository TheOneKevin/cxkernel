/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   irq.h
 * Author: Kevin
 *
 * Created on August 26, 2016, 7:53 AM
 */

#include "common.h"

#ifndef IRQ_H
#define IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

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

typedef void (*irq_t) (regs_t *r);
void install_handler(uint8_t irq, irq_t handler);
void uninstall_handler(uint8_t irq);

#ifdef __cplusplus
}
#endif

#endif /* IRQ_H */

