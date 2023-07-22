#pragma once

#include <ebl/util.h>
#include <stddef.h>
#include <stdint.h>

namespace core {
    struct thread;
}

namespace arch {

    struct spinlock_backend;
    struct spinlock_state;
    struct thread_backend;
    struct irq_vector;
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

    void switch_thread(core::thread& oldthread, core::thread& newthread);

} // namespace arch

// Include the arch-specific interface header last.
#include STRINGIFY(arch/ARCH_FOLDER/include/interface.h)
