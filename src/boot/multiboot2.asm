
%include "defs.asm"
extern x86_64_main


MULTIBOOT2_HEADER_MAGIC                   equ 0xE85250D6
; /*  This should be in %eax. */
MULTIBOOT2_BOOTLOADER_MAGIC               equ 0x36d76289

MULTIBOOT_ARCHITECTURE_I386               equ 0x00000000
MULTIBOOT_ARCHITECTURE_MIPS32             equ 0x00000004

MULTIBOOT_HEADER_TAG_END                  equ 0x0000
MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST  equ 0x0001
MULTIBOOT_HEADER_TAG_ADDRESS              equ 0x0002
MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS        equ 0x0003
MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS        equ 0x0004
MULTIBOOT_HEADER_TAG_FRAMEBUFFER          equ 0x0005
MULTIBOOT_HEADER_TAG_MODULE_ALIGN         equ 0x0006
MULTIBOOT_HEADER_TAG_EFI_BS               equ 0x0007
MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI32  equ 0x0008
MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS_EFI64  equ 0x0009
MULTIBOOT_HEADER_TAG_RELOCATABLE          equ 0x000A

MULTIBOOT_HEADER_TAG_OPTIONAL             equ 0x0001

bits 32

section .multiboot_header
align 8 ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#General-tag-structure
multiboot_header_length equ (multiboot_header_end - multiboot_header_begin)
multiboot_header_begin:
; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Header-layout
  dd MULTIBOOT2_HEADER_MAGIC
  dd MULTIBOOT_ARCHITECTURE_I386
  dd multiboot_header_length ; header length
  dd - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + multiboot_header_length)

  align 8 ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Console-header-tags
  multiboot_header_tag_console_flags_begin:
     ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#The-framebuffer-tag-of-Multiboot2-header
    dw MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS
    dw 0
    dd 12   ; size
    dd 1    ; console_flags
  multiboot_header_tag_console_flags_end:

  align 8 ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#General-tag-structure
  multiboot2_framebuffer_begin:
     ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#The-framebuffer-tag-of-Multiboot2-header
    dw MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    dw 0
    dd 20   ; size
    dd 80   ; width
    dd 25   ; height
    dd 0    ; depth
  multiboot2_framebuffer_end:

  align 8 ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#General-tag-structure
  multiboot_header_tag_entry_address_start:
    ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#The-entry-address-tag-of-Multiboot2-header
    dw MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    dw MULTIBOOT_HEADER_TAG_OPTIONAL
    dd 12
    dd multiboot_entry
  multiboot_header_tag_entry_address_end:

  align 8 ; https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#General-tag-structure
  multiboot_header_tag_end_begin:
    ; Tags are terminated by a tag of type ‘0’ and size ‘8’.
    dw MULTIBOOT_HEADER_TAG_END
    dw 0    ; flags
    dd 8    ; size
  multiboot_header_tag_end_end:

multiboot_header_end:

section .text32
multiboot_entry:

  ;/*  Push the pointer to the Multiboot information structure. */
  push ebx
  ;/*  Push the magic value. */
  push eax

  jmp loader_entry





memset:
  .l1:
  mov byte [edi + ecx - 1], 0
  loop .l1
  ret

%define PDP_NUM 1
%define PD_NUM 1
%define PT_NUM 64
make_PML4:
  mov ecx, (0x1000 + (0x1000 * PDP_NUM) + (0x1000 * PDP_NUM * PD_NUM) + (0x1000 * PDP_NUM * PD_NUM * PT_NUM))
  mov edi, [.tablePtr]
  call memset

  mov edi, [.tablePtr]   ; tablePtr
  mov [.pml4], edi       ; pml4

  mov ecx, PDP_NUM
  mov ebx, 0             ; index
  .l1:
    push ecx
    push ebx

    mov eax, [.tablePtr]
    add eax, 0x1000
    mov edi, [.pml4]
    mov dword [.tablePtr], eax
    mov dword [.pdp], eax
    or eax, 7  ; 可读写
    mov dword [edi + ebx], eax

    mov ecx, PD_NUM
    mov ebx, 0           ; index
    .l2:
      push ecx
      push ebx

      mov eax, [.tablePtr]
      add eax, 0x1000
      mov edi, [.pdp]
      mov dword [.tablePtr], eax
      mov dword [.pd], eax
      or eax, 7  ; 可读写
      mov dword [edi + ebx], eax

      mov ecx, PT_NUM
      mov ebx, 0          ; index
      .l3:
        push ecx
        push ebx

        mov eax, [.tablePtr]
        add eax, 0x1000
        mov edi, [.pd]
        mov dword [.tablePtr], eax
        mov dword [.pt], eax
        or eax, 7  ; 可读写
        mov dword [edi + ebx], eax

        mov ecx, 512
        mov ebx, 0          ; index
        .l4:
         push ecx
         push ebx

         mov eax, [.pagePtr]
         mov edi, [.pt]
         or eax, 7  ; 可读写
         mov [edi + ebx], eax
         add dword [.pagePtr], 4096

         pop ebx
         pop ecx
         add ebx, 8

         loop .l4

        pop ebx
        pop ecx
        add ebx, 8

        loop .l3


      pop ebx
      pop ecx
      add ebx, 8

      dec ecx
      jnz .l2    ; 指令太多,loop短跳转不让用...
;      loop .l2

    pop ebx
    pop ecx
    add ebx, 8
    dec ecx
    jnz .l1      ; 指令太多,loop短跳转不让用...
  ret
  .tablePtr dd 0x100000
  .pagePtr  dd 0
  .pml4     dd 0
  .pdp      dd 0
  .pd       dd 0
  .pt       dd 0

loader_entry:

  .enable_PAE:
    mov eax, cr4
    or  eax, CR4_PAE
    mov cr4, eax

  call make_PML4

  mov eax, 0x100000
  .setup_PML4:
    mov cr3, eax

  %define fastcall_argv0 edi  ; elf64 fastcall
  %define fastcall_argv1 esi  ; elf64 fastcall

  ; 后面重新加载ss寄存器后,就不能pop了...
  pop fastcall_argv0        ;/*  Push the magic value. */
  pop fastcall_argv1        ;/*  Push the pointer to the Multiboot information structure. */

  mov eax, .gdt64_ptr
  .setup_GDT64:
    db 0x66
    lgdt [eax]

    mov eax, (.data64_ring0 - .gdt64)
    mov ds, eax               ; reload segment register.
    mov es, eax               ; reload segment register.
    mov fs, eax               ; reload segment register.
    mov gs, eax               ; reload segment register.
    mov ss, eax               ; reload segment register. the ss register must be a pointer to data segment.

  .goto_IA32E:
    mov ecx, 0C0000080h    ;IA32_EFER
    rdmsr
    or eax, IA32_EFER_LME
    wrmsr

    mov eax, cr0
    or eax, CR0_PE
    or eax, CR0_PG
    mov cr0, eax

    call dword (.code64_ring0 - .gdt64):x86_64_main

  ret

  align 8
  .gdt64:
    .reversed dq 0
;    .data_ring0   dq GDT_FLAGS_64(1) | GDT_LIMIT(0xffffffff) | GDT_ACCESS(0, 1, 0, 0, 1, 0) ; ring 3
;    .code_ring0   dq GDT_FLAGS_64(1) | GDT_LIMIT(0xffffffff) | GDT_ACCESS(0, 0, 0, 1, 1, 0) ; ring 3
;    .data32_ring0 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_SIZE | SEG_GRAN | SEG_PRIV(0) | SEG_DATA_RW ; ring 0
;    .code32_ring0 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_SIZE | SEG_GRAN | SEG_PRIV(0) | SEG_CODE_XR ; ring 0
    .data64_ring0 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_LONG | SEG_GRAN | SEG_PRIV(0) | SEG_DATA_RW ; ring 0
    .code64_ring0 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_LONG | SEG_GRAN | SEG_PRIV(0) | SEG_CODE_XR ; ring 0
;    .data_ring3   dq GDT_FLAGS_64(1) | GDT_LIMIT(0xffffffff) | GDT_ACCESS(0, 1, 0, 0, 1, 3) ; ring 3
;    .code_ring3   dq GDT_FLAGS_64(1) | GDT_LIMIT(0xffffffff) | GDT_ACCESS(0, 0, 0, 1, 1, 3) ; ring 3
;    .data32_ring3 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_SIZE | SEG_GRAN | SEG_PRIV(3) | SEG_DATA_RW ; ring 0
;    .code32_ring3 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_SIZE | SEG_GRAN | SEG_PRIV(3) | SEG_CODE_XR ; ring 0
    .data64_ring3 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_LONG | SEG_GRAN | SEG_PRIV(3) | SEG_DATA_RW ; ring 0
    .code64_ring3 dq SEG_LIMIT(0xffffffff) | SEG_DESCTYPE | SEG_PRES | SEG_LONG | SEG_GRAN | SEG_PRIV(3) | SEG_CODE_XR ; ring 0
  .gdt64_len equ $ - .gdt64

  .gdt64_ptr:
    .limit dw .gdt64_len - 1
    .base dq .gdt64

