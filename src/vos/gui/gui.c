//
// Created by x7cc on 2020/4/27.
//

#include "vos/gui/gui.h"
#include "vos/memory.h"
#include "vos/types.h"

typedef struct
{
  uint8* framebuffer;
  uint   width;
  uint   height;
  uint   pitch;
  uint   depth;
} screen_t;

typedef struct
{
  uint        x, y, w, h;
  const char* caption;
} window_t;

static screen_t* screen;

uint gui_init (uint64 framebuffer, uint64 pitch, uint width, uint height, uint depth)
{
  screen              = (screen_t*)malloc (sizeof (screen_t));
  screen->framebuffer = framebuffer;
  screen->pitch       = pitch;
  screen->depth       = depth;
  screen->width       = width;
  screen->height      = height;

  for (int x = 0; x < screen->width; ++x)
  {
    for (int y = 0; y < screen->height; ++y)
    {
      uint8* pixel = (screen->framebuffer + y * screen->pitch + x * screen->depth);
      pixel[0]     = 0xA0; // B
      pixel[1]     = 0xA0; // G
      pixel[2]     = 0xA0; // R?
      pixel[3]     = 0xFF;
    }
  }

  return 0;
}

uint window_new (uint x, uint y, uint w, uint h)
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

uint window_delete (uint window)
{
  return 0;
}

void window_update (uint window)
{
  uint right  = ((window_t*)window)->x + ((window_t*)window)->w;
  uint bottom = ((window_t*)window)->y + ((window_t*)window)->h;
  for (int x = ((window_t*)window)->x; x < right; ++x)
  {
    for (int y = ((window_t*)window)->y; y < bottom; ++y)
    {
      uint8* pixel = (screen->framebuffer + y * screen->pitch + x * screen->depth);
      pixel[0]     = 0xF5; // B
      pixel[1]     = 0xF5; // G
      pixel[2]     = 0xF5; // R?
      pixel[0]     = 0xF4; // B
      pixel[1]     = 0x85; // G
      pixel[2]     = 0x42; // R?
      pixel[3]     = 255;
    }
  }
  return;
}

uint gui_set_caption (uint window, uint8* text, uint len)
{
  return 0;
}
