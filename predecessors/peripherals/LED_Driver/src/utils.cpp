#include "../inc/utils.h"

char unsigned *short_to_str(short unsigned word)
{
  static char unsigned str[6] = { 0, 0, 0, 0, 0, 0 };
  for(int i = 4; i >= 0; --i)
  {
    str[i] = word % 10 + '0';
    word /= 10;
  }

  return str;
}

char unsigned *byte_to_str(char unsigned byte)
{
  static char unsigned str[4] = { 0, 0, 0, 0 };

  for(int i = 2; i >= 0; --i)
  {
    str[i] = byte % 10 + '0';
    byte /= 10;
  }

  return str;
}
