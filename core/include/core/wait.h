#pragma once

#include "core/thread.h"
#include <ebl/linked_list.h>

namespace core {

    struct wait_queue {
    private:
        core::thread_list_head queue_;

    public:
        wait_queue() : queue_{} {}
        void block();
        void wakeone();
        void wakeall();
    };

}
