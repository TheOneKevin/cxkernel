#pragma once

#include "core/vm.h"
#include "arch/interface.h"
#include "core/spinlock.h"
#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/thread_safety.h>

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
        struct address_space* address_space;
        struct arch::thread_backend backend;
    };

    using thread_list_type = ebl::IntrusiveList<thread, 1>;
    using thread_node = thread_list_type::node;
    using thread_list_head = thread_list_type::list<0>;

    void thread_preempt();
    void thread_yield();
    void schedule_next_thread(thread* oldthread);

} // namespace kernel

// Associate core::thread with thread_mlist's node
MakeLinkedRef(core::thread);
