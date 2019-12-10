
%include 'defs.asm'

org _PROTECTED_MODE_CODE_BASE_
bits 32

BEGIN:
uint16 CODE_LEN

jmp ENTRY
nop

ENTRY:
  mov eax, 0
  jmp $

END:
CODE_LEN equ $ - BEGIN - 2
