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

static void VMMEntry ()
{
  puts("VMMEntry");
  bochs_break ();
  bochs_break ();
  bochs_break ();
  bochs_break ();
}

int vmx_start ()
{
  uint64 cr0 = __read_cr0 ();
  cr0 |= CR0_NE_MASK;
  __write_cr0 (cr0);

  uint64 cr4 = __read_cr4 ();
  cr4 |= CR4_VMXE_MASK;
  __write_cr4 (cr4);

  uint64 msr = __rdmsr (IA32_VMX_BASIC);

  uint64 vmcs_size        = IA32_VMX_BASIC_VMCS_SIZE (msr);
  uint64 vmcs_revision_id = (msr & IA32_VMX_BASIC_VMCS_REVISION_IDENTIFIER_MASK);

  void* host = (void*)VOS_VMX_HOST_PA;
  memset (host, 0, vmcs_size);

  *((uint64*)host) = vmcs_revision_id;

  __vmxon (&host);

  uint64 rflags = __rflags ();
  if ((rflags & FLAGS_CF_MASK) != 0)
  {
    puts ("vmxon failed.");
    return -1;
  }

  puts ("vmxon successful.");

  void* vmcs = (void*)VOS_VMX_VMCS_PA;
  memset (vmcs, 0, vmcs_size);
  *((uint64*)vmcs) = vmcs_revision_id;

  __vmclear (&vmcs);

  __vmptrld (&vmcs);

  gdtr_t gdtr;
  idtr_t idtr;
  __read_gdtr (&gdtr);
  __read_idtr (&idtr);

  __vmwrite (VMCS_GUEST_RIP, &bochs_break);
  __vmwrite (VMCS_32BIT_CONTROL_PIN_BASED_EXEC_CONTROLS, __rdmsr (IA32_VMX_PINBASEDCTLS) & 0xffffffff);
  __vmwrite (VMCS_32BIT_CONTROL_PROCESSOR_BASED_VMEXEC_CONTROLS, __rdmsr (IA32_VMX_PROCBASEDCTLS) & 0xffffffff);
  __vmwrite (VMCS_32BIT_CONTROL_VMEXIT_CONTROLS, (__rdmsr (IA32_VMX_EXITCTLS) |VMX_VMEXIT_CTRL1_HOST_ADDR_SPACE_SIZE) & 0xffffffff);
  __vmwrite (VMCS_32BIT_CONTROL_VMENTRY_CONTROLS, __rdmsr (IA32_VMX_ENTRYCTLS) & 0xffffffff);

  __vmwrite (VMCS_16BIT_HOST_ES_SELECTOR, __read_es () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_CS_SELECTOR, __read_cs () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_SS_SELECTOR, __read_ss () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_DS_SELECTOR, __read_ds () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_FS_SELECTOR, __read_fs () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_GS_SELECTOR, __read_gs () & 0xfff8);
  __vmwrite (VMCS_16BIT_HOST_TR_SELECTOR, 8 & 0xfff8);

  __vmwrite (VMCS_HOST_TR_BASE, 8);
  __vmwrite (VMCS_HOST_GDTR_BASE, gdtr.base);
  __vmwrite (VMCS_HOST_IDTR_BASE, idtr.base);

  //  __vmwrite (VMCS_HOST_IA32_SYSENTER_CS_MSR, __rdmsr(IA32_SYSENTER_CS));
  __vmwrite (VMCS_HOST_IA32_SYSENTER_ESP_MSR, __rdmsr (IA32_SYSENTER_ESP));
  __vmwrite (VMCS_HOST_IA32_SYSENTER_EIP_MSR, __rdmsr (IA32_SYSENTER_EIP));

  __vmwrite (VMCS_HOST_CR0, __read_cr0 ());
  __vmwrite (VMCS_HOST_CR3, __read_cr3 ());
  __vmwrite (VMCS_HOST_CR4, __read_cr4 ());

  __vmwrite (VMCS_HOST_RSP, 0x1000); // 先让他报错
  __vmwrite (VMCS_HOST_RIP, &VMMEntry);

  __vmwrite (VMCS_GUEST_RFLAGS, __rflags());
  __vmwrite (VMCS_GUEST_CR0, __read_cr0 ());
  __vmwrite (VMCS_GUEST_CR3, __read_cr3 ());
  __vmwrite (VMCS_GUEST_CR4, __read_cr4 ());

  __vmlaunch ();

  uint64 error_code = __vmread (VMCS_32BIT_INSTRUCTION_ERROR);
  puts (VM_INSTRUCTION_ERROR_STRING (error_code));

  return 0;
}

int vmx_stop ()
{
  __vmxoff ();

  uint64 cr4 = __read_cr4 ();
  cr4 &= ~CR4_VMXE_MASK;
  __write_cr4 (cr4);

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
