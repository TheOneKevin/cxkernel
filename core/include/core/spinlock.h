#pragma once

#include "arch/interface.h"
#include <ebl/guard.h>
#include <ebl/thread_safety.h>

namespace core {

/**
 * @brief Spinlock WITH disabling interrupts.
 */
class CAPABILITY("mutex") Spinlock final {
    friend class ebl::Guard<Spinlock>;
private:
    // Private policy class for ebl::Guard.
    struct Policy {
    private:
        arch::spinlock_state irq_flags_;
        Spinlock* lock_;
    public:
        Policy(Spinlock* lock) : irq_flags_{}, lock_{lock} {}
        void lock() ACQUIRE(lock_) NO_THREAD_SAFETY_ANALYSIS {
            lock_->lock(irq_flags_);
        }
        void unlock() RELEASE(lock_) NO_THREAD_SAFETY_ANALYSIS {
            if(lock_ != nullptr) [[likely]] {
                lock_->unlock(irq_flags_);
                lock_ = nullptr;
            }
        }
    };
private:
    struct arch::spinlock_backend backend_;
private:
    void lock(arch::spinlock_state& x);
    void unlock(arch::spinlock_state const& x);
public:
    Spinlock() : backend_{} {}
};

/**
 * @brief Spinlock WITHOUT disabling interrupts.
 */
class SpinlockNoIrq final {
    friend class ebl::Guard<SpinlockNoIrq>;
private:
    // Private policy class for ebl::Guard.
    struct Policy {
    private:
        SpinlockNoIrq* lock_;
    public:
        Policy(SpinlockNoIrq* lock) : lock_{lock} {}
        void lock() {
            lock_->lock();
        }
        void unlock() {
            if(lock_ != nullptr) [[likely]] {
                lock_->unlock();
                lock_ = nullptr;
            }
        }
    };
private:
    struct arch::spinlock_backend backend_;
private:
    void lock();
    void unlock();
public:
    SpinlockNoIrq() : backend_{} {}
};

} // namespace core
