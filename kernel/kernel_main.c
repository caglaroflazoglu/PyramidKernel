#include <stdint.h>

#include "descriptor_tables.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "multiboot.h"
#include "serial.h"
#include "string.h"
#include "isr.h"

void main() {
	clear_screen();
	init_descriptor_tables();
	welcome_text();   
	init_prompt();
	init_keyboard();
	return;
}
