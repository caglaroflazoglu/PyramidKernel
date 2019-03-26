#define termColor 0x4; // red color for text
#define LINES 25
#define COLUMNS 80
#define BYTES_PER_VID_ELEMENT 2
#define SCREENSIZE BYTES_PER_VID_ELEMENT * COLUMNS * LINES

char *vgaBuffer=(char *)0xb8000; // VGA text buffer is located at physical memory address 0xB8000.
unsigned int video_loc=0; //  video character location

void clearScreen(){
    // VGA mode 3 provides a text interface 80 characters wide and 25 characters lines per screen.
    const int vga_col = 80,vga_row = 25;
    int i=0;

    while( i < vga_col*vga_row*2){
        vgaBuffer[i++] = 0;
    }
}

void print(const char *string){
    /* Video memory specifies two fields for each display location.
     The first field contains the character to be printed  and the second field specifies the character's properties.
     1 line = 80 characters * 2
     */

    while (*string!='\0'){
    	if(*string=='\n'){  // new line
    		video_loc=(((video_loc/2)/80)+1)*160;
    		*string++;
    		video_loc+=2;
    	}
    	else{
    		vgaBuffer[video_loc]=*string;
    		*string++;  // move to next character
    		video_loc++;
    		vgaBuffer[video_loc]=termColor;
    		video_loc++;
    	}
    }

}

void update_cursor_graphic() {
    unsigned int i = 1;
    while (i < SCREENSIZE) {
        if (vidptr[i] == 0x70) {
            vidptr[i] = 0x07;
        }
        i += 2;
    }
    vidptr[current_loc + 1] = 0x70;
}

void scroll() {
    int offset = 2 * COLUMNS * BYTES_PER_VID_ELEMENT;

    for (int va = 0; va < SCREENSIZE; va++) {
        if (va + offset < SCREENSIZE) {
            char temp = vidptr[va + offset];
            vidptr[va] = temp;
        } else {
            vidptr[va] = (va % 2 == 0 ? ' ' : 0x07);
        }
    }

    //Move cursor up by the offset
    current_loc -= (current_loc - offset >= 0 ? offset : current_loc);
}
