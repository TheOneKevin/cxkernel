#pragma once

#include <ebl/util.h>

namespace platform {



} // namespace platform

// Include the platform-specific interface header last.
#include STRINGIFY(platform/PLAT_TARGET/include/interface.h)
