//
// Created by x7cc on 2020/5/4.
//

#include "vos/guest.h"

uint guest_malloc (vos_guest_t* guest, uint size)
{
  uint ret = guest->memory_base;
  guest->memory_base += (size + VOS_PAGE_SIZE - 1) & ~(VOS_PAGE_SIZE - 1);
  return ret;
}
