//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_ASM_H
#define VOS_ASM_H

#include "vos/types.h"

typedef struct cpuid__ cpuid_t;
typedef struct gdtr__  gdtr_t, ldtr_t, idtr_t;

extern uint64 __read_access_rights ();
extern uint64 __read_cr0 ();
extern uint64 __read_cr3 ();
extern uint64 __read_cr4 ();
extern void   __write_cr0 (uint64 value);
extern void   __write_cr3 (uint64 value);
extern void   __write_cr4 (uint64 value);
extern void   __cpuid (cpuid_t* out, uint64 id);
extern uint64 __read_msr (uint64 id);
extern void   __write_msr (uint64 id, uint64 value);
extern uint64 __rflags ();
extern uint64 __eflags ();
extern uint64 __flags ();

extern uint64 __read_es ();
extern uint64 __read_cs ();
extern uint64 __read_ss ();
extern uint64 __read_ds ();
extern uint64 __read_fs ();
extern uint64 __read_gs ();
extern uint64 __read_tr ();
extern void   __read_gdtr (gdtr_t* gdtr);
extern void   __read_ldtr (ldtr_t* ldtr);
extern void   __read_idtr (idtr_t* idtr);

extern void   __clgi ();
extern void   __stgi ();
extern void   __vmptrld ();
extern void   __vmptrst ();
extern void   __vmclear (uint64 paddr);
extern uint64 __vmread (uint64 field);
extern void   __vmwrite (uint64 field, uint64 value);
extern void   __vmlaunch ();
extern void   __vmresume ();
extern void   __vmxoff ();
extern void   __vmxon (uint64 paddr);
extern void   __invept ();
extern void   __invvpid ();
extern void   __vmcall (uint64 cmd, uint64 arg0, uint64 arg1);
extern void   __vmfunc ();
extern void   __vmexit_handler ();

#endif //VOS_ASM_H
