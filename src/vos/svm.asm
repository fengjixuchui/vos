%include "defs.asm"

bits 64

global __vmrun
global __vmsave
global __vmload

__vmrun:
  mov rax, argv0
  vmrun
  ret

__vmsave:
  vmsave
  ret

__vmload:
  vmload
  ret


__vmmcall:
  vmmcall
  ret
