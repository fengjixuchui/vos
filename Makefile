
all:
	nasm -f bin mbr.asm -o boot/mbr
	mkisofs -J -no-emul-boot -b mbr -boot-load-size 4 -VHELLO -o a.iso boot/

.PHONY : run
run: all
	bochs -f bochs.cfg
	rm -f boot/mbr a.iso
