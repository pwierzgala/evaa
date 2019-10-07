#include "int.h"
#include "utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

ISR(INT1_vect)
{
  if(int1_clbk)
    int1_clbk();
}

void init_int1()
{
  MCUCR = (MCUCR & 0b11111100) | 0b10;     // We choose to locate interrupts on the bootloader section of the flash
  EICRA = (EICRA & 0b11110011) | 0b100;    // Any logical change on INT1 generates interrupt
  EIMSK = (EIMSK & 0b11111100) | 0b10;     // Enable external INT
}

void deinit_int1()
{
  EIMSK = (EIMSK & 0b11111100);            // Disable external INT
  int1_clbk = NULL;
}

void set_int1_clbk(int1_clbk_t callback)
{
  int1_clbk = callback;
}

