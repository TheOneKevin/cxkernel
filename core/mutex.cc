#include "assert.h"
#include "core/mutex.h"
#include "core/thread.h"
#include "core/mp.h"
#include <ebl/guard.h>

using ebl::move;
using ebl::LPtr;
using namespace core;

void Mutex::lock() {
    arch::spin_lock(&lock_);
	if(state_ == State::FREE) {
		// If lock is free, lock it
		state_ = State::LOCKED;
		owner_ = arch::get_current_thread();
        arch::spin_unlock(&lock_);
	} else /* if(state_ == State::LOCKED) */ {
        // Atomically put the thread to sleep -- be careful of wakeup races
        auto* cur_th_ptr = arch::get_current_thread();
        cur_th_ptr->state = threadstate::BLOCKED;
        wait_queue_.push_back_unsafe(thread_node::container_of(cur_th_ptr));
        arch::spin_unlock(&lock_);
        schedule_next_thread(cur_th_ptr);
        arch::spin_lock(&lock_);
		// We must check this condition here, otherwise a race occurs
		assert(!do_i_hold());
		// Set the lock owner now
		owner_ = arch::get_current_thread();
		arch::spin_unlock(&lock_);
	}
}

void Mutex::unlock() {
	// Only the lock holder can release the lock
	assert(do_i_hold(), "");
	// Otherwise, start unlock procedure
	arch::spin_lock(&lock_);
	if(wait_queue_.empty()) {
		// If we're the last holder, unlock the lock (so owner becomes NULL)
		state_ = State::FREE;
		owner_ = nullptr;
	} else {
		// Otherwise, wake up the next thread in the queue
		// and pass on the lock without unlocking it
		auto next_th = wait_queue_.pop_front();
		next_th->state = threadstate::READY;
		core::get_percpu()->thread_queue.push_back(move(next_th));
	}
	arch::spin_unlock(&lock_);
}

bool Mutex::do_i_hold() const {
    return owner_ == arch::get_current_thread();
}
