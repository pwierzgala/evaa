#include "pcint.h"
#include "utils.h"

#include <avr/io.h>
#include <avr/interrupt.h>              // for cli() and sei() and ISR

ISR(PCINT1_vect)
{
    if(pcint1_clbk)
        pcint1_clbk();
}

void pcint1_enable()
{
  PCMSK1 |= (1 << PCINT9);              // Enable pin change interrupt on PCINT9 
                                        // Any logical change on PCINT9 generates interrupt
}

void pcint1_disable()
{
  PCMSK1 = (PCMSK1 & 0x80);             // Disable pin change interrupt
}

void init_pcint1()
{
  PCICR |= (1 << PCIE1);                // Enable pin change interrupt (PCINT)
}

void deinit_pcint1()
{
  pcint1_disable();
  PCICR = (PCICR & 0xF8);               // Disable pin change interrupt (PCINT)
  pcint1_clbk = NULL;
}

void set_pcint1_clbk(pcint1_clbk_t callback)
{
  pcint1_clbk = callback;
}
