#pragma once

#include <ebl/util.h>
#include <stddef.h>
#include <stdint.h>
#include "arch/types.h"

struct LoaderState;
namespace core {
    struct Thread;
}

namespace arch {

    constexpr vaddr_t page_align_down(vaddr_t addr) {
        // NOTE: We expect page_size to be defined by types.h
        return addr & ~(page_size - 1);
    }
    constexpr vaddr_t page_align_up(vaddr_t addr) {
        return (addr + page_size - 1) & ~(page_size - 1);
    }

    struct SpinlockBackend;
    struct SpinlockState;
    struct ThreadBackend;
    struct IrqVector;
    struct LoaderState;
    struct PerCPU;
    struct AddressSpace;
    typedef void (*irq_handler_t)(void*);

    void irq_install_handler(IrqVector vector, irq_handler_t fn);
    void irq_remove_handler(IrqVector vector);
    irq_handler_t irq_get_handler(IrqVector vector);

    void spin_lock(SpinlockBackend* lock);
    void spin_unlock(SpinlockBackend* lock);
    void spin_save_state(SpinlockState* state);
    void spin_restore_state(SpinlockState const* state);

    void enable_interrupts();
    void disable_interrupts();
    [[noreturn]] void halt();

    void switch_thread(core::Thread* oldthread, core::Thread* newthread);
    
    void init(::LoaderState* state);

    int cpu_num();
    PerCPU* get_percpu();
    core::Thread* get_current_thread();

    bool is_heap_address(vaddr_t addr);
    void* grow_heap(unsigned int num_pages);

} // namespace arch

// Include the arch-specific interface header last.
#include STRINGIFY(arch/ARCH_TARGET/interface.h)
