#pragma once
#include <stddef.h>

#if __STDC_HOSTED__ == 0

inline void* operator new (size_t size, void* ptr) noexcept {
    (void) size;
    return ptr;
}

void operator delete(void* ptr) noexcept;

#else
#include <new>
#endif
