
bits 64
extern x86_64_main

x86_64_entry:
  call x86_64_main
  jmp $
