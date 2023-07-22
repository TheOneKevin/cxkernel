#include "core/thread.h"
#include "core/spinlock.h"

using ebl::move;

namespace g {
    decltype(thread_mlist) thread_mlist{};
    decltype(thread_queue) thread_queue{};
    decltype(current_thread) current_thread{nullptr};
}

namespace core {

void thread_preempt() {
    // FIXME: Improve this alongside sched.cc, for now just yield.
    thread_yield();
}

void thread_yield() {
    g::current_thread->state = threadstate::READY;
    g::thread_queue.push_back(move(g::current_thread));
    schedule_next_thread();
}

} // namespace core
