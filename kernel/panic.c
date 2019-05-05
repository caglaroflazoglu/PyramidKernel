#include "panic.h"
#include "string.h"
extern void panic(const char *message, const char *file, uint32_t line){

    // Disable interrupts.
    asm("cli");

    printf("KERNEL PANIC( %s ) at %s: %d\n", message, file, line);
    printf("DAMN SONN!WE'RE IN TROUBLE.\n");
    // Halt by going into an infinite loop.
 	while(1);
}