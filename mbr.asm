
_MBR_PA         equ 0x7c00      ; MBR被加载到的地址.
_VIDEO_PA       equ 0xb800      ; 实模式中显存的段地址.
_DRIVE_NUM      equ 0b00000000  ; Drive number.
_KERNEL_SEGMENT equ 0x1000      ; 内核程序段基址.
_KERNEL_OFFSET  equ 0x0000      ; 内核程序段偏移.

org _MBR_PA
bits 16

BootSector:
  jmp short boot
  nop                    ; 3 BYTEs    jump to executable code
  db '0x7cc', 0, 0, 0    ; 8 BYTEs    OEM name and version

  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_2.0_BPB
  dw 512                 ; WORD       Bytes per logical sector
  db 1                   ; BYTE       Logical sectors per cluster
  dw 1                   ; WORD       Reserved logical sectors
  db 2                   ; BYTE       Number of FATs
  dw 224                 ; WORD       Root directory entries
  dw 2880                ; WORD       Total logical sectors
  db 0xf0                ; BYTE       Media descriptor
  dw 9                   ; WORD       Logical sectors per FAT

  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_3.31_BPB
  dw 18                  ; WORD       Physical sectors per track (identical to DOS 3.0 BPB)
  dw 2                   ; WORD       Number of heads (identical to DOS 3.0 BPB)
  dd 0                   ; DWORD      Hidden sectors (incompatible with DOS 3.0 BPB)
  dd 0                   ; DWORD      Large total logical sectors

  ; https://en.wikipedia.org/wiki/BIOS_parameter_block#DOS_4.0_EBPB
  db _DRIVE_NUM          ; BYTE       Physical drive number (identical to DOS 3.4 EBPB)
  db 0                   ; BYTE       Flags etc. (identical to DOS 3.4 EBPB)
  db 0x29                ; BYTE       Extended boot signature (0x29 aka "4.1") (similar to DOS 3.4 EBPB and NTFS EBPB)
  dd 0                   ; DWORD      Volume serial number (identical to DOS 3.4 EBPB)
  db '0x7cc vos', 0, 0   ; 11 BYTEs   Volume label
  db 'FAT12', 0, 0, 0    ; 8 BYTEs    File-system type

boot:                    ; void boot(void)
  call init
  call work
  call end

init:                    ; void init(void)

  call clear

  push 0
  push 0
  call set_cursor
  add esp, 4

  call reset_floppy

  ret

work:                    ; void work(void)
  push len
  push text
  call puts
  add esp, 4

  push 0
  push len
  call set_cursor
  add esp, 4

  call open_A20_line

  call read_floppy

  ret

reset_floppy:            ; void reset_floppy(void)
  ; reset disk system, ref http://stanislavs.org/helppc/int_13-0.html
  xor ah, ah
  mov dl, 0
  int 0x13
  ret

read_floppy:             ; void read_floppy(uint8 driveNum, uint8 head, uint8 cylinder, uint8 sector)
  call break             ;
  ; http://www.ctyme.com/intr/rb-0607.htm
  mov ax, _KERNEL_SEGMENT
  mov es, ax
  mov ah, 2              ; AH = 02h
  mov al, 1              ; AL = number of sectors to read (must be nonzero)
  mov ch, 0              ; CH = low eight bits of cylinder number
  mov cl, 0b00000010     ; CL = sector number 1-63 (bits 0-5), high two bits of cylinder (bits 6-7, hard disk only)
  mov dh, 0              ; DH = head number
  mov dl, _DRIVE_NUM     ; DL = drive number (bit 7 set for hard disk)
  mov bx, _KERNEL_OFFSET ; ES:BX -> data buffer
  int 0x13               ;
  jc read_floppy

  ret

open_A20_line:
  ; https://wiki.osdev.org/A20_Line#Fast_A20_Gate
  push ax
  in   al, 92h
  or   al, 00000010b
  out  92h,  al
  pop  ax

  cli

  ret

  ; 清空整个屏幕.
clear:                   ; void clear(void)
  ;
  mov ax, _VIDEO_PA
  mov es, ax
  ; 屏幕字符个数.
  mov cx, 80*25

  __clear__:
    mov bx, cx
    dec bx               ; bx = cx - 1
    mov ax, 2
    mul bx               ; ax = (cx - 1) * 2
    mov si, ax           ; 屏幕坐标索引.

    mov dh, 0b00000111   ; 左4bit背景RGB, 右4bit字符RGB
    mov dl, 0            ; ascii
    mov word [es:si], dx
    loop __clear__

  ret

  ; 设置光标的位置.
set_cursor:              ; void set_cursor (uint8 x, uint8 y)
  mov bh, 0              ; BH = page number
  mov dl, [esp + 2]      ; DL = column (00h is left)
  mov dh, [esp + 4]      ; DH = row (00h is top)

  ; http://www.ctyme.com/intr/rb-0087.htm
  mov ah, 2
  int 0x10
  ret

puts:                    ; void puts(uint16 ptr, uint16 len)
  mov cx, 0              ; 初始化.

  __puts__:
    cmp cx, [esp + 4]    ; len
    je __puts_end__      ; 判断字符串是否结束.

    mov bx, cx
    mov ax, 2
    mul bx               ; bx * ax
    mov si, ax           ; 显存间接寻址.

    mov ax, [esp + 2]    ; string address
    add ax, cx

    mov di, ax           ; 数据间接寻址

    mov dh, 0b00000111   ; 左4bit背景RGB, 右4bit字符RGB
    mov dl, [di]         ; ascii
    mov word [es:si], dx

    inc cx

    jmp __puts__

  __puts_end__:
    ret

end:                     ; void end(void)
  jmp $

break:                   ; void break(void)
  xchg bx, bx            ; bochs magic break
  ret

text: db "hello!", 0     ; 定义一个字符串.
len equ ($ - text - 1)   ; 字符串的长度.

times 510 - ($ - $$) nop
db 0x55, 0xaa
