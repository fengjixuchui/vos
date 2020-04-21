//
// Created by x7cc on 2020/4/14.
//

#include "vos/intel/intel.h"
#include "bochs/bochs.h"
#include "vos/asm.h"
#include "vos/cpu.h"
#include "vos/intel/vmx.h"
#include "vos/memory.h"
#include "vos/stdio.h"
#include "vos/vos.h"
#include "vos/x86.h"

int check_vmx ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 1);
  if ((cpuid.ecx & (1 << 5)) == 0)
  {
    puts ("not support vmx");
    return -1;
  }

  uint64 msr = __read_msr (IA32_FEATURE_CONTROL);
  if ((msr & IA32_FEATURE_CONTROL_VMXON_MASK) == 0)
  {
    puts ("vmxon");
    return -1;
  }

  if (msr & IA32_FEATURE_CONTROL_LOCK_MASK)
  {
    puts ("locked");
    //    return -1;
  }

  msr |= IA32_FEATURE_CONTROL_LOCK_MASK;
  __write_msr (IA32_FEATURE_CONTROL, msr);
  return 0;
}

// clang-format off
#define IA32_VMX_EPTVPIDCAP_execute_only_pages_MASK                        ((uint64)1 <<  0)
#define IA32_VMX_EPTVPIDCAP_page_walk_length4_MASK                         ((uint64)1 <<  6)
#define IA32_VMX_EPTVPIDCAP_uncacheble_memory_type_MASK                    ((uint64)1 <<  8)
#define IA32_VMX_EPTVPIDCAP_write_back_memory_type_MASK                    ((uint64)1 << 14)
#define IA32_VMX_EPTVPIDCAP_pde_2mb_pages_MASK                             ((uint64)1 << 16)
#define IA32_VMX_EPTVPIDCAP_pdpte_1_gb_pages_MASK                          ((uint64)1 << 17)
#define IA32_VMX_EPTVPIDCAP_invept_MASK                                    ((uint64)1 << 20)
#define IA32_VMX_EPTVPIDCAP_accessed_and_dirty_flag_MASK                   ((uint64)1 << 21)
#define IA32_VMX_EPTVPIDCAP_single_context_invept_MASK                     ((uint64)1 << 25)
#define IA32_VMX_EPTVPIDCAP_all_context_invept_MASK                        ((uint64)1 << 26)
#define IA32_VMX_EPTVPIDCAP_invvpid_MASK                                   ((uint64)1 << 32)
#define IA32_VMX_EPTVPIDCAP_individual_address_invvpid_MASK                ((uint64)1 << 40)
#define IA32_VMX_EPTVPIDCAP_single_context_invvpid_MASK                    ((uint64)1 << 41)
#define IA32_VMX_EPTVPIDCAP_all_context_invvpid_MASK                       ((uint64)1 << 42)
#define IA32_VMX_EPTVPIDCAP_single_context_retaining_globals_invvpid_MASK  ((uint64)1 << 43)
// clang-format on

int check_ept ()
{
  uint64 result = 0;
  uint64 msr    = __read_msr (IA32_VMX_EPTVPIDCAP);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_page_walk_length4_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_write_back_memory_type_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_invept_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_single_context_invept_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_all_context_invept_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_invvpid_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_individual_address_invvpid_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_single_context_invvpid_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_all_context_invvpid_MASK) == 0);
  result |= ((msr & IA32_VMX_EPTVPIDCAP_single_context_retaining_globals_invvpid_MASK) == 0);

  return result;
}

static void GuestEntry ()
{
  char vendor[13] = {0};
  puts ("GuestEntry 1");
  bochs_break ();
  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  puts (vendor);

  __vmcall (0x1234);
  puts ("GuestEntry 2");
  __vmcall (0x1234);
  puts ("GuestEntry 3");

  bochs_break ();
}

typedef struct GuestContext
{
  uint64 rax;
  uint64 rbx;
  uint64 rcx;
  uint64 rdx;
  uint64 rsi;
  uint64 rdi;
  uint64 arg;
} GuestContext_t;

void VmmVmExitHandler (GuestContext_t* context)
{
  //  puts ("VmmVmExitHandler");

  uint64 guest_rip                 = __vmread (VMX_VMCS_GUEST_RIP);
  uint64 vmexit_reason             = __vmread (VMX_VMCS32_RO_EXIT_REASON);
  uint64 vmexit_instruction_length = __vmread (VMX_VMCS32_RO_EXIT_INSTR_LENGTH);
  uint64 basic_reason              = VMX_EXIT_REASON_BASIC (vmexit_reason);

  // clang-format off
  switch (basic_reason) {
    case VMX_EXIT_INVALID:                  print("VMX_EXIT_INVALID(%d)\n",                  VMX_EXIT_INVALID);                  break;
    case VMX_EXIT_XCPT_OR_NMI:              print("VMX_EXIT_XCPT_OR_NMI(%d)\n",              VMX_EXIT_XCPT_OR_NMI);              break;
    case VMX_EXIT_EXT_INT:                  print("VMX_EXIT_EXT_INT(%d)\n",                  VMX_EXIT_EXT_INT);                  break;
    case VMX_EXIT_TRIPLE_FAULT:             print("VMX_EXIT_TRIPLE_FAULT(%d)\n",             VMX_EXIT_TRIPLE_FAULT);             break;
    case VMX_EXIT_INIT_SIGNAL:              print("VMX_EXIT_INIT_SIGNAL(%d)\n",              VMX_EXIT_INIT_SIGNAL);              break;
    case VMX_EXIT_SIPI:                     print("VMX_EXIT_SIPI(%d)\n",                     VMX_EXIT_SIPI);                     break;
    case VMX_EXIT_IO_SMI:                   print("VMX_EXIT_IO_SMI(%d)\n",                   VMX_EXIT_IO_SMI);                   break;
    case VMX_EXIT_SMI:                      print("VMX_EXIT_SMI(%d)\n",                      VMX_EXIT_SMI);                      break;
    case VMX_EXIT_INT_WINDOW:               print("VMX_EXIT_INT_WINDOW(%d)\n",               VMX_EXIT_INT_WINDOW);               break;
    case VMX_EXIT_NMI_WINDOW:               print("VMX_EXIT_NMI_WINDOW(%d)\n",               VMX_EXIT_NMI_WINDOW);               break;
    case VMX_EXIT_TASK_SWITCH:              print("VMX_EXIT_TASK_SWITCH(%d)\n",              VMX_EXIT_TASK_SWITCH);              break;
    case VMX_EXIT_CPUID:
      // print("VMX_EXIT_CPUID(%d)\n",                    VMX_EXIT_CPUID);
      if (context->rax == 0) {
        context->rax = 0;
        context->rbx = 'bcda';
        context->rdx = 'hgfe';
        context->rcx = 'lkji';
      }
      else {
        cpuid_t cpuid;
        __cpuid(&cpuid, context->rax);
      }
      break;
    case VMX_EXIT_GETSEC:                   print("VMX_EXIT_GETSEC(%d)\n",                   VMX_EXIT_GETSEC);                   break;
    case VMX_EXIT_HLT:                      print("VMX_EXIT_HLT(%d)\n",                      VMX_EXIT_HLT);                      break;
    case VMX_EXIT_INVD:                     print("VMX_EXIT_INVD(%d)\n",                     VMX_EXIT_INVD);                     break;
    case VMX_EXIT_INVLPG:                   print("VMX_EXIT_INVLPG(%d)\n",                   VMX_EXIT_INVLPG);                   break;
    case VMX_EXIT_RDPMC:                    print("VMX_EXIT_RDPMC(%d)\n",                    VMX_EXIT_RDPMC);                    break;
    case VMX_EXIT_RDTSC:                    print("VMX_EXIT_RDTSC(%d)\n",                    VMX_EXIT_RDTSC);                    break;
    case VMX_EXIT_RSM:                      print("VMX_EXIT_RSM(%d)\n",                      VMX_EXIT_RSM);                      break;
    case VMX_EXIT_VMCALL:                   print("VMX_EXIT_VMCALL(%d)\n",                   VMX_EXIT_VMCALL);                   break;
    case VMX_EXIT_VMCLEAR:                  print("VMX_EXIT_VMCLEAR(%d)\n",                  VMX_EXIT_VMCLEAR);                  break;
    case VMX_EXIT_VMLAUNCH:                 print("VMX_EXIT_VMLAUNCH(%d)\n",                 VMX_EXIT_VMLAUNCH);                 break;
    case VMX_EXIT_VMPTRLD:                  print("VMX_EXIT_VMPTRLD(%d)\n",                  VMX_EXIT_VMPTRLD);                  break;
    case VMX_EXIT_VMPTRST:                  print("VMX_EXIT_VMPTRST(%d)\n",                  VMX_EXIT_VMPTRST);                  break;
    case VMX_EXIT_VMREAD:                   print("VMX_EXIT_VMREAD(%d)\n",                   VMX_EXIT_VMREAD);                   break;
    case VMX_EXIT_VMRESUME:                 print("VMX_EXIT_VMRESUME(%d)\n",                 VMX_EXIT_VMRESUME);                 break;
    case VMX_EXIT_VMWRITE:                  print("VMX_EXIT_VMWRITE(%d)\n",                  VMX_EXIT_VMWRITE);                  break;
    case VMX_EXIT_VMXOFF:                   print("VMX_EXIT_VMXOFF(%d)\n",                   VMX_EXIT_VMXOFF);                   break;
    case VMX_EXIT_VMXON:                    print("VMX_EXIT_VMXON(%d)\n",                    VMX_EXIT_VMXON);                    break;
    case VMX_EXIT_MOV_CRX:                  print("VMX_EXIT_MOV_CRX(%d)\n",                  VMX_EXIT_MOV_CRX);                  break;
    case VMX_EXIT_MOV_DRX:                  print("VMX_EXIT_MOV_DRX(%d)\n",                  VMX_EXIT_MOV_DRX);                  break;
    case VMX_EXIT_IO_INSTR:                 print("VMX_EXIT_IO_INSTR(%d)\n",                 VMX_EXIT_IO_INSTR);                 break;
    case VMX_EXIT_RDMSR:                    print("VMX_EXIT_RDMSR(%d)\n",                    VMX_EXIT_RDMSR);                    break;
    case VMX_EXIT_WRMSR:                    print("VMX_EXIT_WRMSR(%d)\n",                    VMX_EXIT_WRMSR);                    break;
    case VMX_EXIT_ERR_INVALID_GUEST_STATE:  print("VMX_EXIT_ERR_INVALID_GUEST_STATE(%d)\n",  VMX_EXIT_ERR_INVALID_GUEST_STATE);  break;
    case VMX_EXIT_ERR_MSR_LOAD:             print("VMX_EXIT_ERR_MSR_LOAD(%d)\n",             VMX_EXIT_ERR_MSR_LOAD);             break;
    case VMX_EXIT_MWAIT:                    print("VMX_EXIT_MWAIT(%d)\n",                    VMX_EXIT_MWAIT);                    break;
    case VMX_EXIT_MTF:                      print("VMX_EXIT_MTF(%d)\n",                      VMX_EXIT_MTF);                      break;
    case VMX_EXIT_MONITOR:                  print("VMX_EXIT_MONITOR(%d)\n",                  VMX_EXIT_MONITOR);                  break;
    case VMX_EXIT_PAUSE:                    print("VMX_EXIT_PAUSE(%d)\n",                    VMX_EXIT_PAUSE);                    break;
    case VMX_EXIT_ERR_MACHINE_CHECK:        print("VMX_EXIT_ERR_MACHINE_CHECK(%d)\n",        VMX_EXIT_ERR_MACHINE_CHECK);        break;
    case VMX_EXIT_TPR_BELOW_THRESHOLD:      print("VMX_EXIT_TPR_BELOW_THRESHOLD(%d)\n",      VMX_EXIT_TPR_BELOW_THRESHOLD);      break;
    case VMX_EXIT_APIC_ACCESS:              print("VMX_EXIT_APIC_ACCESS(%d)\n",              VMX_EXIT_APIC_ACCESS);              break;
    case VMX_EXIT_VIRTUALIZED_EOI:          print("VMX_EXIT_VIRTUALIZED_EOI(%d)\n",          VMX_EXIT_VIRTUALIZED_EOI);          break;
    case VMX_EXIT_GDTR_IDTR_ACCESS:         print("VMX_EXIT_GDTR_IDTR_ACCESS(%d)\n",         VMX_EXIT_GDTR_IDTR_ACCESS);         break;
    case VMX_EXIT_LDTR_TR_ACCESS:           print("VMX_EXIT_LDTR_TR_ACCESS(%d)\n",           VMX_EXIT_LDTR_TR_ACCESS);           break;
    case VMX_EXIT_EPT_VIOLATION:            print("VMX_EXIT_EPT_VIOLATION(%d)\n",            VMX_EXIT_EPT_VIOLATION);            break;
    case VMX_EXIT_EPT_MISCONFIG:            print("VMX_EXIT_EPT_MISCONFIG(%d)\n",            VMX_EXIT_EPT_MISCONFIG);            break;
    case VMX_EXIT_INVEPT:                   print("VMX_EXIT_INVEPT(%d)\n",                   VMX_EXIT_INVEPT);                   break;
    case VMX_EXIT_RDTSCP:                   print("VMX_EXIT_RDTSCP(%d)\n",                   VMX_EXIT_RDTSCP);                   break;
    case VMX_EXIT_PREEMPT_TIMER:            print("VMX_EXIT_PREEMPT_TIMER(%d)\n",            VMX_EXIT_PREEMPT_TIMER);            break;
    case VMX_EXIT_INVVPID:                  print("VMX_EXIT_INVVPID(%d)\n",                  VMX_EXIT_INVVPID);                  break;
    case VMX_EXIT_WBINVD:                   print("VMX_EXIT_WBINVD(%d)\n",                   VMX_EXIT_WBINVD);                   break;
    case VMX_EXIT_XSETBV:                   print("VMX_EXIT_XSETBV(%d)\n",                   VMX_EXIT_XSETBV);                   break;
    case VMX_EXIT_APIC_WRITE:               print("VMX_EXIT_APIC_WRITE(%d)\n",               VMX_EXIT_APIC_WRITE);               break;
    case VMX_EXIT_RDRAND:                   print("VMX_EXIT_RDRAND(%d)\n",                   VMX_EXIT_RDRAND);                   break;
    case VMX_EXIT_INVPCID:                  print("VMX_EXIT_INVPCID(%d)\n",                  VMX_EXIT_INVPCID);                  break;
    case VMX_EXIT_VMFUNC:                   print("VMX_EXIT_VMFUNC(%d)\n",                   VMX_EXIT_VMFUNC);                   break;
    case VMX_EXIT_ENCLS:                    print("VMX_EXIT_ENCLS(%d)\n",                    VMX_EXIT_ENCLS);                    break;
    case VMX_EXIT_RDSEED:                   print("VMX_EXIT_RDSEED(%d)\n",                   VMX_EXIT_RDSEED);                   break;
    case VMX_EXIT_PML_FULL:                 print("VMX_EXIT_PML_FULL(%d)\n",                 VMX_EXIT_PML_FULL);                 break;
    case VMX_EXIT_XSAVES:                   print("VMX_EXIT_XSAVES(%d)\n",                   VMX_EXIT_XSAVES);                   break;
    case VMX_EXIT_XRSTORS:                  print("VMX_EXIT_XRSTORS(%d)\n",                  VMX_EXIT_XRSTORS);                  break;
    case VMX_EXIT_SPP_EVENT:                print("VMX_EXIT_SPP_EVENT(%d)\n",                VMX_EXIT_SPP_EVENT);                break;
    case VMX_EXIT_UMWAIT:                   print("VMX_EXIT_UMWAIT(%d)\n",                   VMX_EXIT_UMWAIT);                   break;
    case VMX_EXIT_TPAUSE:                   print("VMX_EXIT_TPAUSE(%d)\n",                   VMX_EXIT_TPAUSE);                   break;
  }
  // clang-format on

  guest_rip += vmexit_instruction_length;

  // 让Guest执行后面的指令.
  __vmwrite (VMX_VMCS_GUEST_RIP, guest_rip);
}

#define CPUID_0x80000008_EAX_PA_BITS(EAX) (EAX & 0xff)        // 物理地址位宽.
#define CPUID_0x80000008_EAX_LA_BITS(EAX) ((EAX >> 8) & 0xff) // 线性地址位宽.

int vmx_start ()
{
  cpuid_t cpuid;
  void*   host = (void*)VOS_VMX_HOST_PA;

  __cpuid (&cpuid, 0x80000008);
  uint8 pa_width = CPUID_0x80000008_EAX_PA_BITS (cpuid.eax);
  uint8 la_width = CPUID_0x80000008_EAX_LA_BITS (cpuid.eax);

  uint64 msr = __read_msr (IA32_VMX_BASIC);

  uint64 vmcs_size        = IA32_VMX_BASIC_VMCS_SIZE (msr);
  uint64 vmcs_revision_id = (msr & IA32_VMX_BASIC_VMCS_REVISION_IDENTIFIER_MASK);

  memset (host, 0, vmcs_size);

  *((uint64*)host) = vmcs_revision_id;

  uint64 cr0 = __read_cr0 ();
  cr0 |= CR0_NE_MASK;
  __write_cr0 (cr0);

  uint64 cr4 = __read_cr4 ();
  cr4 |= CR4_VMXE_MASK;
  __write_cr4 (cr4);

  __vmxon ((uint64)&host);

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

  __vmclear ((uint64)&vmcs);

  __vmptrld (&vmcs);

  gdtr_t gdtr;
  idtr_t idtr;
  __read_gdtr (&gdtr);
  __read_idtr (&idtr);

  __vmwrite (VMX_VMCS32_CTRL_PIN_EXEC, __read_msr (MSR_IA32_VMX_PINBASED_CTLS) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_PROC_EXEC, __read_msr (MSR_IA32_VMX_PROCBASED_CTLS) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_EXIT, (__read_msr (MSR_IA32_VMX_EXIT_CTLS) | VMX_EXIT_CTLS_HOST_ADDR_SPACE_SIZE) & 0xffffffff);
  __vmwrite (VMX_VMCS32_CTRL_ENTRY, __read_msr (MSR_IA32_VMX_ENTRY_CTLS) & 0xffffffff);

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
    __vmwrite (VMX_VMCS_HOST_GDTR_BASE, gdtr.base); // 居然没有设置limit的接口...
    __vmwrite (VMX_VMCS_HOST_IDTR_BASE, idtr.base); // 居然没有设置limit的接口...

    //  __vmwrite (VMX_VMCS_HOST_IA32_SYSENTER_CS_MSR, __read_msr(MSR_IA32_SYSENTER_CS));
    __vmwrite (VMX_VMCS_HOST_SYSENTER_ESP, __read_msr (MSR_IA32_SYSENTER_ESP));
    __vmwrite (VMX_VMCS_HOST_SYSENTER_EIP, __read_msr (MSR_IA32_SYSENTER_EIP));

    __vmwrite (VMX_VMCS_HOST_CR0, __read_cr0 ());
    __vmwrite (VMX_VMCS_HOST_CR3, __read_cr3 ());
    __vmwrite (VMX_VMCS_HOST_CR4, __read_cr4 ());

    __vmwrite (VMX_VMCS_HOST_RSP, 0x1000); // 返回Host时的栈基指针.
    __vmwrite (VMX_VMCS_HOST_RIP, (uint64)&__vmexit_handler);
  }

  // Guest, See: 24.4 GUEST-STATE AREA
  {
    ;
    __vmwrite (VMX_VMCS_GUEST_RSP, 0x2000); // Guest 中的栈基指针.
    __vmwrite (VMX_VMCS_GUEST_RIP, (uint64)&GuestEntry);

    __vmwrite (VMX_VMCS_GUEST_RFLAGS, __rflags ());
    __vmwrite (VMX_VMCS_GUEST_CR0, __read_cr0 ());
    __vmwrite (VMX_VMCS_GUEST_CR3, make_guest_PML4E ());
    __vmwrite (VMX_VMCS_GUEST_CR4, __read_cr4 ());
    __vmwrite (VMX_VMCS_GUEST_DR7, 0x400);

    __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_FULL, 0xffffffff);
    __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_HIGH, 0xffffffff);

    uint64 msr = __read_msr (IA32_VMX_ENTRYCTLS); // Adjust ???
    msr |= 0b00000000000000000000001000000000;    // IA32e guest
    msr &= (msr >> 32);
    __vmwrite (VMX_VMCS32_CTRL_ENTRY, msr);
    //    __vmwrite(VMX_VMCS32_CTRL_ENTRY, 0b00000000000000000000001000000000);
    //    __vmwrite(VMX_VMCS32_CTRL_ENTRY_MSR_LOAD_COUNT, 0b1000001000000000);

    __vmwrite (VMX_VMCS16_GUEST_ES_SEL, __read_es () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_CS_SEL, __read_cs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_SS_SEL, __read_ss () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_DS_SEL, __read_ds () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_FS_SEL, __read_fs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_GS_SEL, __read_gs () & 0xfff8);
    __vmwrite (VMX_VMCS16_GUEST_TR_SEL, 8 & 0xfff8);

    __vmwrite (VMX_VMCS_GUEST_ES_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_CS_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_SS_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_DS_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_FS_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_GS_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_TR_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_LDTR_BASE, 0);
    __vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
    __vmwrite (VMX_VMCS_GUEST_IDTR_BASE, idtr.base);

    __vmwrite (VMX_VMCS32_GUEST_ES_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_CS_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_SS_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_DS_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_FS_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_GS_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_LDTR_LIMIT, 0xffffffff);
    __vmwrite (VMX_VMCS32_GUEST_TR_LIMIT, 0xffffffff);
    //    bochs_break();
    __vmwrite (VMX_VMCS32_GUEST_GDTR_LIMIT, gdtr.limit);
    __vmwrite (VMX_VMCS32_GUEST_IDTR_LIMIT, idtr.limit);

    __vmwrite (VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS, 0b1000000010010001);
    __vmwrite (VMX_VMCS32_GUEST_CS_ACCESS_RIGHTS, 0b1010000010011101);
    __vmwrite (VMX_VMCS32_GUEST_SS_ACCESS_RIGHTS, 0b1000000010010011);
    __vmwrite (VMX_VMCS32_GUEST_DS_ACCESS_RIGHTS, 0b1000000010010001);
    __vmwrite (VMX_VMCS32_GUEST_FS_ACCESS_RIGHTS, 0b1000000010010001);
    __vmwrite (VMX_VMCS32_GUEST_GS_ACCESS_RIGHTS, 0b1000000010010001);
    __vmwrite (VMX_VMCS32_GUEST_LDTR_ACCESS_RIGHTS, 0b1000000010000010);
    __vmwrite (VMX_VMCS32_GUEST_TR_ACCESS_RIGHTS, 0b1000000010001011);
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
  {
    puts ("VMX check failed");
    return;
  }

  puts ("VMX check successful.");

  if (check_ept () != 0)
  {
    puts ("EPT check failed");
    return;
  }
  puts ("EPT check successful.");

  vmx_start ();
  vmx_stop ();
}
