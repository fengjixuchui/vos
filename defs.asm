
%define BOCHS_MAGIC_BREAK xchg bx, bx

_VIDEO_PA       equ 0xb800      ; 实模式中显存的段地址.
_KERNEL_SEGMENT equ 0x1000      ; 内核程序段基址.
_KERNEL_OFFSET  equ 0x0000      ; 内核程序段偏移.

%define uint8  db
%define uint16 dw
%define uint32 dd
%define uint64 dq

