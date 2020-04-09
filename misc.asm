%ifndef VOS_MISC
%define VOS_MISC


%include "defs.asm"

bits 64

; int (const char* a, const char* b, int len)
__memcmp:
  mov edi, __ARG(0)
  mov esi, __ARG(1)
  mov ecx, __ARG(2)

  __l:
    mov edx, ecx
    dec edx

    mov al, [edi + edx]
    mov bl, [esi + edx]
    cmp al, bl
    jne __err
  loop __l
  mov eax, 0
  ret

  __err:
  mov eax, -1
  ret

; int (char* dst, uint8 v, int len)
__memset:
  mov edi, __ARG(0)  ; dst
  mov al,  __ARG(1)  ; v
  mov ecx, __ARG(2)  ; len

  __ll:
    mov edx, ecx
    dec edx

    mov [edi + edx], al
  loop __ll
  mov eax, 0
  ret

; int (char* dst, const char* src, int len)
__memcpy:
  mov edi, __ARG(0)
  mov esi, __ARG(1)
  mov ecx, __ARG(2)

  __lll:
    mov edx, ecx
    dec edx

    mov bl, [esi + edx]
    mov [edi + edx], bl
  loop __lll
  mov eax, 0
  ret

__putc:
  ret

; void (const char* str, uint64 len)
__puts:

  mov rdi, _VIDEO_PA
;  mov es, 0x0008    ; data segment
  mov rcx, 0              ; 初始化.

  mov byte r8, [__VOS_PA__ + vos_t.terminal_x]
  mov byte r9, [__VOS_PA__ + vos_t.terminal_y]
  mov rcx, __ARG(1)    ; len
  mov r10, __ARG(0)    ; str

  __puts__:
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

    loop __puts__

  __puts_end__:
    ret

%endif ; VOS_MISC
