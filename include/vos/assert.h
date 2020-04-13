//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_ASSERT_H
#define VOS_ASSERT_H

// clang-format off
#define AssertCompile(expr)           _Static_assert(!!(expr), #expr)
#define AssertCompileSize(type, size) AssertCompile (sizeof (type) == (size))
// clang-format on

#endif //VOS_ASSERT_H
