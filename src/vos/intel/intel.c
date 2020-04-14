//
// Created by x7cc on 2020/4/14.
//

#include "vos/intel/intel.h"
#include "vos/intel/vmx.h"
#include "vos/cpu.h"
#include "vos/vos.h"
#include "vos/asm.h"

int check_vmx ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 1);
  if (cpuid.eax & (1 << 5) == 0)
  {
    puts ("not support vmx");
    return;
  }

  uint64 msr = __rdmsr (IA32_FEATURE_CONTROL);
  if (msr & (IA32_FEATURE_CONTROL_VMX_ENABLE_BIT) == 0)
  {
    puts ("vmxon");
    return;
  }
  msr |= IA32_FEATURE_CONTROL_LOCK_BIT;
  __wrmsr (IA32_FEATURE_CONTROL, msr);
  return 0;
}

int vmx_start ()
{
  uint64 cr0 = __readcr0 ();
  cr0 |= CR0_NE;
  __writecr0 (cr0);

  uint64 cr4 = __readcr4 ();
  cr4 |= CR4_VMXE;
  __writecr4 (cr4);

  uint64 msr = __rdmsr (IA32_VMX_BASIC);

  uint64 vmcs_size        = (msr >> 32 & 0b1111111111111);             //  ; get 44:32 bits, 这个值就是vmcs结构的大小.
  uint64 vmcs_revision_id = (msr & 0b1111111111111111111111111111111); //  ; get 30:0 bits, See: A.1 BASIC VMX INFORMATION

  void* vmcs = (void*)VOS_VMCS_PA;
  memset (vmcs, 0, vmcs_size);

  *((uint64*)vmcs) = vmcs_revision_id;

  __vmxon (&vmcs);
}

int vmx_stop ()
{
  __vmxoff ();

  uint64 cr4 = __readcr4 ();
  cr4 &= ~CR4_VMXE;
  __writecr4 (cr4);
}

void intel_entry ()
{
  if (check_vmx () != 0)
    return;

  puts ("intel cpu check completed.");
  vmx_start ();
  vmx_stop ();
}
