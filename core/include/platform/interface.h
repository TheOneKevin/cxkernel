#pragma once

#include <ebl/util.h>

namespace platform {

    void console_init();
    void console_emit(char c);
    void console_puts(char const* c);
    void console_log(char const* c);
    void init();

} // namespace platform

// Include the platform-specific interface header last.
#include STRINGIFY(platform/PLAT_TARGET/interface.h)
