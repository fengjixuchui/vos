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

void setup_vmx_PML4E (vos_guest_t* guest, uint64 guest_VA, uint64 GPA)
{
  // uint    offset  = (GPA >> 0) & (uint)0b111111111111;
  uint    ptIdx   = (GPA >> 12) & (uint)0b111111111;
  uint    pdIdx   = (GPA >> 21) & (uint)0b111111111;
  uint    pdpIdx  = (GPA >> 30) & (uint)0b111111111;
  uint    pml4Idx = (GPA >> 39) & (uint)0b111111111;
  uint64* pml4    = (uint64*)(guest->pml4_HPA);

  uint64* pdpt_gpa = (uint64*)(pml4[pml4Idx] & 0xfffffffff000);
  uint64* pdpt_hpa;
  if (pdpt_gpa == nullptr)
  {
    pdpt_gpa = guest_malloc (guest, VOS_PAGE_SIZE);
    pdpt_hpa = (uint64*)ept_translation ((ept_PML4E_t*)guest->physical_address_translation_pointer, (uint64)pdpt_gpa);
    __memset64 (pdpt_hpa, 0, 512);
    pml4[pml4Idx] = (uint64)pdpt_gpa | 7;
  }
  else
  {
    pdpt_hpa = (uint64*)ept_translation ((ept_PML4E_t*)guest->physical_address_translation_pointer, (uint64)pdpt_gpa);
  }

  if (pdpt_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pdpt_gpa);
    return;
  }

  uint64* pd_gpa = (uint64*)(pdpt_hpa[pdpIdx] & 0xfffffffff000);
  uint64* pd_hpa;
  if (pd_gpa == nullptr)
  {
    pd_gpa = guest_malloc (guest, VOS_PAGE_SIZE);
    pd_hpa = (uint64*)ept_translation (guest->physical_address_translation_pointer, (uint64)pd_gpa);
    __memset64 (pd_hpa, 0, 512);
    pdpt_hpa[pdpIdx] = (uint64)pd_gpa | 7;
  }
  else
  {
    pd_hpa = (uint64*)ept_translation (guest->physical_address_translation_pointer, (uint64)pd_gpa);
  }

  if (pd_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pd_gpa);
    return;
  }

  uint64* pt_gpa = (uint64*)(pd_hpa[pdIdx] & 0xfffffffff000);
  uint64* pt_hpa;
  if (pt_gpa == nullptr)
  {
    pt_gpa = guest_malloc (guest, VOS_PAGE_SIZE);
    pt_hpa = (uint64*)ept_translation (guest->physical_address_translation_pointer, (uint64)pt_gpa);
    __memset64 (pt_hpa, 0, 512);
    pd_hpa[pdIdx] = (uint64)pt_gpa | 7;
  }
  else
  {
    pt_hpa = (uint64*)ept_translation (guest->physical_address_translation_pointer, (uint64)pt_gpa);
  }

  if (pt_hpa == -1)
  {
    print ("ept_translation(guest, 0x%x) failed\n", pt_gpa);
    return;
  }

  pt_hpa[ptIdx] = guest_VA | 7;
}

uint make_vmx_PML4E (vos_guest_t* guest, uint64 page_count)
{
  uint64 page_begin = 0;

  guest->pml4_HPA = ept_translation (guest->physical_address_translation_pointer, guest_malloc (guest, VOS_PAGE_SIZE));
  __memset64 (guest->pml4_HPA, 0, 512);
  for (int i = 0; i < page_count; ++i, page_begin += VOS_PAGE_SIZE)
  {
    setup_vmx_PML4E (guest, page_begin, page_begin);
  }

  return 0;
}

uint make_vmx_gdt (vos_guest_t* guest)
{
  gdtr_t gdtr = {
    .base  = guest_malloc (guest, VOS_PAGE_SIZE),
    .limit = 0x27,
  };

  uint64* descriptor = ept_translation (guest->physical_address_translation_pointer, gdtr.base);
  descriptor[0]      = 0;
  descriptor[1]      = 0x00AF93000000FFFF; // ring 0 data segment
  descriptor[2]      = 0x00AF9B000000FFFF; // ring 0 code segment
  descriptor[3]      = 0x00AFF2000000FFFF; // ring 3 data segment
  descriptor[4]      = 0x00AFFA000000FFFF; // ring 3 code segment

  __vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
  __vmwrite (VMX_VMCS32_GUEST_GDTR_LIMIT, gdtr.limit);

  return 0;
}