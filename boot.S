#include "multiboot.h"

#define KSTACK_SIZE 8192

.align 4
multiboot:
    .long MULTIBOOT_HEADER_MAGIC
    .long 0
    .long -(MULTIBOOT_HEADER_MAGIC)

.globl _start
_start:
    // Paso 1: Configurar el stack antes de llamar a kmain.
    movl $0, %ebp
    movl $kstack, %esp
    push %ebp

    // Paso 2: pasar la información multiboot a kmain. Si el
    // kernel no arrancó vía Multiboot, se debe pasar NULL.
    //
    // Usar una instrucción de comparación (TEST o CMP) para
    // comparar con MULTIBOOT_BOOTLOADER_MAGIC, pero no usar
    // un salto a continuación, sino una instrucción CMOVcc
    // (copia condicional).
    // ...

    movl $0, %ecx 

    movl (%eax), %edx
    cmp %edx, MULTIBOOT_BOOTLOADER_MAGIC

    cmovz %ebx, %ecx

    push %ecx

    call kmain
halt:
    hlt
    jmp halt

.data
.p2align 12 // aling to 4 KiB
kstack:
    .space KSTACK_SIZE