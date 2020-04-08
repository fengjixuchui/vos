
%include "defs.asm"

bits 64

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

