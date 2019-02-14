build:
	nasm -fbin boot.asm -o boot.bin
start:
	qemu-system-x86_64 -fda  boot.bin
iso:
	truncate boot.bin -s 1200k
	mkisofs -o pyramid.iso -b boot.bin ./
clean:
	rm boot.bin pyramid.iso
