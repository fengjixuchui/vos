
bits 32
%include "defs.asm"

extern loader_main

global enable_PAE
global setup_GDT64
global setup_PML4
global goto_IA32E

loader_entry:
  call loader_main
  jmp $

enable_PAE:
  __FUNC_BEGIN_32
  mov eax, cr4
  or  eax, CR4_PAE
  mov cr4, eax
  __FUNC_END_32

setup_PML4:
  __FUNC_BEGIN_32
  mov eax, __ARG(0)
  mov cr3, eax
  __FUNC_END_32

setup_GDT64:
  __FUNC_BEGIN_32
  mov eax, __ARG(0)
  db 0x66
  lgdt [eax]

  mov eax, 0x08
  mov ds, eax               ; reload segment register.
  mov es, eax               ; reload segment register.
  mov fs, eax               ; reload segment register.
  mov gs, eax               ; reload segment register.
  mov ss, eax               ; reload segment register. the ss register must be a pointer to data segment.

  __FUNC_END_32

goto_IA32E:
  mov ecx, 0C0000080h    ;IA32_EFER
  rdmsr
  or eax, IA32_EFER_LME
  wrmsr

  mov eax, cr0
  or eax, CR0_PE
  or eax, CR0_PG
  mov cr0, eax
  jmp dword 0x10:0x9000
