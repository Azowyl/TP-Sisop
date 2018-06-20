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