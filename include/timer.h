#ifndef TIMER_H
#define TIMER_H

#include "common.h"

typedef struct {
    uint8_t timer_mod;
    uint8_t timer;
    /// bit 2-0 : frequency switch, bit 7: general timer enable
    uint8_t timer_enable_register;
    uint8_t div_counter;
} timer;

uint8_t update_timer(timer* t);
uint8_t init_timer(timer* t);
#endif


//interrupts_per_second = clock_frequency / (256 - timer_modulo)