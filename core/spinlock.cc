#include "core/spinlock.h"

using namespace core;

void Spinlock::lock(arch::SpinlockState& x) {
   arch::spin_save_state(&x);
   arch::spin_lock(&backend_);
}

void Spinlock::unlock(arch::SpinlockState const& x) {
   arch::spin_restore_state(&x);
   arch::spin_unlock(&backend_);
}

void SpinlockNoIrq::lock() { arch::spin_lock(&backend_); }

void SpinlockNoIrq::unlock() { arch::spin_unlock(&backend_); }
