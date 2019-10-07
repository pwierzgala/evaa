#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#define NULL 0

#ifndef F_CPU
#define F_CPU 16000000UL               // or whatever may be your frequency
#endif

char unsigned *byte_to_str(char unsigned byte);
char unsigned *short_to_str(short unsigned word);

#endif /* INC_UTILS_H_ */
