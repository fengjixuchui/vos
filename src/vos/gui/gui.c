//
// Created by x7cc on 2020/4/27.
//

#include "vos/gui/gui.h"
#include "vos/memory.h"
#include "vos/types.h"
#include "vos/x86_64.h"

typedef struct
{
  vos_uint8* framebuffer;
  vos_uint   width;
  vos_uint   height;
  vos_uint   pitch;
  vos_uint   depth;
} screen_t;

typedef struct
{
  vos_uint    x, y, w, h;
  const char* caption;
} window_t;

static screen_t* screen;

vos_uint gui_init (vos_uint64 framebuffer, vos_uint64 pitch, vos_uint width, vos_uint height, vos_uint depth)
{
  screen              = (screen_t*)malloc (sizeof (screen_t));
  screen->framebuffer = framebuffer;
  screen->pitch       = pitch;
  screen->depth       = depth;
  screen->width       = width;
  screen->height      = height;

  register vos_uint row = screen->framebuffer;
  for (register int y = 0; y < screen->height; ++y, row += screen->pitch)
  {
    __memset32 (row, 0xFFA0A0A0 /* ARGB */, screen->width);
  }

  return 0;
}

vos_uint window_new (vos_uint x, vos_uint y, vos_uint w, vos_uint h)
{
  window_t* window = (window_t*)malloc (sizeof (window_t));
  window->x        = x;
  window->y        = y;
  window->w        = w;
  window->h        = h;
  window->caption  = NULL;

  window_update (window);

  return window;
}

vos_uint window_delete (vos_uint window)
{
  return 0;
}

void window_update (vos_uint window)
{
  vos_uint right  = ((window_t*)window)->x + ((window_t*)window)->w;
  vos_uint bottom = ((window_t*)window)->y + ((window_t*)window)->h;
  for (int x = ((window_t*)window)->x; x < right; ++x)
  {
    for (int y = ((window_t*)window)->y; y < bottom; ++y)
    {
      vos_uint8* pixel = (screen->framebuffer + y * screen->pitch + x * screen->depth);
      pixel[0]         = 0xF5; // B
      pixel[1]         = 0xF5; // G
      pixel[2]         = 0xF5; // R?
      pixel[0]         = 0xF4; // B
      pixel[1]         = 0x85; // G
      pixel[2]         = 0x42; // R?
      pixel[3]         = 255;
    }
  }
  return;
}

vos_uint gui_set_caption (vos_uint window, vos_uint8* text, vos_uint len)
{
  return 0;
}
