//
// Created by x7cc on 2020/4/15.
//

#include "vos/vmx.h"
#include "vos/ept.h"
#include "vos/types.h"
#include "vos/vos.h"
#include "vos/memory.h"
#include "vos/stdio.h"
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

uint64 GuestPA_To_HostPA (vos_guest_t* guest, uint64 guest_PA)
{
  uint         offset  = (guest_PA >> 0) & (uint)0b111111111111;
  uint         ptIdx   = (guest_PA >> 12) & (uint)0b111111111;
  uint         pdIdx   = (guest_PA >> 21) & (uint)0b111111111;
  uint         pdpIdx  = (guest_PA >> 30) & (uint)0b111111111;
  uint         pml4Idx = (guest_PA >> 39) & (uint)0b111111111;
  ept_PML4E_t* pml4    = (uint64*)(guest->memmap_ptr);
  ept_PDPTE_t* pdpt    = pml4[pml4Idx].pdpt_page_PA << 12;
  if (pdpt == nullptr)
    return -1;

  ept_PDE_t* pd = pdpt[pdpIdx].pd_page_PA << 12;
  if (pd == nullptr)
    return -1;

  ept_PTE_t* pt = pd[pdIdx].pt_page_PA << 12;
  if (pt == nullptr)
    return -1;

  uint64 page = pt[ptIdx].page_PA << 12;
  return page + offset;
}

void setup_vmx_PML4E (vos_guest_t* guest, uint64 guest_VA, uint64 guest_PA)
{
  uint    offset  = (guest_PA >> 0) & (uint)0b111111111111;
  uint    ptIdx   = (guest_PA >> 12) & (uint)0b111111111;
  uint    pdIdx   = (guest_PA >> 21) & (uint)0b111111111;
  uint    pdpIdx  = (guest_PA >> 30) & (uint)0b111111111;
  uint    pml4Idx = (guest_PA >> 39) & (uint)0b111111111;
  uint64* pml     = (uint64*)(guest->PML4E);

  uint64* pdpt_gpa = (uint64*)(pml[pml4Idx] & 0xfffffffff000);
  uint64* pdpt_hpa;
  if (pdpt_gpa == nullptr)
  {
    pdpt_gpa = guest->code_address;
    pdpt_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pdpt_gpa);
    __memset64 (pdpt_hpa, 0, 512);
    guest->code_address += VOS_PAGE_SIZE;
    pml[pml4Idx] = (uint64)pdpt_gpa | 7;
  }
  else
  {
    pdpt_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pdpt_gpa);
  }

  if (pdpt_hpa == -1)
  {
    print ("GuestPA_To_HostPA(guest, 0x%x) failed\n", pdpt_gpa);
    return;
  }

  uint64* pd_gpa = (uint64*)(pdpt_hpa[pdpIdx] & 0xfffffffff000);
  uint64* pd_hpa;
  if (pd_gpa == nullptr)
  {
    pd_gpa = guest->code_address;
    pd_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pd_gpa);
    __memset64 (pd_hpa, 0, 512);
    guest->code_address += VOS_PAGE_SIZE;
    pdpt_hpa[pdpIdx] = (uint64)pd_gpa | 7;
  }
  else
  {
    pd_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pd_gpa);
  }

  if (pd_hpa == -1)
  {
    print ("GuestPA_To_HostPA(guest, 0x%x) failed\n", pd_gpa);
    return;
  }

  uint64* pt_gpa = (uint64*)(pd_hpa[pdIdx] & 0xfffffffff000);
  uint64* pt_hpa;
  if (pt_gpa == nullptr)
  {
    pt_gpa = guest->code_address;
    pt_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pt_gpa);
    __memset64 (pt_hpa, 0, 512);
    guest->code_address += VOS_PAGE_SIZE;
    pd_hpa[pdIdx] = (uint64)pt_gpa | 7;
  }
  else
  {
    pt_hpa = (uint64*)GuestPA_To_HostPA (guest, (uint64)pt_gpa);
  }

  if (pt_hpa == -1)
  {
    print ("GuestPA_To_HostPA(guest, 0x%x) failed\n", pt_gpa);
    return;
  }

  pt_hpa[ptIdx] = guest_VA | 7;
}

uint make_vmx_PML4E (vos_guest_t* guest, uint64 page_count)
{
  uint64 page_begin = 0;

  guest->PML4E = GuestPA_To_HostPA (guest, 0);
  guest->code_address += VOS_PAGE_SIZE;
  __memset64 (guest->PML4E, 0, 512);
  for (int i = 0; i < page_count; ++i, page_begin += VOS_PAGE_SIZE)
  {
    setup_vmx_PML4E (guest, page_begin, page_begin);
  }

  return 0;
}

uint setup_vmx_ept_pt (ept_PML4E_t* pml, uint host_PA, uint guest_PA)
{
  uint offset  = (guest_PA >> 0) & (uint)0b111111111111;
  uint ptIdx   = (guest_PA >> 12) & (uint)0b111111111;
  uint pdIdx   = (guest_PA >> 21) & (uint)0b111111111;
  uint pdpIdx  = (guest_PA >> 30) & (uint)0b111111111;
  uint pml4Idx = (guest_PA >> 39) & (uint)0b111111111;

  ept_PDPTE_t* pdpt_pa = pml[pml4Idx].pdpt_page_PA << 12;
  if (pdpt_pa == nullptr)
  {
    void* ptr                   = calloc (VOS_PAGE_SIZE);
    pdpt_pa                     = VirtualAddressToPhysicalAddress (ptr);
    pml[pml4Idx].pdpt_page_PA   = ((uint)pdpt_pa >> 12);
    pml[pml4Idx].read_access    = 1;
    pml[pml4Idx].write_access   = 1;
    pml[pml4Idx].execute_access = 1;
  }

  ept_PDE_t* pd_pa = pdpt_pa[pdpIdx].pd_page_PA << 12;
  if (pd_pa == nullptr)
  {
    void* ptr                      = calloc (VOS_PAGE_SIZE);
    pd_pa                          = VirtualAddressToPhysicalAddress (ptr);
    pdpt_pa[pdpIdx].pd_page_PA     = ((uint)pd_pa >> 12);
    pdpt_pa[pdpIdx].read_access    = 1;
    pdpt_pa[pdpIdx].write_access   = 1;
    pdpt_pa[pdpIdx].execute_access = 1;
  }

  ept_PTE_t* pt_pa = pd_pa[pdIdx].pt_page_PA << 12;
  if (pt_pa == nullptr)
  {
    void* ptr                   = calloc (VOS_PAGE_SIZE);
    pt_pa                       = VirtualAddressToPhysicalAddress (ptr);
    pd_pa[pdIdx].pt_page_PA     = ((uint)pt_pa >> 12);
    pd_pa[pdIdx].read_access    = 1;
    pd_pa[pdIdx].write_access   = 1;
    pd_pa[pdIdx].execute_access = 1;
  }

  pt_pa[ptIdx].page_PA         = host_PA >> 12;
  pt_pa[ptIdx].execute_access  = 1;
  pt_pa[ptIdx].read_access     = 1;
  pt_pa[ptIdx].write_access    = 1;
  pt_pa[ptIdx].ept_memory_type = 1;

  return &pt_pa[ptIdx];
}

uint make_vmx_ept (uint page_count)
{
  ept_PML4E_t* pml4E = calloc (VOS_PAGE_SIZE);

  uint guest_PA = 0;
  uint host_VA  = malloc (page_count * VOS_PAGE_SIZE);
  uint host_PA  = VirtualAddressToPhysicalAddress (host_VA);
  uint ept_PA   = VirtualAddressToPhysicalAddress (pml4E);
  for (int i = 0; i < page_count; ++i, guest_PA += 4096, host_PA += 4096)
  {
    uint pte = setup_vmx_ept_pt (pml4E, host_PA, guest_PA);
    print ("ept : pte(0x%x), guest_PA(0x%x) => host_PA(0x%x)\n", pte, guest_PA, host_PA);
  }
  print ("ept_PA : 0x%x\n", ept_PA);
  return ept_PA;
}

uint make_vmx_gdt (vos_guest_t* guest)
{
  gdtr_t gdtr = {
    .base  = guest->code_address,
    .limit = 0x27};
  guest->code_address += 4096;

  uint64* descriptor = GuestPA_To_HostPA (guest, gdtr.base);
  descriptor[0]      = 0;
  descriptor[1]      = 0x00AF93000000FFFF; // ring 0 data segment
  descriptor[2]      = 0x00AF9B000000FFFF; // ring 0 code segment
  descriptor[3]      = 0x00AFF2000000FFFF; // ring 3 data segment
  descriptor[4]      = 0x00AFFA000000FFFF; // ring 3 code segment

  __vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
  __vmwrite (VMX_VMCS32_GUEST_GDTR_LIMIT, gdtr.limit);

  return 0;
}