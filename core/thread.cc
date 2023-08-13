#include "core/thread.h"
#include "core/spinlock.h"
#include "core/mp.h"
#include <ebl/thread_safety.h>

using ebl::move;
using ebl::LPtr;
using namespace core;

void core::thread_preempt() {
    // FIXME: Improve this alongside sched.cc, for now just yield.
    thread_yield();
}

void core::thread_yield() {
    auto* oldthread = arch::get_current_thread();
    oldthread->state = ThreadState::READY;
    get_percpu().thread_queue.push_back(thread_list_type::container_of(oldthread));
    schedule_next_thread(oldthread);
}
