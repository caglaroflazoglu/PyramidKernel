build:
	nasm -fbin boot.asm -o boot.bin
clean:
	rm boot.bin
