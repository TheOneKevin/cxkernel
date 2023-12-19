#include "arch/interface.h"
#include <ebl/stdio.h>
#include <ebl/assert.h>

void assert(bool condition, std::source_location const& loc) {
    if(!condition) [[unlikely]] {
        ebl::kerr(
            "Assertion failed at %s:%d in \"%s\"\n",
            loc.file_name(), loc.line(), loc.function_name());
        arch::disable_interrupts();
        arch::halt();
    }
}

void assert(bool condition, const char* message, std::source_location const& loc) {
    if(!condition) [[unlikely]] {
        ebl::kerr(
            "Assertion failed at %s:%d in \"%s\"\nReason: %s\n",
            loc.file_name(), loc.line(), loc.function_name(), message);
        arch::disable_interrupts();
        arch::halt();
    }
}

void panic(const char* message, std::source_location const& loc) {
    ebl::kerr(
        "Kernel panicked at %s:%d in \"%s\"\nReason: %s\n",
        loc.file_name(), loc.line(), loc.function_name(), message);
    arch::disable_interrupts();
    arch::halt();
}
