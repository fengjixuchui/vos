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
#include "vos/x86.h"

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

static void GuestEntry ()
{
  puts ("GuestEntry");
  __vmcall ();
}

static void VMMEntry ()
{
  puts ("VMMEntry");

  bochs_break ();
  uint64 vmexit_reason             = __vmread (VMX_VMCS32_RO_EXIT_REASON);
  uint64 basic_reason              = VMX_EXIT_REASON_BASIC (vmexit_reason);
  uint64 vmexit_instruction_length = __vmread (VMX_VMCS32_RO_EXIT_INSTR_LENGTH);

  // clang-format off
  switch (basic_reason) {
    case VMX_EXIT_INVALID:                  puts("VMX_EXIT_INVALID");                  break;
    case VMX_EXIT_XCPT_OR_NMI:              puts("VMX_EXIT_XCPT_OR_NMI");              break;
    case VMX_EXIT_EXT_INT:                  puts("VMX_EXIT_EXT_INT");                  break;
    case VMX_EXIT_TRIPLE_FAULT:             puts("VMX_EXIT_TRIPLE_FAULT");             break;
    case VMX_EXIT_INIT_SIGNAL:              puts("VMX_EXIT_INIT_SIGNAL");              break;
    case VMX_EXIT_SIPI:                     puts("VMX_EXIT_SIPI");                     break;
    case VMX_EXIT_IO_SMI:                   puts("VMX_EXIT_IO_SMI");                   break;
    case VMX_EXIT_SMI:                      puts("VMX_EXIT_SMI");                      break;
    case VMX_EXIT_INT_WINDOW:               puts("VMX_EXIT_INT_WINDOW");               break;
    case VMX_EXIT_NMI_WINDOW:               puts("VMX_EXIT_NMI_WINDOW");               break;
    case VMX_EXIT_TASK_SWITCH:              puts("VMX_EXIT_TASK_SWITCH");              break;
    case VMX_EXIT_CPUID:                    puts("VMX_EXIT_CPUID");                    break;
    case VMX_EXIT_GETSEC:                   puts("VMX_EXIT_GETSEC");                   break;
    case VMX_EXIT_HLT:                      puts("VMX_EXIT_HLT");                      break;
    case VMX_EXIT_INVD:                     puts("VMX_EXIT_INVD");                     break;
    case VMX_EXIT_INVLPG:                   puts("VMX_EXIT_INVLPG");                   break;
    case VMX_EXIT_RDPMC:                    puts("VMX_EXIT_RDPMC");                    break;
    case VMX_EXIT_RDTSC:                    puts("VMX_EXIT_RDTSC");                    break;
    case VMX_EXIT_RSM:                      puts("VMX_EXIT_RSM");                      break;
    case VMX_EXIT_VMCALL:                   puts("VMX_EXIT_VMCALL");                   break;
    case VMX_EXIT_VMCLEAR:                  puts("VMX_EXIT_VMCLEAR");                  break;
    case VMX_EXIT_VMLAUNCH:                 puts("VMX_EXIT_VMLAUNCH");                 break;
    case VMX_EXIT_VMPTRLD:                  puts("VMX_EXIT_VMPTRLD");                  break;
    case VMX_EXIT_VMPTRST:                  puts("VMX_EXIT_VMPTRST");                  break;
    case VMX_EXIT_VMREAD:                   puts("VMX_EXIT_VMREAD");                   break;
    case VMX_EXIT_VMRESUME:                 puts("VMX_EXIT_VMRESUME");                 break;
    case VMX_EXIT_VMWRITE:                  puts("VMX_EXIT_VMWRITE");                  break;
    case VMX_EXIT_VMXOFF:                   puts("VMX_EXIT_VMXOFF");                   break;
    case VMX_EXIT_VMXON:                    puts("VMX_EXIT_VMXON");                    break;
    case VMX_EXIT_MOV_CRX:                  puts("VMX_EXIT_MOV_CRX");                  break;
    case VMX_EXIT_MOV_DRX:                  puts("VMX_EXIT_MOV_DRX");                  break;
    case VMX_EXIT_IO_INSTR:                 puts("VMX_EXIT_IO_INSTR");                 break;
    case VMX_EXIT_RDMSR:                    puts("VMX_EXIT_RDMSR");                    break;
    case VMX_EXIT_WRMSR:                    puts("VMX_EXIT_WRMSR");                    break;
    case VMX_EXIT_ERR_INVALID_GUEST_STATE:  puts("VMX_EXIT_ERR_INVALID_GUEST_STATE");  break;
    case VMX_EXIT_ERR_MSR_LOAD:             puts("VMX_EXIT_ERR_MSR_LOAD");             break;
    case VMX_EXIT_MWAIT:                    puts("VMX_EXIT_MWAIT");                    break;
    case VMX_EXIT_MTF:                      puts("VMX_EXIT_MTF");                      break;
    case VMX_EXIT_MONITOR:                  puts("VMX_EXIT_MONITOR");                  break;
    case VMX_EXIT_PAUSE:                    puts("VMX_EXIT_PAUSE");                    break;
    case VMX_EXIT_ERR_MACHINE_CHECK:        puts("VMX_EXIT_ERR_MACHINE_CHECK");        break;
    case VMX_EXIT_TPR_BELOW_THRESHOLD:      puts("VMX_EXIT_TPR_BELOW_THRESHOLD");      break;
    case VMX_EXIT_APIC_ACCESS:              puts("VMX_EXIT_APIC_ACCESS");              break;
    case VMX_EXIT_VIRTUALIZED_EOI:          puts("VMX_EXIT_VIRTUALIZED_EOI");          break;
    case VMX_EXIT_GDTR_IDTR_ACCESS:         puts("VMX_EXIT_GDTR_IDTR_ACCESS");         break;
    case VMX_EXIT_LDTR_TR_ACCESS:           puts("VMX_EXIT_LDTR_TR_ACCESS");           break;
    case VMX_EXIT_EPT_VIOLATION:            puts("VMX_EXIT_EPT_VIOLATION");            break;
    case VMX_EXIT_EPT_MISCONFIG:            puts("VMX_EXIT_EPT_MISCONFIG");            break;
    case VMX_EXIT_INVEPT:                   puts("VMX_EXIT_INVEPT");                   break;
    case VMX_EXIT_RDTSCP:                   puts("VMX_EXIT_RDTSCP");                   break;
    case VMX_EXIT_PREEMPT_TIMER:            puts("VMX_EXIT_PREEMPT_TIMER");            break;
    case VMX_EXIT_INVVPID:                  puts("VMX_EXIT_INVVPID");                  break;
    case VMX_EXIT_WBINVD:                   puts("VMX_EXIT_WBINVD");                   break;
    case VMX_EXIT_XSETBV:                   puts("VMX_EXIT_XSETBV");                   break;
    case VMX_EXIT_APIC_WRITE:               puts("VMX_EXIT_APIC_WRITE");               break;
    case VMX_EXIT_RDRAND:                   puts("VMX_EXIT_RDRAND");                   break;
    case VMX_EXIT_INVPCID:                  puts("VMX_EXIT_INVPCID");                  break;
    case VMX_EXIT_VMFUNC:                   puts("VMX_EXIT_VMFUNC");                   break;
    case VMX_EXIT_ENCLS:                    puts("VMX_EXIT_ENCLS");                    break;
    case VMX_EXIT_RDSEED:                   puts("VMX_EXIT_RDSEED");                   break;
    case VMX_EXIT_PML_FULL:                 puts("VMX_EXIT_PML_FULL");                 break;
    case VMX_EXIT_XSAVES:                   puts("VMX_EXIT_XSAVES");                   break;
    case VMX_EXIT_XRSTORS:                  puts("VMX_EXIT_XRSTORS");                  break;
    case VMX_EXIT_SPP_EVENT:                puts("VMX_EXIT_SPP_EVENT");                break;
    case VMX_EXIT_UMWAIT:                   puts("VMX_EXIT_UMWAIT");                   break;
    case VMX_EXIT_TPAUSE:                   puts("VMX_EXIT_TPAUSE");                   break;
  }
  // clang-format on
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

  __vmwrite (VMX_VMCS_GUEST_RIP, &bochs_break);
  __vmwrite (VMX_VMCS32_CTRL_PIN_EXEC, __rdmsr (MSR_IA32_VMX_PINBASED_CTLS) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_PROC_EXEC, __rdmsr (MSR_IA32_VMX_PROCBASED_CTLS) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_EXIT, (__rdmsr (MSR_IA32_VMX_EXIT_CTLS) | VMX_EXIT_CTLS_HOST_ADDR_SPACE_SIZE) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_ENTRY, __rdmsr (MSR_IA32_VMX_ENTRY_CTLS) & 0xffffffff);

  // Host
  {
    __vmwrite (VMX_VMCS16_HOST_ES_SEL, __read_es () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_CS_SEL, __read_cs () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_SS_SEL, __read_ss () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_DS_SEL, __read_ds () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_FS_SEL, __read_fs () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_GS_SEL, __read_gs () & 0xfff8);
    __vmwrite (VMX_VMCS16_HOST_TR_SEL, 8 & 0xfff8);

    __vmwrite (VMX_VMCS_HOST_TR_BASE, 8);
    __vmwrite (VMX_VMCS_HOST_GDTR_BASE, gdtr.base);
    __vmwrite (VMX_VMCS_HOST_IDTR_BASE, idtr.base);

    //  __vmwrite (VMX_VMCS_HOST_IA32_SYSENTER_CS_MSR, __rdmsr(MSR_IA32_SYSENTER_CS));
    __vmwrite (VMX_VMCS_HOST_SYSENTER_ESP, __rdmsr (MSR_IA32_SYSENTER_ESP));
    __vmwrite (VMX_VMCS_HOST_SYSENTER_EIP, __rdmsr (MSR_IA32_SYSENTER_EIP));

    __vmwrite (VMX_VMCS_HOST_CR0, __read_cr0 ());
    __vmwrite (VMX_VMCS_HOST_CR3, __read_cr3 ());
    __vmwrite (VMX_VMCS_HOST_CR4, __read_cr4 ());

    __vmwrite (VMX_VMCS_HOST_RSP, 0x1000); // 先让他报错
    __vmwrite (VMX_VMCS_HOST_RIP, &VMMEntry);
  }

  // Guest
  {
    __vmwrite (VMX_VMCS_GUEST_RSP, 0x2000);
    __vmwrite (VMX_VMCS_GUEST_RIP, &GuestEntry);

    __vmwrite (VMX_VMCS_GUEST_RFLAGS, __rflags ());
    __vmwrite (VMX_VMCS_GUEST_CR0, __read_cr0 ());
    __vmwrite (VMX_VMCS_GUEST_CR3, __read_cr3 ());
    __vmwrite (VMX_VMCS_GUEST_CR4, __read_cr4 ());
    __vmwrite (VMX_VMCS_GUEST_DR7, 0x400);

    __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_FULL, 0xffffffff);
    __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_HIGH, 0xffffffff);

    __vmwrite (VMX_VMCS16_GUEST_ES_SEL, __read_es () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_CS_SEL, __read_cs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_SS_SEL, __read_ss () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_DS_SEL, __read_ds () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_FS_SEL, __read_fs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_GS_SEL, __read_gs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_TR_SEL, 8 & 0xfff8);
    __vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
    __vmwrite (VMX_VMCS_GUEST_IDTR_BASE, idtr.base);
  }

  __vmlaunch ();

  uint64 error_code = __vmread (VMX_VMCS32_RO_VM_INSTR_ERROR);
  puts (VMX_INSTRUCTION_ERROR_STRING (error_code));

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
