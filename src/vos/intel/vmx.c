//
// Created by x7cc on 2020/4/15.
//

#include "vos/intel/vmx.h"
#include "vos/types.h"
#include "vos/vos.h"
#include "vos/memory.h"

const char* VMX_INSTRUCTION_ERROR_STRING (int num)
{
#define XX(N, S) \
  case N:        \
    return S;

  switch (num)
  {
    VMX_INSTRUCTION_ERROR_NUMBERS (XX);
  }
#undef XX
  return "";
}

uint64* make_PML4 (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

uint64* make_PDP (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

uint64* make_PD (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

static uint64 page_begin = 0;
uint64*       make_PT (void* p)
{
  memset (p, 0, 4096);
  for (int i = 0; i < 512; ++i)
  {
    ((uint64*)p)[i] = page_begin | 1;
    page_begin += 4096;
  }
  return (uint64*)p;
}

uint64 make_guest_PML4E ()
{
  page_begin = 0;

  uint64  offset = VOS_GUEST_PML4E_PA;
  uint64* pml4   = make_PML4 ((void*)offset);
  offset += 4096;
  for (int L2 = 0; L2 < 1; ++L2)
  {
    uint64* pbp = make_PDP ((void*)offset);
    offset += 4096;
    pml4[L2] = (uint64)pbp | 1;
    for (int L3 = 0; L3 < 1; ++L3)
    {
      uint64* pb = make_PD ((void*)offset);
      offset += 4096;
      pbp[L3] = (uint64)pb | 1;

      for (int L4 = 0; L4 < 512; ++L4)
      {
        uint64* pt = make_PT ((void*)offset);
        offset += 4096;
        pb[L4] = (uint64)pt | 1;
      }
    }
  }
  return VOS_GUEST_PML4E_PA;
}