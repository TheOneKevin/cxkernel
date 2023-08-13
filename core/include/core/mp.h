#pragma once

#include "core/thread.h"
#include "arch/interface.h"

constexpr int MAX_SMP_CPUS = 32;

namespace core {
    struct percpu {
        core::thread_list_head thread_queue{};
    };
}

// Global variables
namespace g {
    extern struct core::percpu percpu[]; // FIXME: Fix this!
}

namespace core {
    inline struct percpu& get_percpu() {
        return g::percpu[arch::cpu_num()];
    }
}
