
_MBR_PA         equ 0x7c00      ; MBR被加载到的地址.
_VIDEO_PA       equ 0xb800      ; 实模式中显存的段地址.
_DRIVE_NUM      equ 0b00000000  ; Drive number.
_KERNEL_SEGMENT equ 0x1000      ; 内核程序段基址.
_KERNEL_OFFSET  equ 0x0000      ; 内核程序段偏移.

org _MBR_PA
bits 16

BootSector:
  ; http://stanislavs.org/helppc/boot_sector.html
  jmp short boot
  nop                    ; 3bytes     jump to executable code
  db '0x7cc', 0, 0, 0    ; 8bytes     OEM name and version
  dw 512                 ; word       bytes per sector
  db 1                   ; byte       sectors per cluster (allocation unit size)
  dw 1                   ; word       number of reserved sectors (starting at 0)
  db 2                   ; byte       number of FAT's on disk
  dw 224                 ; word       number of root directory entries (directory size)
  dw 2880                ; word       number of total sectors (0 if partition > 32Mb)
  db 0xf0                ; byte       media descriptor byte
  dw 9                   ; word       sectors per FAT
  dw 18                  ; word       sectors per track  (DOS 3.0+)
  dw 2                   ; word       number of heads  (DOS 3.0+)
  dw 0                   ; word       number of hidden sectors  (DOS 3.0+)
  dw 0                   ; no in the doc ??????
  dd 0                   ; dword      (DOS 4+) number of sectors if offset 13 was 0
  db _DRIVE_NUM          ; byte       (DOS 4+) physical drive number
  db 0                   ; byte       (DOS 4+) reserved
  db 0x29                ; byte       (DOS 4+) signature byte (29h)
  dd 0                   ; dword      (DOS 4+) volume serial number
  db '0x7cc vos', 0, 0   ; 11bytes    (DOS 4+) volume label
  db 'FAT12', 0, 0, 0    ; 8bytes     (DOS 4+) reserved

boot:
  call init
  call work
  call end

init:

  call clear

  push 0
  push 0
  call set_cursor
  add esp, 4

  call reset_floppy

  ret

work:
  push len
  push text
  call puts
  add esp, 4

  push 0
  push len
  call set_cursor
  add esp, 4

  call read_floppy

  ret

reset_floppy:
  ; reset disk system, ref http://stanislavs.org/helppc/int_13-0.html
  xor ah, ah
  mov dl, 0
  int 0x13
  ret

read_floppy:
  call break             ;
  ; http://www.ctyme.com/intr/rb-0607.htm
  mov ax, 0x1000
  mov es, ax
  mov ah, 2              ; AH = 02h
  mov al, 1              ; AL = number of sectors to read (must be nonzero)
  mov ch, 0              ; CH = low eight bits of cylinder number
  mov cl, 0b00000001     ; CL = sector number 1-63 (bits 0-5), high two bits of cylinder (bits 6-7, hard disk only)
  mov dh, 0              ; DH = head number
  mov dl, _DRIVE_NUM     ; DL = drive number (bit 7 set for hard disk)
  mov bx, 0              ; ES:BX -> data buffer
  int 0x13               ;
  jc read_floppy

  ret

  ; 清空整个屏幕.
clear:
  ;
  mov ax, _VIDEO_PA
  mov es, ax
  ; 屏幕字符个数.
  mov cx, 80*25

  __clear__:
    mov bx, cx
    dec bx              ; bx = cx - 1
    mov ax, 2
    mul bx              ; ax = (cx - 1) * 2
    mov si, ax          ; 屏幕坐标索引.

    mov dh, 0b00000111  ; 左4bit背景RGB, 右4bit字符RGB
    mov dl, 0           ; ascii
    mov word [es:si], dx
    loop __clear__

  ; 设置光标的位置.
set_cursor:           ; func (uint8 x, uint8 y)
  mov bh, 0           ; BH = page number
  mov dl, [esp + 2]   ; DL = column (00h is left)
  mov dh, [esp + 4]   ; DH = row (00h is top)

  ; http://www.ctyme.com/intr/rb-0087.htm
  mov ah, 2
  int 0x10
  ret

puts:
  mov cx, 0           ; 初始化.

  __puts__:
    cmp cx, [esp + 4]   ; len
    je __puts_end__              ; 判断字符串是否结束.

    mov bx, cx
    mov ax, 2
    mul bx              ; bx * ax
    mov si, ax          ; 显存间接寻址.

    mov ax, [esp + 2]   ; string address
    add ax, cx

    mov di, ax          ; 数据间接寻址

    mov dh, 0b00000111  ; 左4bit背景RGB, 右4bit字符RGB
    mov dl, [di]        ; ascii
    mov word [es:si], dx

    inc cx

    jmp __puts__

  __puts_end__:
    ret

end:
  jmp $

break:
  xchg bx, bx   ; bochs magic break
  ret

text: db "hello!", 0  ; 定义一个字符串.
len equ ($ - text - 1) ; 字符串的长度.

times 510 - ($ - $$) nop
db 0x55, 0xaa
