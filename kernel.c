#include <stdint.h>
#include "pic.h"
#include "pit.h"

void kernel_main(void)
{
    const char *msg = "Hello, kernel!";
    volatile char *vga = (volatile char *)0xB8000;
    for (int i = 0; msg[i]; ++i) {
        vga[i*2]   = msg[i];
        vga[i*2+1] = 0x0F;
    }

    pic_remap(0x20, 0x28);   /* IRQ0-15 → 0x20-0x2F */
    pit_init(1000);          /* 1000 Hz → 每 1 ms tick 一次 */
    /* 打开可屏蔽中断 */
    __asm__ volatile ("sti");

    for (;;) __asm__ volatile ("hlt");
}

