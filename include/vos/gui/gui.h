//
// Created by x7cc on 2020/4/27.
//

#ifndef VOS_GUI_H
#define VOS_GUI_H

#include "vos/types.h"

uint gui_init (uint64 framebuffer, uint64 pitch, uint width, uint height, uint depth);
uint window_new (uint x, uint y, uint w, uint h);
uint window_delete (uint window);
void window_update (uint window);
uint gui_set_caption (uint window, uint8* text, uint len);

#endif //VOS_GUI_H
