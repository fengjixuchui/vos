
%define BOCHS_MAGIC_BREAK xchg bx, bx

%define _VIDEO_PA       0xb800      ; 实模式中显存的段地址.
%define _LOADER_SEGMENT 0x1000      ; 内核程序段基址.
%define _LOADER_OFFSET  0x0000      ; 内核程序段偏移.

%define _PROTECTED_MODE_CODE_BASE_ 0x00100000
%define _IA32E_MODE_CODE_BASE_     0x00200000

%define uint8  db
%define uint16 dw
%define uint32 dd
%define uint64 dq

%define PR 1
%define GDT_BASE(x)                           (((x & 0x00ffffff) << 16) | ((x & 0xff000000) << 32))
%define GDT_LIMIT(x)                          (((x >> 12) & 0x0000ffff) | (((x >> 12) & 0x000f0000) << 32))
%define GDT_ACCESS(AC, RW, DC, EXEC, S, PRL)  (((AC & 1) << 40) | ((RW & 1) << 41) | ((DC & 1) << 42) | ((EXEC & 1) << 43) | ((S & 1) << 44) | ((PRL & 0b11) << 45) | ((PR & 1) << 47))
%define SZ 1
%define GDT_FLAGS_32(GR)                      (((GR & 1) << 55) | (1 << 54))
%define GDT_FLAGS_64(GR)                      (((GR & 1) << 55) | ((1) << 53))

; See : CONTROL REGISTERS
%define CR0_PE (1 << 0)
%define CR0_MP (1 << 1)
%define CR0_EM (1 << 1)
%define CR0_TS (1 << 1)
%define CR0_ET (1 << 1)
%define CR0_NE (1 << 1)
%define CR0_WP (1 << 16)
%define CR0_AM (1 << 18)
%define CR0_NW (1 << 29)
%define CR0_CD (1 << 30)
%define CR0_PG (1 << 31)

%define CR3_PDB_32(x) ((x & 0x000fffff) << 20)
%define CR3_PDB_64(x) ((x & 0x000fffffffffffff) << 20)


