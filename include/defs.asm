
%ifndef VOS_DEFS
%define VOS_DEFS

%define BOCHS_MAGIC_BREAK xchg bx, bx

%define _LOADER_SEGMENT 0x0800      ; 内核程序段基址.
%define _LOADER_OFFSET  0x0000      ; 内核程序段偏移.

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
%define CR0_EM (1 << 2)
%define CR0_TS (1 << 3)
%define CR0_ET (1 << 4)
%define CR0_NE (1 << 5)
%define CR0_WP (1 << 16)
%define CR0_AM (1 << 18)
%define CR0_NW (1 << 29)
%define CR0_CD (1 << 30)
%define CR0_PG (1 << 31)

%define CR4_VME  (1 << 0)   ; Virtual-8086 Mode Extensions (bit 0 of CR4)
%define CR4_PVI  (1 << 1)
%define CR4_TSD  (1 << 2)
%define CR4_DE   (1 << 3)
%define CR4_PSE  (1 << 4)
%define CR4_PAE  (1 << 5)
%define CR4_MCE  (1 << 6)
%define CR4_PGE  (1 << 7)
%define CR4_PCE  (1 << 8)
%define CR4_UMIP (1 << 11)
%define CR4_VMXE (1 << 13)  ;  Enables VMX operation when set. See Chapter 23, “Introduction to Virtual Machine Extensions.”
%define CR4_SMXE (1 << 14)
%define CR4_SMEP (1 << 20)
%define CR4_SMAP (1 << 21)
%define CR4_PKE  (1 << 22)

%define IA32_EFER_SCE (1 << 0)
%define IA32_EFER_LME (1 << 8)
%define IA32_EFER_LMA (1 << 10)
%define IA32_EFER_NXE (1 << 11)

%define __STEP                (__BITS__ / 8)
%define __ARGS_SIZE(N)        (N * __STEP)
%define __ARG(INDEX)          [ebp + (INDEX + 2) * __STEP]
%define __STACK_CLEAR(N)      add esp, __ARGS_SIZE(N)

%macro __FUNC_BEGIN_16 0
.begin:
push bp
mov  bp, sp
push bx
push cx
push dx
push si
push di
%endmacro

%macro __FUNC_END_16 0
.end:
pop di
pop si
pop dx
pop cx
pop bx
pop bp
ret
%endmacro

%macro __FUNC_RETURN_16 1
mov ax, %1
jmp .end
%endmacro

%macro __FUNC_BEGIN_32 0
.begin:
push ebp
mov  ebp, esp
push ebx
push ecx
push edx
push esi
push edi
%endmacro

%macro __FUNC_END_32 0
.end:
pop edi
pop esi
pop edx
pop ecx
pop ebx
pop ebp
ret
%endmacro

%macro __FUNC_RETURN_32 1
mov eax, %1
jmp .end
%endmacro


%endif ; VOS_DEFS

