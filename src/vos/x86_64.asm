%ifndef VOS_X86_64
%define VOS_X86_64

%include "defs.asm"

bits 64

%ifidn __OUTPUT_FORMAT__, elf64
  %define fastcall_argv0 rdi
  %define fastcall_argv1 rsi
  %define argv0 fastcall_argv0
  %define argv1 fastcall_argv1
%else
  %error "目前只支持elf64格式的fastcall"
%endif

global __read_cr0
global __write_cr0
global __read_cr3
global __write_cr3
global __read_cr4
global __write_cr4
global __cpuid
global __rdmsr
global __wrmsr
global __rflags
global __eflags
global __flags

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
global __vmptrld
global __vmptrst
global __vmclear
global __vmread
global __vmwrite
global __vmlaunch
global __vmresume
global __vmxoff
global __vmxon
global __invept
global __invvpid
global __vmcall
global __vmfunc

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

; struct cpuid_t {
;   uint32 eax, ebx, ecx, edx
; }
; Table 3-8. Information Returned by CPUID Instruction
; void __fastcall f (struct cpuid_t*, int)
__cpuid:
  mov rax, fastcall_argv1

  cpuid

  mov dword [argv0 + cpuid_t.eax], eax
  mov dword [argv0 + cpuid_t.ebx], ebx
  mov dword [argv0 + cpuid_t.ecx], ecx
  mov dword [argv0 + cpuid_t.edx], edx

  ret

__rdmsr:
  mov rcx, argv0
  rdmsr
  shl rdx, 32
  or rax, rdx        ; merge to uint64
  ret

__wrmsr:
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

;;;;;;;;;;vmx

__clgi:
  clgi
  ret

__stgi:
  stgi
  ret

__vmptrld:
  mov rax, argv0
  vmptrld [rax]
  ret

__vmptrst:
  vmptrst [argv0]
  ret

__vmclear:
  mov rax, argv0
  vmclear [rax]
  ret

__vmread:
  vmread rax, argv0
  ret

__vmwrite:
  vmwrite argv0, fastcall_argv1
  ret

__vmlaunch:
  vmlaunch
  ret

__vmresume:
  vmresume
  ret

__vmxoff:
  vmxoff
  ret

__vmxon:
  mov rax, argv0
  vmxon [rax]
  ret

__invept:
  invept rax, [argv0]
  ret

__invvpid:
  invvpid rax, [argv0]
  ret

__vmcall:
  vmcall
  ret

__vmfunc:
  vmfunc
  ret


%endif ; VOS_X86_64
