# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel. These are magic
# values that are documented in the multiboot standard. The bootloader will
# search for this signature in the first 8 KiB of the kernel file, aligned at a
# 32-bit boundary. The signature is in its own section so the header can be
# forced to be within the first 8 KiB of the kernel file.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# We want to put the following things into the data section of the file
# This will include paging tables to remap our kernel into the higher half memory
# These data structures (page directory and tables) must be page-aligned (to 0x1000)

.section .bss
.align 0x1000
_kernel_pd:
    .skip 0x1000
_kernel_pt:
    .skip 0x1000
_tempHeap1_pt:
    .skip 0x1000
_tempHeap2_pt:
    .skip 0x1000
_ramdisk_pt:
    .skip 0x1000

# The multiboot standard does not define the value of the stack pointer register
# (esp) and it is up to the kernel to provide a stack. This allocates room for a
# small stack by creating a symbol at the bottom of it, then allocating 16384
# bytes for it, and finally creating a symbol at the top. The stack grows
# downwards on x86. The stack is in its own section so it can be marked nobits,
# which means the kernel file is smaller because it does not contain an
# uninitialized stack. The stack on x86 must be 16-byte aligned according to the
# System V ABI standard and de-facto extensions. The compiler will assume the
# stack is properly aligned and failure to align the stack will result in
# undefined behavior.
.section .bootloader_stack
.align 16
stack_bottom:
.skip 0x4000 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text
.global _start
.type _start, @function
_start:
        xchg %bx, %bx
        # We first need to retrieve the physical address of the page table
        movl $(_kernel_pt - 0xC0000000), %edi
        # Map 0 = 0xC0000000, so 0x100000 = 0xC0100000. Set esi as our "map pointer," increasing the address
        # held within esi by 1 page (0x1000) each time until we reach, or go over _kernel_end
        movl $0, %esi
        # And we want to do this 1023 times
        movl $1023, %ecx

1:
        # We first need to map from 0x0 to the kernel end. So, we check if the "map pointer" AKA esi
        # has reached the _kernel_end or not. If it has reached _kernel_end, then we break (jump to 3)
        cmpl $0, %esi
        jl 2f
        cmpl $(_kernel_end - 0xC0000000), %esi
        jge 10f

        # Map the physical address, and then mark it as "present" and "writable"
        # This also means that .text and .rodata are "writable" as well. Security may be compromised
        movl %esi, %edx
        orl $0x003, %edx # 0x003 = 011, write and present flags are on
        movl %edx, (%edi)

2:
        # Size of each page is 4096 bytes or 4 KiB
        addl $0x1000, %esi
        # We know the size of each entry of a table is 4 bytes long
        addl $4, %edi
        loop 1b

# Map heap
10:
        xchg %bx, %bx
        # We first need to retrieve the physical address of the page table
        movl $(_tempHeap1_pt - 0xC0000000), %edi
        # Map 0 = 0xC0000000, so 0x100000 = 0xC0100000. Set esi as our "map pointer," increasing the address
        # held within esi by 1 page (0x1000) each time until we reach, or go over _kernel_end
        movl $(_kernel_end - 0xC0000000 + 0x1000), %esi
        # And we want to do this 1024 times
        movl $1024, %ecx

11:
        # We first need to map from 0x0 to the kernel end. So, we check if the "map pointer" AKA esi
        # has reached the _kernel_end or not. If it has reached _kernel_end, then we break (jump to 3)
        cmpl $0, %ecx
        jle 3f

        # Map the physical address, and then mark it as "present" and "writable"
        # This also means that .text and .rodata are "writable" as well. Security may be compromised
        movl %esi, %edx
        orl $0x003, %edx # 0x003 = 011, write and present flags are on
        addl $0x400000, %edx
        movl %edx, (%edi)

        addl $0x400000, %edx
        addl $0x1000, %edi # Advance edi to next page table
        movl %edx, (%edi)

        subl $0x800000, %edx # Map ramdisk portion
        addl $0x1000, %edi # Advance edi to the next page table again
        movl %edx, (%edi)

        subl $0x2000, %edi # Reset pointer to the first page table

12:
        # Size of each page is 4096 bytes or 4 KiB
        addl $0x1000, %esi
        # We know the size of each entry of a table is 4 bytes long
        addl $4, %edi
        loop 11b


3:
        # Map the VGA video memory to 0xC03FF000 as "present" and "writable"
        movl $(0x000B8000 | 0x003), _kernel_pt - 0xC0000000 + 1023 * 4

        # Map both the page table to 0x00000000 (identity) and 0xC0000000 to avoid triple faulting
        # when the CPU can't find the table at 0x00000000
        movl $(_kernel_pt - 0xC0000000 + 0x003), _kernel_pd - 0xC0000000 + 0
        movl $(_kernel_pt - 0xC0000000 + 0x003), _kernel_pd - 0xC0000000 + 0xC00
        movl $(_ramdisk_pt - 0xC0000000 + 0x003), _kernel_pd - 0xC0000000 + 0xC10
        movl $(_tempHeap1_pt - 0xC0000000 + 0x003), _kernel_pd - 0xC0000000 + 0xFF8
        movl $(_tempHeap2_pt - 0xC0000000 + 0x003), _kernel_pd - 0xC0000000 + 0xFFC

        # Set the CR3 register to the address of the page directory
        movl $(_kernel_pd - 0xC0000000), %ecx
        movl %ecx, %cr3

        # Enable the paging and write-protect bit in CR0
        movl %cr0, %ecx
        orl $0x80010000, %ecx
        movl %ecx, %cr0

        # Jump to the higher half kernel
        lea 4f, %ecx
        jmp *%ecx

4:
    # Here, we do not umap the identity mapped 1st MB since
    # Our VESA driver will need to use the identity mapped portion

	# The bootloader has loaded us into 32-bit protected mode on a x86
	# machine. Interrupts are disabled. Paging is disabled. The processor
	# state is as defined in the multiboot standard. The kernel has full
	# control of the CPU. The kernel can only make use of hardware features
	# and any code it provides as part of itself. There's no printf
	# function, unless the kernel provides its own <stdio.h> header and a
	# printf implementation. There are no security restrictions, no
	# safeguards, no debugging mechanisms, only what the kernel provides
	# itself. It has absolute and complete power over the
	# machine.

	# To set up a stack, we set the esp register to point to the top of our
	# stack (as it grows downwards on x86 systems). This is necessarily done
	# in assembly as languages such as C cannot function without a stack.
	mov $stack_top, %esp
        addl $0xC0000000, %ebx
        push %ebx
	# This is a good place to initialize crucial processor state before the
	# high-level kernel is entered. It's best to minimize the early
	# environment where crucial features are offline. Note that the
	# processor is not fully initialized yet: Features such as floating
	# point instructions and instruction set extensions are not initialized
	# yet. The GDT should be loaded here. Paging should be enabled here.
	# C++ features such as global constructors and exceptions will require
	# runtime support to work as well.

	# Enter the high-level kernel. The ABI requires the stack is 16-byte
	# aligned at the time of the call instruction (which afterwards pushes
	# the return pointer of size 4 bytes). The stack was originally 16-byte
	# aligned above and we've since pushed a multiple of 16 bytes to the
	# stack since (pushed 0 bytes so far) and the alignment is thus
	# preserved and the call is well defined.
	call kernel_main

	# If the system has nothing more to do, put the computer into an
	# infinite loop. To do that:
	# 1) Disable interrupts with cli (clear interrupt enable in eflags).
	#    They are already disabled by the bootloader, so this is not needed.
	#    Mind that you might later enable interrupts and return from
	#    kernel_main (which is sort of nonsensical to do).
	# 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	#    Since they are disabled, this will lock up the computer.
	# 3) Jump to the hlt instruction if it ever wakes up due to a
	#    non-maskable interrupt occurring or due to system management mode.
	cli
5:	hlt
	jmp 5b



# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start
