%ifndef VOS_MISC
%define VOS_MISC

bits 64
%include "defs.asm"

; int (const char* a, const char* b, int len)
__memcmp:
  __FUNC_BEGIN_64
  mov edi, __ARG(0)
  mov esi, __ARG(1)
  mov ecx, __ARG(2)

  .loop:
    mov edx, ecx
    dec edx

    mov al, [edi + edx]
    mov bl, [esi + edx]
    cmp al, bl
    jne .error
    loop .loop
  __FUNC_RETURN_64(0)
  .error:
  __FUNC_RETURN_64(-1)

  __FUNC_END_64

; int (char* dst, uint8 v, int len)
__memset:
  __FUNC_BEGIN_64
  mov edi, __ARG(0)  ; dst
  mov al,  __ARG(1)  ; v
  mov ecx, __ARG(2)  ; len

  .loop:
    mov edx, ecx
    dec edx

    mov [edi + edx], al
  loop .loop
  mov eax, 0
  __FUNC_END_64

; int (char* dst, const char* src, int len)
__memcpy:
  __FUNC_BEGIN_64
  mov edi, __ARG(0)
  mov esi, __ARG(1)
  mov ecx, __ARG(2)

  .loop:
    mov edx, ecx
    dec edx

    mov bl, [esi + edx]
    mov [edi + edx], bl
  loop .loop
  mov eax, 0
  __FUNC_END_64

__putc:
  ret

; void (const char* str, uint64 len)
__puts:
  __FUNC_BEGIN_64

  mov rdi, 0xb8000
;  mov es, 0x0008    ; data segment
  mov rcx, 0              ; 初始化.

  mov byte r8, [__VOS_PA__ + vos_t.terminal_x]
  mov byte r9, [__VOS_PA__ + vos_t.terminal_y]
  mov rcx, __ARG(1)    ; len
  mov r10, __ARG(0)    ; str

  .loop:
    mov rax, r9
    mov rbx, 2
    mul rbx
    add rax, r8       ; y * 2 + x

    mov ah, 0b00000111    ; 左4bit背景RGB, 右4bit字符RGB
    mov al, [r10]         ; ascii
    mov word [ds:rdi], ax

    inc r10
    inc rdi
    inc rdi

    loop .loop

  __FUNC_END_64

%endif ; VOS_MISC
