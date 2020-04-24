%ifndef VOS_X86_64
%define VOS_X86_64

%include "defs.asm"

bits 64

global __read_cr0
global __write_cr0
global __read_cr3
global __write_cr3
global __read_cr4
global __write_cr4
global __cpuid
global __read_msr
global __write_msr
global __rflags
global __eflags
global __flags

global __read_access_rights
global __read_es
global __read_cs
global __read_ss
global __read_ds
global __read_fs
global __read_gs
global __read_tr
global __read_gdtr
global __read_ldtr
global __read_idtr

global __clgi
global __stgi

__read_access_rights:
  lar rax, argv0
  ret

__read_cr0:
  mov rax, cr0
  ret

__write_cr0:
  mov rax, argv0
  mov cr0, rax
  ret

__read_cr1:
  mov rax, cr1
  ret

__write_cr1:
  mov rax, argv0
  mov cr1, rax
  ret

__read_cr2:
  mov rax, cr2
  ret

__write_cr2:
  mov rax, argv0
  mov cr2, rax
  ret

__read_cr3:
  mov rax, cr3
  ret

__write_cr3:
  mov rax, argv0
  mov cr3, rax
  ret

__read_cr4:
  mov rax, cr4
  ret

__write_cr4:
  mov rax, argv0
  mov cr4, rax
  ret

struc cpuid_t
.eax resb 4
.ebx resb 4
.ecx resb 4
.edx resb 4
endstruc
; Table 3-8. Information Returned by CPUID Instruction
; void __fastcall f (struct cpuid_t*, int)
__cpuid:
  mov rax, argv1

  cpuid

  mov dword [argv0 + cpuid_t.eax], eax
  mov dword [argv0 + cpuid_t.ebx], ebx
  mov dword [argv0 + cpuid_t.ecx], ecx
  mov dword [argv0 + cpuid_t.edx], edx

  ret

__read_msr:
  mov rcx, argv0
  rdmsr
  shl rdx, 32
  or rax, rdx        ; merge to uint64
  ret

__write_msr:
  mov rcx, argv0
  mov rax, argv1     ; low part
  mov rdx, argv1
  shr rdx, 32        ; high part
  wrmsr
  ret

__rflags:
__eflags:
__flags:
  pushfq
  pop rax
  ret

__read_es:
  mov rax, es
  ret

__read_cs:
  mov rax, cs
  ret

__read_ss:
  mov rax, ss
  ret

__read_ds:
  mov rax, ds
  ret

__read_fs:
  mov rax, fs
  ret

__read_gs:
  mov rax, gs
  ret

__read_tr:
  push 0
  str [rsp]
  pop rax
  ret

__read_gdtr:
  sgdt [argv0]
  ret

__read_ldtr:
  sldt [argv0]
  ret

__read_idtr:
  sidt [argv0]
  ret

__clgi:
  clgi
  ret

__stgi:
  stgi
  ret

%endif ; VOS_X86_64
