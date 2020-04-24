
global bochs_break

bochs_break:
  xchg bx, bx
  ret

global gdb_break
gdb_break:
  int 3
  ret
