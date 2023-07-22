#include "core/wait.h"

using namespace core;
using ebl::move;

void wait_queue::block() {
    g::current_thread->state = threadstate::BLOCKED;
    queue_.push_back(move(g::current_thread));
}

void wait_queue::wakeone() {
    auto thread = queue_.pop_front();
    thread->state = threadstate::READY;
    g::thread_queue.push_back(move(thread));
}

void wait_queue::wakeall() {

}
