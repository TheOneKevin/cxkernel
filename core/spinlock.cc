#include "core/spinlock.h"

using namespace core;

void Spinlock::lock(arch::spinlock_state& x) {
    arch::spin_save_state(&x);
    arch::spin_lock(&backend_);
}

void Spinlock::unlock(arch::spinlock_state const& x) {
    arch::spin_restore_state(&x);
    arch::spin_unlock(&backend_);
}

void SpinlockNoIrq::lock() {
    arch::spin_lock(&backend_);
}

void SpinlockNoIrq::unlock() {
    arch::spin_unlock(&backend_);
}
