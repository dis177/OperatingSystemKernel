#ifndef PIC_H
#define PIC_H

void pic_remap(uint8_t offset1, uint8_t offset2);
void pic_send_eoi(uint8_t irq);

#endif /* PIC_H */

