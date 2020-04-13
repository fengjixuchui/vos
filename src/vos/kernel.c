#include "vos/types.h"

const char* str   = "hello! this is my kernel64 program.";
char*       video = (char*)0xb8000;

void cls ()
{
  int fg_color = 7; // 字体RGB
  int bg_color = 1; // 背景RGB
  for (int i = 0; i < 2000; ++i)
  {
    video[i * 2]     = 0;
    video[i * 2 + 1] = (bg_color << 4) | fg_color; // RGB
  }
}

int x86_64_main ()
{
  cls ();

  int i = 0;
  while (str[i])
  {
    video[i * 2] = str[i];
    ++i;
  }

  return 0;
}
