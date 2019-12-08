
X_OBJS=mbr.x loader.x
BOOTIMG=boot.img
TEMPIMG=.boot.img

all: ${X_OBJS}
	cat $^ > ${BOOTIMG}

%.x: %.asm defs.asm
	nasm $< -f bin -o $@

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -f ${X_OBJS} ${BOOTIMG}