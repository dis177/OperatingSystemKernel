#include <stdint.h>
void kernel_main(void) {
    const char *msg = "Hello, kernel!";
    volatile char *vga = (volatile char*)0xB8000;
    for (int i = 0; msg[i]; ++i) {
        vga[i*2]   = msg[i];
        vga[i*2+1] = 0x0F;      // 白字黑底
    }
    for (;;) asm volatile ("hlt");
}

