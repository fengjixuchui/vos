//
// Created by x7cc on 2020/4/24.
//

#ifndef VOS_GUEST_H
#define VOS_GUEST_H

#include "vos/types.h"

typedef struct guest_config
{
  uint64 physical_address;
  uint64 mem_size;
  uint64 page_size;
  uint8  vendor[12];
  uint8  core_num;
  uint8  thread_num;
} guest_config_t;

#endif //VOS_GUEST_H
