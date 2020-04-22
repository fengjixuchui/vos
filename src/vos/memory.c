//
// Created by x7cc on 2020/4/13.
// 暂时只实现一个粗糙的内存管理
//

#include "vos/memory.h"
#include "vos/vos.h"

static uint  page_base;
static uint* page_map;
static uint  page_count;

static inline calc_page_num (uint size)
{
  return (size + VOS_PAGE_SIZE - 1) >> 12;
}

static inline calc_page_idx (uint addr)
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

void memset (void* dest, uint8 c, uint n)
{
  for (int i = 0; i < n; ++i)
  {
    ((uint8*)dest)[i] = c;
  }
}

void memset16 (void* dest, uint16 c, uint n)
{
  for (int i = 0; i < n; ++i)
  {
    ((uint16*)dest)[i] = c;
  }
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
  memset (mem, 0, size);
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
