#include <stdio.h>

#define POLYNOMINAL 0x8C

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
/*
int main()
{
  //char unsigned data[10] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9};
  char unsigned data[10] = {0x01, 0x01, 0x01, 0x00, 0x02, 0x07, 0x01, 0x00, 0x00, 0x00};

  char unsigned crc = 0;
  crc = dallas_crc_8(data, 10);
  printf("crc = 0x%x\n", crc);
  return 0;
}
*/
