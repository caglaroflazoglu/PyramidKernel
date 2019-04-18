global    _gdt_flush    ;This is declared in C as 'extern void gdt_flush();'
global    load_idt

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

load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti     ;turn on interrupts
    ret

