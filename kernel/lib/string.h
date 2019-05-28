#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>
#include <stdint.h>

int32_t strcmp(const int8_t *s1, const int8_t *s2);
char *itoa(int val, char *buf, int radix);
char *uitoa(uint32_t val, char *buf, int radix);
size_t strlen(const char *buf);
void *memset(void *s, int c, size_t n);
void *memmove(void *dst, const void *src, size_t len);
int printf(const char *format, ...);
#endif /* _STRING_H_ */