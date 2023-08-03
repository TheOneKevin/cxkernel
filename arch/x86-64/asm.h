#pragma once

#include <stdint.h>
#include <ebl/util.h>

namespace x86_64 {

static inline void sti() {
    FENCE;
    asm volatile("sti");
}

static inline void cli() {
    asm volatile("cli");
    FENCE;
}

static inline uint32_t save_flags() {
    uint32_t state;
    asm volatile(
        "pushf;"
        "pop %0"
        : "=rm" (state) :: "memory");
    return state;
}


static inline void restore_flags(uint32_t flags) {
    asm volatile(
        "push %0;"
        "popf"
        :: "g" (flags)
        : "memory", "cc");
}

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

} // namespace 64
