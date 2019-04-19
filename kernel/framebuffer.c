#include <stdint.h>
#include "framebuffer.h"
#include "string.h"
#include "io.h"

// A convenient define which gives a char ptr to the frame buffer
#define FB_CHAR_PTR ((unsigned char *) FB_BASE_ADDRESS)

// A define that gives us uint16 ptr to the frame buffer.
// Cells are 2 bytes wide (16 bits) so this makes indexing more natural.
#define FB_UINT16_PTR ((uint16_t *) FB_BASE_ADDRESS)

static unsigned int fb_col = 0;
static unsigned int fb_row = 0;

void fb_write_cell(short i, char c, unsigned char fg, unsigned char bg) {
  unsigned char *fb = FB_CHAR_PTR;
  fb[i*2] = c;
  fb[i*2 + 1] = ((bg & 0x0f) << 4) | (fg & 0x0f);
}

void fb_move_cursor(unsigned short pos) {
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, pos & 0x00FF);
}

void fb_newline() {
  if (fb_row < FB_HEIGHT-1)
  // have room to add new line without scrolling
    fb_row++;
  else
  // must scroll down to add the new line
    fb_scroll_down();

  fb_col = 0;
  fb_move_cursor(fb_col + (fb_row * FB_WIDTH));
}

// advances cursor forward one character
void fb_advance_pos() {
  if (fb_col < FB_WIDTH-1)
    // have room to advance cursor in this row
    fb_col++;
  else
    // wrap around cursor to the start of the next line
    fb_newline();

  fb_move_cursor(fb_col + (fb_row * FB_WIDTH));
}

void fb_back_pos() {
    if (fb_col == 0){
        if(fb_row == 0) return;
        // We go up a row if we're in the first column
        // and not in the first row
        fb_col = FB_WIDTH - 1;
        fb_row--;
    } else {
        //
        fb_col--;
    }
    fb_move_cursor(fb_col + (fb_row * FB_WIDTH));
}

void fb_write(char *buf, unsigned int len) {
  unsigned int i;
  uint16_t pos;
  for (i=0; i<len; i++) {
    char c = buf[i];
    if (c == '\n' || c == '\r') {
      fb_newline();
    } else {
      pos = fb_col + (fb_row * FB_WIDTH);
      fb_write_cell(pos, c, FB_WHITE, FB_BLACK);
      fb_advance_pos();
    }
  }
}

void fb_write_str(char *buf) {
  fb_write(buf, strlen(buf));
}

void fb_clear() {
  fb_col = 0;
  fb_row = 0;

  int i;
  for (i=0; i<FB_WIDTH*FB_HEIGHT; i++) {
    fb_write_cell(i, ' ', FB_WHITE, FB_BLACK);
  }
  fb_move_cursor(0);
}

void fb_clear_row(uint8_t row) {
  size_t i;
  for (i=0; i<FB_WIDTH; i++) {
    fb_write_cell((row*FB_WIDTH)+i, ' ', FB_WHITE, FB_BLACK);
  }
}

void fb_scroll_down() {
  uint16_t *fb = FB_UINT16_PTR;
  memmove(fb, fb+FB_WIDTH, FB_WIDTH*2*(FB_HEIGHT*2-1));
  fb_clear_row(FB_HEIGHT-1);
}