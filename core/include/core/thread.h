#pragma once

#include "core/vm.h"
#include "arch/interface.h"
#include "core/spinlock.h"
#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/thread_safety.h>

namespace core {

    enum class ThreadState {
        RUN, READY, SLEEP, BLOCKED, ZOMBIE
    };

    struct KernStack final {
        vaddr_t base;
        size_t size;
    };

    struct Thread final {
        char name[30];
        enum ThreadState state;
        struct KernStack stack;
        struct AddressSpace* address_space;
        struct arch::ThreadBackend backend;
    };

    using thread_list_type = ebl::IntrusiveList<Thread, 1>;
    using thread_node = thread_list_type::node;
    using thread_list_head = thread_list_type::list<0>;

    void thread_preempt();
    void thread_yield();
    void schedule_next_thread(Thread* oldthread);

} // namespace kernel

// Associate core::thread with thread_mlist's node
MakeLinkedRef(core::Thread, core::thread_node);
