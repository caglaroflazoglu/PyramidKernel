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
char temp[512];
int cmd_index = 0;
int shiftDown = 0;

// clear command
void clear_command(void) {
  for (int i = 0; i < 512; i++)
    command[i] = 0; 
}

void clear_temp(void) {
  for (int i = 0; i < 512; i++)
    temp[i] = 0; 
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
    // Backspace keycode
    if(keycode == 0x08){
      if (cmd_index <= 0){
        // if we're at the first character of the command
        // then avoid backspace to not to delete prompt.
        return;
      }
      cmd_index--;
           
      for (int i = 0; i < cmd_index; ++i)
        temp[i]=command[i];
      clear_command();
      for (int i = 0; i < cmd_index; ++i)
        command[i]=temp[i]; 

      clear_temp();
      back_space();
    }
    // Enter keycode
    else if(keycode == 0x0A) {
      /*
          {
              "annotation": "handler"
          }
      */
      /* start of generated code */
      if(!strcmp(command, "help")){
         printf("\nreboot: Reboots the system");
         printf("\nuname: Shows system information");
         printf("\nsleep: Sleeps given amount of seconds");
      }
      else if(!strcmp(command, "reboot")){
         reboot();
      }
      else if(!strcmp(command, "uname")){
         uname();
      }
      /* end of generated code */

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
