#include "timer_8b.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// PINOUT PB3 - D11 (OC2A)

// TIMER Handle
ISR(TIMER2_OVF_vect)
{
  if(tim_8_clbk)
    tim_8_clbk();
}

void tim_8b_int_enable()
{
  TIMSK2 |= (1 << TOIE2);                       // Enable timer INT
}

void tim_8b_int_disable()
{
  TIMSK2 &= ~(1 << TOIE2);
}

void init_timer_8b()
{
  tim_8_clbk = NULL;

  TCCR2A |= (1 << COM2A1) | (1 << WGM20);
  OCR2A = 128;
}

void set_tim8_clbk(tim_8_clbk_t callback)
{
  tim_8_clbk = callback;
}

void deinit_timer_8b()
{
  TCCR2B |= (TCCR2B & 0xF8);               // CS00 = 0, CS01 = 0, CS02 = 0 - No clock source
  tim_8_clbk = NULL;
}

void start_tim_8b()
{
  TCCR2A |= (1 << COM2A1);
  TCCR2B |= (TCCR2B & 0xF8) | (1 << CS22); // CS00 = 0, CS01 = 1, CS02 = 0 - clkio / 8 (from prescaler)
}

void stop_tim_8b()
{
  TCCR2B |= (TCCR2B & 0xF8);               // CS00 = 0, CS01 = 0, CS02 = 0 - No clock source
  TCCR2A &= ~(1 << COM2A1) & ~(1 << COM2A0);
}
