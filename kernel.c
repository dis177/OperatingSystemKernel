#include <stdint.h>
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "kstdio.h"

void kernel_main(void)
{
    kprintf("Hello, kernel!\n");

    pic_remap(0x20, 0x28);   /* IRQ0-15 → 0x20-0x2F */
    pit_init(1000);          /* 1000 Hz → 每 1 ms tick 一次 */
    
    keyboard_init();       /* ← 注册键盘 */
    
    /* 打开可屏蔽中断 */
    __asm__ volatile ("sti");

    for (;;) __asm__ volatile ("hlt");
}

