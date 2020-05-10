//
// Created by amy on 11/9/19.
//

#ifndef VOS_TYPES_H
#define VOS_TYPES_H

#include "vos/assert.h"

typedef signed char        vos_int8;
typedef short              vos_int16;
typedef int                vos_int32;
typedef long long          vos_int64;
typedef unsigned char      vos_uint8;
typedef unsigned short     vos_uint16;
typedef unsigned int       vos_uint32;
typedef unsigned long long vos_uint64;
AssertCompileSize (vos_int8, 1);
AssertCompileSize (vos_int16, 2);
AssertCompileSize (vos_int32, 4);
AssertCompileSize (vos_int64, 8);
AssertCompileSize (vos_uint8, 1);
AssertCompileSize (vos_uint16, 2);
AssertCompileSize (vos_uint32, 4);
AssertCompileSize (vos_uint64, 8);

// clang-format off
#if __x86_64__ || _WIN64
  typedef vos_uint64 vos_uint;
  typedef vos_uint64 vos_uintptr;
#elif __i386__ || _WIN32
  typedef vos_uint16 vos_uint;
  typedef vos_uint16 vos_uintptr;
#else
  #error "wtf???"
#endif

#ifndef nullptr
  #define nullptr 0
#endif

#ifndef NULL
  #define NULL 0
#endif

// clang-format on

AssertCompile ((sizeof (void*) == sizeof (vos_uintptr)));
AssertCompile ((sizeof (void*) == sizeof (vos_uint)));

#endif //VOS_TYPES_H
