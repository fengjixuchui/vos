//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_VOS_H
#define VOS_VOS_H

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

#define VOS_PAGE_SIZE 4096


#define CMD_CHECK           ((uint32)0x14CD14DB)
#define CMD_HOOK_FUNC       ((uint32)0x14CD14DC)
#define CMD_HIDE_PROCESS    ((uint32)0x14CD14DD)
#define CMD_PROTECT_PROCESS ((uint32)0x14CD14DE)


typedef struct VMExitContext
{
  uint64 rax;
  uint64 rbx;
  uint64 rcx;
  uint64 rdx;
  uint64 rsi;
  uint64 rdi;
  uint64 r8;
  uint64 r9;
  uint64 r10;
  uint64 r11;
  uint64 r12;
  uint64 r13;
  uint64 r14;
  uint64 r15;
  uint64 reason;
} VMExitContext_t;

// clang-format on

#endif //VOS_VOS_H
