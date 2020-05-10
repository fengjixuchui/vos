
%define BOCHS_MAGIC_BREAK xchg bx, bx

%define elf64_fastcall_argv0 rdi
%define elf64_fastcall_argv1 rsi
%define elf64_fastcall_argv2 rdx
%define elf64_fastcall_argv3 rcx

%define win64_fastcall_argv0 rcx
%define win64_fastcall_argv1 rdx
%define win64_fastcall_argv2 r8
%define win64_fastcall_argv3 r9

%define argv0 elf64_fastcall_argv0
%define argv1 elf64_fastcall_argv1
%define argv2 elf64_fastcall_argv2
%define argv3 elf64_fastcall_argv3


%define CMD_CHECK           (0x14CD14DB)
%define CMD_PUTS            (CMD_CHECK + 1)
%define CMD_HOOK_FUNC       (CMD_CHECK + 2)
%define CMD_UNHOOK_FUNC     (CMD_CHECK + 3)
%define CMD_HIDE_PROCESS    (CMD_CHECK + 4)
%define CMD_PROTECT_PROCESS (CMD_CHECK + 5)


;org 0x4000
bits 64

call get_rip
mov rbp, rax

mov argv0, CMD_CHECK
mov argv1, 0x1000
mov argv2, 0x2000
vmcall

call function_old

; 调用vmm提供的hook接口
mov argv0, CMD_HOOK_FUNC
mov rax, rbp
add rax, function_old
mov argv1,rax
mov rax, rbp
add rax, function_new
mov argv2, rax
vmcall

BOCHS_MAGIC_BREAK
; 再次调用被hook的函数,这里将会跳到function_new执行.
call function_old

jmp $

get_rip:
  mov rax, [rsp]  ; 取出返回地址
  sub rax, 5      ; 减去call指令的长度.
  ret

function_old:
  mov argv0, CMD_PUTS
  mov rax, rbp
  add rax, STR01
  mov argv1,rax
  vmcall
  ret

STR01 db 'hello! here is original function.',0
STR02 db 'hello! here is hook function.',0

times 4096 db 0xCC

function_new:
  mov argv0, CMD_PUTS
  mov rax, rbp
  add rax, STR02
  mov argv1,rax
  vmcall
  ret
