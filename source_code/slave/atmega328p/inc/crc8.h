#ifndef _HEADER_CRC_8_
#define _HEADER_CRC_8_

#include <stdio.h>

#define POLYNOMINAL           0x8C


char unsigned calculate_crc_8(char unsigned *data,
                              char unsigned size,
                              char unsigned poly);

char unsigned dallas_crc_8(char unsigned *data,
                           char unsigned size);

#endif // _HEADER_CRC_8_
