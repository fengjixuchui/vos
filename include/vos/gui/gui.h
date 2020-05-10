//
// Created by x7cc on 2020/4/27.
//

#ifndef VOS_GUI_H
#define VOS_GUI_H

#include "vos/types.h"

vos_uint gui_init (vos_uint64 framebuffer, vos_uint64 pitch, vos_uint width, vos_uint height, vos_uint depth);
vos_uint window_new (vos_uint x, vos_uint y, vos_uint w, vos_uint h);
vos_uint window_delete (vos_uint window);
void     window_update (vos_uint window);
vos_uint gui_set_caption (vos_uint window, vos_uint8* text, vos_uint len);

#endif //VOS_GUI_H
