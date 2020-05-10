//
// Created by x7cc on 2020/4/15.
//

#include "vos/ept.h"
#include "vos/guest.h"
#include "vos/memory.h"
#include "vos/stdio.h"
#include "vos/types.h"
#include "vos/vmx.h"
#include "vos/vos.h"
#include "vos/x86.h"
#include "vos/x86_64.h"

vos_vmx_guest_t* guests[8];

const char* VMX_INSTRUCTION_ERROR_STRING (int num)
{
#define XX(N, S) \
  case N:        \
    return S;

  switch (num)
  {
    VMX_INSTRUCTION_ERROR_NUMBERS (XX);
  }
#undef XX
  return "";
}

void setup_vmx_PML4E (vos_vmx_guest_t* guest, vos_uint64 guest_VA, vos_uint64 GPA)
{
  // vos_uint    offset  = (GPA >> 0) & (vos_uint)0b111111111111;
  vos_uint64  ptIdx   = (GPA >> 12) & (vos_uint)0b111111111;
  vos_uint64  pdIdx   = (GPA >> 21) & (vos_uint)0b111111111;
  vos_uint64  pdpIdx  = (GPA >> 30) & (vos_uint)0b111111111;
  vos_uint64  pml4Idx = (GPA >> 39) & (vos_uint)0b111111111;
  vos_uint64* pml4    = (vos_uint64*)(guest->pml4_HPA);

  vos_uint64 *pdpt_gpa, *pdpt_hpa;
  pdpt_gpa = (vos_uint64*)(pml4[pml4Idx] & 0xfffffffff000);
  if (pdpt_gpa == nullptr)
  {
    pdpt_gpa = (vos_uint64*)guest_malloc ((vos_guest_t*)guest, VOS_PAGE_SIZE);
    pdpt_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pdpt_gpa);
    __memset64 (pdpt_hpa, 0, 512);
    pml4[pml4Idx] = (vos_uint64)pdpt_gpa | 7ull;
  }
  else
  {
    pdpt_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pdpt_gpa);
  }

  if ((vos_int)pdpt_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pdpt_gpa);
    return;
  }

  vos_uint64 *pd_gpa, *pd_hpa;
  pd_gpa = (vos_uint64*)(pdpt_hpa[pdpIdx] & 0xfffffffff000ull);
  if (pd_gpa == nullptr)
  {
    pd_gpa = (vos_uint64*)guest_malloc ((vos_guest_t*)guest, VOS_PAGE_SIZE);
    pd_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pd_gpa);
    __memset64 (pd_hpa, 0, 512);
    pdpt_hpa[pdpIdx] = (vos_uint64)pd_gpa | 7ull;
  }
  else
  {
    pd_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pd_gpa);
  }

  if ((vos_int)pd_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pd_gpa);
    return;
  }

  vos_uint64 *pt_gpa, *pt_hpa;
  pt_gpa = (vos_uint64*)(pd_hpa[pdIdx] & 0xfffffffff000ull);
  if (pt_gpa == nullptr)
  {
    pt_gpa = guest_malloc ((vos_guest_t*)guest, VOS_PAGE_SIZE);
    pt_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pt_gpa);
    __memset64 (pt_hpa, 0, 512);
    pd_hpa[pdIdx] = (vos_uint64)pt_gpa | 7ull;
  }
  else
  {
    pt_hpa = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, (vos_uint64)pt_gpa);
  }

  if ((vos_int)pt_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pt_gpa);
    return;
  }

  pt_hpa[ptIdx] = guest_VA | 7ull;
}

vos_uint make_vmx_PML4E (vos_vmx_guest_t* guest, vos_uint64 page_count)
{
  vos_uint64 page_begin = 0;

  guest->pml4_HPA = ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, guest_malloc ((vos_guest_t*)guest, VOS_PAGE_SIZE));
  __memset64 ((void*)guest->pml4_HPA, 0, 512);
  for (int i = 0; i < page_count; ++i, page_begin += VOS_PAGE_SIZE)
  {
    setup_vmx_PML4E (guest, page_begin, page_begin);
  }

  return 0;
}

vos_uint make_vmx_gdt (vos_vmx_guest_t* guest)
{
  gdtr_t gdtr = {
    .base  = guest_malloc ((vos_guest_t*)guest, VOS_PAGE_SIZE),
    .limit = 0x27,
  };

  vos_uint64* descriptor = (vos_uint64*)ept_translation ((ept_PML4E_t*)guest->ept_base_HVA, gdtr.base);
  descriptor[0]          = 0;
  descriptor[1]          = 0x00AF93000000FFFF; // ring 0 data segment
  descriptor[2]          = 0x00AF9B000000FFFF; // ring 0 code segment
  descriptor[3]          = 0x00AFF2000000FFFF; // ring 3 data segment
  descriptor[4]          = 0x00AFFA000000FFFF; // ring 3 code segment

  __vos_vmx_vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
  __vos_vmx_vmwrite (VMX_VMCS32_GUEST_GDTR_LIMIT, gdtr.limit);

  return 0;
}