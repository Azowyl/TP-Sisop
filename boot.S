#define MAGIC 0x1BADB002
#define FLAGS 0
#define CRC ( -(MAGIC + FLAGS) )

.align 4
multiboot:
    .long MAGIC
    .long FLAGS
    .long CRC