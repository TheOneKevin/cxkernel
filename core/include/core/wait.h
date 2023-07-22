#pragma once

#include "core/thread.h"
#include <ebl/linked_list.h>

namespace core {

    struct wait_queue {
    private:
        decltype(g::thread_mlist)::list<0> queue_;

    public:
        wait_queue() : queue_{} {}
        void block();
        void wakeone();
        void wakeall();
    };

}
