.text
.globl vga_write2
vga_write2:
        push %ebp
        movl %esp, %ebp

        push %ecx
        push %edx
        push %eax
        call vga_write

        pop %ecx
        pop %edx
        pop %eax

        leave
        ret
