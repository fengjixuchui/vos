//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_X86_64_H
#define VOS_X86_64_H

#include "vos/types.h"

typedef struct cpuid__ cpuid_t;
typedef struct gdtr__  gdtr_t, ldtr_t, idtr_t;

extern void __memset8 (uint8*, uint8, uint len);
extern void __memset16 (uint8*, uint16, uint len);
extern void __memset32 (uint8*, uint32, uint len);
extern void __memset64 (uint8*, uint64, uint len);

extern uint64 __read_access_rights (uint64 selector);
extern uint64 __read_cr0 ();
extern uint64 __read_cr2 ();
extern uint64 __read_cr3 ();
extern uint64 __read_cr4 ();
extern void   __write_cr0 (uint64 value);
extern void   __write_cr2 (uint64 value);
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
extern void   __write_gdtr (const gdtr_t* gdtr);
extern void   __write_ldtr (const ldtr_t* ldtr);
extern void   __write_idtr (const idtr_t* idtr);

extern void __clgi ();
extern void __stgi ();

void* make_guest_PML4E ();

#endif //VOS_X86_64_H
