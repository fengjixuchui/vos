
X_OBJS=mbr.x loader.x
IMG=boot.img

all: ${X_OBJS}
	cat $^ > ${IMG}

%.x: %.asm
	nasm $< -f bin -o $@

.PHONY : run
run: all
	bochs -f bochs.cfg

.PHONY : clean
clean:
	rm -f ${X_OBJS} ${IMG}