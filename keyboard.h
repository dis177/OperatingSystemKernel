#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);           /* 初始化：解除屏蔽、清缓冲 */
void keyboard_handle_irq(void);     /* 由 IRQ1 调用 */

#endif /* KEYBOARD_H */

