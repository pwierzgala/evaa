#include "timer_16b.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

static char unsigned timer_16b_enabled;

// TIMER Overflow Handle
ISR(TIMER1_OVF_vect)
{
  if(tim_16_ovfl_clbk)
    tim_16_ovfl_clbk();
}

//TIMER Compare match Handle
ISR(TIMER1_COMPA_vect)
{
  if(tim_16_comp_clbk)
    tim_16_comp_clbk();
}

void tim_16b_int_enable()
{
  TIMSK1 |= (1 << TOIE1);            // Timer/Counter Overflow Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);           // Timer/Counter Output Compare a match Interrupt Enable
}

void tim_16b_int_disable()
{
  TIMSK1 &= ~(1 << TOIE1);
  TIMSK1 &= ~(1 << OCIE1A);
}

void init_timer_16b()
{
  // TIMER 16 BIT
  TCCR1A &= 0xFC;                    // Clear WGM11, WGM10 - Timer in normal mode
  TCCR1B &= 0xE7;                    // Clear WGM13, WGM12 - Timer in normal mode
  TCCR1B &= 0xF8;                    // Timer/Counter halted

  OCR1A   = 62500;

  tim_16b_int_enable();

  timer_16b_enabled = 0;             // Is timer running
}

void deinit_timer_16b()
{
  stop_timer_16b();
  tim_16b_int_disable();

  OCR1A   = 0;

  tim_16_ovfl_clbk = NULL;
  tim_16_comp_clbk = NULL;
}

void start_timer_16b(enum timer_16b_prescaler_t prescaler)
{
  timer_16b_enabled = 1;                   // timer has started
  TCCR1B &= 0xF8;                          // Clear prescaler register bits
  TCCR1B |= prescaler;                     // Set prescaler register bits
}

void stop_timer_16b()
{
  TCCR1B &= 0xF8;                          // No clock source
  timer_16b_enabled = 0;                   // timer has stoped
}

short unsigned get_counter_and_reset()
{
  short unsigned counter = TCNT1;
  TCNT1 = 0;                               // Reset counter
  return counter;
}

char unsigned get_timer_16b_status()
{
  return timer_16b_enabled;
}

void set_tim16_ovfl_clbk(tim_16_clbk_t callback)
{
  tim_16_ovfl_clbk = callback;
}

void set_tim16_comp_clbk(tim_16_clbk_t callback)
{
  tim_16_comp_clbk = callback;
}
