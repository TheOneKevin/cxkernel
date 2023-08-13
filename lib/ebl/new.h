#include <stddef.h>

inline void* operator new (size_t size, void* ptr) noexcept {
    (void) size;
    return ptr;
}

void operator delete(void* ptr) noexcept;
