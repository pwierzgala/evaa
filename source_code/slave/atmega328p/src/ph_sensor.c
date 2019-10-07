#include <util/delay.h>

#include "adc.h"

int get_ph()
{
  start_conversion();
  while(is_converting())
  {
    _delay_ms(1);
  }

  return read_result();
}
