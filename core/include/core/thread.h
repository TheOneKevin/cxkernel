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

    using thread_list_node = ebl::IntrusiveListNode<thread, 1>;
    using thread_list_head = thread_list_node::list<0>;

    void thread_preempt();
    void thread_yield();
    void schedule_next_thread();

} // namespace kernel

// Global variables (thread queues etc.)
namespace g {
    extern core::thread_list_head thread_queue;
    extern ebl::LPtr<core::thread> current_thread;
}

// Associate core::thread with thread_mlist's node
template<> struct ebl::LinkedRef<core::thread> {
    typedef core::thread_list_node type;
};
