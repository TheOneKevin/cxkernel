/**
 * @file   icxxabi.cpp
 * @author Kevin Dai \<kevindai02@outlook.com\>
 *
 * @date Created on Monday, October 8th 2018, 1:25:20 pm
 *
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-27T22:58:03-04:00
 *
 * Functions in this file are implemented as specified in the Intel Itanium
 * C++ standard. These functions are required by GCC.
 */

#include <icxxabi.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
unsigned __atexit_funcs_count = 0;
void* __dso_handle = 0;

namespace __cxxabiv1
{

extern "C" int __cxa_atexit(void (* f)(void*), void* objptr, void* dso)
{
    // TODO
    return 0;
}

extern "C" void __cxa_finalize(void* f)
{
    // TODO
}

void __cxa_pure_virtual()
{
    // Do nothing
}

// The ABI requires a 64-bit type.
__extension__ typedef int __guard __attribute__((mode(__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release(__guard *);
extern "C" void __cxa_guard_abort(__guard *);

extern "C" int __cxa_guard_acquire(__guard * g)
{
    return !*(char *) (g);
}

extern "C" void __cxa_guard_release(__guard * g)
{
    *(char *) g = 1;
}

extern "C" void __cxa_guard_abort(__guard *)
{
}

}
