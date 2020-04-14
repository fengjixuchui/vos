#include "vos/types.h"
#include "bochs/bochs.h"
#include "vos/vos.h"
#include "vos/cpu.h"
#include "vos/intel/intel.h"
#include "vos/amd/amd.h"
#include "vos/stdio.h"
#include "vos/asm.h"
#include "vos/memory.h"

int x86_64_main ()
{
  char vendor[13] = {0};

  cls ();

  puts ("hello! welcome to my vos project.");

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  if (memcmp (vendor, "GenuineIntel", 12) == 0)
    intel_entry ();
  else if (memcmp (vendor, "AuthenticAMD", 12) == 0)
    amd_entry ();
  else
    puts ("Unsupported CPU vendors.");

  return 0;
}
