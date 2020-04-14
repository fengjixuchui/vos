
BOOTIMG=vos2.img

all: mbr loader kernel
#	cat $^ > ${BOOTIMG}
	rm -f ${BOOTIMG}
	dd if=mbr     of=${BOOTIMG} ibs=512     conv=sync,notrunc oflag=append
	dd if=loader  of=${BOOTIMG} ibs=4096    conv=sync,notrunc oflag=append
	dd if=kernel  of=${BOOTIMG} ibs=1048576 conv=sync,notrunc oflag=append

mbr: src/boot/mbr.asm
	nasm -Iinclude/ -f bin $< -o $@

%.s32: %.asm
	nasm -Iinclude/ -f elf32 $< -o $@

%.s64: %.asm
	nasm -Iinclude/ -f elf64 $< -o $@

%.c32: %.c
	cc $< -o $@ -c -Iinclude -m32 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

%.c64: %.c
	cc $< -o $@ -c -Iinclude -m64 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

# loader.s32必须排在第一个,否则无法进入真正的入口点.
loader: src/boot/loader.s32 src/boot/loader.c32 src/vos/memory.c32 src/bochs/bochs.s32
	ld --oformat binary -m elf_i386 -s -n -o $@ -T loader.ld $^       # 链接成纯二进制代码

kernel: src/vos/kernel.s64 src/vos/kernel.c64 src/vos/memory.c64 src/bochs/bochs.s64 src/vos/x86_64.s64 src/vos/intel/intel.c64 src/vos/amd/amd.c64 src/vos/stdio.c64
	ld --oformat binary -m elf_x86_64 -s -n -o $@ -T kernel.ld $^     # 链接成纯二进制代码

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -f mbr loader kernel
	find . -name *.img | xargs rm -f
	find . -name *.o   | xargs rm -f
	find . -name *.s32 | xargs rm -f
	find . -name *.s64 | xargs rm -f
	find . -name *.c32 | xargs rm -f
	find . -name *.c64 | xargs rm -f
