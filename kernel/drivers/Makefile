OBJECTS = keyboard.o serial.o isr.o descriptor_tables.o

CC = gcc
CFLAGS = -m32 -fno-stack-protector \
					-ffreestanding \
					-Wall -Wextra -g -c # -Werror

all:$(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
