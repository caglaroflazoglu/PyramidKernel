#include <stdint.h>

#include "descriptor_tables.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "multiboot.h"
#include "serial.h"
#include "string.h"
#include "isr.h"
#include "heap.h"

void main() {
	clear_screen();
	init_descriptor_tables();
	welcome_text();   

	uint32_t a = kmalloc(8);
	uint32_t b = kmalloc(8);
	uint32_t c = kmalloc(8);
	printf("a: %x\n", a);
	printf("b: %x\n", b);
	printf("c: %x\n", c);
	
	init_prompt();
	init_keyboard();
	return;
}
