#ifndef KSTDIO_H
#define KSTDIO_H
#include <stdint.h>

void kputc(char c);
void kprintf(const char *fmt, ...);
__attribute__((noreturn)) void panic(const char *fmt, ...);

#endif /* KSTDIO_H */

