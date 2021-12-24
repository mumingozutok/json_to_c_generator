#include "stdint.h"
#include "..\inc\fb_dout.h"

void fb_dout(fb_dout_t* fb){

    uint16_t* ch = &nodes[fb->ni_ch];
    uint8_t* val = &nodes[fb->ni_out];
    uint8_t* last_val = &nodes[fb->ni_lastval];

	if ( *val != *last_val){
		*last_val = *val;
		if(*val >1) *val = 1;

		hal_gpio_write_pin(*ch, *val);
	}    
}