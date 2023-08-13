#include <ebl/memory.h>

void operator delete(void* ptr) noexcept {
    (void) ptr;
    for(;;);
}
