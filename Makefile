
SRCS := $(shell cd src; find . -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=build/%.o)
DEPS := $(OBJS:.o=.d)

all: $(OBJS)
	mkdir -p build
	as --32 boot.s -o build/boot.o
	ld -m elf_i386 -T linker.ld -o qmos.bin -nostdlib $^ build/boot.o
	grub-file --is-x86-multiboot qmos.bin
	mkdir -p isodir/boot/grub
	cp qmos.bin isodir/boot/qmos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o qmos.iso isodir

build/%.c.o: src/%.c
	mkdir -p $(dir $@)
	gcc-10 -m32 -c $< -o $@ -ffreestanding -O2 -Wall -Wextra -Iinclude/ -MMD -MP -std=gnu11

build/%.s.o: src/%.s
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

clean:
	rm -f qmos.iso
	rm -rf build

-include $(DEPS)
