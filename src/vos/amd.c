//
// Created by x7cc on 2020/4/14.
//

#include "vos/amd.h"
#include "vos/x86_64.h"
#include "vos/cpu.h"
#include "vos/stdio.h"

#define CPUID_80000001_SVM (uint32) (1 << 2)
#define CPUID_8000000A_SVML (uint32) (1 << 2)

#define MSR_VM_CR 0xC0010114
#define MSR_VM_CR_SVMDIS (1 << 4)

// See: 15.4 Enabling SVM
static uint check_svm ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 0x80000001);

  if ((cpuid.ecx & CPUID_80000001_SVM) == 0)
    return -1;

  uint64 vm_cr = __read_msr (MSR_VM_CR);
  if (vm_cr & MSR_VM_CR_SVMDIS)
  {
    __cpuid (&cpuid, 0x8000000A);

    if ((cpuid.edx & CPUID_8000000A_SVML) == 0)
    {
      puts ("the user must change a platform firmware setting to enable SVM");
    }
    else
    {
      puts ("SVMLock may be unlockable; consult platform firmware or TPM to obtain the key.");
    }
    return -1;
  }

  return 0;
}

void amd_entry ()
{
  if (check_svm () != 0)
  {
    puts ("check_svm failed");
    return;
  }

  puts ("check_svm successful.");
}
