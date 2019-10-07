#include "timer_8b.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// TIMER Handle
ISR(TIMER0_OVF_vect)
{
  //++counter;
  //if(counter == treshold)
  //{
  //  PORTD ^= 0b10000;
  //  counter = 0;
  //}
  if(tim_8_clbk)
    tim_8_clbk();
}

void init_timer_8b()
{
  //counter = 0;                             // Helper counter
  //treshold = BLINK_SPEED_3;                // Blink speed prescaler

  tim_8_clbk = NULL;
  TCCR0B |= (TCCR0B & 0b11111000) | 0b010; // CS00 = 0, CS01 = 1, CS02 = 0 - clkio / 8 (from prescaler)
  TIMSK0 |= (1 << TOIE0);                  // Enable timer INT
}

void set_tim8_clbk(tim_8_clbk_t callback)
{
  tim_8_clbk = callback;
}

void deinit_timer_8b()
{
  TCCR0B |= (TCCR0B & 0b11111000);         // CS00 = 0, CS01 = 0, CS02 = 0 - No clock source
  TIMSK0 &= ~(1 << TOIE0);
  tim_8_clbk = NULL;
}
