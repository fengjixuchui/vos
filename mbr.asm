
_MBR_PA   equ 0x7c00 ; MBR被加载到的地址.
_VIDEO_PA equ 0xb800 ; 实模式中显存的段地址.

org	_MBR_PA
bits 16

boot:
  mov ax, _VIDEO_PA
  mov es, ax

  ; 屏幕字符个数.
  mov cx, 80*25

  ; 清空整个屏幕.
clear:
  mov bx, cx
  dec bx              ; bx = cx - 1
  mov ax, 2
  mul bx              ; ax = (cx - 1) * 2
  mov si, ax          ; 屏幕坐标索引.

  mov dh, 0b00000111  ; 高4位字符RGB, 低4位背景RGB
  mov dl, 0           ; ascii
  mov word [es:si], dx
  loop clear

  ; 设置光标的位置.
set_cursor:
  mov ax, 0x200
  mov bx, 0
  mov dx, len
  int 0x10

;  xchg bx, bx         ; bochs magic break

  ; 开始给puts循环提供参数.
  mov cx, 0           ; 初始化.
puts:
  cmp cx, len
  je end              ; 判断字符串是否结束.

  mov bx, cx
  mov ax, 2
  mul bx              ; bx * ax
  mov si, ax          ; 显存间接寻址.

  mov ax, text
  add ax, cx

  mov di, ax          ; 数据间接寻址

  mov dh, 0b00000111  ; 高4位字符RGB, 低4位背景RGB
  mov dl, [di]        ; ascii
  mov word [es:si], dx

  inc cx

  jmp puts

end:
  jmp $

text: db "hello!", 0  ; 定义一个字符串.
len equ ($ - text - 1) ; 字符串的长度.

times 510 - ($ - $$) db 0
db 0x55, 0xaa
