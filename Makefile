ASM=nasm
ASMFLAGS=-felf32
CC=gcc
CCFLAGS=-m32 -c -fno-stack-protector
LD=ld
LDFLAGS=-melf_i386

all:
	mkdir -p obj
	mkdir -p dist
	$(ASM) $(ASMFLAGS) kernel/kernel_start.asm -o obj/kernel_start.o
	$(CC) $(CCFLAGS) kernel/kernel.c -o obj/kernel.o
	$(LD) $(LDFLAGS) -T link.ld -o dist/kernel obj/kernel_start.o obj/kernel.o

iso:
	cp dist/kernel iso_root/boot/
	grub-mkrescue iso_root -o PyramidKernel.iso
run:
	qemu-system-i386 -kernel dist/kernel

clean:
	rm -rf obj/ dist/
