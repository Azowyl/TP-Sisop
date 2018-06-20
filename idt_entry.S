#define PIC1 0x20
#define ACK_IRQ 0x20

.globl ack_irq
ack_irq:
	// Indicar que se manejó la interrupción.
	movl $ACK_IRQ, %eax
	outb %al, $PIC1
	iret

.globl timer_asm
timer_asm:
	// Guardar registros.
	push %ebp
	push %ebx
	push %edi
	push %esi

	call timer
	
	// Restaurar registros.
	pop %esi
	pop %edi
	pop %ebx
	pop %ebp

	jmp ack_irq

.globl breakpoint
breakpoint:
	nop
	test %eax, %eax
	iret
