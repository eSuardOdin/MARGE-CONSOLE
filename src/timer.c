#include "timer.h"

uint8_t update_timer(timer* t)
{
    if(t->timer_enable_register & 0x80) {
        // If overflow of timer, generate interrupt
        if(t->timer == 0xFF)
        {
            t->timer = t->timer_mod;
            return 1;
        }
        
        t->timer++;
    }
    return 0;
}

uint8_t init_timer(timer* t)
{
    t->timer_mod = 0;
    t->timer = 0;
    t->timer_enable_register = 0;
    t->div_counter = 0;

    return 0;
}

