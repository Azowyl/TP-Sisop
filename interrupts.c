#include "decls.h"
#include "interrupts.h"

#define LOW_15_BITS_MASK  0x00007FFF

static struct IDTR idtr;
static struct Gate idt[256];

// Multiboot siempre define "8" como el segmento de código.
// (Ver campo CS en `info registers` de QEMU.)
static const uint8_t KSEG_CODE = 8;

// Identificador de "Interrupt gate de 32 bits" (ver IA32-3A,
// tabla 6-2: IDT Gate Descriptors).
static const uint8_t STS_IG32 = 0xE;

void idt_init(void) {
	// (1) Instalar manejadores ("interrupt service routines").
	idt_install(T_BRKPT, breakpoint);
	idt_install(T_DIVIDE, divzero);

	// (2) Configurar ubicación de la IDT.
	idtr.base = (uintptr_t) idt;
	idtr.limit = 2047;

	// (3) Activar IDT.
	asm("lidt %0" : : "m"(idtr));
}

void irq_init() {
	// (1) Redefinir códigos para IRQs.
	irq_remap();

	// (2) Instalar manejadores.
	idt_install(T_TIMER, timer_asm);
	idt_install(T_KEYBOARD, keyboard);

	// (3) Habilitar interrupciones.
	asm("sti");
}

void idt_install(uint8_t n, void (*handler)(void)) {
	uintptr_t addr = (uintptr_t) handler;

	idt[n].rpl = 0;
	idt[n].type = STS_IG32;
	idt[n].segment = KSEG_CODE;

	idt[n].off_15_0 = addr & LOW_15_BITS_MASK;
	idt[n].off_31_16 = addr >> 16;

	idt[n].present = 1;
}