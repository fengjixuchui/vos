//
// Created by x7cc on 2020/4/13.
//

#include "vos/memory.h"

void memset (void* dest, int c, int n)
{
  for (int i = 0; i < n; ++i)
  {
    ((unsigned char*)dest)[i] = c;
  }
}

void memcpy (void* dest, const void* src, int n)
{
  for (int i = 0; i < n; ++i)
  {
    ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
  }
}

int memcmp (const void* a, const void* b, int n)
{
  for (int i = 0; i < n; ++i)
  {
    if (((unsigned char*)a)[i] < ((unsigned char*)b)[i])
      return -1;

    if (((unsigned char*)a)[i] > ((unsigned char*)b)[i])
      return 1;
  }

  return 0;
}
