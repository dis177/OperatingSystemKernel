/* ---------- Multiboot 1 头 ------------------------------------------------ */
.set MAGIC,     0x1BADB002
.set FLAGS,     0x0
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

/* ---------- 栈 ----------------------------------------------------------- */
.section .bss
.align 16
stack_bottom:
    .space 16384          /* 16 KiB 内核栈 */
stack_top:

/* ---------- 代码入口 ----------------------------------------------------- */
.section .text
.global _start
.extern  kernel_main
.extern  init_descriptor_tables

_start:
    cli
    mov $stack_top, %esp          /* 切栈 */

    call init_descriptor_tables   /* 加载 GDT + 空 IDT */
    call kernel_main              /* 进入 C 内核 */

.halt:
    hlt
    jmp .halt

