#include "isr.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "kstdio.h"
#include <stdint.h>

static void vga_write_dec(uint32_t n)
{
    volatile uint8_t *vga = (uint8_t *)0xB8000;
    uint32_t pos = 79 * 2;          /* 第 0 行第 79 列 */

    char buf[8];                    /* 8 位十进制缓冲 */
    for (int i = 7; i >= 0; --i) {  /* 先把数字倒进数组 */
        buf[i] = '0' + (n % 10);
        n /= 10;
    }

    for (int i = 0; i < 8; ++i) {   /* 再按正常顺序写到屏幕 */
        vga[pos - (7 - i) * 2]     = buf[i];
        vga[pos - (7 - i) * 2 + 1] = 0x0F;   /* 颜色：白字黑底 */
    }
}

static void dump_regs(struct regs *r)
{
    kprintf("\nException %d  err=%x  eip=%x  cs=%x  eflags=%x\n",
            r->int_no, r->err_code, r->eip, r->cs, r->eflags);
    kprintf("eax=%x ebx=%x ecx=%x edx=%x esi=%x edi=%x esp=%x ebp=%x\n",
            r->eax, r->ebx, r->ecx, r->edx,
            r->esi, r->edi, r->esp, r->ebp);
}

void isr_handler(struct regs *r)
{
    switch (r->int_no) {
        case 0x20:                       /* IRQ0：PIT */
            pit_tick();
            vga_write_dec(pit_get_ticks());
            break;

        case 0x21:                       /* IRQ1：键盘 */
            keyboard_handle_irq();
            break;

        default:                         /* 其余：异常或未处理 IRQ */
            dump_regs(r);
            panic("Unhandled interrupt");
    }

    if (r->int_no >= 0x20 && r->int_no <= 0x2F)
        pic_send_eoi(r->int_no - 0x20);
}
