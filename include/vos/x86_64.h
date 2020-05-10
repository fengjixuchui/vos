//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_X86_64_H
#define VOS_X86_64_H

#include "vos/types.h"

typedef struct cpuid__ cpuid_t;
typedef struct gdtr__  gdtr_t, ldtr_t, idtr_t;

extern void __memset8 (void*, vos_uint8, vos_uint64 len);
extern void __memset16 (void*, vos_uint16, vos_uint64 len);
extern void __memset32 (void*, vos_uint32, vos_uint64 len);
extern void __memset64 (void*, vos_uint64, vos_uint64 len);

extern vos_uint64 __read_access_rights (vos_uint64 selector);
extern vos_uint64 __read_cr0 ();
extern vos_uint64 __read_cr2 ();
extern vos_uint64 __read_cr3 ();
extern vos_uint64 __read_cr4 ();
extern void       __write_cr0 (vos_uint64 value);
extern void       __write_cr2 (vos_uint64 value);
extern void       __write_cr3 (vos_uint64 value);
extern void       __write_cr4 (vos_uint64 value);
extern void       __cpuid (cpuid_t* out, vos_uint64 id);
extern vos_uint64 __read_msr (vos_uint64 id);
extern void       __write_msr (vos_uint64 id, vos_uint64 value);
extern vos_uint64 __rflags ();
extern vos_uint64 __eflags ();
extern vos_uint64 __flags ();

extern vos_uint64 __read_es ();
extern vos_uint64 __read_cs ();
extern vos_uint64 __read_ss ();
extern vos_uint64 __read_ds ();
extern vos_uint64 __read_fs ();
extern vos_uint64 __read_gs ();
extern vos_uint64 __read_tr ();
extern void       __read_gdtr (gdtr_t* gdtr);
extern void       __read_ldtr (ldtr_t* ldtr);
extern void       __read_idtr (idtr_t* idtr);
extern void       __write_gdtr (const gdtr_t* gdtr);
extern void       __write_ldtr (const ldtr_t* ldtr);
extern void       __write_idtr (const idtr_t* idtr);

extern void __clgi ();
extern void __stgi ();

void* make_guest_PML4 ();

#endif //VOS_X86_64_H
