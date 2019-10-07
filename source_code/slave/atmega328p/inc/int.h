#ifndef _HEADER_INT_
#define _HEADER_INT_

#include <avr/interrupt.h>             // for cli() and sei() and ISR

typedef void(*int1_clbk_t)(void);
int1_clbk_t int1_clbk;

void int1_enable();
void int1_disable();

void init_int1();
void deinit_int1();

void set_int1_clbk(int1_clbk_t callback);

#endif
