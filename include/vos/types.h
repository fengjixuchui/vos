//
// Created by amy on 11/9/19.
//

#ifndef VOS_TYPES_H
#define VOS_TYPES_H

#include "vos/assert.h"

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

AssertCompileSize (int8, 1);
AssertCompileSize (int16, 2);
AssertCompileSize (int32, 4);
AssertCompileSize (int64, 8);
AssertCompileSize (uint8, 1);
AssertCompileSize (uint16, 2);
AssertCompileSize (uint32, 4);
AssertCompileSize (uint64, 8);

#endif //VOS_TYPES_H
