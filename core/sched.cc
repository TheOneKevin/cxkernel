#include <ebl/assert.h>
#include <ebl/util.h>

#include "arch/interface.h"
#include "core/mp.h"
#include "core/thread.h"

using ebl::move;
using namespace core;

void core::schedule_next_thread(Thread* oldthread) {
   // FIXME: Improve this. For now, just pick the head of the list.
   auto* newthread = get_percpu().thread_queue.pop_front();
   assert(newthread != nullptr);
   assert(oldthread != nullptr);
   arch::switch_thread(oldthread, newthread);
}
