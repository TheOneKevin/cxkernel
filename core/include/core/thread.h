#pragma once

#include "core/vm.h"
#include "arch/interface.h"
#include <ebl/linked_list.h>
#include <ebl/memory.h>

namespace core {

    enum class threadstate {
        RUN, READY, SLEEP, BLOCKED, ZOMBIE
    };

    struct kernstack final {
        vaddr_t base;
        size_t size;
    };

    struct thread final {
        char name[30];
        enum threadstate state;
        struct kernstack stack;
        struct arch::thread_backend backend;
    };

    void thread_preempt();
    void thread_yield();
    void schedule_next_thread();

} // namespace kernel

// Global variables (thread queues etc.)
namespace g {
    extern ebl::IntrusiveMultilist<1, core::thread> thread_mlist;
    extern decltype(thread_mlist)::list<0> thread_queue;
    extern ebl::LPtr<core::thread> current_thread;
}

// Associate core::thread with thread_mlist's node
template<> struct ebl::LinkedRef<core::thread> {
    typedef decltype(g::thread_mlist) type;
};
