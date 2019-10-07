#ifndef _HEADER_TIMER_16_B_
#define _HEADER_TIMER_16_B_

enum timer_16b_prescaler_t
{
    TIM_16B_PRESCALER_1    = 0x01U,
    TIM_16B_PRESCALER_8    = 0x02U,
    TIM_16B_PRESCALER_64   = 0x03U,
    TIM_16B_PRESCALER_256  = 0x04U,
    TIM_16B_PRESCALER_1024 = 0x05U
};

typedef void(*tim_16_clbk_t)(void);
tim_16_clbk_t tim_16_ovfl_clbk;
tim_16_clbk_t tim_16_comp_clbk;

void tim_16b_int_enable();
void tim_16b_int_disable();

void init_timer_16b();
void deinit_timer_16b();

void start_timer_16b(enum timer_16b_prescaler_t prescaler);
void stop_timer_16b();

char unsigned get_timer_16b_status();
short unsigned get_counter_and_reset();

void set_tim16_ovfl_clbk(tim_16_clbk_t callback);
void set_tim16_comp_clbk(tim_16_clbk_t callback);

#endif
