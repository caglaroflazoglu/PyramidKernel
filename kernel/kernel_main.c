#include <stdint.h>

#include "drivers/descriptor_tables.h"
#include "lib/framebuffer.h"
#include "drivers/keyboard.h"
#include "lib/multiboot.h"
#include "drivers/serial.h"
#include "lib/string.h"
#include "mm/paging.h"
#include "drivers/isr.h"
#include "mm/kheap.h"


void main() {
   clear_screen();
   welcome_text();
   init_descriptor_tables();
   init_paging();
   init_keyboard();

   uint32_t a = kmalloc(8);
   uint32_t b = kmalloc(8);
   uint32_t c = kmalloc(8);
   printf("a: %x\n", a);
   printf("b: %x\n", b);
   printf("c: %x\n", c);
   
   init_prompt();
   uint32_t *ptr = (uint32_t *)0xA0000000;
   uint32_t do_page_fault = *ptr;

   return;
}
