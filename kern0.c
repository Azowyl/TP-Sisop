#include "decls.h"

void comienzo(void) {
    vga_write("kern2 loading.............", 8, 0x70);

    while(1)
    	asm("hlt");
}