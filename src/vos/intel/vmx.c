//
// Created by x7cc on 2020/4/15.
//

#include "vos/intel/vmx.h"

const char* VM_INSTRUCTION_ERROR_STRING (int num)
{
#define XX(N, S) \
  case N:        \
    return S;
  switch (num)
  {
    VM_INSTRUCTION_ERROR_NUMBERS (XX);
  }
#undef XX
  return "";
}