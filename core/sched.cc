#include "assert.h"
#include "arch/interface.h"
#include "core/thread.h"
#include <ebl/util.h>

using ebl::move;

namespace core {

void schedule_next_thread() {
    using namespace arch;
    // FIXME: Improve this. For now, just pick the head of the list.
    auto oldthread = move(g::current_thread);
    auto newthread = g::thread_queue.pop_front();
    g::current_thread = move(newthread);
    assert(newthread.get() != nullptr);
    assert(oldthread.get() != nullptr);
    switch_thread(*oldthread, *newthread);
}

} // namespace core
