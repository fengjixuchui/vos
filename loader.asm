
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


section GDT_64

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
BOCHS_MAGIC_BREAK
  db 0x66
  lgdt [GDT_32_PTR - _LOADER_SEGMENT]

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
  BOCHS_MAGIC_BREAK

;  mov dword [0x90000], 0x91007
;  mov dword [0x90800], 0x91007
;  mov dword [0x91000], 0x92007
;  mov dword [0x92000], 0x000083
;  mov dword [0x92008], 0x200083
;  mov dword [0x92010], 0x400083
;  mov dword [0x92018], 0x600083
;  mov dword [0x92020], 0x800083
;  mov dword [0x92028], 0xa00083
  mov dword [_PML4_BASE_], _PDP_BASE_ | 7
  mov dword [_PDP_BASE_], _PD_BASE_ | 7
  mov dword [_PD_BASE_], _PT_BASE_ | 7

  ; 暂时只设置0x200000以内的内存分页, 512*4096 = 2097152 = 0x200000
  ; 0x200000以内的虚拟内存地址就直接等于物理地址
  mov ecx, 512
  .__pt_set__
  mov eax, ecx
  dec eax
  mov ebx, 8          ; 每项是uint64
  mul ebx             ; 得出表项索引
  mov edi, eax

  mov eax, ecx
  dec eax
  mov ebx, 4096       ; 每个页4k大小
  mul ebx             ; 得出表项中的值,即物理内存页的起始地址.

  or eax, 7   ; read and write
  mov dword [_PT_BASE_ + edi], eax

  loop .__pt_set__

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

%include "x86_64.asm"
