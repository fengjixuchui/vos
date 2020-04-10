
%ifndef VOS_DEFS
%define VOS_DEFS

%define BOCHS_MAGIC_BREAK xchg bx, bx

%define _LOADER_SEGMENT 0x0800      ; 内核程序段基址.
%define _LOADER_OFFSET  0x0000      ; 内核程序段偏移.

%define _PML4_BASE_                0x00010000
%define _PDP_BASE_                 0x00011000
%define _PD_BASE_                  0x00012000
%define _PT_BASE_                  0x00013000
%define __VOS_PA__                 0x00014000

%define uint8  db
%define uint16 dw
%define uint32 dd
%define uint64 dq

%define PR 1
%define GDT_BASE(x)                           (((x & 0x00ffffff) << 16) | ((x & 0xff000000) << 32))
%define GDT_LIMIT(x)                          (((x >> 12) & 0x0000ffff) | (((x >> 12) & 0x000f0000) << 32))
%define GDT_ACCESS(AC, RW, DC, EXEC, S, PRL)  (((AC & 1) << 40) | ((RW & 1) << 41) | ((DC & 1) << 42) | ((EXEC & 1) << 43) | ((S & 1) << 44) | ((PRL & 0b11) << 45) | ((PR & 1) << 47))
%define SZ 1
%define GDT_FLAGS_32(GR)                      (((GR & 1) << 55) | (1 << 54))
%define GDT_FLAGS_64(GR)                      (((GR & 1) << 55) | ((1) << 53))

; See : CONTROL REGISTERS
%define CR0_PE (1 << 0)
%define CR0_MP (1 << 1)
%define CR0_EM (1 << 2)
%define CR0_TS (1 << 3)
%define CR0_ET (1 << 4)
%define CR0_NE (1 << 5)
%define CR0_WP (1 << 16)
%define CR0_AM (1 << 18)
%define CR0_NW (1 << 29)
%define CR0_CD (1 << 30)
%define CR0_PG (1 << 31)

%define CR3_PDB_32(x) ((x & 0x000fffff) << 20)
%define CR3_PDB_64(x) ((x & 0x000fffffffffffff) << 20)

%define CR4_VME  (1 << 0)   ; Virtual-8086 Mode Extensions (bit 0 of CR4)
%define CR4_PVI  (1 << 1)
%define CR4_TSD  (1 << 2)
%define CR4_DE   (1 << 3)
%define CR4_PSE  (1 << 4)
%define CR4_PAE  (1 << 5)
%define CR4_MCE  (1 << 6)
%define CR4_PGE  (1 << 7)
%define CR4_PCE  (1 << 8)
%define CR4_UMIP (1 << 11)
%define CR4_VMXE (1 << 13)  ;  Enables VMX operation when set. See Chapter 23, “Introduction to Virtual Machine Extensions.”
%define CR4_SMXE (1 << 14)
%define CR4_SMEP (1 << 20)
%define CR4_SMAP (1 << 21)
%define CR4_PKE  (1 << 22)

%define IA32_EFER_SCE (1 << 0)
%define IA32_EFER_LME (1 << 8)
%define IA32_EFER_LMA (1 << 10)
%define IA32_EFER_NXE (1 << 11)

%define IA32_FEATURE_CONTROL 0x3A
%define IA32_VMX_BASIC       0x480

%if __BITS__ == 64
%define __REG_BP rbp
%define __REG_SP rsp
%else
%define __REG_BP ebp
%define __REG_SP esp
%endif

%define __STEP                (__BITS__ / 8)
%define __ARGS_SIZE(N)        (N * __STEP)
%define __ARG(INDEX)          [__REG_SP + (INDEX + 1) * __STEP]
%define __STACK_CLEAR(N)      add __REG_SP, __ARGS_SIZE(N)

struc vos_t
.vmx_host   resb 4096
.vendor     resb 12       ; CPU 提供商
.cpuid      resb 16
.terminal_x resb 1
.terminal_y resb 1
endstruc

struc cpuid_t
.eax resb 4
.ebx resb 4
.ecx resb 4
.edx resb 4
endstruc

%endif ; VOS_DEFS

