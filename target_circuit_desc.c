#include "stdint.h"
#include "Adaptor\adaptor.h"

uint32_t nodes[16];

//-------------TIMER CYCLIC-------------------------------------
typedef struct {
    uint16_t ni_don;
    uint16_t ni_doff;
    uint16_t ni_out;
    uint16_t ni_cycle_counter;
    void (*callback)();
} fb_timer_cyclic_t;

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

//---------------------------DOUT--------------------------------
typedef struct {
    uint16_t ni_ch;
    uint16_t ni_out;
    uint16_t ni_lastval;
    void (*callback)();
} fb_dout_t;

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
//-----------------------

fb_timer_cyclic_t TIMER_CYCLIC;
fb_dout_t DOUT;

void initiate_circuit(){
    uint16_t node_index_counter = 0;
    //assing node indexes sequentally from timer_cyclic.json
    TIMER_CYCLIC.ni_cycle_counter = node_index_counter++;
    TIMER_CYCLIC.ni_doff = node_index_counter++;
    TIMER_CYCLIC.ni_don = node_index_counter++;
    TIMER_CYCLIC.ni_out = node_index_counter++;

    //assing node indexes sequentally from dout.json
    DOUT.ni_ch = node_index_counter++;
    DOUT.ni_lastval = node_index_counter++;
    DOUT.ni_out = node_index_counter++;

    //Set direct param assignments from program json
    nodes[TIMER_CYCLIC.ni_doff] = 250;
    nodes[TIMER_CYCLIC.ni_don] = 1000;
    nodes[DOUT.ni_ch] = 0;

    //make node connections
}

void run_circuit_one_cycle(){
    fb_timer_cyclic(&TIMER_CYCLIC);
    fb_dout(&DOUT);
}