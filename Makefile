build:
	nasm -f elf32 kernel_start.asm -o kernel_start.o
	gcc -m32 -c kernel.c -o kernel.o
	ld -m elf_i386 -T link.ld -o kernel kernel_start.o kernel.o
start:
	qemu-system-i386 -kernel kernel
iso:
	cp kernel iso_root/boot/
	grub-mkrescue iso_root -o PyramidKernel.iso
clean:
	rm kernel_start.o kernel.o kernel PyramidKernel.iso
