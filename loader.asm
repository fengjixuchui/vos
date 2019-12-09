
%include "defs.asm"

_KERNEL_BASE_ equ (_KERNEL_SEGMENT << 4 + _KERNEL_OFFSET)
org _KERNEL_BASE_
jmp loader_ENTRY16

SECTION GDT_32

LABEL_GDT32:  dd 0,0
LABEL_DESC_CODE32: dd 0x0000FFFF,0x00CF9A00
LABEL_DESC_DATA32: dd 0x0000FFFF,0x00CF9200
dq SEG_BASE(0x01234567) | SEG_LIMIT(0xffffffff) | SEG_ACCESS_AC | SEG_ACCESS_RING0 | SEG_ACCESS_S | SEG_FLAGS_SZ | SEG_ACCESS_PR | SEG_FLAGS_GR
dq SEG_BASE(0x01234567) | SEG_LIMIT(0xffffffff) | SEG_ACCESS_AC | SEG_ACCESS_RING0 | SEG_FLAGS_SZ | SEG_ACCESS_PR | SEG_FLAGS_GR

GdtLen equ $ - LABEL_GDT32

GDT_PTR_32:
limit32 dw GdtLen - 1
base32  dd LABEL_GDT32

SelectorCode32 equ LABEL_DESC_CODE32 - LABEL_GDT32
SelectorData32 equ LABEL_DESC_DATA32 - LABEL_GDT32

SECTION GDT_64

LABEL_GDT64:  dq 0x0000000000000000
LABEL_DESC_CODE64: dq 0x0020980000000000
LABEL_DESC_DATA64: dq 0x0000920000000000

GdtLen64 equ $ - LABEL_GDT64

GDT_PTR_64:
limit64 dw GdtLen64 - 1
base64  dq LABEL_GDT64

SelectorCode64 equ LABEL_DESC_CODE64 - LABEL_GDT64
SelectorData64 equ LABEL_DESC_DATA64 - LABEL_GDT64

section code
bits 16

loader_ENTRY16:
  mov ax, _KERNEL_SEGMENT
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

  BOCHS_MAGIC_BREAK

  cli

  db 0x66
  lgdt [GDT_PTR_32]

  mov eax, cr0
  or eax, 1
  mov cr0, eax             ; 进入保护模式.

  mov ax, SelectorData32
;  mov cs, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov eax, cr0
  and al, 11111110b
  mov cr0, eax             ; 退出保护模式.

  sti

  BOCHS_MAGIC_BREAK

  ret

