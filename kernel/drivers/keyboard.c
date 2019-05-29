#include <stdbool.h>

#include "keyboard.h"
#include "isr.h"
#include "io.h"
#include "../lib/string.h"
#include "../lib/shell.h"
#include "keyboard_map.h"
#include "shellcmds.h"

#define KBD_DATA_PORT 0x60
static void keyboard_cb();
extern unsigned char keyboard_map[128];

char command[512]; // max 512 bytes in a command
int cmd_index = 0;
int shiftDown = 0;

void clear_command(void) {
  for (int i = 0; i < 512; i++)
    command[i] = 0; // clear command
}

static void keyboard_cb() {
  unsigned char scan_code = inb(KBD_DATA_PORT);
  unsigned char keycode = keyboard_map[scan_code];

  if(scan_code & 0x80){
    if(scan_code == 0xAA || scan_code == 0xB6){
      shiftDown = 0;
    }
  }
  else{
    if(keycode == 0x0A) {
      if(!strcmp(command,"help")){
        printf("\nhelp - Print this text\n");
        printf("reboot - Reboot the kernel");
      }

      else if(!strcmp(command,"reboot")){
        reboot();
      }

      cmd_index=0;
      clear_command();
      init_prompt();
    }
    else{
      printf("%c", keycode);
      command[cmd_index] = keycode;
      cmd_index+=1;
    }
  }

}

void init_keyboard() {
  register_interrupt_handler(IRQ1, &keyboard_cb);
}