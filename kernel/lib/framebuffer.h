#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <stdint.h>

#define FB_BLACK          0
/*
#define FB_BLUE           1
#define FB_GREEN          2
#define FB_CYAN           3
#define FB_RED            4
#define FB_MAGENTA        5
#define FB_BROWN          6
#define FB_LIGHT_GREY     7
#define FB_DARK_GREY      8
#define FB_LIGHT_BLUE     9
#define FB_LIGHT_GREEN    10
#define FB_LIGHT_CYAN     11
#define FB_LIGHT_RED      12
#define FB_LIGHT_MAGENTA  13
#define FB_LIGHT_BROWN    14
*/
#define FB_WHITE    15

#define FB_BASE_ADDRESS 0x000B8000
#define FB_WIDTH 80
#define FB_HEIGHT 25


/* The framebuffer I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The framebuffer I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/**
 * Writes a character with the given foreground and background to position i
 * within the frame buffer.
 * fb and bg must be between 0 and 15
 * i must be between 0 and fb_width*fb_height = 80*25 = 2000
 *
 * Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
 * Content: | ASCII                 | FG      | BG      |
 *
 */
void write_cell(short i, char c, unsigned char fg, unsigned char bg);

// Moves cursor foward in the framebuffer
void move_cursor(unsigned short pos);

// Moves cursor forwards or backwards a single cell
void advance_pos();
void back_pos();

// Writes C string to framebuffer and advances cursor 
void write(char *buf, unsigned int len);
void write_str(char *buf);

//initialize prompt
void init_prompt();
void welcome_text();

// clear the screen
void clear_screen();
void clear_row(uint8_t row);

// scroll down one row
void scroll_down();
void newline();

#endif