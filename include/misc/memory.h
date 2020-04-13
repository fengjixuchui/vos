//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_MEMORY_H
#define VOS_MEMORY_H

int strlen (const char* s);

void memset (void* dest, int c, int n);

void memcpy (void* dest, const void* src, int n);

int memcmp (const void* a, const void* b, int n);

#endif //VOS_MEMORY_H
