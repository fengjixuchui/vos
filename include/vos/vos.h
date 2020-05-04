//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_VOS_H
#define VOS_VOS_H

#include "vos/types.h"

// clang-format off

#define elf64_fastcall_argv0 rdi
#define elf64_fastcall_argv1 rsi
#define elf64_fastcall_argv2 rdx
#define elf64_fastcall_argv3 rcx

#define win64_fastcall_argv0 rcx
#define win64_fastcall_argv1 rdx
#define win64_fastcall_argv2 r8
#define win64_fastcall_argv3 r9

#if __x86_64__ || _WIN64
  #define argv0 elf64_fastcall_argv0
  #define argv1 elf64_fastcall_argv1
  #define argv2 elf64_fastcall_argv2
  #define argv3 elf64_fastcall_argv3
#elif __i386__ || _WIN32
  #define argv0 win64_fastcall_argv0
  #define argv1 win64_fastcall_argv1
  #define argv2 win64_fastcall_argv2
  #define argv3 win64_fastcall_argv3
#else
  #error "目前只支持elf64格式的fastcall"
#endif

#define IN
#define OUT

#define VOS_PAGE_SIZE 4096

enum {
  CMD_CHECK           = ((uint)0x14CD14DB),
  CMD_HOOK_FUNC       ,
  CMD_UNHOOK_FUNC     ,
  CMD_HIDE_PROCESS    ,
  CMD_PROTECT_PROCESS ,
};

// clang-format on

typedef struct
{

} vos_host_t;

#endif //VOS_VOS_H
