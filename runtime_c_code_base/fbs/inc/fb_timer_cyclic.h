#ifndef _FB_TIMER_CYCLIC_H
#define _FB_TIMER_CYCLIC_H

typedef struct {
    uint16_t ni_don;
    uint16_t ni_doff;
    uint16_t ni_out;
    uint16_t ni_cycle_counter;
    void (*callback)();
} fb_timer_cyclic_t;

#endif