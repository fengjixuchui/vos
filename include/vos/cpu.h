//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS2_CPU_H
#define VOS2_CPU_H

#include "vos/types.h"
#include "vos/assert.h"

typedef struct cpuid__
{
  uint32 eax;
  uint32 ebx;
  uint32 ecx;
  uint32 edx;
} cpuid_t;
AssertCompileSize (struct cpuid__, 16);

#pragma pack(1)
typedef struct gdtr__{
  uint16 limit;
  uint64 base;
} gdtr_t, ldtr_t, idtr_t;
#pragma pack()
AssertCompileSize (struct gdtr__, 10);

#endif //VOS2_CPU_H
