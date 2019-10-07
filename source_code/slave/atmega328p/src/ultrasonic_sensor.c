#include "gpio.h"
#include "timer_16b.h"
#include "ultrasonic_sensor.h"
#include "utils.h"

#include <util/delay.h>

void us_sensor_handler()
{
  if(get_timer_16b_status())
  {
    stop_timer_16b();
  }
  else
  {
    start_timer_16b(TIM_16B_PRESCALER_8);
  }
}

int unsigned get_distance()
{
  // fclkio    = 16Mhz
  // prescaler = 8
  // 1 / (fclkio / prescaler) = time
  // time      = 0.5[us]
  // const     = 58
  uint32_t counter = get_counter_and_reset();
  return counter * 250u / 29u; //100.0*get_counter_and_reset() *0.5*10.0/58.0; // unit: 10^(-2)[mm]
}

void trigger_us_sensor()
{
  set_gpio_output(&PORTD, 0x02);
  set_gpio_high(&PORTD, 0x02);
  _delay_us(10);
  set_gpio_low(&PORTD, 0x02);
 
}
