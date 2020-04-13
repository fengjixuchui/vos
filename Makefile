
SXX_OBJS=mbr.bin
C32_OBJS=loader
C64_OBJS=kernel
BOOTIMG=boot.img

all: ${SXX_OBJS} ${C32_OBJS} ${C64_OBJS}
	cat $^ > ${BOOTIMG}

%.bin: %.asm
	nasm $< -f bin -o $@

%.s32: %.asm
	nasm $< -f elf32 -o $@

%.s64: %.asm
	nasm $< -f elf64 -o $@

%.c32: %.c
	cc $< -o $@ -c -Iinclude -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

%.c64: %.c
	cc $< -o $@ -c -Iinclude -m64 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

# loader.s32必须排在第一个,否则无法进入真正的入口点.
loader: src/loader/loader.s32 src/loader/loader.c32 src/misc/memory.c32 src/bochs/bochs.s32
	ld --oformat binary -m elf_i386 -s -n -o $@ -T loader.ld $^       # 链接成纯二进制代码

kernel: src/vos/kernel.c64 src/misc/memory.c64 src/bochs/bochs.s64
	ld --oformat binary -m elf_x86_64 -s -n -o $@ -T kernel.ld $^     # 链接成纯二进制代码

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -rf *.bin *.img *.o *.s32 *.s64 *.c32 *.c64 loader kernel
