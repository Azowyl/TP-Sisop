#include "decls.h"
#include "multiboot.h"
#include "lib/string.h"

#define BUF_SIZE 256

extern void two_stacks();

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    two_stacks();
}