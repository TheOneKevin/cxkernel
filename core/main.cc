#include "platform/interface.h"

extern "C" void(*init_array_start_)();
extern "C" void(*init_array_end_)();

extern "C" void kernel_entry() {
    // Run .init_array
    for(auto* fn = &init_array_start_; fn != &init_array_end_; fn++) (*fn)();

    platform::console_init();
    platform::console_puts("Hello, world (from the kernel)!\n");

    for(;;);
}
