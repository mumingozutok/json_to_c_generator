#include "stdint.h"
#include "..\inc\fb_timer_cyclic.h"

void fb_timer_cyclic(fb_timer_cyclic_t* fb){

    int32_t delta_time = 0;

    int32_t* don = &nodes[fb->ni_don];
    int32_t* doff = &nodes[fb->ni_doff];
    int32_t* out = &nodes[fb->ni_out];
    uint32_t* cycle_counter = &nodes[fb->ni_cycle_counter];

	//get cycle_counter
	if(*cycle_counter == 0xFFFFFFFF) //initial value
	{
		*cycle_counter = hal_get_tick();
	}   

	else {
		delta_time = hal_get_tick() - *cycle_counter;

		if(delta_time < 0){
			delta_time = 0xFFFFFFFF - *cycle_counter + hal_get_tick();
		}

		if(delta_time < *doff){
			*out = 0;
		}
		else if(delta_time < *doff + *don){
			*out = 1;
		}
		else{
			*cycle_counter = hal_get_tick(); //resample again
		}
	}    
}