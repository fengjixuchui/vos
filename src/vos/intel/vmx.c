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

void* make_guest_PML4E ()
{
  uint64 page_begin = 0;

  uint64* pml4 = (uint64*)calloc (4096);
  for (int L2 = 0; L2 < 1; ++L2)
  {
    uint64* pbp = (uint64*)calloc (4096);
    pml4[L2]    = (uint64)pbp | 1;
    for (int L3 = 0; L3 < 1; ++L3)
    {
      uint64* pb = (uint64*)calloc (4096);
      pbp[L3]    = (uint64)pb | 1;

      for (int L4 = 0; L4 < 512; ++L4)
      {
        uint64* pt = (uint64*)calloc (4096);
        for (int i = 0; i < 512; ++i)
        {
          ((uint64*)pt)[i] = page_begin | 1;
          page_begin += 4096;
        }
        pb[L4] = (uint64)pt | 1;
      }
    }
  }
  return pml4;
}