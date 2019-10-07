/*
 * led.h
 *
 *  Created on: 8 sie 2019
 *      Author: pmikolajczyk1
 */

#ifndef INC_LED_H_
#define INC_LED_H_

void init_timer0(void);
void init_timer1(void);
void init_timer2(void);

void set_PB3_duty_cycle(int width);
void set_PD3_duty_cycle(int width);
void set_PB1_duty_cycle(int width);
void set_PB2_duty_cycle(int width);
void set_PD5_duty_cycle(int width);
void set_PD6_duty_cycle(int width);

#endif /* INC_LED_H_ */
