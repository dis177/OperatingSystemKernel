#ifndef PIT_H
#define PIT_H
#include <stdint.h>

void pit_init(uint32_t freq);
uint32_t pit_get_ticks(void);
void pit_tick(void);

#endif /* PIT_H */

