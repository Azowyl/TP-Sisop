#include "decls.h"

#define SCREEN_WIDTH 80

void vga_write(const char *s, int8_t linea, uint8_t color) { 
	volatile char *video = (volatile char*)0xB8000;
   
	for (int i = 0; i < linea * SCREEN_WIDTH; i++) { video++; }

    while(*s != 0)
    {
        *video++ = *s++;
        *video++ = color;
    }
}