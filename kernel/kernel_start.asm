[BITS 32]			 ;All instructions should be 32-bit.

section .multiboot_header
header_start:
  dd 0xe85250d6                ; magic number
  dd 0                         ; protected mode code
  dd header_end - header_start ; header length

  ; checksum
  dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

  ; required end tag
  dw 0  ; type
  dw 0  ; flags
  dd 8  ; size
header_end:


global start	;Kernel entry point
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
	sti 	;turn on interrupts
	ret

keyboard_handler:
	call keyboard_handler_main
	iretd

start:
	cli 	;Disable interrupts.
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
