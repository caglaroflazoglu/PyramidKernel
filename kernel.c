#define colour 0x4 // red color for text

void clean_screen(){
    // VGA text buffer is located at physical memory address 0xB8000.
    char *vga = (char*)0xb8000;
    //VGA mode 3 provides a text interface 80 characters wide and 25 characters lines per screen.
    int i=0,vga_height=80,vga_width=25;

    while( i < vga_height*vga_width*2){
        vga[i++] = 0;
    }

}

void print(const char *string){
    char *vga = (char*)0xb8000;

    while(*string){
        *vga++ = *string++;
        *vga++ = colour;
    }

}

void main(){
    clean_screen();
    print("Hi!");
}
