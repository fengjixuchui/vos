
_MBR_PA   equ 0x7c00 ; MBR被加载到的地址.
_VIDEO_PA equ 0xb800 ; 实模式中显存的段地址.

org	_MBR_PA
bits 16

  jmp	short boot
  nop

fat12:
  BS_OEMName       db	'0x7cc', 0, 0, 0
  BPB_BytesPerSec  dw	512
  BPB_SecPerClus   db	1
  BPB_RsvdSecCnt   dw	1
  BPB_NumFATs      db	2
  BPB_RootEntCnt   dw	224
  BPB_TotSec16     dw	2880
  BPB_Media        db	0xf0
  BPB_FATSz16      dw	9
  BPB_SecPerTrk    dw	18
  BPB_NumHeads     dw	2
  BPB_HiddSec      db	0, 0, 0, 0
  BPB_TotSec32     db	0, 0, 0, 0
  BS_DrvNum        db	0
  BS_Reserved1     db	0
  BS_BootSig       db	0x29
  BS_VolID         db	0, 0, 0, 0
  BS_VolLab        db	'0x7cc vos', 0, 0
  BS_FileSysType   db	'FAT12', 0, 0, 0

boot:
  call break          ;
  call init
  call work
  call break          ;
  call end

init:

  call clear

  push 0
  push 0
  call set_cursor
  add esp, 4
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

times 510 - ($ - $$) db 0
db 0x55, 0xaa
