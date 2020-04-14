%ifndef VOS_X86_64
%define VOS_X86_64

%include "defs.asm"

bits 64

%define fastcall_argv0 rdi
%define fastcall_argv1 rsi

global __readcr0
global __writecr0
global __readcr3
global __writecr3
global __readcr4
global __writecr4
global __cpuid
global __rdmsr
global __wrmsr
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

; uint64 ()
__readcr0:
  mov rax, cr0
  ret

; void (uint64)
__writecr0:
  mov rax, fastcall_argv0  ; output pointer
  mov cr0, rax
  ret

; uint64 ()
__readcr3:
  mov rax, cr3
  ret

; void (uint64)
__writecr3:
  mov rax, fastcall_argv0  ; output pointer
  mov cr3, rax
  ret

; uint64 ()
__readcr4:
  mov rax, cr4
  ret

; void (uint64)
__writecr4:
  mov rax, fastcall_argv0  ; output pointer
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

  mov dword [fastcall_argv0 + cpuid_t.eax], eax
  mov dword [fastcall_argv0 + cpuid_t.ebx], ebx
  mov dword [fastcall_argv0 + cpuid_t.ecx], ecx
  mov dword [fastcall_argv0 + cpuid_t.edx], edx

  ret

; uint64 __fastcall f(uint64 id)
__rdmsr:
  mov rcx, fastcall_argv0
  rdmsr
  shl rdx, 32
  or rax, rdx           ; merge to uint64
  ret

__wrmsr:
; void __fastcall f(uint64 id, uint64 value)
  mov rcx, fastcall_argv0
  mov rax, fastcall_argv1     ; low part
  mov rdx, fastcall_argv1
  shr rdx, 32                 ; high part
  wrmsr
  ret

__vmptrld:
  ;vmptrld
  ret

__vmptrst:
  ;vmptrst
  ret

__vmclear:
  ;vmclear
  ret

__vmread:
  ;vmread
  ret

__vmwrite:
  ;vmwrite
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
  mov rax, fastcall_argv0
  vmxon [rax]

__invept:
  invept rax, [fastcall_argv0]
  ret

__invvpid:
  invvpid rax, [fastcall_argv0]
  ret

__vmcall:
  vmcall
  ret

__vmfunc:
  vmfunc
  ret


%endif ; VOS_X86_64
