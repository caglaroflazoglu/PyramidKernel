#define LINES 25
#define COLUMNS 80
#define BYTES_PER_VID_ELEMENT 2

unsigned int current_loc = 0;// Location of the cursor, to write as an offset of vidptr
char *vidptr = (char*)0xb8000;// Video memory starts at this address

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
