//
// Created by x7cc on 2020/4/13.
// 暂时只实现一个粗糙的内存管理
//

#include "vos/memory.h"
#include "vos/vos.h"
#include "vos/x86_64.h"

static uint  page_base;
static uint* page_map;
static uint  page_count;

static inline uint calc_page_num (uint size)
{
  return (size + VOS_PAGE_SIZE - 1) >> 12;
}

static inline uint calc_page_idx (uint addr)
{
  return (addr - page_base) >> 12;
}

void init_memory (uint addr, uint len)
{
  page_count = calc_page_num (len);
  page_map   = addr;
  for (int i = 0; i < page_count; ++i)
  {
    page_map[i] = 0;
  }
  page_base = addr + (((sizeof (*page_map) * page_count) + VOS_PAGE_SIZE - 1) & ~(uint)0xfff);
}

static uint alloc_mem (uint size)
{
  uint n   = calc_page_num (size);
  uint end = page_count - n + 1;
  for (int i = 0; i < end; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      if (page_map[i + j] != 0)
      {
        goto fail;
      }
    }

    // 将每个内存页都标识为使用状态.
    for (int k = 0; k < n; ++k)
    {
      page_map[i + k] = 1;
    }
    page_map[i] |= (n << 1); // 标识这块内存有多少个页.
    // print ("alloc_mem : 0x%x, pageIdx : %d, pageNum : %d, blockSize : %d\n", (page_base + (i * VOS_PAGE_SIZE)), i, n, size);
    // bochs_break();
    return (page_base + (i * VOS_PAGE_SIZE));

  fail:
    continue;
  }
  return 0;
}

void memcpy (void* dest, const void* src, uint n)
{
  for (int i = 0; i < n; ++i)
  {
    ((uint8*)dest)[i] = ((uint8*)src)[i];
  }
}

int memcmp (const void* a, const void* b, uint n)
{
  for (int i = 0; i < n; ++i)
  {
    if (((uint8*)a)[i] < ((uint8*)b)[i])
      return -1;

    if (((uint8*)a)[i] > ((uint8*)b)[i])
      return 1;
  }

  return 0;
}

void* malloc (uint64 size)
{
  return (void*)alloc_mem (size);
}

void* calloc (uint64 size)
{
  void* mem = (void*)alloc_mem (size);
  __memset8 (mem, 0, size);
  return mem;
}

void free (void* mem)
{
  uint idx = calc_page_idx (mem);
  uint n   = page_map[idx] >> 1; // 标识这块内存有多少个页.
  for (int i = 0; i < n; ++i)
  {
    page_map[idx + i] = 0;
  }
  // print("free : 0x%x, pageIdx : %d, pageNum : %d\n", mem, idx, n);
}

// 9-9-9-9-12
uint64 VirtualAddressToPhysicalAddress (uint64 va)
{
  uint64 PML4    = __read_cr3 ();
  uint64 offset  = (va >> 0) & (uint64)0b111111111111;
  uint64 ptIdx   = (va >> 12) & (uint64)0b111111111;
  uint64 pdIdx   = (va >> 21) & (uint64)0b111111111;
  uint64 pdpIdx  = (va >> 30) & (uint64)0b111111111;
  uint64 pml4Idx = (va >> 39) & (uint64)0b111111111;

  uint64* p = ((uint64*)PML4)[pml4Idx] & (~(uint64)0b111111111111);
  p         = (uint64) (p[pdpIdx] & (~(uint64)0b111111111111));
  p         = (uint64) (p[pdIdx] & (~(uint64)0b111111111111));
  p         = (uint64) (p[ptIdx] & (~(uint64)0b111111111111));
  return ((uint64)p) + offset;
}
