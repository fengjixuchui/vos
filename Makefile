
BOOTIMG=vos.iso
CC_ARGS=
KERNEL_OBJS= \
            src/boot/multiboot2.s64 \
            src/bochs/bochs.s64 \
            src/vos/amd/amd.c64 \
            src/vos/intel/intel.c64 \
            src/vos/intel/vmx.c64 \
            src/vos/kernel.c64 \
            src/vos/memory.c64 \
            src/vos/stdio.c64 \
            src/vos/x86_64.s64 \
            src/vos/vmx.s64 \
            src/vos/svm.s64 \


all: kernel
	rm -f ${BOOTIMG} disc/kernel
	#dd if=kernel  of=disc/kernel64 ibs=1048576 conv=sync,notrunc oflag=append
	mv kernel disc/
	grub-mkrescue --output ${BOOTIMG} disc

%.s32: %.asm
	nasm -Iinclude/ -f elf32 $< -o $@

%.s64: %.asm
	nasm -Iinclude/ -f elf64 $< -o $@

%.c32: %.c
	cc $< -o $@ -c -Iinclude -m32 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic -O0 -g

%.c64: %.c
	cc $< -o $@ -c -Iinclude -m64 -Wall -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -fno-pic -O0 -g

kernel: ${KERNEL_OBJS}
	#ld --oformat binary -m elf_x86_64 -s -n -o $@ -T kernel.ld $^  # 链接成纯二进制代码
	#ld -m elf_x86_64 -s -n -o $@ -T kernel.ld $^
	ld -m elf_x86_64 -n -o $@ -T kernel.ld $^                       # 去掉-s就可以保留调试信息了

.PHONY : run-qemu
run-qemu: all
	sudo qemu-system-x86_64 -cpu host -cdrom ${BOOTIMG} --enable-kvm -no-reboot -no-shutdown

.PHONY : run-qemu-debug
run-qemu-debug: all
	sudo qemu-system-x86_64 -cpu host -cdrom ${BOOTIMG} --enable-kvm -no-reboot -no-shutdown -gdb tcp::1234

.PHONY : run-qemu-amd-debug
run-qemu-amd-debug: all
	qemu-system-x86_64 -cdrom ${BOOTIMG} -no-reboot -no-shutdown -gdb tcp::1234

.PHONY : run-bochs
run-bochs: all
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
