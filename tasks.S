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
