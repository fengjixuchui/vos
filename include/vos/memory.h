//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_MEMORY_H
#define VOS_MEMORY_H

#include "vos/types.h"
#include "vos/vos.h"

extern void __memset8 (void*, vos_uint8, vos_uint len);
extern void __memset16 (void*, vos_uint16, vos_uint len);
extern void __memset32 (void*, vos_uint32, vos_uint len);
extern void __memset64 (void*, vos_uint64, vos_uint len);

void init_memory (vos_uint addr, vos_uint len);

void memcpy (void* dest, const void* src, vos_uint n);

int memcmp (const void* a, const void* b, vos_uint n);

void* malloc (vos_uint64 size);
void* calloc (vos_uint64 size);
void  free (void* mem);

vos_uint64 HVA_to_HPA (vos_uintptr va);

/// Page Frame Number To Page Address
/// \param pfn
/// \return
static inline vos_uintptr pfn_to_pa (vos_uint pfn)
{
  return pfn << VOS_PAGE_SHIFT;
}

/// Page Address To Page Frame Number To
/// \param pfn
/// \return
static inline vos_uint pa_to_pfn (vos_uintptr pa)
{
  return pa >> VOS_PAGE_SHIFT;
}

#endif //VOS_MEMORY_H
