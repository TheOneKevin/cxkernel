#pragma once

#include "core/thread.h"
#include "arch/interface.h"
#include "ebl/linked_list.h"

constexpr int MAX_SMP_CPUS = 32;

namespace core {
    struct PerCPU {
        ebl::IntrusiveList<Thread> thread_queue{};
    };
}

// Global variables
namespace g {
    extern struct core::PerCPU percpu[]; // FIXME: Fix this!
}

namespace core {
    inline struct PerCPU& get_percpu() {
        return g::percpu[arch::cpu_num()];
    }
}
