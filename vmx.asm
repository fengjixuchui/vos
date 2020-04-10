
%include "defs.asm"

bits 64

__start_vm:

  push IA32_FEATURE_CONTROL
  call __rdmsr
  __STACK_CLEAR(1)
  or rax, 1 ; lock
  push rax
  push IA32_FEATURE_CONTROL
  call __wrmsr
  __STACK_CLEAR(2)

  BOCHS_MAGIC_BREAK
  push vos_t_size
  push 0
  push __VOS_PA__ + vos_t
  call __memset
  __STACK_CLEAR(3)

  push IA32_VMX_BASIC
  call __rdmsr
  __STACK_CLEAR(1)

  ; 设置vmx版本号
  mov dword [__VOS_PA__ + vos_t.vmx_host], eax

  shr rax, 32
  and rax, 0b1111111111111  ; get 44:32 bits, 这个值就是vmcs结构的大小.

  call __readcr0
  or rax, CR0_NE       ; 不知道开虚拟机为啥要设置这个标志位,看bochs源码里面会检测这个.
  push rax
  call __writecr0
  __STACK_CLEAR(1)

  call __readcr4
  or rax, CR4_VMXE
  push rax
  call __writecr4
  __STACK_CLEAR(1)

  push __VOS_PA__ + vos_t.vmx_host
  call __vmxon
  __STACK_CLEAR(1)
  ret

__stop_vm:
  call __vmxoff
  ret

__check_vmx:
  push 1
  push __VOS_PA__ + vos_t.cpuid
  call __cpuid
  __STACK_CLEAR(2)

  mov rax, [__VOS_PA__ + vos_t.cpuid + cpuid_t.ecx]
  and rax, ( 1 << 5 ) ; Virtual Machine Technology
  cmp rax, 0
  je __Unsupport      ; 不支持vt-x

  push IA32_FEATURE_CONTROL
  call __rdmsr
  __STACK_CLEAR(1)
  and rax, ( 1 << 2 ) ; vmxon
  cmp rax, 0
  je __Unsupport      ; 不支持vt-x
  mov rax, 0
  ret

  __Unsupport:
    mov rax, 1
    ret

__check_ept:
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
  mov rax, __ARG(0)
  vmxon [rax]
  ret

__invept:
  invept rax, __ARG(0)
  ret

__invvpid:
  invvpid rax, __ARG(0)
  ret

__vmcall:
  vmcall
  ret

__vmfunc:
  vmfunc
  ret

