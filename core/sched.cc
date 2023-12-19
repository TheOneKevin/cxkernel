#include <ebl/assert.h>
#include "arch/interface.h"
#include "core/thread.h"
#include "core/mp.h"
#include <ebl/util.h>

using ebl::move;
using namespace core;

void core::schedule_next_thread(Thread* oldthread) {
    // FIXME: Improve this. For now, just pick the head of the list.
    auto* newthread = get_percpu().thread_queue.pop_front_unsafe();
    assert(newthread != nullptr);
    assert(oldthread != nullptr);
    arch::switch_thread(oldthread, newthread);
}
