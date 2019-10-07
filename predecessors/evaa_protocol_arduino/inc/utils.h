#ifndef _HEADER_UTILS_
#define _HEADER_UTILS_

#define NULL 0

#ifndef F_CPU
#define F_CPU 16000000UL               // or whatever may be your frequency
#endif

char unsigned *byte_to_str(char unsigned byte);
char unsigned *short_to_str(short unsigned word);

#endif
