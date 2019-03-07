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
global load_gdt

extern boot
extern main
extern keyboard_handler_main

; GDT with a NULL Descriptor, a 32-Bit code Descriptor
; and a 32-bit Data Descriptor
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

; GDT descroptor record
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Load GDT and set selectors for a flat memory model
load_gdt:
    lgdt [gdt_descriptor]
    jmp CODE_SEG:.setcs              ; Set CS seelctor with far JMP
.setcs:
    mov eax, DATA_SEG                ; Set the Data selectors to defaults
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    ret

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
    call load_gdt
    call main
    hlt

section .bss
    resb 8192	;8KB for stack
    stack_area:
