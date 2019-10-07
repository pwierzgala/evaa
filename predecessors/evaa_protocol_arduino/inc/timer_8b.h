#ifndef _HEADER_TIMER_8_B_
#define _HEADER_TIMER_8_B_

typedef void (*tim_8_clbk_t)(void);
tim_8_clbk_t tim_8_clbk;

void tim_8b_int_enable();
void tim_8b_int_disable();

void init_timer_8b();
void deinit_timer_8b();

void start_tim_8b();
void stop_tim_8b();

void set_tim8_clbk(tim_8_clbk_t callback);

#endif
