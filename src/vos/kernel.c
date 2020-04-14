#include "vos/types.h"
#include "bochs/bochs.h"

typedef struct
{
  uint32 eax;
  uint32 ebx;
  uint32 ecx;
  uint32 edx;
} cpuid_t;

uint64 __readcr0 ();
uint64 __readcr3 ();
uint64 __readcr4 ();
void   __writecr0 (uint64 value);
void   __writecr3 (uint64 value);
void   __writecr4 (uint64 value);
void   __cpuid (cpuid_t* out, uint64 id);
uint64 __rdmsr (uint64 id);
void   __wrmsr (uint64 id, uint64 value);
void   __vmptrld ();
void   __vmptrst ();
void   __vmclear ();
void   __vmread ();
void   __vmwrite ();
void   __vmlaunch ();
void   __vmresume ();
void   __vmxoff ();
void   __vmxon ();
void   __invept ();
void   __invvpid ();
void   __vmcall ();
void   __vmfunc ();

/*  Some screen stuff. */
/*  The number of columns. */
#define COLUMNS 80
/*  The number of lines. */
#define LINES 25
/*  The attribute of an character. */
#define ATTRIBUTE 7
/*  The video memory address. */
#define VIDEO 0xB8000
/*  Variables. */
/*  Save the X position. */
static int xpos;
/*  Save the Y position. */
static int ypos;
/*  Point to the video memory. */
static unsigned char* video;

void putc (char c)
{
  if (c == '\n')
  {
  newline:
    xpos = 0;
    ypos++;
    if (ypos >= LINES)
    {
      int FULLSIZE = COLUMNS * LINES * 2;

      ypos = LINES - 1;
      memcpy (video, video + (2 * COLUMNS), FULLSIZE - 2 * COLUMNS);
      memset (video + FULLSIZE - 2 * COLUMNS, 0, COLUMNS * 2);
    }
    return;
  }

  *(video + (xpos + ypos * COLUMNS) * 2)     = c;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

void puts (const char* s)
{
  int i = 0;
  while (s[i])
  {
    putc (s[i]);
    ++i;
  }
  putc ('\n');
}

void cls ()
{
  video        = (unsigned char*)VIDEO;
  int fg_color = 7; // 字体RGB
  int bg_color = 1; // 背景RGB
  for (int i = 0; i < 2000; ++i)
  {
    video[i * 2]     = 0;
    video[i * 2 + 1] = (bg_color << 4) | fg_color; // RGB
  }
}

#define IA32_FEATURE_CONTROL 0x3A
#define IA32_VMX_BASIC 0x480

void intel_entry ()
{
  cpuid_t cpuid;
  __cpuid (&cpuid, 1);
  if (cpuid.eax & (1 << 5) == 0)
  {
    puts ("not support vmx");
    return;
  }

  bochs_break ();
  uint64 msr = __rdmsr (IA32_FEATURE_CONTROL);
  if (msr & (1 << 2) == 0)
  {
    puts ("vmxon");
    return;
  }

  puts ("intel cpu check completed.");
}

void amd_entry ()
{
}

int x86_64_main ()
{
  char vendor[13] = {0};

  cls ();

  puts ("hello! welcome to my vos project.");

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);
  uint64 msr = __rdmsr (1);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  if (memcmp (vendor, "GenuineIntel", 12) == 0)
    intel_entry ();
  else if (memcpy (vendor, "AuthenticAMD", 12) == 0)
    amd_entry ();
  else
    puts ("Unsupported CPU vendors.");

  return 0;
}
