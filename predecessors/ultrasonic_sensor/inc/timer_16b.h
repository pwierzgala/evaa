#ifndef _HEADER_TIMER_16_B_
#define _HEADER_TIMER_16_B_

typedef void(*tim_16_clbk_t)(void);
tim_16_clbk_t tim_16_clbk;

void init_timer_16b();
void deinit_timer_16b();

void start_timer_16b();
void stop_timer_16b();

short unsigned get_counter();
short unsigned get_distance();

void set_tim16_clbk(tim_16_clbk_t callback);

#endif
