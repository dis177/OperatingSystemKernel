#ifndef ISR_H
#define ISR_H
#include <stdint.h>

struct regs {
    uint32_t gs, fs, es, ds;      /* pushad 保存的段寄存器 */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void isr_install(void);                  /* 装载汇编桩位置到 IDT */
void isr_handler(struct regs *r);        /* C 级分发函数 */

#endif /* ISR_H */

