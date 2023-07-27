#pragma once

#include <stdint.h>
#include "asm.h"
#include "thread.h"
#include "types.h"

namespace arch {

    struct spinlock_backend {
        volatile uint32_t x = 0;
    };

    struct spinlock_state {
        uint32_t flags;
    };

    struct irq_vector {
        uint8_t vector;
    };

    struct loader_state {
    };

    inline void spin_lock(spinlock_backend* lock) {
        #define NL "\n"
        asm volatile(
            "lock bts $0, (%0)"     NL
            "jnc 2f"                NL
            "1:"                    NL
                "pause"             NL
                "testl $1, (%0)"    NL
                "jne 1b"            NL
                "lock bts $0, (%0)" NL
                "jc 1b"             NL
            "2:"
        : : "r"(lock->x) : "memory");
        #undef NL
    }

    inline void spin_unlock(spinlock_backend* lock) {
        lock->x = 0;
    }

    inline void spin_save_state(spinlock_state* state) {
        state->flags = x86_64::save_flags();
        disable_interrupts();
    }

    inline void spin_restore_state(spinlock_state const* state) {
        // Restore will re-enable interrupts.
        x86_64::restore_flags(state->flags);
    }

    inline void enable_interrupts() {
        x86_64::sti();
    }

    inline void disable_interrupts() {
        x86_64::cli();
    }

}
