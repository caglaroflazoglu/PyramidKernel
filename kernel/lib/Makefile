OBJECTS = panic.o framebuffer.o string.o
CC = gcc
CFLAGS = -m32 -fno-stack-protector \
					-ffreestanding \
					-Wall -Wextra -g -c # -Werror

all:$(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

