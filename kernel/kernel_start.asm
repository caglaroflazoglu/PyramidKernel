[BITS 32]
global start
extern main

MAGIC equ 0x1badb002
FLAGS equ 0x00
CHECKSUM equ (0-(MAGIC+FLAGS))

section .text
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


global start
global keyboard_handler
global read_port
global write_port
global load_idt

extern boot
extern keyboard_handler_main

read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

keyboard_handler:
	call    keyboard_handler_main
	iretd


start:
    cli
    mov esp, stack_area
    call main
    hlt

section .bss
    resb 8192	;8KB for stack
    stack_area:
