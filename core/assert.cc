#include "assert.h"
#include <ebl/stdio.h>

void assert(bool condition) {
    if(!condition) [[unlikely]] {
        for(;;);
    }
}

void assert(bool condition, const char* message) {
    if(!condition) [[unlikely]] {
        ebl::kerr("Assertion failed: %s\n", message);
        for(;;);
    }
}
