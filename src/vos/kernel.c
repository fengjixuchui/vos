#include "vos/types.h"

const char* str   = "hello! this is my kernel64 program.";
char*       video = (char*)0xb8000;

int x86_64_main ()
{
  int i = 0;
  while (str[i])
  {
    video[i * 2] = str[i];
    ++i;
  }

  return 0;
}
