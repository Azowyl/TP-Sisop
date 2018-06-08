#include "decls.h"

void vga_write(const char *s, int8_t linea, uint8_t color) { 
volatile char *video = (volatile char*)0xB8000;
    
	int space_code = 0x20;

	for (int i = 0; i < linea * 80; i++) {
		*video++ = space_code;
		*video++ = 0;
	}

    while(*s != 0)
    {
        *video++ = *s++;
        *video++ = color;
    }
}