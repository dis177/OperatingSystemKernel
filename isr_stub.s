/* ---------- IRQ0-15 汇编桩，向 C 层传 int_no = 0x20-0x2F --------- */
.set IRQ_BASE, 32           /* 0x20 */

.macro IRQ num
    .global irq\num
irq\num:
    cli
    pushl $0                /* 伪 err_code */
    pushl $(IRQ_BASE+\num)  /* int_no      */
    jmp   isr_common
.endm

/* 展开 16 个宏 */
IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

/* ---------- 通用保存 / 恢复 & iret ------------------------------- */
.extern isr_handler

isr_common:
    pusha
    push   %ds
    push   %es
    push   %fs
    push   %gs

    mov    $0x10, %ax
    mov    %ax, %ds
    mov    %ax, %es
    mov    %ax, %fs
    mov    %ax, %gs

    mov    %esp, %eax        /* &regs */
    push   %eax
    call   isr_handler
    add    $4, %esp

    pop    %gs
    pop    %fs
    pop    %es
    pop    %ds
    popa
    add    $8, %esp          /* err_code + int_no */
    sti
    iret

