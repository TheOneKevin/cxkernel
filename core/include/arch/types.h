#pragma once

#include <ebl/type_traits.h>
#include <ebl/util.h>

// clang-format off
#include STRINGIFY(arch/ARCH_TARGET/types.h)
// clang-format on

// Check that the arch-specific header has defined all the required types.
static_assert(ebl::is_type_complete<vaddr_t>());
static_assert(ebl::is_type_complete<paddr_t>());
static_assert(ebl::is_type_complete<timestamp_t>());
static_assert(ebl::is_type_complete<arch::mmu_flags>());
