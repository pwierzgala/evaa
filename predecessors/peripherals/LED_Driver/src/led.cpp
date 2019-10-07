#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void init_timer0(void){
	DDRD |= (1<<PORTD5) | (1<<PORTD6); // PORTD.5 and PORTD.6 is output
	TCCR0A |= (1<<WGM01) | (1<<WGM00); /* Waveform Generation Mode 3 - Fast PWM */
	TCCR0A |= (1<<COM0A1); /* non inverting mode */
	TCCR0A |= (1<<COM0B1); /* non-inverting mode */
	TCCR0B |= (1<<CS00);	//clock source 1

}

void init_timer1(void){
    DDRB |= (1<<PB1) | (1<<PB2);    //PWM output PB1 and PB2
    // Clear Timer/Counter Control Registers
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1A |= (1 << COM1A1) | (1<<COM1B1); // Set non-inverting mode
    TCCR1A |= (1<<WGM10);// Set fast PWM Mode 5 (16 bit timer as 8-bit fast PWM)
    TCCR1B |= (1 << WGM12); //
    TCCR1B |= (1 << CS10); // Set prescaler to 64 and starts PWM
}

void init_timer2(void){
	DDRB |= (1<<PB3);	//PWM output PB3 (register OCR2A)
	DDRD |= (1<<PD3);	//PWM output PD3 (register OCR2B)
	TCCR2A |= (1<<WGM21) | (1<<WGM20);
	TCCR2A |= (1<<COM2A1); /* non inverting mode */
	TCCR2A |= (1<<COM2B1); /* non-inverting mode */
	TCCR2B |= (1<<CS00);
}

void set_PB3_duty_cycle(int width){  //timer2 OCR2A reg sets duty cycle of PB3
	OCR2A = width;
}

void set_PD3_duty_cycle(int width){	//timer2 OCR2B reg sets duty cycle of PD2
	OCR2B = width;
}

void set_PB1_duty_cycle(int width){	//timer1 OCR1A reg sets duty cycle of PB1
	OCR1A = width;
}
void set_PB2_duty_cycle(int width){ //timer1 OCR1B reg sets duty cycle of PB2
	OCR1B = width;
}

void set_PD5_duty_cycle(int width){ //timer0 OCR0B reg sets duty cycle of PD5
	OCR0B = width;
}

void set_PD6_duty_cycle(int width){ //timer0 OCR0A reg sets duty cycle of PD6
	OCR0A = width;
}
