[BITS 32]
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
extern main
extern keyboard_handler_main


read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

write_port:
	mov	edx, [esp + 4]
	mov	al, [esp + 4 + 4]
	out	dx, al
	ret

load_idt:
	mov	edx, [esp + 4]
	lidt [edx]
	sti               		  	;turn on interrupts
	ret

keyboard_handler:
	call keyboard_handler_main
	iretd

start:
	cli
	mov esp, stack_area
	call main
	hlt


; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'

global _gdt_flush
extern gp
_gdt_flush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2
flush2:
	ret

section .bss
	resb 8192   ;8KB for stack
	stack_area:
