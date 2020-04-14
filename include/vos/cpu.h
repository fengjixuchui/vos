//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS2_CPU_H
#define VOS2_CPU_H

#include "vos/types.h"

typedef struct cpuid__
{
  uint32 eax;
  uint32 ebx;
  uint32 ecx;
  uint32 edx;
} cpuid_t;

#endif //VOS2_CPU_H
