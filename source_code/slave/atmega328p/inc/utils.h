#ifndef _HEADER_UTILS_
#define _HEADER_UTILS_

#ifndef F_CPU
#define F_CPU 16000000UL               // or whatever may be your frequency
#endif

#include <stddef.h>
#include <util/delay.h>

char *byte_to_str(char unsigned byte);
char *short_to_str(short unsigned word);

#endif
