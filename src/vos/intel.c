//
// Created by x7cc on 2020/4/14.
//

#include <vos/ept.h>
#include "vos/intel.h"
#include "vos/debug.h"
#include "vos/x86_64.h"
#include "vos/memory.h"
#include "vos/stdio.h"
#include "vos/vos.h"
#include "vos/x86.h"
#include "vos/vmx.h"

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

  if ((msr & IA32_FEATURE_CONTROL_LOCK_MASK) == 0)
  {
    msr |= IA32_FEATURE_CONTROL_LOCK_MASK;
    __write_msr (IA32_FEATURE_CONTROL, msr);
  }

  return 0;
}

// clang-format off
#define IA32_VMX_EPTVPIDCAP_execute_only_pages_MASK                        (1ull <<  0)
#define IA32_VMX_EPTVPIDCAP_page_walk_length4_MASK                         (1ull <<  6)
#define IA32_VMX_EPTVPIDCAP_uncacheble_memory_type_MASK                    (1ull <<  8)
#define IA32_VMX_EPTVPIDCAP_write_back_memory_type_MASK                    (1ull << 14)
#define IA32_VMX_EPTVPIDCAP_pde_2mb_pages_MASK                             (1ull << 16)
#define IA32_VMX_EPTVPIDCAP_pdpte_1_gb_pages_MASK                          (1ull << 17)
#define IA32_VMX_EPTVPIDCAP_invept_MASK                                    (1ull << 20)
#define IA32_VMX_EPTVPIDCAP_accessed_and_dirty_flag_MASK                   (1ull << 21)
#define IA32_VMX_EPTVPIDCAP_single_context_invept_MASK                     (1ull << 25)
#define IA32_VMX_EPTVPIDCAP_all_context_invept_MASK                        (1ull << 26)
#define IA32_VMX_EPTVPIDCAP_invvpid_MASK                                   (1ull << 32)
#define IA32_VMX_EPTVPIDCAP_individual_address_invvpid_MASK                (1ull << 40)
#define IA32_VMX_EPTVPIDCAP_single_context_invvpid_MASK                    (1ull << 41)
#define IA32_VMX_EPTVPIDCAP_all_context_invvpid_MASK                       (1ull << 42)
#define IA32_VMX_EPTVPIDCAP_single_context_retaining_globals_invvpid_MASK  (1ull << 43)
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
  puts ("GuestEntry begin");

  bochs_break ();
  *((uint*)0) = 0x123456789abcdef0;

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  print ("CPU Vendor : %s\n", vendor);

  __vmcall (CMD_CHECK, 0, 0);
  __vmcall (CMD_HOOK_FUNC, 0x1000, 0x2000);

  puts ("GuestEntry end");

  while (1)
    ;
}

uint VmmVmExitHandler (VmxVMExitContext_t* context)
{
  uint64 reason       = __vmread (VMX_VMCS32_RO_EXIT_REASON);
  uint64 basic_reason = VMX_EXIT_REASON_BASIC (reason);

  switch (basic_reason)
  {
    case VMX_EXIT_INVALID:
      print ("VMX_EXIT_INVALID(%d)\n", VMX_EXIT_INVALID);
      break;
    case VMX_EXIT_XCPT_OR_NMI:
      print ("VMX_EXIT_XCPT_OR_NMI(%d)\n", VMX_EXIT_XCPT_OR_NMI);
      break;
    case VMX_EXIT_EXT_INT:
      print ("VMX_EXIT_EXT_INT(%d)\n", VMX_EXIT_EXT_INT);
      break;
    case VMX_EXIT_TRIPLE_FAULT:
      print ("VMX_EXIT_TRIPLE_FAULT(%d)\n", VMX_EXIT_TRIPLE_FAULT);
      break;
    case VMX_EXIT_INIT_SIGNAL:
      print ("VMX_EXIT_INIT_SIGNAL(%d)\n", VMX_EXIT_INIT_SIGNAL);
      break;
    case VMX_EXIT_SIPI:
      print ("VMX_EXIT_SIPI(%d)\n", VMX_EXIT_SIPI);
      break;
    case VMX_EXIT_IO_SMI:
      print ("VMX_EXIT_IO_SMI(%d)\n", VMX_EXIT_IO_SMI);
      break;
    case VMX_EXIT_SMI:
      print ("VMX_EXIT_SMI(%d)\n", VMX_EXIT_SMI);
      break;
    case VMX_EXIT_INT_WINDOW:
      print ("VMX_EXIT_INT_WINDOW(%d)\n", VMX_EXIT_INT_WINDOW);
      break;
    case VMX_EXIT_NMI_WINDOW:
      print ("VMX_EXIT_NMI_WINDOW(%d)\n", VMX_EXIT_NMI_WINDOW);
      break;
    case VMX_EXIT_TASK_SWITCH:
      print ("VMX_EXIT_TASK_SWITCH(%d)\n", VMX_EXIT_TASK_SWITCH);
      break;
    case VMX_EXIT_CPUID:
      // print("VMX_EXIT_CPUID(%d)\n",                    VMX_EXIT_CPUID);
      if (context->rax == 0)
      {
        context->rax = 0;
        context->rbx = 'uneG';
        context->rdx = '0eni';
        context->rcx = 'cc7x';
      }
      else
      {
        cpuid_t cpuid;
        __cpuid (&cpuid, context->rax);
        context->rax = cpuid.eax;
        context->rbx = cpuid.ebx;
        context->rcx = cpuid.ecx;
        context->rdx = cpuid.edx;
      }
      return 0;
    case VMX_EXIT_GETSEC:
      print ("VMX_EXIT_GETSEC(%d)\n", VMX_EXIT_GETSEC);
      break;
    case VMX_EXIT_HLT:
      print ("VMX_EXIT_HLT(%d)\n", VMX_EXIT_HLT);
      break;
    case VMX_EXIT_INVD:
      print ("VMX_EXIT_INVD(%d)\n", VMX_EXIT_INVD);
      break;
    case VMX_EXIT_INVLPG:
      print ("VMX_EXIT_INVLPG(%d)\n", VMX_EXIT_INVLPG);
      break;
    case VMX_EXIT_RDPMC:
      print ("VMX_EXIT_RDPMC(%d)\n", VMX_EXIT_RDPMC);
      break;
    case VMX_EXIT_RDTSC:
      print ("VMX_EXIT_RDTSC(%d)\n", VMX_EXIT_RDTSC);
      break;
    case VMX_EXIT_RSM:
      print ("VMX_EXIT_RSM(%d)\n", VMX_EXIT_RSM);
      break;
    case VMX_EXIT_VMCALL:
      gdb_break ();
      print ("VMX_EXIT_VMCALL(%d)\n", VMX_EXIT_VMCALL);
      switch (context->argv0)
      {
        case CMD_CHECK:
          puts ("CMD_CHECK");
          context->rax = context->argv0;
          return 0;
        case CMD_HOOK_FUNC:
          print ("CMD_HOOK_FUNC from : 0x%x to 0x%x\n", context->argv1, context->argv2);
          context->rax = context->argv0;
          return 0;
        case CMD_HIDE_PROCESS:
          puts ("CMD_HIDE_PROCESS");
          context->rax = context->argv0;
          return 0;
        case CMD_PROTECT_PROCESS:
          puts ("CMD_PROTECT_PROCESS");
          context->rax = context->argv0;
          return 0;
        default:
          return -1;
      }
      break;
    case VMX_EXIT_VMCLEAR:
      print ("VMX_EXIT_VMCLEAR(%d)\n", VMX_EXIT_VMCLEAR);
      break;
    case VMX_EXIT_VMLAUNCH:
      print ("VMX_EXIT_VMLAUNCH(%d)\n", VMX_EXIT_VMLAUNCH);
      break;
    case VMX_EXIT_VMPTRLD:
      print ("VMX_EXIT_VMPTRLD(%d)\n", VMX_EXIT_VMPTRLD);
      break;
    case VMX_EXIT_VMPTRST:
      print ("VMX_EXIT_VMPTRST(%d)\n", VMX_EXIT_VMPTRST);
      break;
    case VMX_EXIT_VMREAD:
      print ("VMX_EXIT_VMREAD(%d)\n", VMX_EXIT_VMREAD);
      break;
    case VMX_EXIT_VMRESUME:
      print ("VMX_EXIT_VMRESUME(%d)\n", VMX_EXIT_VMRESUME);
      break;
    case VMX_EXIT_VMWRITE:
      print ("VMX_EXIT_VMWRITE(%d)\n", VMX_EXIT_VMWRITE);
      break;
    case VMX_EXIT_VMXOFF:
      print ("VMX_EXIT_VMXOFF(%d)\n", VMX_EXIT_VMXOFF);
      break;
    case VMX_EXIT_VMXON:
      print ("VMX_EXIT_VMXON(%d)\n", VMX_EXIT_VMXON);
      break;
    case VMX_EXIT_MOV_CRX:
      print ("VMX_EXIT_MOV_CRX(%d)\n", VMX_EXIT_MOV_CRX);
      break;
    case VMX_EXIT_MOV_DRX:
      print ("VMX_EXIT_MOV_DRX(%d)\n", VMX_EXIT_MOV_DRX);
      break;
    case VMX_EXIT_IO_INSTR:
      print ("VMX_EXIT_IO_INSTR(%d)\n", VMX_EXIT_IO_INSTR);
      break;
    case VMX_EXIT_RDMSR:
      print ("VMX_EXIT_RDMSR(%d)\n", VMX_EXIT_RDMSR);
      break;
    case VMX_EXIT_WRMSR:
      print ("VMX_EXIT_WRMSR(%d)\n", VMX_EXIT_WRMSR);
      break;
    case VMX_EXIT_ERR_INVALID_GUEST_STATE:
      print ("VMX_EXIT_ERR_INVALID_GUEST_STATE(%d)\n", VMX_EXIT_ERR_INVALID_GUEST_STATE);
      print ("rax : 0x%x, rbx : 0x%x, rcx : 0x%x, rdx : 0x%x\n", context->rax, context->rbx, context->rcx, context->rdx);
      print ("rsi : 0x%x, rdi : 0x%x, rsp : 0x%x, rip : 0x%x\n", context->rsi, context->rdi, __vmread (VMX_VMCS_GUEST_RSP), __vmread (VMX_VMCS_GUEST_RIP));
      print ("r8  : 0x%x, r9  : 0x%x, r10 : 0x%x, r11 : 0x%x\n", context->r8, context->r9, context->r10, context->r11);
      print ("r12 : 0x%x, r13 : 0x%x, r14 : 0x%x, r15 : 0x%x\n", context->r12, context->r13, context->r14, context->r15);
      print ("VMX_VMCS32_CTRL_PIN_EXEC : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PIN_EXEC));
      print ("VMX_VMCS32_CTRL_PROC_EXEC : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PROC_EXEC));
      print ("VMX_VMCS32_CTRL_EXCEPTION_BITMAP : 0x%x\n", __vmread (VMX_VMCS32_CTRL_EXCEPTION_BITMAP));
      print ("VMX_VMCS32_CTRL_PAGEFAULT_ERROR_MASK : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PAGEFAULT_ERROR_MASK));
      print ("VMX_VMCS32_CTRL_PAGEFAULT_ERROR_MATCH : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PAGEFAULT_ERROR_MATCH));
      print ("VMX_VMCS32_CTRL_CR3_TARGET_COUNT : 0x%x\n", __vmread (VMX_VMCS32_CTRL_CR3_TARGET_COUNT));
      print ("VMX_VMCS32_CTRL_EXIT : 0x%x\n", __vmread (VMX_VMCS32_CTRL_EXIT));
      print ("VMX_VMCS32_CTRL_EXIT_MSR_STORE_COUNT : 0x%x\n", __vmread (VMX_VMCS32_CTRL_EXIT_MSR_STORE_COUNT));
      print ("VMX_VMCS32_CTRL_EXIT_MSR_LOAD_COUNT : 0x%x\n", __vmread (VMX_VMCS32_CTRL_EXIT_MSR_LOAD_COUNT));
      print ("VMX_VMCS32_CTRL_ENTRY : 0x%x\n", __vmread (VMX_VMCS32_CTRL_ENTRY));
      print ("VMX_VMCS32_CTRL_ENTRY_MSR_LOAD_COUNT : 0x%x\n", __vmread (VMX_VMCS32_CTRL_ENTRY_MSR_LOAD_COUNT));
      print ("VMX_VMCS32_CTRL_ENTRY_INTERRUPTION_INFO : 0x%x\n", __vmread (VMX_VMCS32_CTRL_ENTRY_INTERRUPTION_INFO));
      print ("VMX_VMCS32_CTRL_ENTRY_EXCEPTION_ERRCODE : 0x%x\n", __vmread (VMX_VMCS32_CTRL_ENTRY_EXCEPTION_ERRCODE));
      print ("VMX_VMCS32_CTRL_ENTRY_INSTR_LENGTH : 0x%x\n", __vmread (VMX_VMCS32_CTRL_ENTRY_INSTR_LENGTH));
      print ("VMX_VMCS32_CTRL_TPR_THRESHOLD : 0x%x\n", __vmread (VMX_VMCS32_CTRL_TPR_THRESHOLD));
      print ("VMX_VMCS32_CTRL_PROC_EXEC2 : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PROC_EXEC2));
      print ("VMX_VMCS32_CTRL_PLE_GAP : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PLE_GAP));
      print ("VMX_VMCS32_CTRL_PLE_WINDOW : 0x%x\n", __vmread (VMX_VMCS32_CTRL_PLE_WINDOW));
      print ("VMX_VMCS16_GUEST_ES_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_ES_SEL));
      print ("VMX_VMCS16_GUEST_CS_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_CS_SEL));
      print ("VMX_VMCS16_GUEST_SS_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_SS_SEL));
      print ("VMX_VMCS16_GUEST_DS_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_DS_SEL));
      print ("VMX_VMCS16_GUEST_FS_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_FS_SEL));
      print ("VMX_VMCS16_GUEST_GS_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_GS_SEL));
      print ("VMX_VMCS16_GUEST_LDTR_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_LDTR_SEL));
      print ("VMX_VMCS16_GUEST_TR_SEL : 0x%x\n", __vmread (VMX_VMCS16_GUEST_TR_SEL));
      print ("VMX_VMCS16_GUEST_INTR_STATUS : 0x%x\n", __vmread (VMX_VMCS16_GUEST_INTR_STATUS));
      print ("VMX_VMCS16_GUEST_PML_INDEX : 0x%x\n", __vmread (VMX_VMCS16_GUEST_PML_INDEX));
      print ("VMX_VMCS64_RO_GUEST_PHYS_ADDR_FULL : 0x%x\n", __vmread (VMX_VMCS64_RO_GUEST_PHYS_ADDR_FULL));
      print ("VMX_VMCS64_RO_GUEST_PHYS_ADDR_HIGH : 0x%x\n", __vmread (VMX_VMCS64_RO_GUEST_PHYS_ADDR_HIGH));
      print ("VMX_VMCS64_GUEST_VMCS_LINK_PTR_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_VMCS_LINK_PTR_FULL));
      print ("VMX_VMCS64_GUEST_VMCS_LINK_PTR_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_VMCS_LINK_PTR_HIGH));
      print ("VMX_VMCS64_GUEST_DEBUGCTL_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_DEBUGCTL_FULL));
      print ("VMX_VMCS64_GUEST_DEBUGCTL_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_DEBUGCTL_HIGH));
      print ("VMX_VMCS64_GUEST_PAT_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PAT_FULL));
      print ("VMX_VMCS64_GUEST_PAT_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PAT_HIGH));
      print ("VMX_VMCS64_GUEST_EFER_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_EFER_FULL));
      print ("VMX_VMCS64_GUEST_EFER_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_EFER_HIGH));
      print ("VMX_VMCS64_GUEST_PERF_GLOBAL_CTRL_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PERF_GLOBAL_CTRL_FULL));
      print ("VMX_VMCS64_GUEST_PERF_GLOBAL_CTRL_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PERF_GLOBAL_CTRL_HIGH));
      print ("VMX_VMCS64_GUEST_PDPTE0_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE0_FULL));
      print ("VMX_VMCS64_GUEST_PDPTE0_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE0_HIGH));
      print ("VMX_VMCS64_GUEST_PDPTE1_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE1_FULL));
      print ("VMX_VMCS64_GUEST_PDPTE1_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE1_HIGH));
      print ("VMX_VMCS64_GUEST_PDPTE2_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE2_FULL));
      print ("VMX_VMCS64_GUEST_PDPTE2_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE2_HIGH));
      print ("VMX_VMCS64_GUEST_PDPTE3_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE3_FULL));
      print ("VMX_VMCS64_GUEST_PDPTE3_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_PDPTE3_HIGH));
      print ("VMX_VMCS64_GUEST_BNDCFGS_FULL : 0x%x\n", __vmread (VMX_VMCS64_GUEST_BNDCFGS_FULL));
      print ("VMX_VMCS64_GUEST_BNDCFGS_HIGH : 0x%x\n", __vmread (VMX_VMCS64_GUEST_BNDCFGS_HIGH));
      print ("VMX_VMCS32_GUEST_ES_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_ES_LIMIT));
      print ("VMX_VMCS32_GUEST_CS_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_CS_LIMIT));
      print ("VMX_VMCS32_GUEST_SS_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_SS_LIMIT));
      print ("VMX_VMCS32_GUEST_DS_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_DS_LIMIT));
      print ("VMX_VMCS32_GUEST_FS_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_FS_LIMIT));
      print ("VMX_VMCS32_GUEST_GS_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_GS_LIMIT));
      print ("VMX_VMCS32_GUEST_LDTR_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_LDTR_LIMIT));
      print ("VMX_VMCS32_GUEST_TR_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_TR_LIMIT));
      print ("VMX_VMCS32_GUEST_GDTR_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_GDTR_LIMIT));
      print ("VMX_VMCS32_GUEST_IDTR_LIMIT : 0x%x\n", __vmread (VMX_VMCS32_GUEST_IDTR_LIMIT));
      print ("VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_CS_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_CS_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_SS_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_SS_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_DS_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_DS_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_FS_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_FS_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_GS_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_GS_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_LDTR_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_LDTR_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_TR_ACCESS_RIGHTS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_TR_ACCESS_RIGHTS));
      print ("VMX_VMCS32_GUEST_INT_STATE : 0x%x\n", __vmread (VMX_VMCS32_GUEST_INT_STATE));
      print ("VMX_VMCS32_GUEST_ACTIVITY_STATE : 0x%x\n", __vmread (VMX_VMCS32_GUEST_ACTIVITY_STATE));
      print ("VMX_VMCS32_GUEST_SMBASE : 0x%x\n", __vmread (VMX_VMCS32_GUEST_SMBASE));
      print ("VMX_VMCS32_GUEST_SYSENTER_CS : 0x%x\n", __vmread (VMX_VMCS32_GUEST_SYSENTER_CS));
      print ("VMX_VMCS_RO_GUEST_LINEAR_ADDR : 0x%x\n", __vmread (VMX_VMCS_RO_GUEST_LINEAR_ADDR));
      print ("VMX_VMCS_GUEST_CR0 : 0x%x\n", __vmread (VMX_VMCS_GUEST_CR0));
      print ("VMX_VMCS_GUEST_CR3 : 0x%x\n", __vmread (VMX_VMCS_GUEST_CR3));
      print ("VMX_VMCS_GUEST_CR4 : 0x%x\n", __vmread (VMX_VMCS_GUEST_CR4));
      print ("VMX_VMCS_GUEST_ES_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_ES_BASE));
      print ("VMX_VMCS_GUEST_CS_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_CS_BASE));
      print ("VMX_VMCS_GUEST_SS_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_SS_BASE));
      print ("VMX_VMCS_GUEST_DS_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_DS_BASE));
      print ("VMX_VMCS_GUEST_FS_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_FS_BASE));
      print ("VMX_VMCS_GUEST_GS_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_GS_BASE));
      print ("VMX_VMCS_GUEST_LDTR_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_LDTR_BASE));
      print ("VMX_VMCS_GUEST_TR_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_TR_BASE));
      print ("VMX_VMCS_GUEST_GDTR_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_GDTR_BASE));
      print ("VMX_VMCS_GUEST_IDTR_BASE : 0x%x\n", __vmread (VMX_VMCS_GUEST_IDTR_BASE));
      print ("VMX_VMCS_GUEST_DR7 : 0x%x\n", __vmread (VMX_VMCS_GUEST_DR7));
      print ("VMX_VMCS_GUEST_RSP : 0x%x\n", __vmread (VMX_VMCS_GUEST_RSP));
      print ("VMX_VMCS_GUEST_RIP : 0x%x\n", __vmread (VMX_VMCS_GUEST_RIP));
      print ("VMX_VMCS_GUEST_RFLAGS : 0x%x\n", __vmread (VMX_VMCS_GUEST_RFLAGS));
      print ("VMX_VMCS_GUEST_PENDING_DEBUG_XCPTS : 0x%x\n", __vmread (VMX_VMCS_GUEST_PENDING_DEBUG_XCPTS));
      print ("VMX_VMCS_GUEST_SYSENTER_ESP : 0x%x\n", __vmread (VMX_VMCS_GUEST_SYSENTER_ESP));
      print ("VMX_VMCS_GUEST_SYSENTER_EIP : 0x%x\n", __vmread (VMX_VMCS_GUEST_SYSENTER_EIP));
      return -1;
    case VMX_EXIT_ERR_MSR_LOAD:
      print ("VMX_EXIT_ERR_MSR_LOAD(%d)\n", VMX_EXIT_ERR_MSR_LOAD);
      break;
    case VMX_EXIT_MWAIT:
      print ("VMX_EXIT_MWAIT(%d)\n", VMX_EXIT_MWAIT);
      break;
    case VMX_EXIT_MTF:
      print ("VMX_EXIT_MTF(%d)\n", VMX_EXIT_MTF);
      break;
    case VMX_EXIT_MONITOR:
      print ("VMX_EXIT_MONITOR(%d)\n", VMX_EXIT_MONITOR);
      break;
    case VMX_EXIT_PAUSE:
      print ("VMX_EXIT_PAUSE(%d)\n", VMX_EXIT_PAUSE);
      break;
    case VMX_EXIT_ERR_MACHINE_CHECK:
      print ("VMX_EXIT_ERR_MACHINE_CHECK(%d)\n", VMX_EXIT_ERR_MACHINE_CHECK);
      break;
    case VMX_EXIT_TPR_BELOW_THRESHOLD:
      print ("VMX_EXIT_TPR_BELOW_THRESHOLD(%d)\n", VMX_EXIT_TPR_BELOW_THRESHOLD);
      break;
    case VMX_EXIT_APIC_ACCESS:
      print ("VMX_EXIT_APIC_ACCESS(%d)\n", VMX_EXIT_APIC_ACCESS);
      break;
    case VMX_EXIT_VIRTUALIZED_EOI:
      print ("VMX_EXIT_VIRTUALIZED_EOI(%d)\n", VMX_EXIT_VIRTUALIZED_EOI);
      break;
    case VMX_EXIT_GDTR_IDTR_ACCESS:
      print ("VMX_EXIT_GDTR_IDTR_ACCESS(%d)\n", VMX_EXIT_GDTR_IDTR_ACCESS);
      break;
    case VMX_EXIT_LDTR_TR_ACCESS:
      print ("VMX_EXIT_LDTR_TR_ACCESS(%d)\n", VMX_EXIT_LDTR_TR_ACCESS);
      break;
    case VMX_EXIT_EPT_VIOLATION:
      print ("VMX_EXIT_EPT_VIOLATION(%d)\n", VMX_EXIT_EPT_VIOLATION);
      break;
    case VMX_EXIT_EPT_MISCONFIG:
      print ("VMX_EXIT_EPT_MISCONFIG(%d)\n", VMX_EXIT_EPT_MISCONFIG);
      break;
    case VMX_EXIT_INVEPT:
      print ("VMX_EXIT_INVEPT(%d)\n", VMX_EXIT_INVEPT);
      break;
    case VMX_EXIT_RDTSCP:
      print ("VMX_EXIT_RDTSCP(%d)\n", VMX_EXIT_RDTSCP);
      break;
    case VMX_EXIT_PREEMPT_TIMER:
      print ("VMX_EXIT_PREEMPT_TIMER(%d)\n", VMX_EXIT_PREEMPT_TIMER);
      break;
    case VMX_EXIT_INVVPID:
      print ("VMX_EXIT_INVVPID(%d)\n", VMX_EXIT_INVVPID);
      break;
    case VMX_EXIT_WBINVD:
      print ("VMX_EXIT_WBINVD(%d)\n", VMX_EXIT_WBINVD);
      break;
    case VMX_EXIT_XSETBV:
      print ("VMX_EXIT_XSETBV(%d)\n", VMX_EXIT_XSETBV);
      break;
    case VMX_EXIT_APIC_WRITE:
      print ("VMX_EXIT_APIC_WRITE(%d)\n", VMX_EXIT_APIC_WRITE);
      break;
    case VMX_EXIT_RDRAND:
      print ("VMX_EXIT_RDRAND(%d)\n", VMX_EXIT_RDRAND);
      break;
    case VMX_EXIT_INVPCID:
      print ("VMX_EXIT_INVPCID(%d)\n", VMX_EXIT_INVPCID);
      break;
    case VMX_EXIT_VMFUNC:
      print ("VMX_EXIT_VMFUNC(%d)\n", VMX_EXIT_VMFUNC);
      break;
    case VMX_EXIT_ENCLS:
      print ("VMX_EXIT_ENCLS(%d)\n", VMX_EXIT_ENCLS);
      break;
    case VMX_EXIT_RDSEED:
      print ("VMX_EXIT_RDSEED(%d)\n", VMX_EXIT_RDSEED);
      break;
    case VMX_EXIT_PML_FULL:
      print ("VMX_EXIT_PML_FULL(%d)\n", VMX_EXIT_PML_FULL);
      break;
    case VMX_EXIT_XSAVES:
      print ("VMX_EXIT_XSAVES(%d)\n", VMX_EXIT_XSAVES);
      break;
    case VMX_EXIT_XRSTORS:
      print ("VMX_EXIT_XRSTORS(%d)\n", VMX_EXIT_XRSTORS);
      break;
    case VMX_EXIT_SPP_EVENT:
      print ("VMX_EXIT_SPP_EVENT(%d)\n", VMX_EXIT_SPP_EVENT);
      break;
    case VMX_EXIT_UMWAIT:
      print ("VMX_EXIT_UMWAIT(%d)\n", VMX_EXIT_UMWAIT);
      break;
    case VMX_EXIT_TPAUSE:
      print ("VMX_EXIT_TPAUSE(%d)\n", VMX_EXIT_TPAUSE);
      break;
    default:
      return -1;
  }

  return 0;
}

static uint AdjustMSR (uint msr, uint bits)
{
  uint64 v = __read_msr (msr);
  // bit == 0 in high word ==> must be zero
  bits &= (v >> 32);
  // bit == 1 in low word  ==> must be one
  bits |= (v & 0xffffffff);

  return bits;
}

#define CPUID_0x80000008_EAX_PA_BITS(EAX) (EAX & 0xff)        // 物理地址位宽.
#define CPUID_0x80000008_EAX_LA_BITS(EAX) ((EAX >> 8) & 0xff) // 线性地址位宽.

int vmx_start (vos_guest_t* guest)
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 0x80000008);
  uint8 pa_width = CPUID_0x80000008_EAX_PA_BITS (cpuid.eax);
  uint8 la_width = CPUID_0x80000008_EAX_LA_BITS (cpuid.eax);

  uint64 msr = __read_msr (IA32_VMX_BASIC);

  uint64 vmcs_size        = IA32_VMX_BASIC_VMCS_SIZE (msr);
  uint64 vmcs_revision_id = (msr & IA32_VMX_BASIC_VMCS_REVISION_IDENTIFIER_MASK);

  guest->host_vmcs = (void*)calloc (4096);

  *((uint64*)guest->host_vmcs) = vmcs_revision_id;

  print ("VMCS revision id : 0x%x\n", vmcs_revision_id);

  uint64 cr0 = __read_cr0 ();
  cr0 |= CR0_NE_MASK;
  __write_cr0 (cr0);

  uint64 cr4 = __read_cr4 ();
  cr4 |= CR4_VMXE_MASK;
  __write_cr4 (cr4);

  uint64 hostPA = VirtualAddressToPhysicalAddress ((uint64)guest->host_vmcs);
  __vmxon ((uint64)&hostPA);

  uint64 rflags = __rflags ();
  if ((rflags & FLAGS_CF_MASK) != 0)
  {
    puts ("vmxon failed.");
    return -1;
  }

  puts ("vmxon successful.");

  guest->guest_vmcs             = calloc (vmcs_size);
  *((uint64*)guest->guest_vmcs) = vmcs_revision_id;

  uint64 vmcsPA = VirtualAddressToPhysicalAddress ((uint64)guest->guest_vmcs);
  __vmclear ((uint64)&vmcsPA);

  __vmptrld ((uint64)&vmcsPA);

  gdtr_t gdtr;
  idtr_t idtr;
  __read_gdtr (&gdtr);
  __read_idtr (&idtr);

  ept_PML4E_t* ept_PA  = (ept_PML4E_t*)make_vmx_ept (guest->mem_page_count);
  guest->memmap_ptr    = ept_PA;
  EptPointer ptr       = {0};
  ptr.memory_type      = 6;
  ptr.page_walk_length = 4 - 1; // 4级页表
  ptr.pml4_address     = (guest->memmap_ptr >> 12);

  uint vmret = 0;
  // See: Definitions of Pin-Based VM-Execution Controls
  vmret |= __vmwrite (VMX_VMCS32_CTRL_PIN_EXEC, AdjustMSR (MSR_IA32_VMX_PINBASED_CTLS, 0));

  // See: Definitions of Primary Processor-Based VM-Execution Controls
  vmret |= __vmwrite (VMX_VMCS32_CTRL_PROC_EXEC, AdjustMSR (MSR_IA32_VMX_PROCBASED_CTLS, 1ull << 31));

  // See: Definitions of Secondary Processor-Based VM-Execution Controls
  vmret |= __vmwrite (VMX_VMCS32_CTRL_PROC_EXEC2, AdjustMSR (MSR_IA32_VMX_PROCBASED_CTLS2, 0b100100000000000001010)); // EPT
  vmret |= __vmwrite (VMX_VMCS32_CTRL_EXIT, AdjustMSR (MSR_IA32_VMX_EXIT_CTLS, VMX_EXIT_CTLS_HOST_ADDR_SPACE_SIZE));
  vmret |= __vmwrite (VMX_VMCS32_CTRL_ENTRY, AdjustMSR (MSR_IA32_VMX_ENTRY_CTLS, 0));
  vmret |= __vmwrite (VMX_VMCS64_CTRL_EPTP_FULL, ptr.bits);
  vmret |= __vmwrite (VMX_VMCS64_GUEST_PDPTE0_FULL, ept_PA[0].pdpt_page_PA << 12);
  //vmret |= __vmwrite (VMX_VMCS64_GUEST_PDPTE1_FULL, ept_PA[1].pdpt_page_PA << 12);
  //vmret |= __vmwrite (VMX_VMCS64_GUEST_PDPTE2_FULL, ept_PA[2].pdpt_page_PA << 12);
  //vmret |= __vmwrite (VMX_VMCS64_GUEST_PDPTE3_FULL, ept_PA[3].pdpt_page_PA << 12);

  // Host
  {
    vmret |= __vmwrite (VMX_VMCS16_HOST_ES_SEL, __read_es () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_CS_SEL, __read_cs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_SS_SEL, __read_ss () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_DS_SEL, __read_ds () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_FS_SEL, __read_fs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_GS_SEL, __read_gs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_HOST_TR_SEL, 8 & 0xfff8);

    vmret |= __vmwrite (VMX_VMCS_HOST_TR_BASE, 8);
    vmret |= __vmwrite (VMX_VMCS_HOST_GDTR_BASE, gdtr.base); // 居然没有设置limit的接口...
    vmret |= __vmwrite (VMX_VMCS_HOST_IDTR_BASE, idtr.base); // 居然没有设置limit的接口...

    //  vmret |= __vmwrite (VMX_VMCS_HOST_IA32_SYSENTER_CS_MSR, __read_msr(MSR_IA32_SYSENTER_CS));
    vmret |= __vmwrite (VMX_VMCS_HOST_SYSENTER_ESP, __read_msr (MSR_IA32_SYSENTER_ESP));
    vmret |= __vmwrite (VMX_VMCS_HOST_SYSENTER_EIP, __read_msr (MSR_IA32_SYSENTER_EIP));

    vmret |= __vmwrite (VMX_VMCS_HOST_CR0, __read_cr0 ());
    vmret |= __vmwrite (VMX_VMCS_HOST_CR3, __read_cr3 ());
    vmret |= __vmwrite (VMX_VMCS_HOST_CR4, __read_cr4 ());

    vmret |= __vmwrite (VMX_VMCS_HOST_RSP, 4096 + (uint64)calloc (4096)); // 返回Host时的栈底指针.栈是向下增长,所以把指针指向内存末尾.
    vmret |= __vmwrite (VMX_VMCS_HOST_RIP, (uint64)&__vmexit_handler);
  }

  // Guest, See: 24.4 GUEST-STATE AREA
  {
    bochs_break ();
    vmret |= __vmwrite (VMX_VMCS_GUEST_RFLAGS, __rflags ());
    vmret |= __vmwrite (VMX_VMCS_GUEST_CR0, __read_cr0 ());
    print ("VMX_VMCS_GUEST_CR0 : 0x%x\n", __vmread (VMX_VMCS_GUEST_CR0));

    vmret |= __vmwrite (VMX_VMCS_GUEST_CR3, (uint64)make_vmx_PML4E (guest, guest->mem_page_count));
    vmret |= __vmwrite (VMX_VMCS_GUEST_CR4, __read_cr4 ());
    vmret |= __vmwrite (VMX_VMCS_GUEST_DR7, 0x400);

    vmret |= __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_FULL, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS64_GUEST_VMCS_LINK_PTR_HIGH, 0xffffffff);

    vmret |= __vmwrite (VMX_VMCS32_CTRL_ENTRY, AdjustMSR (IA32_VMX_ENTRYCTLS, 0b1000000000)); // IA32e guest
    //    vmret |= __vmwrite(VMX_VMCS32_CTRL_ENTRY, 0b00000000000000000000001000000000);
    //    vmret |= __vmwrite(VMX_VMCS32_CTRL_ENTRY_MSR_LOAD_COUNT, 0b1000001000000000);

    vmret |= __vmwrite (VMX_VMCS16_GUEST_ES_SEL, __read_es () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_CS_SEL, __read_cs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_SS_SEL, __read_ss () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_DS_SEL, __read_ds () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_FS_SEL, __read_fs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_GS_SEL, __read_gs () & 0xfff8);
    vmret |= __vmwrite (VMX_VMCS16_GUEST_TR_SEL, 8 & 0xfff8);

    vmret |= __vmwrite (VMX_VMCS_GUEST_ES_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_CS_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_SS_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_DS_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_FS_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_GS_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_TR_BASE, 0);
    vmret |= __vmwrite (VMX_VMCS_GUEST_LDTR_BASE, 0);
    // vmret |= __vmwrite (VMX_VMCS_GUEST_GDTR_BASE, gdtr.base);
    // vmret |= __vmwrite (VMX_VMCS32_GUEST_GDTR_LIMIT, gdtr.limit);
    vmret |= __vmwrite (VMX_VMCS_GUEST_IDTR_BASE, idtr.base);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_IDTR_LIMIT, idtr.limit);
    make_vmx_gdt (guest);

    vmret |= __vmwrite (VMX_VMCS32_GUEST_ES_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_CS_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_SS_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_DS_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_FS_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_GS_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_LDTR_LIMIT, 0xffffffff);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_TR_LIMIT, 0xffffffff);

    vmret |= __vmwrite (VMX_VMCS32_GUEST_ES_ACCESS_RIGHTS, 0b1000000010010001);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_CS_ACCESS_RIGHTS, 0b1010000010011101);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_SS_ACCESS_RIGHTS, 0b1000000010010011);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_DS_ACCESS_RIGHTS, 0b1000000010010001);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_FS_ACCESS_RIGHTS, 0b1000000010010001);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_GS_ACCESS_RIGHTS, 0b1000000010010001);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_LDTR_ACCESS_RIGHTS, 0b1000000010000010);
    vmret |= __vmwrite (VMX_VMCS32_GUEST_TR_ACCESS_RIGHTS, 0b1000000010001011);

    { // temp
      uint8*        bin   = GuestPA_To_HostPA (guest, guest->code_address);
      unsigned char a[]   = {0xbf, 0xdb, 0x14, 0xcd, 0x14, 0xbe, 0x00, 0x10, 0x00, 0x00, 0xba, 0x00, 0x20, 0x00, 0x00, 0x0f, 0x01, 0xc1, 0xeb, 0xfe};
      unsigned int  a_len = 20;
      memcpy (bin, a, a_len);
      print ("VMX_VMCS_GUEST_CR3 : 0x%x\n", __vmread (VMX_VMCS_GUEST_CR3));
    }

    //vmret |= __vmwrite (VMX_VMCS_GUEST_RSP, 4096 + (uint64)calloc (4096)); // Guest 中的栈底指针.栈是向下增长,所以把指针指向内存末尾.
    //vmret |= __vmwrite (VMX_VMCS_GUEST_RIP, (uint64)&GuestEntry);
    vmret |= __vmwrite (VMX_VMCS_GUEST_RSP, VOS_PAGE_SIZE * guest->mem_page_count); // Guest 中的栈底指针.栈是向下增长,所以把指针指向内存末尾.
    vmret |= __vmwrite (VMX_VMCS_GUEST_RIP, guest->code_address);
  }

  if (vmret != 0)
  {
    print ("vmwrite fail : 0x%x\n", vmret);
    return -1;
  }
  print ("vmcs : 0x%x\n", guest->guest_vmcs);

  bochs_break ();

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
  vos_guest_t guest;
  memset8 (&guest, 0, sizeof (guest));
  guest.enable_shadow_hook = 1;
  //guest.mem_page_count     = 0x100000ull >> 12; // 1 MiB
  guest.mem_page_count = 0x10000ull >> 12; // 1 MiB

  if (check_vmx () != 0)
  {
    puts ("VMX check failed");
    return;
  }

  puts ("VMX check successful.");

  if (guest.enable_shadow_hook && check_ept () != 0)
  {
    puts ("EPT check failed");
    return;
  }
  puts ("EPT check successful.");

  vmx_start (&guest);
  vmx_stop ();
}
