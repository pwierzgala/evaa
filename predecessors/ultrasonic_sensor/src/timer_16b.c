#include "timer_16b.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// TIMER Handle
ISR(TIMER1_OVF_vect)
{
  if(tim_16_clbk)
    tim_16_clbk();
}

void init_timer_16b()
{
  // TIMER 16 BIT
  TCCR1A &= 0b11111100;                    // Clear WGM11, WGM10 - Timer in normal mode
  TCCR1B &= 0b11100111;                    // Clear WGM13, WGM12 - Timer in normal mode
  TCCR1B &= 0b11111000;                    // Timer/Counter halted
}

void deinit_timer_16b()
{
  stop_timer_16b();
  tim_16_clbk = NULL;
}

void start_timer_16b()
{
  TCCR1B |= (1 << CS12);                   // Prescaler = 256
}

void stop_timer_16b()
{
  TCCR1B &= 0b11111000;                    // No clock source
}

short unsigned get_counter()
{
  short unsigned counter = TCNT1;
  TCNT1 = 0;                             // Reset counter
  return counter;
}

short unsigned get_distance()
{
  // fclkio    = 16Mhz
  // prescaler = 256
  // 1 / (fclkio / prescaler) = time
  // time      = 16[us]
  // const     = 58
  return 160u * get_counter() / 58u; // in [mm]
}

void set_tim16_clbk(tim_16_clbk_t callback)
{
  tim_16_clbk = callback;
}
