#include <stdint.h>
#include "../lib/keyboard_map.h"

#define LINES 25
#define COLUMNS 80
#define BYTES_PER_VID_ELEMENT 2
#define SCREENSIZE BYTES_PER_VID_ELEMENT * COLUMNS * LINES
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

// Location of the cursor, to write as an offset of vidptr
unsigned int current_loc = 0;
// Video memory starts at this address
char *vidptr = (char*)0xb8000;

struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

void idt_init(void) {
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    // IDT for keyboard
    keyboard_address = (unsigned long) keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    // ICW1 to begin initialization
    write_port(0x20 , 0x11);
    write_port(0xA0 , 0x11);
    // ICW2
    write_port(0x21 , 0x20);
    write_port(0xA1 , 0x28);
    // ICW3
    write_port(0x21 , 0x00);
    write_port(0xA1 , 0x00);
    // ICW4, which is the environment info
    write_port(0x21 , 0x01);
    write_port(0xA1 , 0x01);
    // Initialization finished
    write_port(0x21 , 0xff);
    write_port(0xA1 , 0xff);

    idt_address = (unsigned long)IDT ;
    idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16 ;
    load_idt(idt_ptr);
}

void puts(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        vidptr[current_loc++] = str[i++];
        vidptr[current_loc++] = 0x05;
    }
}

void next_line(void) {
    unsigned int line_size = BYTES_PER_VID_ELEMENT * COLUMNS;
    current_loc = current_loc + (line_size - current_loc % (line_size));
}

void new_prompt(void) {
    const char *prompt = "Pyramid@Kernel~$ ";
    next_line();
    puts(prompt);
}

void keyboard_init(void) {
    write_port(0x21 , 0xFD);
}

void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    /** Sending end-of-interrup acknowledgement */
    write_port(0x20, 0x20);

    /** Read the status from the keyboard status port */
    status = read_port(KEYBOARD_STATUS_PORT);

    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);

        if (keycode < 0)
            return;

        if (keycode == ENTER_KEY_CODE) {
            new_prompt();
            return;
        }

        vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
        vidptr[current_loc++] = 0x07;
    }
}

// VGA text buffer is located at physical memory address 0xB8000.
volatile uint16_t* vga_buffer = (uint16_t*)0xb8000;
//VGA mode 3 provides a text interface 80 characters wide and 25 characters lines per screen.
const int vga_col = 80,vga_row = 25;
int term_col,term_row = 0;
uint8_t term_color = 0x4; // red color for text

void clean_screen(){
    // VGA text buffer is located at physical memory address 0xB8000.
    char *vga = (char*)0xb8000;
    int i=0;

    while( i < vga_col*vga_row*2){
        vga[i++] = 0;
    }
}

void print(const char* string){
    for (int i = 0; string[i] != '\0'; i ++){
        switch (string[i]){
            // Newline characters should return the column to 0, and increment the row.
            case '\n':{
                term_col = 0;
                term_row ++;
                break;
            }
            // Normal characters just get displayed and then increment the column
            default:{
                int  index = (vga_col * term_row) + term_col;
                vga_buffer[index] = ((uint16_t)term_color << 8) | string[i];
                term_col ++;
                break;
                }
            }
            // What happens if we get past the last column?
            //We need to reset the column to 0, and increment the row to get to a new line.
            if (term_col >= vga_col){
                term_col = 0;
                term_row ++;
            }
            // What happens if we get past the last row?
            // We need to reset both column and row to 0 in order to loop back to the top of the screen.
            if (term_row >= vga_row)   {
                term_col = 0;
                term_row = 0;
            }
    }
}

void main(){
    clean_screen();
    print("Welcome to PyramidKernel!\n");
    next_line();
    new_prompt();
    idt_init();
    keyboard_init();

    while(1); //for continuously input
}
