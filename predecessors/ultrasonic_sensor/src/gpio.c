#include "gpio.h"
#include <avr/io.h>

void init_gpio()
{
  // PORT B
  DDRB  = 0b00000000;                      // Configure all B ports as inputs
  PORTB = 0b00000000;                      // Set to LOW

  // PORT D
  DDRD  = 0b00010100;                      // Configure all D ports as inputs except PD4
  PORTD = 0b00000000;                      // Set to LOW
}
