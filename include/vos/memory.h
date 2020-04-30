//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_MEMORY_H
#define VOS_MEMORY_H

#include "vos/types.h"

#define memset8 __memset8
#define memset16 __memset16
#define memset32 __memset32
#define memset64 __memset64

void init_memory (uint addr, uint len);

void memcpy (void* dest, const void* src, uint n);

int memcmp (const void* a, const void* b, uint n);

void* malloc (uint64 size);
void* calloc (uint64 size);
void  free (void* mem);

uint64 VirtualAddressToPhysicalAddress (uint64 va);

#endif //VOS_MEMORY_H
