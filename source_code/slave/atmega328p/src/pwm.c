#include "pwm_8b.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

// PWM OUTPUT - PORT PD5

void init_pwm_8b()
{
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  //TCCR0B |= (1 << WGM02);
}

void deinit_pwm_8b()
{
  TCCR0B |= (TCCR0B & 0xF8);               // CS00 = 0, CS01 = 0, CS02 = 0 - No clock source
  TCCR0A &= (~(1 << COM0B1) & ~(1 << WGM01) & ~(1 << WGM00));
  TCCR0B &= ~(1 << WGM02);
}

void pwm_8b_start()
{
  TCCR0A |= (1 << COM0B1);
  TCCR0B |= (TCCR0B & 0xF8) | (1 << CS02); // Preskaler 64
}

void pwm_8b_stop()
{
  TCCR0A &= (~(1 << COM0B1)) & (~(1 << COM0B0));
  TCCR0B &= (TCCR0B & 0xF8);               // CS00 = 0, CS01 = 0, CS02 = 0 - No clock source
}

void pwm_8b_set_duty(char unsigned duty)
{
  OCR0B = duty;
}
