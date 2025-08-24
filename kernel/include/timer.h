#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(uint32_t frequency);
uint64_t timer_ticks(void);
void timer_wait(uint64_t ticks);

#endif
