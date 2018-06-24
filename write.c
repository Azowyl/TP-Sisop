#include "decls.h"

#define SCREEN_WIDTH 80

// retorna el ancho del numero val
static size_t int_width(uint64_t val) {
	size_t counter = 0;

	while (val > 0) {
		val = val / 10;
		counter++;
	}

	return counter;
}

bool fmt_int(uint64_t val, char *s, size_t bufsize) {
	size_t l = int_width(val);

	if (l >= bufsize)  // >= porque si es igual no queda espacio para el fin de linea \0
		return false;

	s += l;
	*(s--) = '\0';

	while (val > 0) {
		char digit = val % 10;
		*(s--) = digit + '0';
		val = val / 10;
	}

	return true;
}

void vga_write(const char *s, int8_t linea, uint8_t color) { 
	volatile char *video = (volatile char*)0xB8000;

	for (int i = 0; i < linea * SCREEN_WIDTH; i++) { video += 2; }

	while(*s != 0)
	{
		*video++ = *s++;
		*video++ = color;
	}
}

void __attribute__((regparm(2)))
vga_write_cyan(const char *s, int8_t linea) {
	vga_write(s, linea, 0xB0);
}