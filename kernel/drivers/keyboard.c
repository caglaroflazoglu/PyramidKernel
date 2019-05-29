#include <stdbool.h>

#include "keyboard.h"
#include "isr.h"
#include "io.h"
#include "../lib/string.h"
#include "../lib/shell.h"
#include "keyboard_map.h"

#include "shellcmds.h"

#include <stdint.h>

#define KBD_DATA_PORT 0x60

/* keyboard interface IO port: data and control
   READ:   status port
   WRITE:  control register */
#define KBRD_INTRFC 0x64
 
/* keyboard interface bits */
#define KBRD_BIT_KDATA 0 /* keyboard data is in buffer (output buffer is empty) (bit 0) */
#define KBRD_BIT_UDATA 1 /* user data is in buffer (command buffer is empty) (bit 1) */
 
#define KBRD_IO 0x60 /* keyboard IO port */
#define KBRD_RESET 0xFE /* reset CPU command */
 
#define bit(n) (1<<(n)) /* Set bit n to 1 */
 
/* Check if bit n in flags is set */
#define check_flag(flags, n) ((flags) & bit(n))

void reboot(){
    uint8_t temp;
    asm volatile ("cli"); /* disable all interrupts */
 
     /* Clear all keyboard buffers (output and command buffers) */
    do{
      temp = inb(KBRD_INTRFC); /* empty user data */
      if(check_flag(temp, KBRD_BIT_KDATA) != 0)
        inb(KBRD_IO); /* empty keyboard data */
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);
 
    outb(KBRD_INTRFC, KBRD_RESET); /* pulse CPU reset line */
loop:
    asm volatile ("hlt"); /* if that didn't work, halt the CPU */
    goto loop; /* if a NMI is received, halt again */
}

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