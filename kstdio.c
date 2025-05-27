#include "kstdio.h"
#include <stdarg.h>

#define VGA_BASE ((volatile uint8_t *)0xB8000)
static uint16_t cursor = 0;          /* 行列 = cursor / 80, cursor % 80 */

static void scroll_if_needed(void)
{
    if (cursor < 80 * 25) return;

    /* 上滚一行 */
    for (uint16_t i = 0; i < 24 * 80 * 2; ++i)
        VGA_BASE[i] = VGA_BASE[i + 80 * 2];
    /* 清最后一行 */
    for (uint16_t i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
        VGA_BASE[i]     = ' ';
        VGA_BASE[i + 1] = 0x0F;
    }
    cursor = 24 * 80;
}

void kputc(char c)
{
    if (c == '\n') {
        cursor = (cursor / 80 + 1) * 80;
    } else if (c == '\b') {
        if (cursor) cursor--;
    } else {
        VGA_BASE[cursor*2]     = c;
        VGA_BASE[cursor*2 + 1] = 0x0F;
        cursor++;
    }
    scroll_if_needed();
}

/* 十进制 / 十六进制打印辅助 */
static void print_uint(uint32_t val, int base)
{
    char buf[32]; int i = 0;
    if (!val) { kputc('0'); return; }
    while (val) {
        uint32_t d = val % base;
        buf[i++] = d < 10 ? '0' + d : 'A' + d - 10;
        val /= base;
    }
    while (i--) kputc(buf[i]);
}

void kprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; ++fmt) {
        if (*fmt != '%') { kputc(*fmt); continue; }
        switch (*++fmt) {
            case 's': {
                const char *s = va_arg(ap, const char*);
                while (*s) kputc(*s++);
            } break;
            case 'd': {
                int n = va_arg(ap, int);
                if (n < 0) { kputc('-'); n = -n; }
                print_uint((uint32_t)n, 10);
            } break;
            case 'x': {
                uint32_t n = va_arg(ap, uint32_t);
                print_uint(n, 16);
            } break;
            case '%': kputc('%'); break;
            default:  kputc('?');  break;
        }
    }
    va_end(ap);
}

void panic(const char *fmt, ...)
{
    kprintf("\n\n--- PANIC ---\n");
    va_list ap;
    va_start(ap, fmt);
    kprintf(fmt, ap);   /* 递交给上一层格式化 */
    va_end(ap);
    kprintf("\nSystem halted.\n");
    for (;;) __asm__ volatile ("cli; hlt");
}

