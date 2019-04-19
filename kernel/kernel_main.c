#include <stdint.h>

#include "descriptor_tables.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "multiboot.h"
#include "serial.h"
#include "string.h"
#include "isr.h"

void main() {
   fb_clear();
   init_descriptor_tables();
   printf("Pyramid@Kernel~$");
   init_keyboard();
   return;
}
