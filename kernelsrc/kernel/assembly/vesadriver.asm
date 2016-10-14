use32
global enablevesa

; This little piece of sh*t caused me to rewrite this entire script That's why there's a macro here
; because of some obscure bracket rule that I apparently needed to know...
%define c(x) (((x) - lowerhalf) + 0x7C00) ; This annoying thing that we have to c every address bc we moved
%define d(x, y) ((x) - (y))

enablevesa:
    ;xchg bx, bx
    cli
    pusha
    mov [stack32p], esp ; Save our stack right now, because our stack pointer is going to change soon
    mov esi, lowerhalf
    mov edi, 0x7C00
    mov ecx, (lafin - lowerhalf)
    cld
    rep movsb ; Move the entire code below to 0x7C00 because realmode can't handle > 1MB. Feels bad man
    jmp 0x7C00

lowerhalf:
    sgdt [c(sgdtptr)]
    sidt [c(sidtptr)]
    lgdt [c(gdtptr)]
    jmp word 0x18 : c(pmodereal)

pmodereal: use16
    mov ax, 0x20 ; 16 bit protected mode data selector from out GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; Save the value in CR0
    mov eax, cr0
    mov [c(savecr0)], eax
    and eax, 0x7FFFFFFe ; Disable the paging bit and enable 16 bit pmode
    mov cr0, eax
    jmp word 0x0000 : c(goreal) ; Far jump to set CS


; ==================================================================================================== ;
; ==================================== Beginning of our VESA code ==================================== ;
goreal: use16
    mov sp, 0x5000 ; Pick some random place to start a fresh stack (according to the Wiki), make sure it won't get corrupted by us writing to RAM
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    lidt [c(rmodeivt)] ; Load the real mode idt
    sti ; Enable interrupts
    
    ; See if VESA is already enabled or not. If it is, we don't need to do anything
    ; and assume the enabling action was taken by the user
    mov ax, 0x4F03
    int 10h
    shr bx, 8
    cmp bx, 0
    jne vbe_exists ; We handle it
    
    xchg bx, bx
    
    ; Put the memory segment address into es, and offset into di
    push 0x5000
    pop es
    mov di, 0
    ; And call the "Get VESA Info" interrupt
    mov ax, 0x4F00
    int 10h
    ; Do some checkings
    cmp ax, 0x004F ; Check if al is 0x004F and ah is 0
    jne vbe_fail
    
    lds si, [es : di + 14]
    add di, 0x200
    jmp 0x0000 : c(loop_vbe)
    
; ==================================== GET VBE INFO ==================================== ;
loop_vbe: use16
    mov cx, [si]
    cmp cx, 0xFFFF
    je done
    
    mov ax, 0x4F01
    int 0x10
    cmp ax, 0x004F ; Check if al is 0x004F and ah is 0
    jne nextEntry
    
    push ds ; Save ds
    push es ; Push es
    pop ds  ; Pop es into ds
    
    ;xchg bx, bx
    
    ; Check if this graphics mode supports LFB
    mov ax, [di]
    and al, 0x99
    cmp al, 0x99
    jne nextEntry
    
    ; Check if this graphics mode is packed pixel or direct colour
    mov al, [di + 27]
    and al, 0xFD ; Make sure the memory model isn't 4 nor 6
    cmp al, 4
    jne nextEntry
    
    ; Check if hardware support is given
    mov ax, [di]
    shr ax, 0
    and ax, 1
    cmp ax, 1
    jne nextEntry
    
    ; Get the f*cking LFB address
    mov eax, DWORD[di + 40]
    push ds
    push 0
    pop ds
    mov [c(framebuffer1)], eax
    pop ds
    
    ; Find the largest width
    mov dx, [di + 18]
    push ds ; To read from the lastwidth variable, we must set ds to 0
    push 0
    pop ds ; ds = 0
    cmp dx, [c(lastwidth)]
    pop ds ; restore ds = es
    jae stodim ; Store it if it is
    
    ; Find the largest height
    mov dx, [di + 20]
    push ds
    push 0
    pop ds
    cmp dx, [c(lastheight)]
    pop ds
    jae stodim ; Store it if it is
    
    ; We set the segment here bc we modified the ES and DS
    jmp 0x0000 : c(nextEntry)

stodim: use16 ; Here, we store both the height and the width
    ; Fetch the variables again
    mov dx, [di + 18]
    
    ; Make sure width doesn't exceed an amount
    cmp dx, 800
    ja nextEntry
    
    mov bx, [di + 20]

    cmp bx, 1200
    ja nextEntry
    
    mov al, [di + 25]
    ; Everytime you see the push DS thing, it's to set it to 0 so I can save the things
    push ds ; Save ds
    push 0 ; Make ds = 0
    pop ds
    ; Move the values into RAM
    mov [c(lastheight)], bx
    mov [c(lastwidth)], dx
    mov [c(largestMode)], cx
    mov [c(bpp)], al
    mov bx, [c(lastheight)]
    mov dx, [c(lastwidth)]
    pop ds ; Restore ds
    
    mov ax, [di + 16]
    push ds
    push 0
    pop ds
    mov [c(pitch)], ax
    pop ds
    
    ; Continue
    jmp 0x0000 : c(nextEntry)

nextEntry: use16
    pop ds ; Pop the pushed ds
    inc si
    inc si
    jmp 0x0000 : c(loop_vbe)
    
done: use16
    mov ax, 0
    mov ds, ax ; Reset the modified segments es and ds
    mov es, ax
    xchg bx, bx
    ; ================================= ENABLE VIDEO MODE ===================================== ;
    
    ; Enable the mode we found best fit
    mov bx, [c(largestMode)]
    or bx, 0x4000
    mov di, 0 ; Clear DI
    mov ax, 0x4F02
    int 10h
    cmp ax, 0x004F
    jne vbe_fail_wrap
    
    ; ======================================================================================== ;
    
    mov ax, 1
    mov [c(returnstatus)], ax
    jmp 0x0000 : c(switchpm) ; Jump to protected real mode


; ================================== VESA FAIL ACTIONS ====================================== ;

vbe_fail: use16
    xor ax, ax
    mov [c(returnstatus)], ax
    jmp 0x0000 : c(switchpm)
    
vbe_fail_wrap: use16
    jmp 0x0000 : c(vbe_fail)
    
vbe_exists: use16
    ; Set our error code to 2 (VBE_EXISTS)
    mov ax, 2
    mov [c(returnstatus)], ax
    ; Return
    jmp 0x0000 : c(switchpm)
    
; ==================================== END of our VESA code ==================================== ;
; ==================================================================================================== ;
   
switchpm: use16
    cli ; Turn off interrupts and prepare to enter pmode
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; I don't even know why we have this CR0 switch here when we saved the CR0, maybe I'm just stupid
    mov eax, cr0
    inc eax
    mov cr0, eax
    ; Restore our CR0
    mov eax, [c(savecr0)] ; Get our CR0 back
    mov cr0, eax
    jmp dword 0x08 : c(getback) ;Far jump to our pmode code
    
getback: use32
    ; Set up 32 bit data segment stack
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; Load our old gdt and idts
    lgdt [c(sgdtptr)]
    lidt [c(sidtptr)]
    mov sp, [stack32p] ; We are going to restore our stack
    popa ; Pop out the stack, so now our stack is back when the code started
    ;sti ; We dont enable interrupts until very late in the boot proccess!
    mov eax, c(returnstatus) ; Return status address
    xchg bx, bx
    ret ; Return to the address pushed into the original stack
    jmp $ ; Make sure we don't execute past this (if ret somehow failed)

dd 0xDEADBEEF ; Marker to the start of the variable section

; ================================ Variables and all that stuff ====================================== ;
; We need somewhere to save our 32-bit stack
stack32p:
    dd 0x0000000
; We also needed to save the location of our original IDT
; Saving this along with the GDT is here as an extra precaution, as this code is going to be called BEFORE the OS messes around with anything
; So the likelyhood this is needed is small. Thus, I'm not putting that in here :)

; And point to the real mode IDT
rmodeivt:
    dw 0x03FF       ; The table is 256 entries @ 4b each
    dd 0x00000000   ; The real mode IVT is at 0x0000
; We need to save the state of our CR0
savecr0:
    dd 0
    
sidtptr:
    dw 0
    dd 0

sgdtptr:
    dw 0
    dd 0

; A temporary GDT that nobody is going to use
gdt16:
; Null
    dd 0x00000000
    dd 0x00000000
; Code
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
; Data
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
    
.rcode:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0x0F
    db 0x00
    
.rdata:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0x0F
    db 0x00

; Our gdt pointer structure
gdtptr:
    dw (gdtptr - gdt16 - 1)
    dd gdt16

returnstatus: dw 0
lastwidth:    dw 0
lastheight:   dw 0
largestMode:  dw 0
bpp:          dw 0
pitch:        dw 0
framebuffer1: dd 0

dd 0xBEBAD00D ; Marker to the end of the variable section

;La Fin, the end, done with this crap
lafin: