//
// Created by x7cc on 2020/4/14.
//

#include "vos/intel/intel.h"
#include "vos/intel/vmx.h"
#include "vos/cpu.h"
#include "vos/vos.h"
#include "vos/asm.h"
#include "vos/stdio.h"
#include "vos/memory.h"
#include "bochs/bochs.h"

int check_vmx ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 1);
  if (cpuid.eax & (1 << 5) == 0)
  {
    puts ("not support vmx");
    return -1;
  }

  uint64 msr = __rdmsr (IA32_FEATURE_CONTROL);
  if (msr & (IA32_FEATURE_CONTROL_VMX_MASK) == 0)
  {
    puts ("vmxon");
    return -1;
  }
  msr |= IA32_FEATURE_CONTROL_LOCK_MASK;
  __wrmsr (IA32_FEATURE_CONTROL, msr);
  return 0;
}

int vmx_start ()
{
  uint64 cr0 = __readcr0 ();
  cr0 |= CR0_NE_MASK;
  __writecr0 (cr0);

  uint64 cr4 = __readcr4 ();
  cr4 |= CR4_VMXE_MASK;
  __writecr4 (cr4);

  uint64 msr = __rdmsr (IA32_VMX_BASIC);

  uint64 vmcs_size        = IA32_VMX_BASIC_VMCS_SIZE (msr);
  uint64 vmcs_revision_id = (msr & IA32_VMX_BASIC_VMCS_REVISION_IDENTIFIER_MASK);

  void* vmcs = (void*)VOS_VMCS_PA;
  memset (vmcs, 0, vmcs_size);

  *((uint64*)vmcs) = vmcs_revision_id;

  __vmxon (&vmcs);

  uint64 rflags = __rflags ();
  if ((rflags & FLAGS_CF_MASK) != 0)
  {
    puts ("vmxon failed.");
    return -1;
  }

  puts ("vmxon successful.");

  return 0;
}

int vmx_stop ()
{
  __vmxoff ();

  uint64 cr4 = __readcr4 ();
  cr4 &= ~CR4_VMXE_MASK;
  __writecr4 (cr4);

  return 0;
}

void intel_entry ()
{
  if (check_vmx () != 0)
    return;

  puts ("intel cpu check completed.");
  vmx_start ();
  vmx_stop ();
}
