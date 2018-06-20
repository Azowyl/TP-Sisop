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
		// (1) Guardar registros.
		push %ebp
		push %ebx
		push %edi
		push %esi

		// (2) Preparar argumentos de la llamada.
		movl $0xB0, %ecx
		movl $14, %edx
		movl $breakpoint_msg, %eax

		// (3) Invocar a vga_write2()
		call vga_write2

		// (4) Restaurar registros.
		pop %esi
		pop %edi
		pop %ebx
		pop %ebp

		// (5) Finalizar ejecución del manejador.
		iret

.data
breakpoint_msg:
		.asciz "Hello, breakpoint"
