//
// Created by x7cc on 2020/5/4.
//

#include "vos/guest.h"

vos_uint guest_malloc (vos_guest_t* guest, vos_uint size)
{
  vos_uint ret = guest->memory_base_tmep;
  guest->memory_base_tmep += (size + VOS_PAGE_SIZE - 1) & ~(VOS_PAGE_SIZE - 1);
  return ret;
}
