[org 0x7c00] ;memory address for master boot record

mov si, str
call welcome

jmp $ ; loop

welcome:
    pusha
    loop:
        mov al, [si]
        cmp al,0
        jne print
        popa
        ret
    
    print:
        mov ah,0x0e ;function number = 0Eh : Display Character
        int 0x10 ;BIOS video service
        add si,1
        jmp loop

str: db 'Welcome!',0

times 510-($-$$) db 0
dw 0xaa55 ; Magic Number
