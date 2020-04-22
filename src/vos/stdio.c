//
// Created by x7cc on 2020/4/15.
//

#include "vos/stdio.h"
#include "vos/memory.h"
#include "bochs/bochs.h"
#include "vos/types.h"
#include <stdarg.h>

/*  Some screen stuff. */
/*  The number of columns. */
#define COLUMNS 80
/*  The number of lines. */
#define LINES 25
/*  The attribute of an character. */
#define ATTRIBUTE 23
/*  The video memory address. */
#define VIDEO 0xB8000
/*  Variables. */
/*  Save the X position. */
static int xpos;
/*  Save the Y position. */
static int ypos;
/*  Point to the video memory. */
static unsigned char* video;

void putc (char c)
{
  if (c == '\n')
  {
  newline:
    xpos = 0;
    ypos++;
    if (ypos >= LINES)
    {
      int FULLSIZE = COLUMNS * LINES * 2;

      ypos = LINES - 1;
      memcpy (video, video + (2 * COLUMNS), FULLSIZE - 2 * COLUMNS);
      memset16 (video + FULLSIZE - (2 * COLUMNS), ATTRIBUTE << 8, COLUMNS);
    }
    return;
  }

  *(video + (xpos + ypos * COLUMNS) * 2)     = c;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

void puts (const char* s)
{
  int i = 0;
  while (s[i])
  {
    putc (s[i]);
    ++i;
  }
  putc ('\n');
}

void cls ()
{
  video        = (unsigned char*)VIDEO;
  int fg_color = 7; // 字体RGB
  int bg_color = 1; // 背景RGB
  for (int i = 0; i < 2000; ++i)
  {
    video[i * 2]     = 0;
    video[i * 2 + 1] = (bg_color << 4) | fg_color; // RGB
  }
}

void itoa (char* buf, int base, int d)
{
  char*         p = buf;
  char *        p1, *p2;
  unsigned long ud      = d;
  int           divisor = 10;

  /*  If %d is specified and D is minus, put ‘-’ in the head. */
  if (base == 'd' && d < 0)
  {
    *p++ = '-';
    buf++;
    ud = -d;
  }
  else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do
  {
    int remainder = ud % divisor;

    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
  } while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;

  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
  {
    char tmp = *p1;
    *p1      = *p2;
    *p2      = tmp;
    p1++;
    p2--;
  }
}

/*  Format a string and print it on the screen, just like the libc
   function printf. */
void print (const char* format, ...)
{
  va_list p_args;

  va_start (p_args, format);

  int  c;
  char buf[20];

  while ((c = *format++) != 0)
  {
    if (c != '%')
      putc (c);
    else
    {
      char *p, *p2;
      int   pad0 = 0, pad = 0;

      c = *format++;
      if (c == '0')
      {
        pad0 = 1;
        c    = *format++;
      }

      if (c >= '0' && c <= '9')
      {
        pad = c - '0';
        c   = *format++;
      }

      switch (c)
      {
        case 'd':
        case 'u':
        case 'x':
          itoa (buf, c, va_arg (p_args, uint64));
          p = buf;
          goto string;
          break;

        case 's':
          p = va_arg (p_args, char*);
          if (!p)
            p = "(null)";

        string:
          for (p2 = p; *p2; p2++)
            ;
          for (; p2 < p + pad; p2++)
            putc (pad0 ? '0' : ' ');
          while (*p)
            putc (*p++);
          break;

        default:
          putc (va_arg (p_args, uint8));
          break;
      }
    }
  }
  va_end (p_args);
}
