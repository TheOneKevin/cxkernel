/*
 * File:   apic.h
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-09-27T17:06:15-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:44:21-05:00
*/

#pragma once

#include "common.h"

/* ========================================================================== */
// LAPIC Registers

#define LAPIC_REG_ID        0x0020 // Local APIC ID
#define LAPIC_REG_VER       0x0030 // Local APIC Version
#define LAPIC_REG_TPR       0x0080 // Task Priority Register
#define LAPIC_REG_APR       0x0090 // Arbitration Priority Register
#define LAPIC_REG_PPR       0x00A0 // Processor Priority Register
#define LAPIC_REG_EOI       0x00B0 // EOI Register
#define LAPIC_REG_RRD       0x00C0 // Remote Read Register
#define LAPIC_REG_LDR       0x00D0 // Logical Destination Register
#define LAPIC_REG_DFR       0x00E0 // Destination Format Register
#define LAPIC_REG_SIVR      0x00F0 // Spurious Interrupt Vector Register
#define LAPIC_REG_ISR       0x0100 // In-Service Register (8 Registers)
#define LAPIC_REG_TMR       0x0180 // Trigger Mode Register (8)
#define LAPIC_REG_IRR       0x0200 // Interrupt Request Register (8)
#define LAPIC_REG_ESR       0x0280 // Error Status Register
#define LAPIC_REG_LVT_CMCIR 0x02F0 // LVT Corrected Machine Check Interrupt
#define LAPIC_REG_ICR       0x0300 // Interrupt Command Register (2, 64 bits)
#define LAPIC_REG_LVT_TIMER 0x0320 // Local Vector Table (LVT) Timer
#define LAPIC_REG_LVT_THERM 0x0330 // LVT Thermal Sensor
#define LAPIC_REG_LVT_PRFMC 0x0340 // LVT Performance Monitoring Counters
#define LAPIC_REG_LVT_LINT0 0x0350 // LVT LINT0 (Local Interrupt)
#define LAPIC_REG_LVT_LINT1 0x0360 // LVT LINT1
#define LAPIC_REG_LVT_ERROR 0x0370 // LVT Error
#define LAPIC_REG_LVT_INITC 0x0380 // Initial Count Register for Timers
#define LAPIC_REG_LVT_CURRC 0x0390 // Current Count Register for Timers
#define LAPIC_REG_DCR       0x03E0 // Divide Config Register for Timers

/* ========================================================================== */
// Command Register Masks
// Delivery Mode
#define LAPIC_ICR_MASK_FIXED    0 << 8
#define LAPIC_ICR_MASK_LPR      1 << 8
#define LAPIC_ICR_MASK_SMI      2 << 8
#define LAPIC_ICR_MASK_NMI      4 << 8
#define LAPIC_ICR_MASK_INIT     5 << 8
#define LAPIC_ICR_MASK_STARTUP  6 << 8
// Destination Mode
#define LAPIC_ICR_MASK_PHYSICAL 0 << 11
#define LAPIC_ICR_MASK_LOGICAL  1 << 11
// Delivery Status
#define LAPIC_ICR_MASK_IDLE     0 << 12
#define LAPIC_ICR_MASK_SENDPEND 1 << 12
// Level
#define LAPIC_ICR_MASK_DEASSERT 0 << 14
#define LAPIC_ICR_MASK_ASSERT   1 << 14
// Trigger Mode
#define LAPIC_ICR_MASK_EDGE     0 << 15
#define LAPIC_ICR_MASK_LEVEL    1 << 15
// Destination Shorthand
#define LAPIC_ICR_MASK_NOSHORT  0 << 18
#define LAPIC_ICR_MASK_SELF     1 << 18
#define LAPIC_ICR_MASK_ALLINCL  2 << 18
#define LAPIC_ICR_MASK_ALLEXCL  3 << 18

#define LAPIC_ICR_SHIFT_DEST   24

#ifdef __cplusplus
extern "C" {
#endif

void enable_APIC(void);

#ifdef __cplusplus
}
#endif
