#ifndef _HEADER_GPIO_
#define _HEADER_GPIO_

#include <avr/io.h>

void init_gpio();

void set_gpio_input(char unsigned volatile *port, char unsigned number);
void set_gpio_output(char unsigned volatile *port, char unsigned number);
void set_gpio_low(char unsigned volatile *port, char unsigned number);
void set_gpio_high(char unsigned volatile *port, char unsigned number);

#endif
