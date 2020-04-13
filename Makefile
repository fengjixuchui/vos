
X_OBJS=mbr.x loader.x kernel64
BOOTIMG=boot.img
ARCH_LD=elf_x86_64
CC_ARGS= -c -Iinclude -m64 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic #-O3

all: ${X_OBJS}
	cat $^ > ${BOOTIMG}

%.x: %.asm defs.asm x86_64.asm vmx.asm misc.asm
	nasm $< -f bin -o $@

%.o: %.c
	cc $< -o $@ ${CC_ARGS}

kernel64: kernel64.o linker.ld
	ld --oformat binary -m ${ARCH_LD} -s -n -o $@ -T linker.ld kernel64.o      # 链接成纯二进制代码

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -f *.x *.img *.o kernel64
