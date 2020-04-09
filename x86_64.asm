%ifndef VOS_X86_64
%define VOS_X86_64

%include "vmx.asm"
%include "defs.asm"
%include "misc.asm"

bits 64

VENDOR_INTEL db "GenuineIntel"
VENDOR_AMD   db "AuthenticAMD"

__x86_64_ENTRYPOINT:

  push 0
  push __VOS_PA__ + vos_t.cpuid
  call __cpuid
  __STACK_CLEAR(2)

  ; 把vendor字符串在内存中构造出来
  mov eax, [__VOS_PA__ + vos_t.cpuid + cpuid_t.ebx]
  mov dword [__VOS_PA__ + vos_t.vendor + 0], eax
  mov eax, [__VOS_PA__ + vos_t.cpuid + cpuid_t.edx]
  mov dword [__VOS_PA__ + vos_t.vendor + 4], eax
  mov eax, [__VOS_PA__ + vos_t.cpuid + cpuid_t.ecx]
  mov dword [__VOS_PA__ + vos_t.vendor + 8], eax

  push 12
  push __VOS_PA__ + vos_t.vendor
  call __puts
  __STACK_CLEAR(2)

  push 12
  push __VOS_PA__ + vos_t.vendor
  push VENDOR_INTEL        ; 判断是否为Intel CPU
  call __memcmp
  __STACK_CLEAR(3)

  cmp eax, 0
  je __GenuineIntel_ENTRY

  push 12
  push __VOS_PA__ + vos_t.vendor
  push VENDOR_AMD          ; 判断是否为AMD CPU
  call __memcmp
  __STACK_CLEAR(3)

  cmp eax, 0
  je __AuthenticAMD_ENTRY

  jmp __Unknow_ENTRY

  jmp $
  ret

__GenuineIntel_ENTRY:

  call __check_vmx
  cmp rax, 0
  jne __check_vmx_failed

  call __check_ept
  cmp rax, 0
  jne __check_ept_failed

  call __start_vm
  cmp rax, 0
  jne __start_vm_failed

  call __stop_vm
  cmp rax, 0
  jne __stop_vm_failed

  ret

  EPT_FAILD DB "ept check failed"
  __check_ept_failed:
    push 16
    push EPT_FAILD
    call __puts
    __STACK_CLEAR(2)
    jmp $

  VMX_FAILD DB "vmx check failed"
  __check_vmx_failed:
    push 16
    push VMX_FAILD
    call __puts
    __STACK_CLEAR(2)
    jmp $

  STARTVM_FAILD DB "start vm failed"
  __start_vm_failed:
    push 15
    push STARTVM_FAILD
    call __puts
    __STACK_CLEAR(2)
    jmp $

  STOPVM_FAILD DB "stop vm failed"
  __stop_vm_failed:
    push 15
    push STOPVM_FAILD
    call __puts
    __STACK_CLEAR(2)
    jmp $

__AuthenticAMD_ENTRY:
  mov eax, 1
  ret

__Unknow_ENTRY:
  jmp $

; uint64 ()
__readcr0:
  mov rax, cr0
  ret

; void (uint64)
__writecr0:
  mov rax, __ARG(0)  ; output pointer
  mov cr0, rax
  ret

; uint64 ()
__readcr3:
  mov rax, cr3
  ret

; void (uint64)
__writecr3:
  mov rax, __ARG(0)  ; output pointer
  mov cr3, rax
  ret

; uint64 ()
__readcr4:
  mov rax, cr4
  ret

; void (uint64)
__writecr4:
  mov rax, __ARG(0)  ; output pointer
  mov cr4, rax
  ret

; struct cpuid_t {
;   uint32 eax, ebx, ecx, edx
; }
; Table 3-8. Information Returned by CPUID Instruction
; void (struct cpuid_t*, int)
__cpuid:

  mov esi, __ARG(0)  ; output pointer
  mov eax, __ARG(1)  ; function id
  cpuid

  mov dword [esi + cpuid_t.eax], eax
  mov dword [esi + cpuid_t.ebx], ebx
  mov dword [esi + cpuid_t.ecx], ecx
  mov dword [esi + cpuid_t.edx], edx

  ret

; uint64 krdmsr(uint64 id)
__rdmsr:
  mov rcx, __ARG(0)
  rdmsr
  ret

__wrmsr:
; void (uint64 id, uint64 value)
  mov rcx, __ARG(0)
  mov rax, __ARG(1)
  wrmsr
  ret

%endif ; VOS_X86_64
