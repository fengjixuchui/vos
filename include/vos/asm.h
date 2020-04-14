//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS2_ASM_H
#define VOS2_ASM_H

#include "vos/types.h"

typedef struct cpuid__ cpuid_t;

extern uint64 __readcr0 ();
extern uint64 __readcr3 ();
extern uint64 __readcr4 ();
extern void   __writecr0 (uint64 value);
extern void   __writecr3 (uint64 value);
extern void   __writecr4 (uint64 value);
extern void   __cpuid (cpuid_t* out, uint64 id);
extern uint64 __rdmsr (uint64 id);
extern void   __wrmsr (uint64 id, uint64 value);
extern void   __vmptrld ();
extern void   __vmptrst ();
extern void   __vmclear ();
extern void   __vmread ();
extern void   __vmwrite ();
extern void   __vmlaunch ();
extern void   __vmresume ();
extern void   __vmxoff ();
extern void   __vmxon (uint64 paddr);
extern void   __invept ();
extern void   __invvpid ();
extern void   __vmcall ();
extern void   __vmfunc ();

#endif //VOS2_ASM_H
