#pragma once

#include "arch/interface.h"
#include "ebl/linked_list.h"
#include <ebl/guard.h>
#include <ebl/thread_safety.h>

namespace core {

struct Thread;

/**
 * @brief Mutex
 */
class CAPABILITY("mutex") Mutex final {
    friend class ebl::Guard<Mutex>;
private:
    struct Policy {
    private:
        Mutex* lock_;
    public:
        Policy(Mutex* lock) : lock_{lock} {}
        void lock() ACQUIRE(lock_) NO_THREAD_SAFETY_ANALYSIS {
            lock_->lock();
        }
        void unlock() RELEASE(lock_) NO_THREAD_SAFETY_ANALYSIS {
            if(lock_ != nullptr) [[likely]] {
                lock_->unlock();
                lock_ = nullptr;
            }
        }
    };
private:
    enum class State {
        FREE,
        LOCKED,
    };
    struct arch::SpinlockBackend lock_;
    ebl::IntrusiveList<Thread> wait_queue_;
    Thread* owner_;
    State state_;
private:
    void lock();
    void unlock();
    bool do_i_hold() const;
public:
    Mutex() : lock_{}, wait_queue_{}, owner_{nullptr}, state_{State::FREE} {}
};

}
