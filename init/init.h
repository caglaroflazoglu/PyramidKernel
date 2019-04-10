#include "../lib/keyboard_map.h"
#include "../lib/kernel/tty.h"
#include "../lib/stdio.h"
#include "../lib/gdt.h"
#include "../lib/idt.h"

void init_system(){
    gdt_install();
    next_line();
    new_prompt();
    idt_init();
    keyboard_init();
    print("Welcome to PyramidKernel!\n");
}
