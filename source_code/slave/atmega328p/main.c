#include "fsm.h"
#include "int.h"
#include "adc.h"
#include "pcint.h"
#include "gpio.h"
#include "utils.h"
#include "usart.h"
#include "pwm_8b.h"
#include "timer_8b.h"
#include "timer_16b.h"
#include "ultrasonic_sensor.h"
#include "frame_receiver.h"
#include "humidity_sensor.h"

#include "frame_decoder.h"

// #define _ONE_WIRE_SENSOR_TEST_

int unsigned treshold;

void int1_handle()
{
  us_sensor_handler();
}

void pcint1_handle()
{
  dht_sensor_handler();
}

void timer_8b_callback(void)
{
  static short unsigned counter = 0;
  if(counter == treshold)
  {
    PORTD ^= 0x10;
    counter = 0;
  }
  ++counter;
}

void usart_callback(char unsigned data)
{
  write_byte_to_rx_buffer(get_buffer(), data);
  if(is_frame_ready_to_read(get_buffer()))
  {
    kickstart(get_instance_fsm());
  }
}

void timer_16b_ovfl_callback()
{

}

void timer_16b_comp_callback()
{
  clear_slot();
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

  // PCINT
  init_pcint1();
  set_pcint1_clbk(pcint1_handle);

  // TIMER 8 BIT
  init_timer_8b();
  set_tim8_clbk(timer_8b_callback);

  // TIMER 16 BIT
  init_timer_16b();
  set_tim16_ovfl_clbk(timer_16b_ovfl_callback);
  set_tim16_comp_clbk(timer_16b_comp_callback);

  // USART
  init_usart();
  set_usart_clbk(usart_callback);

  init_pwm_8b();

  // GPIO
  init_gpio();

  // ADC
  adc_admux_reg_t admux;
  admux.volt_ref                  = AREF_INTERNAL_VREF_OFF;
  admux.result_adj                = RESULT_RIGHT_ADJUST;
  admux.input_channel             = ADC0;

  adc_sra_reg_t srareg;
  srareg.converter_enable         = ADC_ENABLE;
  srareg.trigger_conversion       = ADC_STOP_CONVERSION;
  srareg.auto_trigger_conversion  = ADC_AUTO_TRIGGER_DISABLE;
  srareg.conversion_end_flag      = ADC_INTERRUPT_FLAG_CLEAR;
  srareg.conversion_end_interrupt = ADC_INTERRUPT_ENABLE;
  srareg.prescaler_configuration  = DIV_FACTOR_128;

  adc_srb_reg_t srbreg;
  srbreg.auto_trigger_src         = FREE_RUNNING_MODE;

  init_adc(&admux, &srareg, &srbreg);

  sei();                                                      // Enable global interrupts
}

int main(void)
{
  fsm_t *fsm = get_instance_fsm();
  init();
  fsm_init(fsm);
  fsm->run();
}
