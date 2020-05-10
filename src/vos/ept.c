//
// Created by x7cc on 2020/5/5.
//

#include "vos/ept.h"
#include "vos/vos.h"
#include "vos/memory.h"
#include "vos/stdio.h"

vos_uint ept_translation (ept_PML4E_t* pml4_HVA, vos_uint GPA)
{
  register vos_uint     offset  = (GPA >> 0) & (vos_uint)0b111111111111;
  register vos_uint     ptIdx   = (GPA >> 12) & (vos_uint)0b111111111;
  register vos_uint     pdIdx   = (GPA >> 21) & (vos_uint)0b111111111;
  register vos_uint     pdpIdx  = (GPA >> 30) & (vos_uint)0b111111111;
  register vos_uint     pml4Idx = (GPA >> 39) & (vos_uint)0b111111111;
  register ept_PDPTE_t* pdpt    = pfn_to_pa (pml4_HVA[pml4Idx].pdpt_page_PA);
  if (pdpt == nullptr)
    return -1;

  register ept_PDE_t* pd = pfn_to_pa (pdpt[pdpIdx].pd_page_PA);
  if (pd == nullptr)
    return -1;

  register ept_PTE_t* pt = pfn_to_pa (pd[pdIdx].pt_page_PA);
  if (pt == nullptr)
    return -1;

  register vos_uint64 page = pfn_to_pa (pt[ptIdx].page_PA);

  return page + offset;
}

vos_uint ept_pt_set (ept_PML4E_t* pml4_HVA, vos_uint HPA, vos_uint GPA)
{
  register ept_PTE_t* pt_pa = ept_pt_get (pml4_HVA, GPA);

  pt_pa->page_PA         = pa_to_pfn (HPA);
  pt_pa->execute_access  = 1;
  pt_pa->read_access     = 1;
  pt_pa->write_access    = 1;
  pt_pa->ept_memory_type = 1;

  return 0;
}

ept_PTE_t* ept_pt_get (ept_PML4E_t* pml4_HVA, vos_uint HPA)
{
  register vos_uint offset  = (HPA >> 0) & (vos_uint)0b111111111111;
  register vos_uint ptIdx   = (HPA >> 12) & (vos_uint)0b111111111;
  register vos_uint pdIdx   = (HPA >> 21) & (vos_uint)0b111111111;
  register vos_uint pdpIdx  = (HPA >> 30) & (vos_uint)0b111111111;
  register vos_uint pml4Idx = (HPA >> 39) & (vos_uint)0b111111111;

  register ept_PDPTE_t* pdpt_pa = pfn_to_pa (pml4_HVA[pml4Idx].pdpt_page_PA);
  if (pdpt_pa == nullptr)
  {
    void* ptr                        = calloc (VOS_PAGE_SIZE);
    pdpt_pa                          = HVA_to_HPA (ptr);
    pml4_HVA[pml4Idx].pdpt_page_PA   = pa_to_pfn ((vos_uint)pdpt_pa);
    pml4_HVA[pml4Idx].read_access    = 1;
    pml4_HVA[pml4Idx].write_access   = 1;
    pml4_HVA[pml4Idx].execute_access = 1;
  }

  register ept_PDE_t* pd_pa = pfn_to_pa (pdpt_pa[pdpIdx].pd_page_PA);
  if (pd_pa == nullptr)
  {
    void* ptr                      = calloc (VOS_PAGE_SIZE);
    pd_pa                          = HVA_to_HPA (ptr);
    pdpt_pa[pdpIdx].pd_page_PA     = pa_to_pfn ((vos_uint)pd_pa);
    pdpt_pa[pdpIdx].read_access    = 1;
    pdpt_pa[pdpIdx].write_access   = 1;
    pdpt_pa[pdpIdx].execute_access = 1;
  }

  register ept_PTE_t* pt_pa = pfn_to_pa (pd_pa[pdIdx].pt_page_PA);
  if (pt_pa == nullptr)
  {
    void* ptr                   = calloc (VOS_PAGE_SIZE);
    pt_pa                       = HVA_to_HPA (ptr);
    pd_pa[pdIdx].pt_page_PA     = pa_to_pfn ((vos_uint)pt_pa);
    pd_pa[pdIdx].read_access    = 1;
    pd_pa[pdIdx].write_access   = 1;
    pd_pa[pdIdx].execute_access = 1;
  }

  return &pt_pa[ptIdx];
}

ept_PML4E_t* ept_init (vos_uint page_count)
{
  register ept_PML4E_t* pml4E = calloc (VOS_PAGE_SIZE);

  register vos_uint GPA    = 0;
  register vos_uint HVA    = malloc (page_count * VOS_PAGE_SIZE);
  register vos_uint HPA    = HVA_to_HPA (HVA);
  register vos_uint ept_PA = HVA_to_HPA (pml4E);
  for (register int i = 0; i < page_count; ++i, GPA += 4096, HPA += 4096)
  {
    vos_uint pte = ept_pt_set (pml4E, HPA, GPA);
    print ("ept : pte(0x%x), GPA(0x%x) => HPA(0x%x)\n", pte, GPA, HPA);
  }
  print ("ept_HPA : 0x%x\n", ept_PA);
  return pml4E;
}
