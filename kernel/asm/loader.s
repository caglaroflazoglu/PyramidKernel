[BITS 32]		;All instructions should be 32-bit.

global start	;Kernel entry point
extern main

start:
	cli 		;Disable interrupts.
	mov esp, stack_area
	call main
	hlt

section .bss
	resb 8192	;8KB for stack
	stack_area:
