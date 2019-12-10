
%include "defs.asm"

_LOADER_BASE_ equ (_LOADER_SEGMENT << 4 + _LOADER_OFFSET)
org _LOADER_BASE_

BEGIN:
jmp loader_ENTRY16

section GDT_32
; https://wiki.osdev.org/Global_Descriptor_Table
GDT_32_BEGIN:
dq 0
GDT_32_REAL_MODE_DATA    dq GDT_BASE(0x00000000)                 | GDT_LIMIT(0x000fffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_32_PROTECT_MODE_DATA dq GDT_BASE(_PROTECTED_MODE_CODE_BASE_) | GDT_LIMIT(0x000fffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_32_PROTECT_MODE_CODE dq GDT_BASE(_PROTECTED_MODE_CODE_BASE_) | GDT_LIMIT(0x000fffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
GDT_32_IA32E_MODE_DATA   dq GDT_BASE(_IA32E_MODE_CODE_BASE_)     | GDT_LIMIT(0x000fffff) | GDT_FLAGS_64(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_32_IA32E_MODE_CODE   dq GDT_BASE(_IA32E_MODE_CODE_BASE_)     | GDT_LIMIT(0x000fffff) | GDT_FLAGS_64(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
;                         dq GDT_BASE(0x10000000)                 | GDT_LIMIT(0x0fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
;                         dq GDT_BASE(0x20000000)                 | GDT_LIMIT(0x0fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 3)
;                         dq GDT_BASE(0x30000000)                 | GDT_LIMIT(0x0fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 3)
GDT_32_END:

GdtLen equ GDT_32_END - GDT_32_BEGIN

GDT_32_PTR:
limit32 dw GdtLen - 1
base32  dd GDT_32_BEGIN

section GDT_64

GDT_64_BEGIN:
dq 0
;LABEL_DESC_CODE64: dq 0x0020980000000000
;LABEL_DESC_DATA64: dq 0x0000920000000000
GDT_64_SELECTOR_1 dq GDT_BASE(0x00000000) | GDT_LIMIT(0x0fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
GDT_64_SELECTOR_2 dq GDT_BASE(0x10000000) | GDT_LIMIT(0x1fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_64_SELECTOR_3 dq GDT_BASE(0x20000000) | GDT_LIMIT(0x2fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 3)
GDT_64_SELECTOR_4 dq GDT_BASE(0x30000000) | GDT_LIMIT(0x3fffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 3)
GDT_64_END:

GdtLen64 equ $ - GDT_64_BEGIN

GDT_PTR_64:
limit64 dw GdtLen64 - 1
base64  dq GDT_64_BEGIN

section IDT_32
IDT_32_BEGIN:
times 0x50 dd 0
IDT_32_END:

IDT_32_PTR:
dw    IDT_32_END - IDT_32_BEGIN - 1
dd    IDT_32_BEGIN

section code
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
  lgdt [GDT_32_PTR]

;  db 0x66
;  lidt [IDT_32_PTR]

  mov eax, cr0
  or eax, CR0_PE           ; set PE flag.
  mov cr0, eax             ; enter protected mode.

  ; <<<<<<<<<<<<<<<<<<<<<<< 由于暂时不想实现文件系统, 这里先将保护模式的代码拷贝到32位代码区, 代码长度由 protected_mode.asm 的头开头 uint16 指定.
  mov ax, (GDT_32_PROTECT_MODE_DATA - GDT_32_BEGIN)
  mov fs, ax
  mov cx, [cs:END]
  __cp__:
    mov bx, cx
    add bx, END + 2 - 1
    mov di, cx
    add di, -1
    mov byte al, [cs:bx]
    mov byte [fs:di], al
    loop __cp__
  ; >>>>>>>>>>>>>>>>>>>>>>>

  mov ax, (GDT_32_PROTECT_MODE_CODE - GDT_32_BEGIN)
;  mov cs, ax
  mov ds, ax               ; reload segment register.
  mov es, ax               ; reload segment register.
  mov fs, ax               ; reload segment register.
  mov gs, ax               ; reload segment register.

  mov ax, (GDT_32_PROTECT_MODE_DATA - GDT_32_BEGIN)
  mov ss, ax               ; reload segment register. the ss register must be a pointer to data segment.

  BOCHS_MAGIC_BREAK

  ; 此时, 段的高13bits就是gdt表中的索引.
  jmp dword (GDT_32_PROTECT_MODE_CODE - GDT_32_BEGIN):0 ; jump to 32 bit code.

  mov eax, cr0
  and al, ~(CR0_PE)        ; clear PE flag.
  mov cr0, eax             ; exit protected mode.

  sti


  ret

END:
