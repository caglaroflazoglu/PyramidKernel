#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "string.h"
#include "framebuffer.h"

int32_t strcmp(const int8_t *s1, const int8_t *s2) {
  while ((*s1) && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return (*(uint8_t *)s1 - *(uint8_t *)s2);
}

char *itoa(int val, char *buf, int radix) {
  uint32_t i = 0;
  uint32_t start = i;

  if (val < 0 && radix == 10) {
    buf[i++] = '-';
    start = i;
  }

  if (radix == 16) {
    buf[i++] = '0';
    buf[i++] = 'x';
    start = i;
  }

  int x = val;
  do {
    int a = x % radix;
    if (a < 10) buf[i++] = a + '0';
    else buf[i++] = a + 'a' - 10;
  } while (x /= radix);

  char *s = buf+start;
  char *e = buf+(i-1);

  while(s < e) {
    char t = *s;
    *s = *e;
    *e = t;
    s++; e--;
  }

  buf[i] = 0;
  return buf;
}

char *uitoa(uint32_t val, char *buf, int radix) {
  uint32_t i = 0;
  uint32_t start = i;
  uint32_t x = val;

  if (radix == 16) {
    buf[i++] = '0';
    buf[i++] = 'x';
    start = i;
  }

  do {
    uint32_t a = x % radix;
    if (a < 10) buf[i++] = a + '0';
    else buf[i++] = a + 'a' - 10;
  } while (x /= radix);

  char *s = buf+start;
  char *e = buf+(i-1);

  while(s < e) {
    char t = *s;
    *s = *e;
    *e = t;
    s++; e--;
  }

  buf[i] = 0;
  return buf;
}

size_t strlen(const char *buf) {
  unsigned int i=0;
  while(buf[i] != 0) i++;
  return i;
}

// Terribly naive implementation of memset to get things compiling
// See http://www.xs-labs.com/en/blog/2013/08/06/optimising-memset/
// for more details
void *memset(void *s, int c, size_t n) {
  char *mem = (char*)s;

  for (size_t i=0; i<n; i++) 
    mem[i] = (uint8_t)c;
  
  return s;
}

void *memmove(void *dst, const void *src, size_t len) {
  char *dstmem = (char*)dst;
  char *srcmem = (char*)src;

  for (size_t i=0; i<len; i++) 
    dstmem[i] = srcmem[i];
  
  return dstmem;
}

void strupper(char *str){
  for(unsigned int i = 0; i < strlen(str); i++)
    if(str[i] >= 'a' && str[i] < 'z') 
      str[i] = str[i] & 0x4F;
}

void strlower(char *str){
    for(unsigned int i = 0; i < strlen(str); i++)
      if(str[i] >= 'A' && str[i] < 'Z') 
        str[i] = str[i] | 0x60;
}

int printf(const char *format, ...) {
  char buf[20],c,*s;
  int val;
  int32_t uval;
  va_list ap;

  va_start(ap, format);

  for (size_t i=0; i<strlen(format); i++) {
    if (format[i] == '%') {
      i++;
      while (format[i] == ' ') i++;

      switch(format[i]) {
        case 'i':
          val = va_arg(ap, int);
          itoa(val, buf, 10);
          write_str(buf);
          break;
        case 'x':
          uval = va_arg(ap, uint32_t);
          uitoa(uval, buf, 16);
          write_str(buf);
          break;
        case 'd':
          uval = va_arg(ap, uint32_t);
          uitoa(uval, buf, 10);
          write_str(buf);
          break;
        case 'c':
          c = (char)va_arg(ap, int);
          write(&c, 1);
          break;
        case 's':
          s = va_arg(ap, char*);
          write_str(s);
          break;
        default:
          write((char*)format+i, 1);
      }
    } else {
      write((char*)format+i, 1);
    }
  }

  va_end(ap);
  return 0;
}