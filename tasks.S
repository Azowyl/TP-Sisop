// Realiza una llamada a "entry" sobre el stack proporcionado.
// void task_exec(uintptr_t entry, uintptr_t stack);

.text
.globl task_exec
task_exec:
	// Preámbulo estándar
    push %ebp
    movl %esp, %ebp

   	movl %ebx, 8(%ebp) // se obtiene entry
   	movl %eax, 12(%ebp) // se obtiene stack

    movl %eax, %esp	// se setea el stack pointer al stack pasado como parametro

    call %ebx // se llama a entry

    leave 
    ret
