ASM=nasm
ASMFLAGS=-felf32
CC=gcc
CCFLAGS=-m32 -c -fno-stack-protector -ffreestanding -O2 -Wall -Wextra
LD=ld
LDFLAGS=-melf_i386 -nostdlib -O2

all:
	mkdir -p obj/ dist/
	$(ASM) $(ASMFLAGS) kernel/kernel_start.asm -o obj/kernel_start.o
	$(CC) $(CCFLAGS) kernel/kernel.c -o obj/kernel.o
	$(LD) $(LDFLAGS) -T link.ld -o dist/kernel obj/kernel_start.o obj/kernel.o
iso:
	cp dist/kernel iso_root/boot/
	grub-mkrescue iso_root -o PyramidKernel.iso
run:
	qemu-system-i386 -kernel dist/kernel
clean:
	rm -rf obj/ dist/ iso_root/boot/kernel
