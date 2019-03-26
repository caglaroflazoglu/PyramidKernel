#include "../lib/keyboard_map.h"
#include "../lib/kernel/tty.h"
#include "../lib/stdio.h"
#include "../lib/gdt.h"
#include "../lib/idt.h"

void main(){
    gdt_install();
    clearScreen();
    print("Welcome to PyramidKernel!\n");
    next_line();
    new_prompt();
    idt_init();
    keyboard_init();

    while(1); //for continuously input
}
