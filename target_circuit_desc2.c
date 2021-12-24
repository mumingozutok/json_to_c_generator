#include "stdint.h"
#include "Adaptor\adaptor.h"

uint32_t nodes[16];
#define TYPE_ANY uint16_t

//-------------TIMER CYCLIC-------------------------------------
typedef struct {
    TYPE_ANY RST, IN, DON, DOFF, MODE; //input ports
    TYPE_ANY OUT, STAT, VAL; //output ports
    TYPE_ANY CYCLE_COUNTER; //internal static vars
    void (*func)(void* p);
} fb_timer_cyclic_t;

void fb_timer_cyclic(fb_timer_cyclic_t* fb){

    int32_t delta_time = 0;

    int32_t* don = &nodes[fb->DON];
    int32_t* doff = &nodes[fb->DOFF];
    int32_t* out = &nodes[fb->OUT];
    uint32_t* cycle_counter = &nodes[fb->CYCLE_COUNTER];

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
    TYPE_ANY CH, VAL, MODE; //input ports
    TYPE_ANY STAT;//output ports
    TYPE_ANY LAST_VAL; //internal statics
    void (*func)(void* p);
} fb_dout_t;

void fb_dout(fb_dout_t* fb){

    uint16_t* ch = &nodes[fb->CH];
    uint8_t* val = &nodes[fb->VAL];
    uint8_t* last_val = &nodes[fb->LAST_VAL];

	if ( *val != *last_val){
		*last_val = *val;
		if(*val >1) *val = 1;

		hal_gpio_write_pin(*ch, *val);
	}    
}
//-----------------------

fb_timer_cyclic_t TIMER_CYCLIC;
fb_dout_t DOUT;

void action_write_connection(uint16_t source, int32_t destination){
    nodes[destination] = source;
}

void action_create_connection(uint16_t source, int32_t destination){
    destination = source ;
}

void initiate_circuit(){
    uint16_t node_index_counter = 0;

    //assing node indexes sequentally from timer_cyclic.json
    TIMER_CYCLIC.RST = node_index_counter++;
    TIMER_CYCLIC.IN = node_index_counter++;
    TIMER_CYCLIC.DON = node_index_counter++;
    TIMER_CYCLIC.DOFF = node_index_counter++;
    TIMER_CYCLIC.MODE = node_index_counter++;

    TIMER_CYCLIC.OUT = node_index_counter++;
    TIMER_CYCLIC.STAT = node_index_counter++;
    TIMER_CYCLIC.VAL = node_index_counter++;

    TIMER_CYCLIC.CYCLE_COUNTER = node_index_counter++;

    TIMER_CYCLIC.func = fb_timer_cyclic;
    

    //assing node indexes sequentally from dout.json
    DOUT.CH = node_index_counter++;
    DOUT.VAL = node_index_counter++;
    DOUT.MODE = node_index_counter++;

    DOUT.STAT = node_index_counter++;

    DOUT.LAST_VAL = node_index_counter++;

    DOUT.func = fb_dout;

    //Set direct param assignments from program json
    action_write_connection(1000, TIMER_CYCLIC.DON);
    action_write_connection(250, TIMER_CYCLIC.DOFF);
    action_write_connection(0, DOUT.CH);

    action_create_connection(TIMER_CYCLIC.OUT, DOUT.VAL);
}

void run_circuit_one_cycle(){
    TIMER_CYCLIC.func(&TIMER_CYCLIC);
    DOUT.func(&DOUT);
}