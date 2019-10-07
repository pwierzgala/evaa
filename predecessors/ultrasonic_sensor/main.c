#include <avr/io.h>                    // adding header files
#include <avr/interrupt.h>             // for cli() and sei() and ISR

#include "utils.h"
#include <util/delay.h>                // for _delay_ms()

#include "int.h"
#include "gpio.h"
#include "usart.h"
#include "timer_8b.h"
#include "timer_16b.h"

int counter;
int treshold;

char unsigned timer_enabled = 0;

// For handling interrupts
void int1_handle()
{
  if(timer_enabled)
  {
    timer_enabled = 0;
    stop_timer_16b();
  }
  else
  {
    timer_enabled = 1;
    start_timer_16b();
  }
}

void timer_8b_callback(void)
{
  static short unsigned counter = 0;
  if(counter == treshold)
  {
    PORTD ^= 0b10000;
    counter = 0;
  }
  ++counter;
}

void usart_callback(char unsigned data)
{

}

void timer_16b_callback()
{

}

void enable_ultrasonic_sensor()
{
  PORTD = 0b00000100;
  _delay_us(10);
  PORTD = 0b00000000;
}

void init()
{
  // Electrical connection Arduino Nano
  // PD2(D2) - ultrasound triger [OUTPUT]
  // PD3(D3) - echo              [INPUT]
  // PD4(D4) - diode blinking    [OUTPUT]

  cli();                                   // Disable global interrupts

  // INT1
  init_int1();
  set_int1_clbk(int1_handle);

  // TIMER 8 BIT
  init_timer_8b();
  set_tim8_clbk(timer_8b_callback);

  // TIMER 16 BIT
  init_timer_16b();
  set_tim16_clbk(timer_16b_callback);

  // USART
  init_usart();
  set_usart_clbk(usart_callback);

  init_gpio();

  sei();                                   // Enable global interrupts
}

int main(void)
{
  init();

  unsigned short value = 0;
  while(1)
  {
    usart_send_str("Begin of iterration... \n\r");
    enable_ultrasonic_sensor();
    _delay_ms(2000);

    usart_send_str(short_to_str(get_distance()));
    usart_send_str("[mm]\n\r");

    usart_send_str("End of iterration \n\r");
  }
}
