#pragma once

#include <ebl/util.h>
#include <stddef.h>
#include <stdint.h>
#include "arch/types.h"

struct loader_state;
namespace core {
    struct thread;
}

namespace arch {

    constexpr vaddr_t page_align_down(vaddr_t addr) {
        // NOTE: We expect page_size to be defined by types.h
        return addr & ~(page_size - 1);
    }
    constexpr vaddr_t page_align_up(vaddr_t addr) {
        return (addr + page_size - 1) & ~(page_size - 1);
    }

    struct spinlock_backend;
    struct spinlock_state;
    struct thread_backend;
    struct irq_vector;
    struct loader_state;
    struct percpu;
    struct address_space;
    typedef void (*irq_handler_t)(void*);

    void irq_install_handler(irq_vector vector, irq_handler_t fn);
    void irq_remove_handler(irq_vector vector);
    irq_handler_t irq_get_handler(irq_vector vector);

    void spin_lock(spinlock_backend* lock);
    void spin_unlock(spinlock_backend* lock);
    void spin_save_state(spinlock_state* state);
    void spin_restore_state(spinlock_state const* state);

    void enable_interrupts();
    void disable_interrupts();
    [[noreturn]] void halt();

    void switch_thread(core::thread* oldthread, core::thread* newthread);
    
    void init(::loader_state* state);

    int cpu_num();
    percpu* get_percpu();
    core::thread* get_current_thread();

    bool is_heap_address(vaddr_t addr);
    void* grow_heap(unsigned int num_pages);

} // namespace arch

// Include the arch-specific interface header last.
#include STRINGIFY(arch/ARCH_TARGET/interface.h)
