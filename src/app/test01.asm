
bits 64

mov rdi, 0x14CD14DB
mov rsi, 0x1000
mov rdx, 0x2000
vmcall

jmp $

