%ifndef VOS_X86_64
%define VOS_X86_64

%include "defs.asm"

bits 64

%define elf64_fastcall_argv0 rdi
%define elf64_fastcall_argv1 rsi
%define elf64_fastcall_argv2 rdx
%define elf64_fastcall_argv3 rcx

%define win64_fastcall_argv0 rcx
%define win64_fastcall_argv1 rdx
%define win64_fastcall_argv2 r8
%define win64_fastcall_argv3 r9

%ifidn __OUTPUT_FORMAT__, elf64
  %define argv0 elf64_fastcall_argv0
  %define argv1 elf64_fastcall_argv1
  %define argv2 elf64_fastcall_argv2
  %define argv3 elf64_fastcall_argv3
%elifidn __OUTPUT_FORMAT__, win64
  %define argv0 win64_fastcall_argv0
  %define argv1 win64_fastcall_argv1
  %define argv2 win64_fastcall_argv2
  %define argv3 win64_fastcall_argv3
%else
  %error "目前只支持elf64格式的fastcall"
%endif

extern VmmVmExitHandler

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
global __vmexit_handler

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
  vmwrite argv0, argv1
  ret

; 若执行成功,代码执行流程将改变,不会返回.
__vmlaunch:
  vmlaunch
  ret

; 若执行成功,代码执行流程将改变,不会返回.
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

struc VMExitContext
.ax     resb 8
.bx     resb 8
.cx     resb 8
.dx     resb 8
.si     resb 8
.di     resb 8
.reason resb 8
endstruc

%define VMX_VMCS_GUEST_RIP                                      0x681e
%define VMX_VMCS32_RO_EXIT_REASON                               0x4402
%define VMX_VMCS32_RO_EXIT_INSTR_LENGTH                         0x440c

__vmexit_handler:
  push rbp
  mov rbp, rsp

  sub rsp, VMExitContext_size

  mov [rsp + VMExitContext.ax], rax
  mov [rsp + VMExitContext.bx], rbx
  mov [rsp + VMExitContext.cx], rcx
  mov [rsp + VMExitContext.dx], rdx
  mov [rsp + VMExitContext.si], rsi
  mov [rsp + VMExitContext.di], rdi

  mov argv0, VMX_VMCS32_RO_EXIT_REASON
  vmread rax, argv0
  mov [rsp + VMExitContext.reason], rax

  mov argv0, rsp  ; context

  call VmmVmExitHandler

  cmp rax, 0
  jne .fail      ; 判断是否执行失败.

  mov argv0, VMX_VMCS_GUEST_RIP
  vmread rax, argv0

  mov argv0, VMX_VMCS32_RO_EXIT_INSTR_LENGTH
  vmread rbx, argv0
  add rbx, rax

  mov argv0, VMX_VMCS_GUEST_RIP
  vmwrite argv0, rbx

  mov rax, [rsp + VMExitContext.ax]
  mov rbx, [rsp + VMExitContext.bx]
  mov rcx, [rsp + VMExitContext.cx]
  mov rdx, [rsp + VMExitContext.dx]
  mov rsi, [rsp + VMExitContext.si]
  mov rdi, [rsp + VMExitContext.di]

  add rsp, VMExitContext_size

  pop rbp

  call __vmresume        ; 这条命令执行成功将改变执行流程,不会返回.

  .fail:
  int 3
  ret

%endif ; VOS_X86_64
