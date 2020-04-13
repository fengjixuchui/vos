
%include "defs.asm"

_LOADER_BASE_ equ (_LOADER_SEGMENT << 4 + _LOADER_OFFSET)
org _LOADER_BASE_

LOADER_BEGIN:
jmp loader_ENTRY16

;section GDT_32
; https://wiki.osdev.org/Global_Descriptor_Table
GDT_32_BEGIN:
dq 0
GDT_32_PROTECT_MODE_DATA dq GDT_BASE(0) | GDT_LIMIT(0xffffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_32_PROTECT_MODE_CODE dq GDT_BASE(0) | GDT_LIMIT(0xffffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
GDT_32_END:

GdtLen equ GDT_32_END - GDT_32_BEGIN

GDT_32_PTR:
limit32 dw GdtLen - 1
base32  dd GDT_32_BEGIN


;section GDT_64

GDT_64_BEGIN:
dq 0
GDT_64_IA32E_MODE_DATA dq GDT_FLAGS_64(0) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_64_IA32E_MODE_CODE dq GDT_FLAGS_64(0) | GDT_ACCESS(0, 0, 0, 1, 1, 0)
GDT_64_END:

GdtLen64 equ $ - GDT_64_BEGIN

GDT_PTR_64:
limit64 dw GdtLen64 - 1
base64  dq GDT_64_BEGIN

;section IDT_32
IDT_32_BEGIN:
times 0x50 dd 0
IDT_32_END:

IDT_32_PTR:
dw    IDT_32_END - IDT_32_BEGIN - 1
dd    IDT_32_BEGIN

;section code
bits 16

loader_ENTRY16:
  mov ax, _LOADER_SEGMENT
  mov ds, ax
  call open_A20_line

  jmp $

open_A20_line:
  ; https://wiki.osdev.org/A20_Line#Fast_A20_Gate
  push ax
  in   al, 92h
  or   al, 00000010b
  out  92h,  al
  pop  ax

;  BOCHS_MAGIC_BREAK

  cli

  db 0x66
  lgdt [GDT_32_PTR - _LOADER_BASE_]

;  db 0x66
;  lidt [IDT_32_PTR]

  mov eax, cr0
  or eax, CR0_PE           ; set PE flag.
  mov cr0, eax             ; enter protected mode.

  mov ax, (GDT_32_PROTECT_MODE_DATA - GDT_32_BEGIN)
  mov ds, ax               ; reload segment register.
  mov es, ax               ; reload segment register.
  mov fs, ax               ; reload segment register.
  mov gs, ax               ; reload segment register.
  mov ss, ax               ; reload segment register. the ss register must be a pointer to data segment.

;  BOCHS_MAGIC_BREAK

  ; 此时, 段的高13bits就是gdt表中的索引.
  jmp dword (GDT_32_PROTECT_MODE_CODE - GDT_32_BEGIN):PROTECTED_CODE ; jump to 32 bit code.

  mov eax, cr0
  and al, ~(CR0_PE)        ; clear PE flag.
  mov cr0, eax             ; exit protected mode.

  sti

  ret

PROTECTED_CODE:
bits 32

  push 4096
  push 0
  push _PML4_BASE_
  call __memset32
  __STACK_CLEAR(1)
  push _PDP_BASE_
  call __memset32
  __STACK_CLEAR(1)
  push _PD_BASE_
  call __memset32
  __STACK_CLEAR(1)
  push _PT_BASE_
  call __memset32
  __STACK_CLEAR(3)

  mov dword [_PML4_BASE_], _PDP_BASE_ | 7
  mov dword [_PDP_BASE_], _PD_BASE_ | 7

  ; 暂时只设置0x4000000以内的内存分页
  ; 0x4000000以内的虚拟内存地址就直接等于物理地址
;  BOCHS_MAGIC_BREAK
  mov esi, _PD_BASE_
  mov edi, _PT_BASE_
  mov ecx, 32            ; 初始化32个内存页, 32*512*4096 =0x4000000= 64MiB
  mov eax, 0
  .loop
    push eax
    mov eax, edi
    or eax, 7
    mov dword [esi], eax
    pop eax

    push 4096
    push 0
    push edi
    call __memset32
    __STACK_CLEAR(3)

    push eax
    push edi
    call __gen_pt

    pop edi
    pop eax

    inc eax
    add esi, 8
    add edi, 4096
    loop .loop

  db 0x66
  lgdt [GDT_PTR_64]

  mov eax, (GDT_64_IA32E_MODE_DATA - GDT_64_BEGIN)
  mov ds, eax               ; reload segment register.
  mov es, eax               ; reload segment register.
  mov fs, eax               ; reload segment register.
  mov gs, eax               ; reload segment register.
  mov ss, eax               ; reload segment register. the ss register must be a pointer to data segment.

;  BOCHS_MAGIC_BREAK

  mov eax, cr4
  or  eax, CR4_PAE
  mov cr4, eax

  mov eax, _PML4_BASE_
  mov cr3, eax
  
  mov ecx, 0C0000080h    ;IA32_EFER
  rdmsr
  or eax, IA32_EFER_LME
  wrmsr

  mov eax, cr0
  or eax, CR0_PE
  or eax, CR0_PG
  mov cr0, eax

  jmp dword (GDT_64_IA32E_MODE_CODE - GDT_64_BEGIN):__x86_64_ENTRYPOINT


; int (char* dst, uint8 v, int len)
__memset32:
  __FUNC_BEGIN_32
  mov edi, __ARG(0)  ; dst
  mov al,  __ARG(1)  ; v
  mov ecx, __ARG(2)  ; len

  .loop:
    mov edx, ecx
    dec edx

    mov [edi + edx], al
    loop .loop
  mov eax, 0
  __FUNC_END_32

; void (uint32 base, uint32 pageIndex)
__gen_pt:
  __FUNC_BEGIN_32
  mov eax, __ARG(1)
  mov ebx, 4096*512
  mul ebx
  mov edx, eax
  ; edx = 内存页的起始地址

  mov esi, __ARG(0)

  mov ecx, 512
  mov ebx, 0
  .loop

    mov eax, edx
    or eax, 7   ; read and write
    mov dword [esi], eax

    add esi, 8            ; 得出表项索引
    add edx, 4096     ; 每个页4k大小

    loop .loop
  __FUNC_END_32


%include "x86_64.asm"

times 0x1000 - ($ - $$) nop
