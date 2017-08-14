/******************************************************************************
*	mailbox.s
*	 by Alex Chadwick
*
*	A sample assembly code implementation of the screen01 operating system.
*	See main.s for details.
*
*	mailbox.s contains code that interacts with the mailbox for communication
*	with various devices.
******************************************************************************/

/* NEW
* GetMailboxBase returns the base address of the mailbox region as a physical
* address in register r0.
* C++ Signature: void* GetMailboxBase()
*/
.globl GetMailboxBase
GetMailboxBase: 
	ldr r0,=0x2000B880
	mov pc,lr

/* NEW
* MailboxWrite writes the value given in the top 28 bits of r0 to the channel
* given in the low 4 bits of r1.
* C++ Signature: void MailboxWrite(u32 value, u8 channel)
*/
.globl MailboxWrite
MailboxWrite: 
	tst r0,#0b1111
	movne pc,lr
	cmp r1,#15
	movhi pc,lr

	channel .req r1
	value .req r2
	mov value,r0
	push {lr}
	bl GetMailboxBase
	mailbox .req r0
		
	wait1$:
		status .req r3
		ldr status,[mailbox,#0x18]

		tst status,#0x80000000
		.unreq status
		bne wait1$

	add value,channel
	.unreq channel
	
	str value,[mailbox,#0x20]
	.unreq value
	.unreq mailbox
	pop {pc}

/* NEW
* MailboxRead returns the current value in the mailbox addressed to a channel
* given in the low 4 bits of r0, as the top 28 bits of r0.
* C++ Signature: u32 MailboxRead(u8 channel)
*/
.globl MailboxRead
MailboxRead: 
	cmp r0,#15
	movhi pc,lr

	channel .req r1
	mov channel,r0
	push {lr}
	bl GetMailboxBase
	mailbox .req r0
	
	rightmail$:
		wait2$:
			status .req r2
			ldr status,[mailbox,#0x18]
			
			tst status,#0x40000000
			.unreq status
			bne wait2$
		
		mail .req r2
		ldr mail,[mailbox,#0]

		inchan .req r3
		and inchan,mail,#0b1111
		teq inchan,channel
		.unreq inchan
		bne rightmail$
	.unreq mailbox
	.unreq channel

	and r0,mail,#0xfffffff0
	.unreq mail
	pop {pc}
