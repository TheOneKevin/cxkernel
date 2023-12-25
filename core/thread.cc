#include "core/thread.h"

#include <ebl/thread_safety.h>

#include "core/mp.h"
#include "core/spinlock.h"

using namespace core;

void core::thread_preempt() {
   // FIXME: Improve this alongside sched.cc, for now just yield.
   thread_yield();
}

void core::thread_yield() {
   auto* oldthread = arch::get_current_thread();
   oldthread->state = ThreadState::READY;
   get_percpu().thread_queue.push_back(oldthread);
   schedule_next_thread(oldthread);
}
