#include "gdt.h"
#include "idt.h"

/* 对外统一入口，给 boot.s 调用 */
void init_descriptor_tables(void)
{
    gdt_init();
    idt_init();
}

