#pragma once

#include "loaderabi.h"
#include "core/vm.h"

namespace core {
    
};

namespace g {
    extern decltype(loader_state_.pfndb_freelist)& pfndb_freelist;
    extern decltype(loader_state_.pfndb_rsrvlist)& pfndb_rsrvlist;
    extern decltype(loader_state_.total_phys_pgs)& total_phys_pgs;
}
