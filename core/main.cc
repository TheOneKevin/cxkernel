#include "assert.h"
#include "loaderabi.h"
#include "platform/interface.h"
#include "core/pmm.h"
#include <ebl/stdio.h>

namespace g {
    loader_state loader_state_{};
    auto& pfndb_freelist = loader_state_.pfndb_freelist;
    auto& pfndb_rsrvlist = loader_state_.pfndb_rsrvlist;
    auto& total_phys_pgs = loader_state_.total_phys_pgs;
}

extern "C" void(*init_array_start_)();
extern "C" void(*init_array_end_)();

extern "C" void kernel_entry(uint64_t state_ptr) {
    // Run .init_array
    for(auto* fn = &init_array_start_; fn != &init_array_end_; fn++) (*fn)();
    
    // Kick off console
    platform::console_init();
    platform::console_puts("Hello, world (from the kernel)!\n");

    // Save state and check integrity
    g::loader_state_ = *(loader_state*) state_ptr;
    assert(
        g::loader_state_.magic_start == LOADER_ABI_MAGIC_START &&
        g::loader_state_.magic_end == LOADER_ABI_MAGIC_END,
        "Invalid loader struct magics (possible ABI incompatibility)");

    // Traverse pfndb to check PFN DB integrity
    {
        const auto x = (uint64_t) g::pfndb_freelist.size();
        const auto y = (uint64_t) g::pfndb_rsrvlist.size();
        assert(x+y == g::total_phys_pgs,
            "PFN DB size mismatch, possible corruption");
    }

    arch::disable_interrupts();
    arch::halt();
}
