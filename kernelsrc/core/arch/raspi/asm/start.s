/******************************************************************************
*	main.s
*	 by Alex Chadwick
*
*	A sample assembly code implementation of the screen01 operating system, that
*	creates a frame buffer and continually renders to it.
*
*	main.s contains the main operating system, and IVT code.
******************************************************************************/

/*
* .globl is a directive to our assembler, that tells it to export this symbol
* to the elf file. Convention dictates that the symbol _start is used for the
* entry point, so this all has the net effect of setting the entry point here.
* Ultimately, this is useless as the elf itself is not used in the final
* result, and so the entry point really doesn't matter, but it aids clarity,
* allows simulators to run the elf, and also stops us getting a linker warning
* about having no entry point.
*/
.section .init
.globl _start
_start:

/*
* Branch to the actual main code.
*/
b main

/*
* This command tells the assembler to put this code with the rest.
*/
.section .text

/*
* main is what we shall call our main operating system method. It never
* returns, and takes no parameters.
* C++ Signature: void main(void)
*/
main:

/*
* Set the stack point to 0x8000.
*/
    mov sp,#0x8000

/* NEW
* Setup the screen.
*/
    mov r0,#1024
    mov r1,#768
    mov r2,#16
    bl InitialiseFrameBuffer

/* NEW
* Check for a failed frame buffer.
*/
    teq r0,#0
    bne noError$

    mov r0,#16
    mov r1,#1
    bl SetGpioFunction

    mov r0,#16
    mov r1,#0
    bl SetGpio

error$:
        b error$

noError$:
    ldr r0,=0x20200000
    mov r1,#1
    lsl r1,#18
    str r1,[r0,#4]
    mov r1,#1
    lsl r1,#16
    str r1,[r0,#40]

    fbInfoAddr .req r4
    mov fbInfoAddr,r0

/* NEW
* Set pixels forevermore.
*/
render$:
    fbAddr .req r3
    ldr fbAddr,[fbInfoAddr,#32]

/* NEW
* We will use r0 to keep track of the current colour.
*/
    colour .req r0
    y .req r1
    mov y,#768
drawRow$:
    x .req r2
    mov x,#1024
drawPixel$:
    strh colour,[fbAddr]
    add fbAddr,#2
    sub x,#1
    teq x,#0
    bne drawPixel$
    sub y,#1
    add colour,#1
    teq y,#0
    bne drawRow$

b render$

.unreq fbAddr
.unreq fbInfoAddr
