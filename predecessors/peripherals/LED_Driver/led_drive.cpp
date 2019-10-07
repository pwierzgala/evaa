#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "inc/int.h"
#include "inc/usart.h"
#include "inc/led.h"


int main (void)
{
	init_usart();
	init_timer0();
	init_timer1();
	init_timer2();

	int width =200;
	set_PB3_duty_cycle(width);
	set_PD3_duty_cycle(width);
	set_PB1_duty_cycle(width);
	set_PB2_duty_cycle(width);
	set_PD5_duty_cycle(width);
	set_PD6_duty_cycle(width);

	return 0;
}
