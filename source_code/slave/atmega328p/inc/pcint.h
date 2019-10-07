#ifndef _HEADER_PCINT_
#define _HEADER_PCINT_

typedef void(*pcint1_clbk_t)(void);
pcint1_clbk_t pcint1_clbk;

void pcint1_enable();
void pcint1_disable();

void init_pcint1();
void deinit_pcint1();

void set_pcint1_clbk(pcint1_clbk_t callback);

#endif
