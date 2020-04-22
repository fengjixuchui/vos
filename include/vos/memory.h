//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_MEMORY_H
#define VOS_MEMORY_H

#include "vos/types.h"

void init_memory (uint addr, uint len);

void memset (void* dest, uint8 c, uint n);

void memset16 (void* dest, uint16 c, uint n);

void memcpy (void* dest, const void* src, uint n);

int memcmp (const void* a, const void* b, uint n);

void* malloc (uint64 size);
void* calloc (uint64 size);
void  free (void* mem);

#endif //VOS_MEMORY_H
