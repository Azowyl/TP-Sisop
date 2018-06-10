CFLAGS := -g -std=c99 -Wall -Wextra -Wpedantic -fasm
CFLAGS += -m32 -O1 -ffreestanding

SRCS := $(wildcard *.c) $(wildcard lib/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

kern2: boot.o $(OBJS)
	ld -m elf_i386 -Ttext 0x100000 $^ -o $@
# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS) -c $<
	echo Build done

clean:
	rm -f kern2 *.o lib/*.o core
	echo Clean done

.PHONY: clean

QEMU := qemu-system-i386 -serial mon:stdio
KERN ?= kern2
BOOT := -kernel $(KERN) $(QEMU_EXTRA)

qemu: $(KERN)
	echo Qemu initialized
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -kernel kern2 -S -gdb tcp:127.0.0.1:7508 $(BOOT)
	echo Debugging initialized

gdb:
	gdb -q -s kern2 -n -ex 'target remote 127.0.0.1:7508'

.PHONY: qemu qemu-gdb gdb

$(V) .SILENT: