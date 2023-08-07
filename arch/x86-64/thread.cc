#include "core/thread.h"
#include "arch/interface.h"
#include "arch/x86-64/thread.h"

#define NL "\n"

namespace arch {

void switch_thread(core::thread* oldthread, core::thread* newthread) {
    get_percpu()->curthread = newthread;

    // Assumes SystemV ABI
    // ref: https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf
    // FIXME: Should I move this to a separate assembly file?
    asm volatile (
        "pushf"             NL // Push flags
        "push %%rbx"        NL // Push callee-save regs
        "push %%rbp"        NL
        "push %%r12"        NL
        "push %%r13"        NL
        "push %%r14"        NL
        "push %%r15"        NL
        "mov %%rsp, (%0)"   NL // Save old rsp
        "mov %1, %%rsp"     NL // Load new rsp
        "pop %%r15"         NL // Restore callee-save regs
        "pop %%r14"         NL
        "pop %%r13"         NL
        "pop %%r12"         NL
        "pop %%rbp"         NL
        "pop %%rbx"         NL
        "popf"              NL // Restore flags
        :: "r"(&oldthread->backend.sp), "r"(&newthread->backend.sp)
    );
}

} // namespace arch
