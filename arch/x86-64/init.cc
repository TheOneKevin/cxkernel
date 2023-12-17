#include "arch/interface.h"
#include "x86-64/private.h"
#include "core/mp.h"
#include "loaderabi.h"
#include <ebl/stdio.h>
#include "assert.h"
#include "core/thread.h"

static arch::PerCPU percpu_array[MAX_SMP_CPUS]{};
static core::Thread boot_thread {
    "Kernel boot thread"
};
static core::AddressSpace boot_aspace {
    // Nothing to be done here
};

void arch::init() {
    // Initialize percpu array
    for(int i = 0; i < MAX_SMP_CPUS; i++) {
        percpu_array[i].self = &percpu_array[i];
        percpu_array[i].cpu_num = i;
        percpu_array[i].curthread = nullptr;
    }

    // Load gs with percpu pointer
    x86_64::wrmsr(x86_64::MSR::IA32_KERNEL_GS_BASE, (uint64_t) &percpu_array[0]);
    asm volatile("swapgs");
    assert(arch::cpu_num() == 0, "Boot CPU number is not 0");
    assert(arch::get_percpu()->self == &percpu_array[0], "Boot percpu pointer is not correct");

    // Install placeholder thread struct
    boot_aspace.arch() = arch::AddressSpace {
        .pml4 = x86_64::read_cr3()
    };
    boot_thread.address_space = ebl::AdoptRef(&boot_aspace);
    percpu_array[0].curthread = &boot_thread;
    assert(arch::get_current_thread() == &boot_thread, "Boot thread is not current thread");
    
    // Install interrupt handlers
    x86_64::init_idt();
}
