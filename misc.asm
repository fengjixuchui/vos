%ifndef VOS_MISC
%define VOS_MISC


%include "defs.asm"


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

%endif ; VOS_MISC
