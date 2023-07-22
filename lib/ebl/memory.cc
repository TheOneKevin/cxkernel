#include "ebl/memory.h"

void operator delete(void* ptr) {
    (void) ptr;
    for(;;);
}
