#include "utils.h"

char *short_to_str(short unsigned word)
{
  static char str[6] = { 0, 0, 0, 0, 0, 0 };
  for(int i = 4; i >= 0; --i)
  {
    str[i] = word % 10 + '0';
    word /= 10;
  }

  return str;
}

char *byte_to_str(char unsigned byte)
{
  static char str[4] = { 0, 0, 0, 0 };

  for(int i = 2; i >= 0; --i)
  {
    str[i] = byte % 10 + '0';
    byte /= 10;
  }

  return str;
}

