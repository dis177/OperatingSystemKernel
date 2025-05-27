#include "keyboard.h"
#include "ports.h"
#include "pic.h"
#include <stdint.h>

/* ========== 简易扫描码表 (US ANSI, Set-1) ========== */
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',   /* 0x00–0x0E */
    '\t','q','w','e','r','t','y','u','i','o','p','[',']', '\n',      /* 0x0F–0x1C */
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',            /* 0x1D–0x28 */
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0, '*',        /* 0x29–0x37 */
    0,  ' ', 0                                                      /* 0x38– */
};

/* VGA 文本模式：简单 putc ———— */
static uint16_t cursor = 80 * 1;          /* 从第 1 行开始打印 (0 行放 hello) */

static inline void vga_putc(char c)
{
    volatile uint8_t *vga = (uint8_t *)0xB8000;

    if (c == '\n') {               /* 换行 */
        cursor = (cursor / 80 + 1) * 80;
    } else if (c == '\b') {        /* 退格 */
        if (cursor) cursor--;
        vga[cursor*2] = ' ';
    } else {
        vga[cursor*2]     = c;
        vga[cursor*2 + 1] = 0x0F;
        cursor++;
    }

    if (cursor >= 80 * 25)         /* 到底则清屏回顶 (简易点) */
        cursor = 0;
}

/* ========== 对外函数 ========== */
void keyboard_handle_irq(void)
{
    uint8_t sc = inb(0x60);
    if (sc & 0x80) return;         /* 忽略按键松开 */

    char ascii = scancode_to_ascii[sc];
    if (ascii) vga_putc(ascii);
}

void keyboard_init(void)
{
    /* 解除主 PIC 的 IRQ1 屏蔽位 (位 1) */
    uint8_t mask = inb(0x21);
    mask &= ~0x02;
    outb(0x21, mask);

    /* 清空 PS/2 输出缓冲，避免旧数据 */
    inb(0x60);
}

