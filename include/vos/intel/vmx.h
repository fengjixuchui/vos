//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_VMX_H
#define VOS_VMX_H

// clang-format off

// See : CONTROL REGISTERS
#define CR0_PE (1 << 0)
#define CR0_MP (1 << 1)
#define CR0_EM (1 << 2)
#define CR0_TS (1 << 3)
#define CR0_ET (1 << 4)
#define CR0_NE (1 << 5)
#define CR0_WP (1 << 16)
#define CR0_AM (1 << 18)
#define CR0_NW (1 << 29)
#define CR0_CD (1 << 30)
#define CR0_PG (1 << 31)

#define CR3_PDB_32(x) ((x & 0x000fffff) << 20)
#define CR3_PDB_64(x) ((x & 0x000fffffffffffff) << 20)

#define CR4_VME  (1 << 0)   // Virtual-8086 Mode Extensions (bit 0 of CR4)
#define CR4_PVI  (1 << 1)
#define CR4_TSD  (1 << 2)
#define CR4_DE   (1 << 3)
#define CR4_PSE  (1 << 4)
#define CR4_PAE  (1 << 5)
#define CR4_MCE  (1 << 6)
#define CR4_PGE  (1 << 7)
#define CR4_PCE  (1 << 8)
#define CR4_UMIP (1 << 11)
#define CR4_VMXE (1 << 13)  //  Enables VMX operation when set. See Chapter 23, “Introduction to Virtual Machine Extensions.”
#define CR4_SMXE (1 << 14)
#define CR4_SMEP (1 << 20)
#define CR4_SMAP (1 << 21)
#define CR4_PKE  (1 << 22)

#define IA32_FEATURE_CONTROL_LOCK_BIT          0x1
#define IA32_FEATURE_CONTROL_VMX_ENABLE_BIT    0x4

#define IA32_FEATURE_CONTROL 0x3A
#define IA32_VMX_BASIC       0x480
// clang-format on

#endif //VOS_VMX_H
