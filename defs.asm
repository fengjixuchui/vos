
%define BOCHS_MAGIC_BREAK xchg bx, bx

_VIDEO_PA       equ 0xb800      ; 实模式中显存的段地址.
_KERNEL_SEGMENT equ 0x1000      ; 内核程序段基址.
_KERNEL_OFFSET  equ 0x0000      ; 内核程序段偏移.

%define uint8  db
%define uint16 dw
%define uint32 dd
%define uint64 dq

%define PR 1
%define GDT32_BASE(x)                           (((x & 0x00ffffff) << 16) | ((x & 0xff000000) << 32))
%define GDT32_LIMIT(x)                          (((x >> 12) & 0x0000ffff) | (((x >> 12) & 0x000f0000) << 32))
%define GDT32_ACCESS(AC, RW, DC, EXEC, S, PRL)  (((AC & 1) << 40) | ((RW & 1) << 41) | ((DC & 1) << 42) | ((EXEC & 1) << 43) | ((S & 1) << 44) | ((PRL & 0b11) << 45) | ((PR & 1) << 47))
%define GDT32_FLAGS(GR, SZ)                     (((GR & 1) << 55) | ((SZ & 1) << 54))
