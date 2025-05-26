ENTRY(_start)
SECTIONS
{
  . = 2M;                     /* 更兼容的加载地址 */
  .text : { *(.multiboot) *(.text*) }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss  : { *(.bss*) *(COMMON) }
}

