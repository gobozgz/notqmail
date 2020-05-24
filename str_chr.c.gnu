#define _GNU_SOURCE
#include "str.h"

#include <string.h>

unsigned int str_chr(const char *s, int c)
{
  return strchrnul(s, c) - s;
}
