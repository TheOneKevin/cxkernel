#include "assert.h"

void assert(bool condition) {
    if(!condition) [[unlikely]] {
        for(;;);
    }
}

void assert(bool condition, const char* message) {
    (void) message;
    if(!condition) [[unlikely]] {
        for(;;);
    }
}
