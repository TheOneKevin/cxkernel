#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "core/vm.h"
#include <malloc.h>

namespace kmem {
    void* alloc(unsigned int size) {
        return malloc(size);
    }
}

void assert(bool cond, char const* msg) {
    CHECK(cond);
}

void check_vmregion_validity(core::VmRegion const& region) {

}

TEST_CASE("vmm creation") {
    auto as = core::AddressSpace::Create();
    
}
