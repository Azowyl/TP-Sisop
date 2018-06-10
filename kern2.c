#include "decls.h"
#include "multiboot.h"
#include "string.h"

#define BUF_SIZE 256

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    if (mbi->flags) {
    	// if multiboot info its ok

        char buf[BUF_SIZE] = "cmdline: ";
        char *cmdline = (void *) mbi->cmdline;
       
        strlcat(buf, cmdline, BUF_SIZE - strlen(buf));

        vga_write(buf, 9, 0x07);
    }
}