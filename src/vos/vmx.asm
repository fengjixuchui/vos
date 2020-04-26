%include "vos.asm"

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

%define VMX_VMCS_GUEST_RIP                                      0x681e
%define VMX_VMCS32_RO_EXIT_INSTR_LENGTH                         0x440c

__vmexit_handler:
  push rbp
  mov rbp, rsp

  sub rsp, VmxVMExitContext_size

  mov [rsp + VmxVMExitContext.ax], rax
  mov [rsp + VmxVMExitContext.bx], rbx
  mov [rsp + VmxVMExitContext.cx], rcx
  mov [rsp + VmxVMExitContext.dx], rdx
  mov [rsp + VmxVMExitContext.si], rsi
  mov [rsp + VmxVMExitContext.di], rdi
  mov [rsp + VmxVMExitContext.r8], r8
  mov [rsp + VmxVMExitContext.r9], r9
  mov [rsp + VmxVMExitContext.r10], r10
  mov [rsp + VmxVMExitContext.r11], r11
  mov [rsp + VmxVMExitContext.r12], r12
  mov [rsp + VmxVMExitContext.r13], r13
  mov [rsp + VmxVMExitContext.r14], r14
  mov [rsp + VmxVMExitContext.r15], r15

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

  mov rax, [rsp + VmxVMExitContext.ax]
  mov rbx, [rsp + VmxVMExitContext.bx]
  mov rcx, [rsp + VmxVMExitContext.cx]
  mov rdx, [rsp + VmxVMExitContext.dx]
  mov rsi, [rsp + VmxVMExitContext.si]
  mov rdi, [rsp + VmxVMExitContext.di]
  mov r8,  [rsp + VmxVMExitContext.r8]
  mov r9,  [rsp + VmxVMExitContext.r9]
  mov r10, [rsp + VmxVMExitContext.r10]
  mov r11, [rsp + VmxVMExitContext.r11]
  mov r12, [rsp + VmxVMExitContext.r12]
  mov r13, [rsp + VmxVMExitContext.r13]
  mov r14, [rsp + VmxVMExitContext.r14]
  mov r15, [rsp + VmxVMExitContext.r15]

  add rsp, VmxVMExitContext_size

  pop rbp

  call __vmresume        ; 这条命令执行成功将改变执行流程,不会返回.

  .fail:
  int 3
  ret

