#pragma once

#include <stdint.h>
#include "asm.h"
#include "thread.h"
#include "types.h"

namespace arch {

    struct SpinlockBackend {
        volatile uint32_t x = 0;
    };

    struct SpinlockState {
        uint32_t flags;
    };

    struct IrqVector {
        uint8_t vector;
    };

    struct LoaderState {
    };

    struct PerCPU {
        struct PerCPU* self;
        int cpu_num;
        core::Thread* curthread;
    };

    struct AddressSpace {
        x86_64::pml4e* pml4;
    };

    inline void spin_lock(SpinlockBackend* lock) {
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

    inline void spin_unlock(SpinlockBackend* lock) {
        lock->x = 0;
    }

    inline void spin_save_state(SpinlockState* state) {
        state->flags = x86_64::save_flags();
        disable_interrupts();
    }

    inline void spin_restore_state(SpinlockState const* state) {
        // Restore will re-enable interrupts.
        x86_64::restore_flags(state->flags);
    }

    inline void enable_interrupts() {
        x86_64::sti();
    }

    inline void disable_interrupts() {
        x86_64::cli();
    }

    [[noreturn]] inline void halt() {
        asm volatile("hlt");
        for(;;);
        __builtin_unreachable();
    }

    inline int cpu_num() {
        return (int) x86_64::read_gs_offset64(offsetof(PerCPU, cpu_num));
    }

    inline PerCPU* get_percpu() {
        return (PerCPU*) x86_64::read_gs_offset64(offsetof(PerCPU, self));
    }

    inline core::Thread* get_current_thread() {
        return get_percpu()->curthread;
    }

    inline bool is_heap_address(vaddr_t addr) {
        (void) addr;
        return false;
    }

    inline void* grow_heap(unsigned int num_pages) {
        (void) num_pages;
        return nullptr;
    }

} // namespace arch
