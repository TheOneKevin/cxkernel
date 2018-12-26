/**
 * @file   llio.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Saturday, October 13th 2018, 5:44:15 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-11-11T08:26:54-05:00
 */

#pragma once

#include "common.h"
#include "arch/arch_types.h"

__BEGIN_CDECLS

static inline void io_wait(void)
{
    // Port 0x80 is used for 'checkpoints' during POST
    // The Linux kernel seems to think it is free for use :-/
    //// %%al instead of %0 makes no difference. Does the register need to be zeroed?
    asm volatile ("outb %%al, $0x80" : : "a" (0));
}

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline void outw(uint16_t port, uint16_t value)
{
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

static inline arch_sz_t read_cr0(void)
{
    arch_sz_t val;
    asm volatile ("mov %%cr0, %0" : "=r" (val));
    return val;
}

static inline arch_sz_t read_cr2(void)
{
    arch_sz_t val;
    asm volatile ("mov %%cr2, %0" : "=r" (val));
    return val;
}

static inline arch_sz_t read_cr3(void)
{
    arch_sz_t val;
    asm volatile ("mov %%cr3, %0" : "=r" (val));
    return val;
}

static inline arch_sz_t read_cr4(void)
{
    arch_sz_t val;
    asm volatile ("mov %%cr4, %0" : "=r" (val));
    return val;
}

static inline void write_cr0(arch_sz_t val)
{
    asm volatile("mov %0, %%cr0" : : "r" (val));
}

static inline void write_cr2(arch_sz_t val)
{
    asm volatile("mov %0, %%cr2" : : "r" (val));
}

static inline void write_cr3(arch_sz_t val)
{
    asm volatile("mov %0, %%cr3" : : "r" (val));
}

static inline void write_cr4(arch_sz_t val)
{
    asm volatile("mov %0, %%cr4" : : "r" (val));
}

/**
 * Issue a CPUID command
 */
static inline void cpuid(uint32_t l, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile ("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (l));
}

/**
 * Issue a CPUID command when the initial ecx value matters
 */
static inline void cpuidc(uint32_t l, uint32_t c, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    asm volatile ("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (l), "c" (c));
}

__END_CDECLS