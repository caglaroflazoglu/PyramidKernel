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

start:
    cli
    mov esp, stack_area
    call main
    hlt

section .bss
    resb 8192	;8KB for stack
    stack_area:
