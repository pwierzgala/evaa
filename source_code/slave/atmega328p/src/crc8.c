#include "crc8.h"

char unsigned calculate_crc_8(char unsigned *data, char unsigned size, char unsigned poly)
{
  char unsigned crc = 0;
  for(int i = 0; i < size; ++i)
  {
    char unsigned inbyte = data[i];
    for(int j = 0; j < 8; ++j)
    {
      char unsigned mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if(mix)
        crc ^= poly;
      inbyte >>= 1;
    }
  }
  return crc;
}

char unsigned dallas_crc_8(char unsigned *data, char unsigned size)
{
  return calculate_crc_8(data, size, POLYNOMINAL);
}
