#ifndef _HEADER_TIMER_8_B_
#define _HEADER_TIMER_8_B_

#define THRESHOLD_1 1
#define THRESHOLD_2 2
#define THRESHOLD_3 4
#define THRESHOLD_4 8
#define THRESHOLD_5 16
#define THRESHOLD_6 32

typedef void (*tim_8_clbk_t)(void);
tim_8_clbk_t tim_8_clbk;

void init_timer_8b();
void deinit_timer_8b();

void start_tim_8b();
void stop_tim_8b();

void set_tim8_clbk(tim_8_clbk_t callback);

#endif
