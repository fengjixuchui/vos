
%define BOCHS_MAGIC_BREAK xchg bx, bx

_VIDEO_PA       equ 0xb800      ; 实模式中显存的段地址.
_KERNEL_SEGMENT equ 0x1000      ; 内核程序段基址.
_KERNEL_OFFSET  equ 0x0000      ; 内核程序段偏移.

%define uint8  db
%define uint16 dw
%define uint32 dd
%define uint64 dq

;%define SEG_DESCTYPE(x)  ((x) << 0x04)             ; Descriptor type (0 for system, 1 for code/data)
;%define SEG_PRES(x)      ((x) << 0x07)             ; Present
;%define SEG_SAVL(x)      ((x) << 0x0C)             ; Available for system use
;%define SEG_LONG(x)      ((x) << 0x0D)             ; Long mode
;%define SEG_SIZE(x)      ((x) << 0x0E)             ; Size (0 for 16-bit, 1 for 32)
;%define SEG_GRAN(x)      ((x) << 0x0F)             ; Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
;%define SEG_PRIV(x)      (((x) &  0x03) << 0x05)   ; Set privilege level (0 - 3)
;
;%define SEG_DATA_RD        0x00 ; Read-Only
;%define SEG_DATA_RDA       0x01 ; Read-Only, accessed
;%define SEG_DATA_RDWR      0x02 ; Read/Write
;%define SEG_DATA_RDWRA     0x03 ; Read/Write, accessed
;%define SEG_DATA_RDEXPD    0x04 ; Read-Only, expand-down
;%define SEG_DATA_RDEXPDA   0x05 ; Read-Only, expand-down, accessed
;%define SEG_DATA_RDWREXPD  0x06 ; Read/Write, expand-down
;%define SEG_DATA_RDWREXPDA 0x07 ; Read/Write, expand-down, accessed
;%define SEG_CODE_EX        0x08 ; Execute-Only
;%define SEG_CODE_EXA       0x09 ; Execute-Only, accessed
;%define SEG_CODE_EXRD      0x0A ; Execute/Read
;%define SEG_CODE_EXRDA     0x0B ; Execute/Read, accessed
;%define SEG_CODE_EXC       0x0C ; Execute-Only, conforming
;%define SEG_CODE_EXCA      0x0D ; Execute-Only, conforming, accessed
;%define SEG_CODE_EXRDC     0x0E ; Execute/Read, conforming
;%define SEG_CODE_EXRDCA    0x0F ; Execute/Read, conforming, accessed
;
;%define GDT_CODE_PL0 (SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(0) | SEG_CODE_EXRD)
;
;%define GDT_DATA_PL0 (SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | SEG_LONG(0) | SEG_SIZE(1) | SEG_GRAN(1) | SEG_PRIV(0) | SEG_DATA_RDWR)

%define SEG_ACCESS_AC       (1 << 40)
%define SEG_ACCESS_CODE_R   (1 << 41) ; Readable bit for code selectors: Whether read access for this segment is allowed. Write access is never allowed for code segments.
%define SEG_ACCESS_DATA_W   (1 << 41) ; Writable bit for data selectors: Whether write access for this segment is allowed. Read access is always allowed for data segments.
%define SEG_ACCESS_DC       (1 << 42) ;
%define SEG_ACCESS_EXEC     (1 << 43) ;
%define SEG_ACCESS_S        (1 << 44) ; Descriptor type (0 for system, 1 for code/data)
%define SEG_ACCESS_RING0    (0 << 45) ;
%define SEG_ACCESS_RING1    (1 << 45) ;
%define SEG_ACCESS_RING2    (2 << 45) ;
%define SEG_ACCESS_RING3    (3 << 45) ;
%define SEG_ACCESS_PR       (1 << 47) ;

%define SEG_FLAGS_GR        (1 << 55) ; Granularity bit. If 0 the limit is in 1 B blocks (byte granularity), if 1 the limit is in 4 KiB blocks (page granularity).
%define SEG_FLAGS_SZ        (1 << 54) ; Size bit. If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit selectors at once.

%define SEG_BASE(x)   (((x & 0x00ffffff) << 16) | ((x & 0xff000000) << 32))
%define SEG_LIMIT(x)  ((x & 0x0000ffff) | (x & 0x000f0000) << 32)
%define SEG_FLAGS(x)  (((x << 8) & 0x00F0FF00) << 32)

