#include "gdt.h"

/* ==== GDT 数据结构 ====================================================== */

struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;     // 段界限 0–15
    uint16_t base_low;      // 基址  0–15
    uint8_t  base_mid;      // 基址 16–23
    uint8_t  access;        // 访问标志
    uint8_t  gran;          // 粒度 & 段界限 16–19
    uint8_t  base_high;     // 基址 24–31
};

struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;         // GDT 总大小 - 1
    uint32_t base;          // GDT 首地址
};

/* 三条描述符：null / flat code / flat data */
static struct gdt_entry gdt[3];
static struct gdt_ptr   gp;

/* 内联汇编：加载 GDT 并刷新段寄存器 */
static inline void gdt_flush(uint32_t gdt_ptr_addr)
{
    __asm__ volatile (
        "lgdt (%0)\n"
        "mov $0x10, %%ax\n"   // 数据段选择子 = 0x10
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "jmp $0x08,$next\n"   // 远跳切到代码段选择子 0x08
        "next:\n"
        :
        : "r"(gdt_ptr_addr)
        : "ax"
    );
}

/* 填写单条 GDT 描述符 */
static void gdt_set_gate(int num, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran)
{
    gdt[num].base_low  = base & 0xFFFF;
    gdt[num].base_mid  = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].gran      = (limit >> 16) & 0x0F;

    gdt[num].gran |= gran & 0xF0;
    gdt[num].access = access;
}

/* 对外入口：创建并加载 GDT */
void gdt_init(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base  = (uint32_t)&gdt;

    /* 0. 空描述符 */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* 1. 平坦 0–4 GB 代码段，Ring 0 */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* 2. 平坦 0–4 GB 数据段，Ring 0 */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t)&gp);
}

