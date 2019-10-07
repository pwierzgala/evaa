#ifndef _HEADER_ADC_
#define _HEADER_ADC_

#include "stdint.h"
#include "stdbool.h"

typedef struct adc_config adc_config;
typedef struct adc_admux_reg_t adc_admux_reg_t;
typedef struct adc_sra_reg_t adc_sra_reg_t;
typedef struct adc_srb_reg_t adc_srb_reg_t;

// ADMUX - ADC MULTIPLEXER SELECTION REGISTER

enum volt_ref_sel_t
{
  AREF_INTERNAL_VREF_OFF          = 0x00,
  AREF_TO_AVCC_EXTERNAL_CAPACITOR = 0x40,
  AREF_INTERNAL_TO_1_1_V          = 0xC0
};

enum result_adjust_t
{
  RESULT_RIGHT_ADJUST = 0x00,
  RESULT_LEFT_ADJUST  = 0x20
};


enum input_channel_sel_t
{
  ADC0  = 0x00,
  ADC1  = 0x01,
  ADC2  = 0x02,
  ADC3  = 0x03,
  ADC4  = 0x04,
  ADC5  = 0x05,
  ADC6  = 0x06,
  ADC7  = 0x07,
  ADC8  = 0x08,
  V_1_1 = 0x0E,
  V_1   = 0x0F,
};

// ADCSRA - ADC CONTROL AND STATUS REGISTER A

enum converter_enabled_t
{
  ADC_DISABLE = 0x00,
  ADC_ENABLE  = 0x80
};

enum trigger_conversion_t
{
  ADC_START_CONVERSION = 0x40,
  ADC_STOP_CONVERSION  = 0x00
};

enum auto_trigger_conversion_t
{
  ADC_AUTO_TRIGGER_DISABLE = 0x00,
  ADC_AUTO_TRIGGER_ENABLE  = 0x20
};

enum conversion_end_flag_t
{
  ADC_INTERRUPT_FLAG_CLEAR = 0x10,
  ADC_INTERRUPT_FLAG_SET   = 0x00
};

enum conversion_end_interrupt_t
{
  ADC_INTERRUPT_DISABLE = 0x00,
  ADC_INTERRUPT_ENABLE  = 0x08
};

enum prescaler_configuration_t
{
  DIV_FACTOR_NOMINAL = 0x00,
  DIV_FACTOR_2       = 0x01,
  DIV_FACTOR_4       = 0x02,
  DIV_FACTOR_8       = 0x03,
  DIV_FACTOR_16      = 0x04,
  DIV_FACTOR_32      = 0x05,
  DIV_FACTOR_64      = 0x06,
  DIV_FACTOR_128     = 0x07,
};

// ADCSRB - ADC CONTROL AND STATUS REGISTER B

enum auto_trigger_source_t
{
  FREE_RUNNING_MODE        = 0x00,
  ANALOG_COMPARATOR        = 0x01,
  EXTERNAL_INPUT_REQUEST_0 = 0x02,
  T_C_0_COMP_MATCH_A       = 0x03,
  T_C_0_OVERFLOW_A         = 0x04,
  T_C_0_COMP_MATCH_B       = 0x05,
  T_C_0_OVERFLOW_B         = 0x06,
  T_C_CAPTURE_EVENT        = 0x07
};

struct adc_config_t
{
  uint8_t admux;
  uint8_t adcsra;
  uint8_t adcsrb;
};

struct adc_admux_reg_t
{
  enum volt_ref_sel_t      volt_ref;
  enum result_adjust_t     result_adj;
  enum input_channel_sel_t input_channel;
};

struct adc_sra_reg_t
{
  enum converter_enabled_t        converter_enable;
  enum trigger_conversion_t       trigger_conversion;
  enum auto_trigger_conversion_t  auto_trigger_conversion;
  enum conversion_end_flag_t      conversion_end_flag;
  enum conversion_end_interrupt_t conversion_end_interrupt;
  enum prescaler_configuration_t  prescaler_configuration;
};

struct adc_srb_reg_t
{
  enum auto_trigger_source_t auto_trigger_src;
};


void init_adc(adc_admux_reg_t *admux,
              adc_sra_reg_t   *adcsra,
              adc_srb_reg_t   *adcsrb);


void start_conversion();
bool is_converting();
int unsigned read_result();
void clear_int_flag();

#endif
