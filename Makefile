
all:
	nasm -f bin mbr.asm -o boot.img

.PHONY : run
run: all
	bochs -f bochs.cfg
	rm -f boot/mbr boot.img
