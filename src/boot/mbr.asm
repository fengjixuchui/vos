
bits 16
%include "defs.asm"

_MBR_PA         equ 0x7c00      ; MBR被加载到的地址.
_DRIVE_NUM      equ 0b00000000  ; Drive number.
_LOADER_PA_ equ (_LOADER_SEGMENT << 4 + _LOADER_OFFSET)

org _MBR_PA

BootSector:
  jmp short ENTRY16
  nop                    ; 3 BYTEs    jump to executable code

;  db '0x7cc', 0, 0, 0    ; 8 BYTEs    OEM name and version
;
;  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_2.0_BPB
;  dw 512                 ; WORD       Bytes per logical sector
;  db 1                   ; BYTE       Logical sectors per cluster
;  dw 1                   ; WORD       Reserved logical sectors
;  db 2                   ; BYTE       Number of FATs
;  dw 224                 ; WORD       Root directory entries
;  dw 2880                ; WORD       Total logical sectors
;  db 0xf0                ; BYTE       Media descriptor
;  dw 9                   ; WORD       Logical sectors per FAT
;
;  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_3.31_BPB
;  dw 18                  ; WORD       Physical sectors per track (identical to DOS 3.0 BPB)
;  dw 2                   ; WORD       Number of heads (identical to DOS 3.0 BPB)
;  dd 0                   ; DWORD      Hidden sectors (incompatible with DOS 3.0 BPB)
;  dd 0                   ; DWORD      Large total logical sectors
;
;  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_4.0_EBPB
;  db _DRIVE_NUM          ; BYTE       Physical drive number (identical to DOS 3.4 EBPB)
;  db 0                   ; BYTE       Flags etc. (identical to DOS 3.4 EBPB)
;  db 0x29                ; BYTE       Extended boot signature (0x29 aka "4.1") (similar to DOS 3.4 EBPB and NTFS EBPB)
;  dd 0                   ; DWORD      Volume serial number (identical to DOS 3.4 EBPB)
;  db '0x7cc vos', 0, 0   ; 11 BYTEs   Volume label
;  db 'FAT12', 0, 0, 0    ; 8 BYTEs    File-system type

GDT_32_BEGIN:
dq 0
GDT_32_PROTECT_MODE_DATA dq GDT_BASE(0) | GDT_LIMIT(0xffffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 0, 1, 0)
GDT_32_PROTECT_MODE_CODE dq GDT_BASE(0) | GDT_LIMIT(0xffffffff) | GDT_FLAGS_32(1) | GDT_ACCESS(0, 1, 0, 1, 1, 0)
GDT_32_END:
GdtLen equ GDT_32_END - GDT_32_BEGIN

GDT_32_PTR:
limit32 dw GdtLen - 1
base32  dd GDT_32_BEGIN

ENTRY16:                 ; void boot(void)
  call init
  call work

init:                    ; void init(void)

  call reset_floppy

  ret

work:                    ; void work(void)
;  BOCHS_MAGIC_BREAK             ;

  push 60                ; uint8 count        读取扇区数量
  push 2                 ; uint8 sectorNum    扇区号
  push 0                 ; uint8 cylinder     磁道
  push 0                 ; uint8 head         磁头
  push _DRIVE_NUM        ; uint8 driveNum     磁盘
  push _LOADER_OFFSET    ; uint16 offset
  push _LOADER_SEGMENT   ; uint16 segment
  call read_floppy
  __STACK_CLEAR(7)

;  BOCHS_MAGIC_BREAK             ;

  call open_A20_line
  call switch_protected_mode

;  BOCHS_MAGIC_BREAK

  ; 此时, 段的高13bits就是gdt表中的索引.
  jmp dword GDT_32_PROTECT_MODE_CODE - GDT_32_BEGIN : _LOADER_PA_ ; jump to 32 bit code.

  jmp $

open_A20_line:
  ; https://wiki.osdev.org/A20_Line#Fast_A20_Gate
  push ax
  in   al, 92h
  or   al, 00000010b
  out  92h,  al
  pop  ax

  ret

switch_protected_mode:
  mov ax, 0
  mov ds, ax
  db 0x66
  lgdt [GDT_32_PTR]

  mov eax, cr0
  or eax, CR0_PE           ; set PE flag.
  mov cr0, eax             ; enter protected mode.

  mov ax, (GDT_32_PROTECT_MODE_DATA - GDT_32_BEGIN)
  mov ds, ax               ; reload segment register.
  mov es, ax               ; reload segment register.
  mov fs, ax               ; reload segment register.
  mov gs, ax               ; reload segment register.
  mov ss, ax               ; reload segment register. the ss register must be a pointer to data segment.
  ret

reset_floppy:            ; void reset_floppy(void)
  ; reset disk system, ref http://stanislavs.org/helppc/int_13-0.html
  xor ah, ah
  mov dl, 0
  int 0x13
  ret

; void (uint16 segment, uint16 offset, uint8 driveNum, uint8 head, uint8 cylinder, uint8 sectorNum, uint8 count)
read_floppy:
  ; http://www.ctyme.com/intr/rb-0607.htm
  __FUNC_BEGIN_16
  mov ax, __ARG(0)      ; segment
  mov es, ax
  mov ah, 2              ; AH = 02h
  mov al, __ARG(6)     ; AL = number of sectors to read (must be nonzero)
  mov ch, __ARG(4)     ; CH = low eight bits of cylinder number
  mov cl, __ARG(5)     ; CL = sector number 1-63 (bits 0-5), high two bits of cylinder (bits 6-7, hard disk only)
  mov dh, __ARG(3)      ; DH = head number
  mov dl, __ARG(2)      ; DL = drive number (bit 7 set for hard disk)
  mov bx, __ARG(1)      ; ES:BX -> data buffer
  int 0x13               ;
  jc read_floppy

  __FUNC_END_16

times 510 - ($ - $$) nop
db 0x55, 0xaa
