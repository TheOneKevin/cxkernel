#pragma once

#include "core/vm.h"
#include "arch/interface.h"
#include "core/spinlock.h"
#include <ebl/linked_list.h>
#include <ebl/memory.h>
#include <ebl/thread_safety.h>

namespace core {

    struct Thread;

    enum class ThreadState {
        RUN, READY, SLEEP, BLOCKED, ZOMBIE
    };

    struct KernStack final {
        vaddr_t base;
        size_t size;
    };

    constexpr int thread_name_max = 30;

    struct Thread final : ebl::RefCountable<Thread>, ebl::IntrusiveListNode<Thread> {
        Thread(const char* name) noexcept {
            for(int i = 0; i < thread_name_max; ++i)
                this->name[i] = name[i];
        }
        char name[thread_name_max];
        enum ThreadState state;
        struct KernStack stack;
        ebl::RefPtr<AddressSpace> address_space;
        struct arch::ThreadBackend backend;
    };

    void thread_preempt();
    void thread_yield();
    void schedule_next_thread(Thread* oldthread);

} // namespace kernel
