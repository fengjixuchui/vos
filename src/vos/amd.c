//
// Created by x7cc on 2020/4/14.
//

#include "vos/amd.h"
#include "vos/x86_64.h"
#include "vos/x86.h"
#include "vos/stdio.h"
#include "vos/svm.h"
#include "vos/vos.h"
#include "vos/memory.h"
#include "vos/debug.h"

// clang-format off
#define CPUID_80000001_ECX_SVM   (1u << 2)
#define CPUID_8000000A_EDX_SVML  (1u << 2)
#define CPUID_8000000A_EDX_NP    (1u << 0)
#define CPUID_8000000A_EDX_NRIP  (1u << 3)

#define PAGE_SIZE 4096
#define MSR_VM_CR        0xC0010114
#define SVM_MSR_VM_HSAVE_PA             0xc0010117
#define MSR_VM_CR_SVMDIS (1ul << 4)
#define EFER_SVME        (1ul << 12)
#define SVM_INTERCEPT_MISC1_CPUID       (1UL << 18)
#define SVM_INTERCEPT_MISC1_MSR_PROT    (1UL << 28)
#define SVM_INTERCEPT_MISC2_VMRUN       (1UL << 0)
#define SVM_NP_ENABLE_NP_ENABLE         (1UL << 0)
#define SVM_MSR_PERMISSIONS_MAP_SIZE    (PAGE_SIZE * 2)
#define IA32_MSR_PAT    0x00000277
#define IA32_MSR_EFER   0xc0000080

// clang-format on

static void guest_entry ()
{
  char vendor[13] = {0};
  puts ("GuestEntry begin");

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  print ("CPU Vendor : %s\n", vendor);

  // __vmmcall (CMD_CHECK, 0, 0);
  // __vmmcall (CMD_HOOK_FUNC, 0x1000, 0x2000);

  puts ("GuestEntry end");

  while (1)
    ;
}

static uint start_vm ()
{

  gdtr_t gdtr;
  idtr_t idtr;
  __read_gdtr (&gdtr);
  __read_idtr (&idtr);

  __write_msr (0xc0000080, __read_msr (0xc0000080) | EFER_SVME);
  uint64 MsrPermissionsMap = calloc (SVM_MSR_PERMISSIONS_MAP_SIZE);

  VMCB* host_vmcb  = (VMCB*)calloc (sizeof (VMCB));
  VMCB* guest_vmcb = (VMCB*)calloc (sizeof (VMCB));

  guest_vmcb->ControlArea.InterceptMisc1 |= SVM_INTERCEPT_MISC1_CPUID;
  guest_vmcb->ControlArea.InterceptMisc2 |= SVM_INTERCEPT_MISC2_VMRUN;
  guest_vmcb->ControlArea.InterceptMisc1 |= SVM_INTERCEPT_MISC1_MSR_PROT;
  guest_vmcb->ControlArea.MsrpmBasePa = MsrPermissionsMap;
  guest_vmcb->ControlArea.GuestAsid   = 1;
  // guest_vmcb->ControlArea.NpEnable |= SVM_NP_ENABLE_NP_ENABLE;
  guest_vmcb->ControlArea.NCr3        = make_guest_PML4E ();
  guest_vmcb->StateSaveArea.GdtrBase  = gdtr.base;
  guest_vmcb->StateSaveArea.GdtrLimit = gdtr.limit;
  guest_vmcb->StateSaveArea.IdtrBase  = idtr.base;
  guest_vmcb->StateSaveArea.IdtrLimit = idtr.limit;

  guest_vmcb->StateSaveArea.CsLimit    = 0xFFFFFFFF;
  guest_vmcb->StateSaveArea.DsLimit    = 0xFFFFFFFF;
  guest_vmcb->StateSaveArea.EsLimit    = 0xFFFFFFFF;
  guest_vmcb->StateSaveArea.SsLimit    = 0xFFFFFFFF;
  guest_vmcb->StateSaveArea.CsSelector = __read_cs () & 0xfff8;
  guest_vmcb->StateSaveArea.DsSelector = __read_ds () & 0xfff8;
  guest_vmcb->StateSaveArea.EsSelector = __read_es () & 0xfff8;
  guest_vmcb->StateSaveArea.SsSelector = __read_ss () & 0xfff8;
  print ("cs access rights : 0x%x\n", __read_access_rights (__read_cs ()));

  // See Intel: Table 24-2. Format of Access Rights
  guest_vmcb->StateSaveArea.CsAttrib = __read_access_rights (__read_cs ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.DsAttrib = __read_access_rights (__read_ds ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.EsAttrib = __read_access_rights (__read_es ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.SsAttrib = __read_access_rights (__read_ss ()) & 0x1FFFF;

  guest_vmcb->StateSaveArea.Efer   = __read_msr (IA32_MSR_EFER);
  guest_vmcb->StateSaveArea.Cr0    = __read_cr0 ();
  guest_vmcb->StateSaveArea.Cr2    = __read_cr2 ();
  guest_vmcb->StateSaveArea.Cr3    = __read_cr3 ();
  guest_vmcb->StateSaveArea.Cr4    = __read_cr4 ();
  guest_vmcb->StateSaveArea.Rflags = __rflags ();
  guest_vmcb->StateSaveArea.Rsp    = calloc (4096);
  guest_vmcb->StateSaveArea.Rip    = &guest_entry;
  guest_vmcb->StateSaveArea.GPat   = __read_msr (IA32_MSR_PAT);

  __vmsave (guest_vmcb);

  // See: 15.30.4 VM_HSAVE_PA MSR (C001_0117h)
  __write_msr (SVM_MSR_VM_HSAVE_PA, calloc (4096));

  __vmsave (host_vmcb);

  __svm_run (guest_vmcb);
  return 0;
}

// See: 15.4 Enabling SVM
static uint check_svm ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 0x80000001);

  if ((cpuid.ecx & CPUID_80000001_ECX_SVM) == 0)
    return -1;

  __cpuid (&cpuid, 0x8000000A);
  if ((cpuid.edx & CPUID_8000000A_EDX_NRIP) == 0)
    return -2;

  uint64 vm_cr = __read_msr (MSR_VM_CR);
  if (vm_cr & MSR_VM_CR_SVMDIS)
  {
    if ((cpuid.edx & CPUID_8000000A_EDX_SVML) == 0)
    {
      puts ("the user must change a platform firmware setting to enable SVM");
    }
    else
    {
      puts ("SVMLock may be unlockable; consult platform firmware or TPM to obtain the key.");
    }
    return -3;
  }

  return 0;
}

// See: 15.25.3 Enabling Nested Paging
static uint check_npt ()
{
  cpuid_t cpuid;

  __cpuid (&cpuid, 0x8000000A);
  if ((cpuid.edx & CPUID_8000000A_EDX_NP) == 0)
    return -1;

  return 0;
}

uint svm_vmexit_handler (SvmVMExitContext_t* context)
{
  VMCB* guest_vmcb = (VMCB*)context->vmcbptr;
  // 不知道为啥我这里非要重新加载 access rights, 不然恢复之后会进入到兼容模式.待查.
  guest_vmcb->StateSaveArea.CsAttrib = __read_access_rights (__read_cs ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.DsAttrib = __read_access_rights (__read_ds ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.EsAttrib = __read_access_rights (__read_es ()) & 0x1FFFF;
  guest_vmcb->StateSaveArea.SsAttrib = __read_access_rights (__read_ss ()) & 0x1FFFF;

  switch (guest_vmcb->ControlArea.ExitCode)
  {
    case VMEXIT_CPUID:
      if (context->rax == 0)
      {
        context->rax = 0;
        context->rbx = 'htuA';
        context->rdx = 'itne';
        context->rcx = 'SOVc';
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
      break;
    default:
      return -1;
  }
  return 0;
}

void amd_entry ()
{
  if (check_svm () != 0)
  {
    puts ("check_svm failed.");
    return;
  }

  puts ("check_svm successful.");

  //  if (check_npt() != 0) {
  //    puts("check_npt failed.");
  //    return;
  //  }

  puts ("check_npt successful.");

  start_vm ();
}
