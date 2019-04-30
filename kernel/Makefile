CC=gcc
CCFLAGS=-m32 -c -fno-stack-protector -ffreestanding -O2 -Wall -Wextra

all:
	$(CC) $(CCFLAGS) keyboard.c -o keyboard.o
	$(CC) $(CCFLAGS) serial.c -o serial.o
	$(CC) $(CCFLAGS) framebuffer.c -o framebuffer.o
	$(CC) $(CCFLAGS) kernel_main.c -o kernel_main.o
	$(CC) $(CCFLAGS) string.c -o string.o
	$(CC) $(CCFLAGS) descriptor_tables.c -o descriptor_tables.o
	$(CC) $(CCFLAGS) isr.c -o isr.o
	$(CC) $(CCFLAGS) heap.c -o heap.o
