#include "idt.h"

/* ====== 与 isr_stub.s 中的全局符号保持一致 ===== */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

struct __attribute__((packed)) idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
};

struct __attribute__((packed)) idt_ptr {
    uint16_t limit;
    uint32_t base;
};

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static inline void idt_flush(uint32_t addr)
{
    __asm__ volatile ("lidt (%0)" :: "r"(addr));
}

static void idt_set_gate(uint8_t num, uint32_t base,
                         uint16_t sel, uint8_t flags)
{
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

void idt_init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    for (int i = 0; i < 256; ++i)
        idt_set_gate(i, 0, 0x08, 0x8E);

    /* ----- 0x20–0x2F → IRQ0–15 ----- */
    idt_set_gate(0x20, (uint32_t)irq0 , 0x08, 0x8E);
    idt_set_gate(0x21, (uint32_t)irq1 , 0x08, 0x8E);
    idt_set_gate(0x22, (uint32_t)irq2 , 0x08, 0x8E);
    idt_set_gate(0x23, (uint32_t)irq3 , 0x08, 0x8E);
    idt_set_gate(0x24, (uint32_t)irq4 , 0x08, 0x8E);
    idt_set_gate(0x25, (uint32_t)irq5 , 0x08, 0x8E);
    idt_set_gate(0x26, (uint32_t)irq6 , 0x08, 0x8E);
    idt_set_gate(0x27, (uint32_t)irq7 , 0x08, 0x8E);
    idt_set_gate(0x28, (uint32_t)irq8 , 0x08, 0x8E);
    idt_set_gate(0x29, (uint32_t)irq9 , 0x08, 0x8E);
    idt_set_gate(0x2A, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(0x2B, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(0x2C, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(0x2D, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(0x2E, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(0x2F, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idtp);
}

