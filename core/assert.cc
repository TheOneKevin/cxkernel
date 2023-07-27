#include "assert.h"
#include "arch/interface.h"
#include <ebl/stdio.h>

void assert(bool condition) {
    if(!condition) [[unlikely]] {
        arch::disable_interrupts();
        arch::halt();
    }
}

void assert(bool condition, const char* message) {
    if(!condition) [[unlikely]] {
        ebl::kerr("Assertion failed: %s\n", message);
        arch::disable_interrupts();
        arch::halt();
    }
}
