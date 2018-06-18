#include "decls.h"
#include "multiboot.h"

#define USTACK_SIZE 4096

void kmain(const multiboot_info_t *mbi) {
	vga_write("kern2 loading.............", 8, 0x70);

	two_stacks();
	two_stacks_c();		//	kern2-exec

	contador_run();		// Nueva llamada ej. kern2-swap.

    vga_write2("Funciona vga_write2?", 18, 0xE0);	//	kern2-regcall
}

static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void two_stacks_c() {
	// Inicializar al *tope* de cada pila.
	uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE - 1];
	uintptr_t *b = (uintptr_t *) &stack2[USTACK_SIZE - 1];

	// Preparar, en stack1, la llamada:
	// vga_write("vga_write() from stack1", 15, 0x57);

	// AYUDA 1: se puede usar alguna forma de pre- o post-
	// incremento/decremento, según corresponda:
	//
	//     *(a++) = ...
	//     *(++a) = ...
	//     *(a--) = ...
	//     *(--a) = ...

	// AYUDA 2: para apuntar a la cadena con el mensaje,
	// es suficiente con el siguiente cast:
	//
	//   ... a ... = (uintptr_t) "vga_write() from stack1";

	*(a--) = 0x57;
	*(a--) = 15;
	*a = (uintptr_t) "vga_write() from stack1";

	task_exec((uintptr_t) vga_write, (uintptr_t) a);

	// Preparar, en s2, la llamada:
	// vga_write("vga_write() from stack2", 16, 0xD0);

	// AYUDA 3: para esta segunda llamada, usar esta forma de
	// asignación alternativa:
	b -= 2;
	b[0] = (uintptr_t) "vga_write() from stack2";
	b[1] = 16;
	b[2] = 0xD0;

	// Primera llamada usando task_exec().
	// task_exec((uintptr_t) vga_write, (uintptr_t) b);

	// Segunda llamada con ASM directo. Importante: no
	// olvidar restaurar el valor de %esp al terminar, y
	// compilar con: -fasm -fno-omit-frame-pointer.
	
	asm("movl %%esp,%%ebp; movl %0,%%esp; call *%1; movl %%ebp,%%esp"
		: // no outputs
		: "r"(b), "r"(vga_write));
}
