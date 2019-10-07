#include "../inc/int.h"
#include "../inc/utils.h"

#include <avr/interrupt.h>             // for cli() and sei() and ISR

ISR(INT1_vect)
{

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
}

