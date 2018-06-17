// Realiza una llamada a "entry" sobre el stack proporcionado.
// void task_exec(uintptr_t entry, uintptr_t stack);

.text
.globl task_exec
task_exec:
	// Preámbulo estándar
	push %ebp
	movl %esp, %ebp

	movl 8(%ebp), %ebx	// Se obtiene entry
	movl 12(%ebp), %eax	// Se obtiene stack

	movl %eax, %esp		// Se setea el stack pointer al stack pasado como parametro

	call %ebx			// Se llama a entry

	leave 
	ret

.globl task_swap
task_swap:

	//	Callee-saved registers: EBP, EBX, EDI, ESI
	//	Guardo callee-saved regisers
	push %ebp
	push %ebx
	push %edi
	push %esi

	//	Swap de stack pointers -> CAMBIO DE STACKS!!1111
	movl %esp, %eax		// guardo el stack pointer actual en eax
	movl 16(%ebp), %ebx // guardo el argumento en ebx

	movl %eax, 16(%ebp) // guardo esp en donde estaba el argumento stack nuevo
	movl %ebx, %esp		// guardo en esp el argumento que seria el stack nuevo

	// restauro los registros callee-saved
	pop %esi
	pop %edi
	pop %ebx
	pop %ebp

	ret