#include "gpio.h"
#include <avr/io.h>
#include "usart.h"

#define ADDRESS_OF_PORTB &PORTB

void init_gpio()
{
  // PORT B
  DDRB  = 0x09;                      // Configure all B ports as inputs
  PORTB = 0x09;                      // Set to LOW

  // PORT D
  DDRD  = 0xB0;                      // Configure all D ports as inputs except PD4
  PORTD = 0x80;                      // Set to LOW

  // PORT C
  DDRC  = 0x00;
  PORTC = 0x00;

}
static char unsigned volatile *get_pin_based_on_port(char unsigned volatile *port)
{
  if(&PORTB == port)
    return &PINB;
  else if(&PORTC == port)
    return &PINC;
  else
    return &PIND;

  return 0;
}

static char unsigned volatile *get_direction_based_on_port(char unsigned volatile *port)
{
  if(&PORTB == port)
    return &DDRB;
  else if(&PORTC == port)
    return &DDRC;
  else
    return &DDRD;

  return 0;
}

void set_gpio_output(char unsigned volatile *port, char unsigned number)
{
  char unsigned volatile *dir = get_direction_based_on_port(port);
  *dir |= (1 << number);
}

void set_gpio_input(char unsigned volatile *port, char unsigned number)
{
  char unsigned volatile *dir = get_direction_based_on_port(port);
  *dir &= ~(1 << number);
}

void set_gpio_high(char unsigned volatile *port, char unsigned number)
{
  *port |= (1 << number);
}

void set_gpio_low(char unsigned volatile *port, char unsigned number)
{
  *port &= ~(1 << number);
}

unsigned char get_gpio_pin_value(char unsigned volatile *port, char unsigned number)
{
    unsigned char pinBuf = *get_pin_based_on_port(port);
    return (pinBuf&(1<<number))>>number;
}
