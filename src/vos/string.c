//
// Created by x7cc on 2020/4/14.
//

#include "vos/string.h"

int strlen (const char* s)
{
  int i = 0;
  while (s[i])
    ++i;
  return i;
}
