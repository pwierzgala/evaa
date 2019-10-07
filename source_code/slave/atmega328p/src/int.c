#include "int.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

ISR(INT1_vect)
{
  if(int1_clbk)
    int1_clbk();
}

void int1_enable()
{
  EIMSK = (EIMSK & 0xFC) | 0x02; // Enable external INT
}

void int1_disable()
{
  EIMSK = (EIMSK & 0xFC);        // Disable external INT
}

void init_int1()
{
  MCUCR = (MCUCR & 0xFC) | 0x02;     // We choose to locate interrupts on the bootloader section of the flash
  EICRA = (EICRA & 0xF3) | 0x04;    // Any logical change on INT1 generates interrupt
  int1_enable();
}

void deinit_int1()
{
  int1_disable();
  int1_clbk = NULL;
}


void set_int1_clbk(int1_clbk_t callback)
{
  int1_clbk = callback;
}

