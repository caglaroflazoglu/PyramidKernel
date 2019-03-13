// VGA text buffer is located at physical memory address 0xB8000.
volatile uint16_t* vga_buffer = (uint16_t*)0xb8000;
//VGA mode 3 provides a text interface 80 characters wide and 25 characters lines per screen.
const int vga_col = 80,vga_row = 25;
int term_col,term_row = 0;
uint8_t term_color = 0x4; // red color for text

void clean_screen(){
    char *vga = (char*)0xb8000; // VGA text buffer is located at physical memory address 0xB8000.
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
