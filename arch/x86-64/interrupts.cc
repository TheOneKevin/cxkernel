#include "arch/types.h"
#include "arch/interface.h"
#include "x86-64/asm.h"
#include <ebl/stdio.h>

static x86_64::idt_entry idt[256] {};
static x86_64::idt_ptr idt_reg_ptr {
    .limit = sizeof(idt) - 1,
    .base = (uint64_t) &idt
};
static arch::irq_handler_t irq_handlers[256] { };
static void exception_handler(x86_64::int_frame& r, uint64_t rsp);
extern "C" uint64_t isr_stub_table;

extern "C" void isr_handler(void* arg) {
    auto* regs = (x86_64::int_frame*) arg;
    auto handler = irq_handlers[regs->vector];
    if(handler != nullptr) {
        handler(regs);
    } else if(regs->vector < 32) {
        exception_handler(*regs, (uint64_t) arg);
    } else {
        ebl::kerr("Unhandled interrupt: %d\n", regs->vector);
        arch::halt();
    }
}

void arch::irq_install_handler(irq_vector vector, irq_handler_t fn) {
    irq_handlers[vector.vector] = fn;
}

void arch::irq_remove_handler(irq_vector vector) {
    irq_handlers[vector.vector] = nullptr;
}

arch::irq_handler_t arch::irq_get_handler(irq_vector vector) {
    return irq_handlers[vector.vector];
}

static const char* exception_messages[] = {
    "#DE Division By Zero",
    "#DB Debug",
    "Non Maskable Interrupt",
    "#BP Breakpoint",
    "#OF Into Detected Overflow",
    "#BR Out of Bounds",
    "#UD Invalid Opcode",
    "#NM No Coprocessor",
    "#DF Double Fault",
    "Coprocessor Segment Overrun",
    "#TS Bad TSS",
    "#NP Segment Not Present",
    "#SS Stack-Segment Fault",
    "#GP General Protection Fault",
    "#PF Page Fault",
    "Unknown Interrupt",
    "#MF Coprocessor Fault",
    "#AC Alignment Check",
    "#MC Machine Check",
    "#XF SIMD Floating-Point Exception",
    "#VE Virtualization Exception",
    "Reserved",
    "#SX Security Exception",
    "Reserved",
    "Triple Fault",
    "#FERR Legacy",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static const char* page_fault_messages[] = {
    "Supervisory process tried to read a non-present page entry",
    "Supervisory process tried to read a page and caused a protection fault",
    "Supervisory process tried to write to a non-present page entry",
    "Supervisory process tried to write a page and caused a protection fault",
    "User process tried to read a non-present page entry",
    "User process tried to read a page and caused a protection fault",
    "User process tried to write to a non-present page entry",
    "User process tried to write a page and caused a protection fault"
};

void exception_handler(x86_64::int_frame& r, uint64_t rsp) {

    ebl::kerr(
        "\n"
        "=== KERNEL UNHANDLED EXCEPTION 0x%02lX ===\n"
        "=== %s\n"
        "=== Dumping register contents below\n"
        "> rax: 0x%016lX rbx: 0x%016lX rcx: 0x%016lX rdx: 0x%016lX\n"
        "> rsi: 0x%016lX rdi: 0x%016lX rsp: 0x%016lX rbp: 0x%016lX\n"
        "> r8:  0x%016lX r9:  0x%016lX r10: 0x%016lX r11: 0x%016lX\n"
        "> r12: 0x%016lX r13: 0x%016lX r14: 0x%016lX r15: 0x%016lX\n"
        "> rip: 0x%016lX eflags: 0x%lX\n"
        "=== Error code: 0x%016lX\n",
        r.vector, exception_messages[r.vector],
        r.rax, r.rbx, r.rcx, r.rdx, r.rsi, r.rdi, rsp, r.rbp,
        r.r8, r.r9, r.r10, r.r11, r.r12, r.r13, r.r14, r.r15,
        r.rip, r.flags,
        r.err_code
    );
    // Additional page fault data
    if(r.vector == 14) {
        ebl::kerr(
            "=== Page fault data:\n"
            "=== %s\n"
            "=== Faulting address: 0x%016lX\n",
            page_fault_messages[r.err_code],
            x86_64::read_cr2()
        );
    }
    ebl::kerr("\n");

    ebl::kerr(
        // Mini dead computer ASCII art
        // ref: https://www.asciiart.eu/computers/computers
        "+===============================================+\n"
        "|                                               |\n"
        "|  Kernel panic: The system has halted.         |\n"
        "|  Please reboot the system.                    |\n"
        "|  If you see this message again, please report |\n"
        "|  this to the developers.                      |\n"
        "|               _                               |\n"
        "|         __   |-|           THE SYSTEM         |\n"
        "|  jgs   [Ll]  |=|          IS DEAD! x.x        |\n"
        "|        ====`o'^'                              |\n"
        "+===============================================+\n\n"
    );

    arch::halt();
}

void x86_64::init_idt() {
    for(int i = 0; i < 256; i++) {
        idt[i] = x86_64::idt_entry {
            (&isr_stub_table)[i],
            x86_64::descriptor_type::INTERRUPT_GATE
        };
        irq_handlers[i] = nullptr;
    }
    asm volatile("lidt %0" : : "m" (idt_reg_ptr));
}
