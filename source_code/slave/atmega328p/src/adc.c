#include "adc.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static int unsigned result = 0;

ISR(ADC_vect)
{
  result = ADC;
}

static void set_admux_register(adc_admux_reg_t *admux)
{
  ADMUX = admux->volt_ref + admux->result_adj + admux->input_channel;
}

static void set_adcsra_register(adc_sra_reg_t *adcsra)
{
  ADCSRA = adcsra->converter_enable         +
           adcsra->trigger_conversion       +
           adcsra->auto_trigger_conversion  +
           adcsra->conversion_end_flag      + // is needed ????
           adcsra->conversion_end_interrupt +
           adcsra->prescaler_configuration;
}

static void set_adcsrb_register(adc_srb_reg_t *adcsrb)
{
  ADCSRB = adcsrb->auto_trigger_src;
}

void init_adc(adc_admux_reg_t *admux,
              adc_sra_reg_t   *adcsra,
              adc_srb_reg_t   *adcsrb)
{
  set_admux_register(admux);
  set_adcsrb_register(adcsrb);
  set_adcsra_register(adcsra);
}

bool is_converting()
{
  return ADCSRA & (1 << ADSC);
}

void start_conversion()
{
  ADCSRA |= 1 << ADSC;
}

int unsigned read_result()
{
  return result;
}

void clear_int_flag()
{
  ADCSRA |= (1 << ADIF);
}
