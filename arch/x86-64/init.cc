#include "arch/interface.h"
#include "loaderabi.h"
#include <ebl/stdio.h>

void arch::init(::loader_state* state) {
    (void) state;
    x86_64::init_idt();
}
