#include <stdint.h>
#define PANIC(msg) panic(msg, __FILE__, __LINE__);
extern void panic(const char *message, const char *file, uint32_t line);