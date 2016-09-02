/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "arch/idt.h"
#include "arch/exceptions.h"
#include "system/tdisplay.h"

// Firstly, let's set up all our error messages
char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// Hehe
void print_dalek()
{
    console_putc('\n');
    console_writeline("                                         ___");
    console_writeline("                                  D=G==='   '.");
    console_writeline("                                       |======|");
    console_writeline("                                       |======|");
    console_writeline("                                   }--/]IIIIII]");
    console_writeline("                                      |_______|");
    console_writeline("                                      C O O O D");
    console_writeline("                                     C O  O  O D");
    console_writeline("                                    C  O  O  O  D");
    console_writeline("                                    C__O__O__O__D");
    console_writeline("                                    [___________]");
    console_writeline("                                     EXTERMINATE!");
}

// Here, we add the entries of our ISR to the IDT
void load_isr()
{
    idt_set_gate( 0, (unsigned)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (unsigned)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (unsigned)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (unsigned)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (unsigned)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (unsigned)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (unsigned)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (unsigned)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (unsigned)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (unsigned)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}

void write_err(regs_t *r)
{
    console_print_center("Uh oh, something went very wrong here!"); console_putc('\n');
    if (r->int_no < 32)
    {
        //Something very wrong happened here, to the code
        console_write("                          Received interrupt: "); console_write_dec(r->int_no);
        console_putc('('); console_write_hex(r->int_no); console_putc(')');  console_putc('\n');
        // Dump the contents of the registers onto the screen
        console_write(" REGDUMP ");
        console_write(" EAX: "); console_write_hex(r->eax); console_write(" EBX: "); console_write_hex(r->ebx);
        console_write(" ECX: "); console_write_hex(r->ecx); console_write(" EDX: "); console_write_hex(r->edx);
        console_write(" ESI: "); console_write_hex(r->esi); console_write(" EDI: "); console_write_hex(r->edi);
        console_putc('\n');
        console_write(" EBP: "); console_write_hex(r->ebp); console_write(" EIP: "); console_write_hex(r->eip);
        console_write(" CS: "); console_write_hex(r->cs); console_write(" EFLAGS: "); console_write_hex(r->eflags);
        console_write(" ESP: "); console_write_hex(r->useresp); console_write(" SS: "); console_write_hex(r->ss);
        console_putc('\n');
        console_write(" GS: "); console_write_hex(r->gs); console_write(" FS: "); console_write_hex(r->fs);
        console_write(" ES: "); console_write_hex(r->es); console_write(" DS: "); console_write_hex(r->ds);
        
        // Check if it was just a breakpoint or debug thing... (no need to halt)
        if(r->int_no == 3 || r->int_no == 2)
        {
            console_putc('\n');
            console_print_center(exception_messages[r->int_no]); //Get the message
            print_dalek();
        }
        // Check if there is an error code, then show it
        else if(r->int_no == 8 || (r->int_no >= 10 && r->int_no <= 14) || r->int_no == 17 || r->int_no == 20)
        {
            console_putc('\n');
            console_write(" Received error code: ");
            console_write_dec(r->err_code);
            console_putc('\n');
            console_print_center(exception_messages[r->int_no]); //Get the message
            print_dalek();
            for(;;); //Halt
        }
        // Halt for everything else
        else
        {
            console_putc('\n');
            console_print_center(exception_messages[r->int_no]); //Get the message
            print_dalek();
            for(;;); //Halt
        }
    }
}

void isr_handler(regs_t *r) //We use a pointer reference to our struct
{
    console_clear(COLOR_LIGHT_RED); //Make it all pink so you feel happy and not panic when you see the BSOD (now PSOD bc its pink) ;)
    //console_clear(COLOR_BLUE);
    console_write("\n\n\n                                    "); console_setbg(COLOR_LIGHT_BLUE); console_write("LiquiDOS\n"); console_setbg(COLOR_LIGHT_RED);
    write_err(r);
}