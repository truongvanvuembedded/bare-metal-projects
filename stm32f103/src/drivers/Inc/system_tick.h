#ifndef SYSTEM_TICK_H
#define SYSTEM_TICK_H

#include <stdint.h>

void system_tick_init(void);
void delay_ms(uint32_t ms);
uint32_t get_tick(void);

#endif