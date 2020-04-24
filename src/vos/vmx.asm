%include "defs.asm"

bits 64

extern VmmVmExitHandler

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
.r8     resb 8
.r9     resb 8
.r10    resb 8
.r11    resb 8
.r12    resb 8
.r13    resb 8
.r14    resb 8
.r15    resb 8
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
  mov [rsp + VMExitContext.r8], r8
  mov [rsp + VMExitContext.r9], r9
  mov [rsp + VMExitContext.r10], r10
  mov [rsp + VMExitContext.r11], r11
  mov [rsp + VMExitContext.r12], r12
  mov [rsp + VMExitContext.r13], r13
  mov [rsp + VMExitContext.r14], r14
  mov [rsp + VMExitContext.r15], r15

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
  mov r8,  [rsp + VMExitContext.r8]
  mov r9,  [rsp + VMExitContext.r9]
  mov r10, [rsp + VMExitContext.r10]
  mov r11, [rsp + VMExitContext.r11]
  mov r12, [rsp + VMExitContext.r12]
  mov r13, [rsp + VMExitContext.r13]
  mov r14, [rsp + VMExitContext.r14]
  mov r15, [rsp + VMExitContext.r15]

  add rsp, VMExitContext_size

  pop rbp

  call __vmresume        ; 这条命令执行成功将改变执行流程,不会返回.

  .fail:
  int 3
  ret

