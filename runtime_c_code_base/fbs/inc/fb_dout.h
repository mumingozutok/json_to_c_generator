#ifndef FB_DOUT_H
#define FB_DOUT_H

typedef struct {
    uint16_t ni_ch;
    uint16_t ni_out;
    uint16_t ni_lastval;
    void (*callback)();
} fb_dout_t;

#endif