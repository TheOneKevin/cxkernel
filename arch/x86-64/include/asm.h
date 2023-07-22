#pragma once

#include <stdint.h>
#include <ebl/util.h>

static inline void x86_sti() {
    FENCE;
    asm volatile("sti");
}

static inline void x86_cli() {
    asm volatile("cli");
    FENCE;
}

static inline uint32_t x86_save_flags() {
    uint32_t state;
    asm volatile(
        "pushf;"
        "pop %0"
        : "=rm" (state) :: "memory");
    return state;
}


static inline void x86_restore_flags(uint32_t flags) {
    asm volatile(
        "push %0;"
        "popf"
        :: "g" (flags)
        : "memory", "cc");
}

