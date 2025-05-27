#include "ports.h"
#include "pic.h"
#include <stdint.h>

uint32_t ticks = 0;

/* IRQ0 在 isr_handler() 中调用 */
void pit_tick(void) { ++ticks; }

uint32_t pit_get_ticks(void) { return ticks; }

void pit_init(uint32_t freq)
{
    /* PIT 输入时钟 1.193182 MHz */
    uint32_t divisor = 1193182 / freq;

    outb(0x43, 0x36);                 /* 通道0，低+高字节，模式3 */
    outb(0x40, divisor & 0xFF);       /* 低字节 */
    outb(0x40, divisor >> 8);         /* 高字节 */
}

