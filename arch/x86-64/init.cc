#include "arch/interface.h"
#include "loaderabi.h"
#include "x86-64/internal.h"
#include <ebl/stdio.h>

void arch::init(::loader_state* state) {
    (void) state;
    x86_64::init_idt();
    ebl::kout("Hello, world (from the arch)!\n");
    asm volatile("int $3");
    arch::halt();
}
