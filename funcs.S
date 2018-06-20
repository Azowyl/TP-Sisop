.text
.globl vga_write2
vga_write2:
        push %ebp
        movl %esp, %ebp

        push %ecx // el color
        push %edx // la linea
        push %eax // el texto
        call vga_write

        pop %ecx
        pop %edx
        pop %eax

        leave
        ret
