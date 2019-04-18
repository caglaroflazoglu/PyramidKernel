global keyboard_handler
global read_port
global write_port

extern keyboard_handler_main

read_port:
    mov edx,[esp+4]
    in al,dx
    ret

write_port:
    mov edx,[esp+4]
    mov al,[esp+4+4]
    out dx,al
    ret

keyboard_handler:
    call keyboard_handler_main
    iretd
