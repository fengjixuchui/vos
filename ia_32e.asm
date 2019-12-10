
%include 'defs.asm'

org _IA32E_MODE_CODE_BASE_
bits 64

BEGIN:
uint16 CODE_LEN

jmp ENTRY
nop

ENTRY:
  mov rax, 0
  jmp $

END:
CODE_LEN equ $ - BEGIN - 2
