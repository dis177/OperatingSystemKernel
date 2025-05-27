#include "ports.h"
#include "pic.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01
#define PIC_EOI 0x20

void pic_remap(uint8_t offset1, uint8_t offset2)
{
    /* 保存当前掩码 */
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    /* 初始化 */
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);   /* ICW2: 主PIC vector offset */
    outb(PIC2_DATA, offset2);   /* ICW2: 从PIC vector offset */
    outb(PIC1_DATA, 4);         /* ICW3: 告诉主 PIC 有从片在 IRQ2(0000 0100) */
    outb(PIC2_DATA, 2);         /* ICW3: 从片级联号为 2 */
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /* 还原掩码 */
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8) outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

