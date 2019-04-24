#include <stdbool.h>

#include "keyboard.h"
#include "isr.h"
#include "io.h"
#include "string.h"
#include "framebuffer.h"
#include "keyboard_map.h"

#define KBD_DATA_PORT 0x60

int capsLock = 0;
int shiftDown = 0;

static void keyboard_cb();

extern unsigned char keyboard_map[128];

static void keyboard_cb() {
  unsigned char scan_code = inb(KBD_DATA_PORT);
  unsigned char keycode = keyboard_map[scan_code];
  if(scan_code & 0x80){
      // Key was just released.
      if(scan_code == 0xAA || scan_code == 0xB6){
           shiftDown = 0;
      }
  }else{

      if(keycode == 17) {
          shiftDown = 1;
          return;
      } else if(keycode == 174) {
          // Left arrow key
          back_pos();
          return;
      } else if(keycode == 175) {
          // Right arrow key
          advance_pos();
          return;
      }
      /*
       * An ASCII Latin lowercase letter and its
       * uppercase counterpart differ in the 5th 
       * bit. 
       * Example: A = 01000001
       *          a = 01100001
       *
       * To make uppercase, we simply AND a Latin letter
       * with 0b1101111, or 0xDF. 
       * To toggle the case, we can XOR the letter with
       * 0b00100000, or 0x20, to toggle the 5th bit.
       */
      if(shiftDown){
        keycode = keyboard_map[scan_code + 90];
      }

      if(keycode == 0x0A) {
          init_prompt();
      }
      else{
        printf("%c", keycode);
      }
  }

}

void init_keyboard() {
  register_interrupt_handler(IRQ1, &keyboard_cb);
}
