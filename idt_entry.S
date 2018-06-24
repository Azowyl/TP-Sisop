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
	// Guardar registros e invocar handler
	pusha
	call timer

	// Ack *antes* de llamar a sched()
	movl $ACK_IRQ, %eax
	outb %al, $PIC1

	// Llamada a sched con argumento
	push %esp
	call sched

	// Retornar (si se volvió de sched)
	addl $4, %esp
	popa
	iret
	
.globl keyboard_asm
keyboard_asm:
	// Guardar registros.
	push %ebp
	push %ebx
	push %edi
	push %esi

	call keyboard
	
	// Restaurar registros.
	pop %esi
	pop %edi
	pop %ebx
	pop %ebp

	jmp ack_irq

.globl divzero
divzero:
	// (1) Guardar registros.
	push %ebp
	push %ebx
	push %edi
	push %esi

	// (2) Incrementar ebx
	inc %ebx

	// (3) Llamada a vga_write_cyan
	movl $17, %edx				//	linea
	movl $divzero_sol, %eax		//	texto

	// (4) Invocar a vga_write2()
	call vga_write_cyan

	// (5) Restaurar registros.
	pop %esi
	pop %edi
	pop %ebx
	pop %ebp

	// (6) Finalizar ejecución del manejador.
	jmp ack_irq

.data
divzero_sol:
		.asciz "Se divide por ++ebx"


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
