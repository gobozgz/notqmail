#define _GNU_SOURCE
#include <string.h>

int main()
{
  const char *s = "abc";
  return strchrnul(s, 'd') == s + 3 ? 0 : 1;
}
