//
// Created by x7cc on 2020/4/13.
//

#include "bochs/bochs.h"
#include "vos/vos.h"
#include "vos/types.h"
#include "vos/assert.h"
#include "vos/memory.h"

extern void enable_PAE ();
extern void setup_GDT64 (uint32 paddr);
extern void setup_PML4 (uint32 paddr);
extern void goto_IA32E ();

typedef struct
{
  uint64 reversed;
  uint64 ds; // 数据段
  uint64 cs; // 代码段
} GDT64_t;

AssertCompileSize (GDT64_t, 24);

#pragma pack(1)
typedef struct
{
  uint16 limit;
  uint32 base;
} GDTPTR_t;
AssertCompileSize (GDTPTR_t, 6);
#pragma pack()

uint64 make_ds ()
{
  uint64 rw           = (uint64)1 << 41;
  uint64 code_or_data = (uint64)1 << 44;
  uint64 PR           = (uint64)1 << 47;
  uint64 L            = (uint64)1 << 53;

  uint64 paddr = 0xffffffff; // 虽然64位下不需要limit,但是加载完后若没有立即切换64位模式,将导致数据无法访问.
  uint64 limit = (((paddr >> 12) & 0x0000ffff) | (((paddr >> 12) & 0x000f0000) << 32));
  return limit | rw | code_or_data | PR | L;
}

uint64 make_cs ()
{
  uint64 exec         = (uint64)1 << 43;
  uint64 code_or_data = (uint64)1 << 44;
  uint64 PR           = (uint64)1 << 47;
  uint64 L            = (uint64)1 << 53;
  return exec | code_or_data | PR | L;
}

uint64* make_PML4 (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

uint64* make_PDP (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

uint64* make_PD (void* p)
{
  memset (p, 0, 4096);
  return (uint64*)p;
}

static uint64 page_begin = 0;
uint64*       make_PT (void* p)
{
  memset (p, 0, 4096);
  for (int i = 0; i < 512; ++i)
  {
    ((uint64*)p)[i] = page_begin | 7;
    page_begin += 4096;
  }
  return (uint64*)p;
}

void init_GDT64 ()
{
  GDT64_t* gdt64 = (GDT64_t*)VOS_GDT64_PA;
  memset (gdt64, 0, sizeof (GDT64_t));
  gdt64->cs = make_cs ();
  gdt64->ds = make_ds ();

  GDTPTR_t* gdtptr = (GDTPTR_t*)VOS_GDT_PTR_PA;
  gdtptr->base     = VOS_GDT64_PA;
  gdtptr->limit    = sizeof (GDT64_t) - 1;

  setup_GDT64 (VOS_GDT_PTR_PA);
}

void init_PML4 ()
{
  // 全局变量在裸机下需要手动初始化.默认值无效.
  page_begin = 0;

  uint32  offset = VOS_PML4_PA;
  uint64* pml4   = make_PML4 ((void*)offset);
  offset += 4096;
  for (int L2 = 0; L2 < 1; ++L2)
  {
    uint64* pbp = make_PDP ((void*)offset);
    offset += 4096;
    pml4[L2] = (uint32)pbp | 7;
    for (int L3 = 0; L3 < 1; ++L3)
    {
      uint64* pb = make_PD ((void*)offset);
      offset += 4096;
      pbp[L3] = (uint32)pb | 7;

      for (int L4 = 0; L4 < 10; ++L4)
      {
        uint64* pt = make_PT ((void*)offset);
        offset += 4096;
        pb[L4] = (uint32)pt | 7;
      }
    }
  }

  setup_PML4 (VOS_PML4_PA);
}

void loader_main ()
{
  enable_PAE ();
  init_GDT64 ();
  init_PML4 ();
  goto_IA32E ();
}
