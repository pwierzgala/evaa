#ifndef _HEADER_INT_
#define _HEADER_INT_

typedef void(*int1_clbk_t)(void);
int1_clbk_t int1_clbk;

void init_int1();
void deinit_int1();

void set_int1_clbk(int1_clbk_t callback);

#endif
