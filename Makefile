
BOOTIMG=vos2.iso

all: kernel
	rm -f ${BOOTIMG} disc/kernel64
	#dd if=kernel  of=disc/kernel64 ibs=1048576 conv=sync,notrunc oflag=append
	mv kernel disc/
	grub-mkrescue --output ${BOOTIMG} disc

%.s32: %.asm
	nasm -Iinclude/ -f elf32 $< -o $@

%.s64: %.asm
	nasm -Iinclude/ -f elf64 $< -o $@

%.c32: %.c
	cc $< -o $@ -c -Iinclude -m32 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

%.c64: %.c
	cc $< -o $@ -c -Iinclude -m64 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

kernel: src/boot/multiboot2.s64 src/vos/kernel.c64 src/vos/memory.c64 src/bochs/bochs.s64 src/vos/x86_64.s64 src/vos/intel/intel.c64 src/vos/intel/vmx.c64 src/vos/amd/amd.c64 src/vos/stdio.c64
	#ld --oformat binary -m elf_x86_64 -s -n -o $@ -T kernel.ld $^     # 链接成纯二进制代码
	ld -m elf_x86_64 -s -n -o $@ -T kernel.ld $^     # 链接成纯二进制代码

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -f mbr loader kernel
	find . -name '*.iso' | xargs rm -f
	find . -name '*.o'   | xargs rm -f
	find . -name '*.s32' | xargs rm -f
	find . -name '*.s64' | xargs rm -f
	find . -name '*.c32' | xargs rm -f
	find . -name '*.c64' | xargs rm -f
