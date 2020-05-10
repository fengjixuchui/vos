#include "vos/types.h"
#include "vos/debug.h"
#include "vos/vos.h"
#include "vos/x86.h"
#include "vos/intel.h"
#include "vos/amd.h"
#include "vos/stdio.h"
#include "vos/x86_64.h"
#include "vos/memory.h"
#include "grub/multiboot2.h"
#include "vos/gui/gui.h"

extern void interrupt_0 ();
extern void interrupt_1 ();
extern void interrupt_2 ();
extern void interrupt_3 ();
extern void interrupt_4 ();
extern void interrupt_5 ();
extern void interrupt_6 ();
extern void interrupt_7 ();
extern void interrupt_8 ();
extern void interrupt_9 ();
extern void interrupt_10 ();
extern void interrupt_11 ();
extern void interrupt_12 ();

typedef struct
{
  vos_uint16 offset_1;  // offset bits 0..15
  vos_uint16 selector;  // a code segment selector in GDT or LDT
  vos_uint8  ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
  vos_uint8  type_attr; // type and attributes
  vos_uint16 offset_2;  // offset bits 16..31
  vos_uint32 offset_3;  // offset bits 32..63
  vos_uint32 zero;      // reserved
} IDTDesc64;

void make_interrupt (void* idt, vos_uint idx, vos_uint64 addr, vos_uint dpl, vos_uint cs, vos_uint p)
{
  IDTDesc64* desc = (IDTDesc64*)idt + idx;
  desc->selector  = cs;
  desc->ist       = 0;
  desc->offset_1  = addr & 0xffff;
  desc->offset_2  = (addr >> 16) & 0xffff;
  desc->offset_3  = (addr >> 32) & 0xffffffff;
  desc->zero      = 0;
  desc->type_attr = 0b1110 | (dpl << 5) | (p << 7);
}

void init_idt ()
{
  vos_uint64* idt = (vos_uint64*)calloc (4096);
  make_interrupt ((void*)idt, 3, (vos_uint64)&interrupt_3, 0, __read_cs (), 1);
  idtr_t idtr;
  idtr.base  = idt;
  idtr.limit = 4096;
  // bochs_break ();
  __write_idtr (&idtr);
}

int x86_64_main (unsigned long magic, unsigned long addr)
{
  char vendor[13] = {0};

  struct multiboot_tag* tag;
  unsigned              size;
  cls ();

  /*  Am I booted by a Multiboot-compliant boot loader? */
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
  {
    print ("Invalid magic number: 0x%x\n", (unsigned)magic);
    return -1;
  }

  if (addr & 7)
  {
    print ("Unaligned mbi: 0x%x\n", addr);
    return -1;
  }

  size = *(unsigned*)addr;
  print ("Announced mbi size 0x%x\n", size);
  for (tag = (struct multiboot_tag*)(addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7)))
  {
    //    print ("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
    switch (tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        print ("Command line = %s\n",
               ((struct multiboot_tag_string*)tag)->string);
        break;
      case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
        print ("Boot loader name = %s\n",
               ((struct multiboot_tag_string*)tag)->string);
        break;
      case MULTIBOOT_TAG_TYPE_MODULE:
        print ("Module at 0x%x-0x%x. Command line %s\n",
               ((struct multiboot_tag_module*)tag)->mod_start,
               ((struct multiboot_tag_module*)tag)->mod_end,
               ((struct multiboot_tag_module*)tag)->cmdline);
        break;
      case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        print ("mem_lower = %uKB, mem_upper = %uKB\n",
               ((struct multiboot_tag_basic_meminfo*)tag)->mem_lower,
               ((struct multiboot_tag_basic_meminfo*)tag)->mem_upper);
        break;
      case MULTIBOOT_TAG_TYPE_BOOTDEV:
        print ("Boot device 0x%x,%u,%u\n",
               ((struct multiboot_tag_bootdev*)tag)->biosdev,
               ((struct multiboot_tag_bootdev*)tag)->slice,
               ((struct multiboot_tag_bootdev*)tag)->part);
        break;
      case MULTIBOOT_TAG_TYPE_MMAP:
      {
        multiboot_memory_map_t* mmap;

        print ("mmap\n");

        vos_uint64 addr, len = 0;
        for (mmap = ((struct multiboot_tag_mmap*)tag)->entries;
             (multiboot_uint8_t*)mmap < (multiboot_uint8_t*)tag + tag->size;
             mmap = (multiboot_memory_map_t*)((unsigned long)mmap + ((struct multiboot_tag_mmap*)tag)->entry_size))
        {
          print (" base_addr = 0x%x%x,"
                 " length = 0x%x%x, type = 0x%x\n",
                 (unsigned)(mmap->addr >> 32),
                 (unsigned)(mmap->addr & 0xffffffff),
                 (unsigned)(mmap->len >> 32),
                 (unsigned)(mmap->len & 0xffffffff),
                 (unsigned)mmap->type);
          // 找出最大的一块可用内存
          if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE && mmap->len > len)
          {
            addr = mmap->addr;
            len  = mmap->len;
          }
        }

        init_memory (0x2000000, len - (0x2000000 - addr));
      }
      break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
      {
        multiboot_uint32_t                color;
        unsigned                          i;
        struct multiboot_tag_framebuffer* tagfb = (struct multiboot_tag_framebuffer*)tag;
        void*                             fb    = (void*)(unsigned long)tagfb->common.framebuffer_addr;

        switch (tagfb->common.framebuffer_type)
        {
          case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
          {
            unsigned                best_distance, distance;
            struct multiboot_color* palette;

            palette = tagfb->framebuffer_palette;

            color         = 0;
            best_distance = 4 * 256 * 256;

            for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
            {
              distance = (0xff - palette[i].blue)
                           * (0xff - palette[i].blue)
                         + palette[i].red * palette[i].red
                         + palette[i].green * palette[i].green;
              if (distance < best_distance)
              {
                color         = i;
                best_distance = distance;
              }
            }
          }
          break;

          case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
            color = ((1 << tagfb->framebuffer_blue_mask_size) - 1)
                    << tagfb->framebuffer_blue_field_position;
            // for (int x = 0; x < tagfb->common.framebuffer_width; ++x)
            // {
            //   for (int y = 0; y < tagfb->common.framebuffer_height; ++y)
            //   {
            //     uint8* pixel = fb + (y * tagfb->common.framebuffer_pitch) + (x * 4);
            //     pixel[0]     = 0xF0; // B
            //     pixel[1]     = 0xF0; // G
            //     pixel[2]     = 0xF0; // R?
            //     pixel[3]     = 255;
            //   }
            // }
            gui_init (fb, tagfb->common.framebuffer_pitch, tagfb->common.framebuffer_width, tagfb->common.framebuffer_height, 4);
            window_new (100, 100, 320, 240);
            break;

          case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
            color = 23 << 8;
            break;

          default:
            color = 0xffffffff;
            break;
        }

        print ("framebuffer_red_mask_size : %x, framebuffer_red_field_position : %d\n", tagfb->framebuffer_red_mask_size, tagfb->framebuffer_red_field_position);
        print ("framebuffer_green_mask_size : %x, framebuffer_green_field_position : %d\n", tagfb->framebuffer_green_mask_size, tagfb->framebuffer_green_field_position);
        print ("framebuffer_blue_mask_size : %x, framebuffer_blue_field_position : %d\n", tagfb->framebuffer_blue_mask_size, tagfb->framebuffer_blue_field_position);
        print ("framebuffer_addr : %x, framebuffer_type : %d\n", tagfb->common.framebuffer_addr, tagfb->common.framebuffer_type);
        print ("framebuffer_width : %d, framebuffer_height : %d\n", tagfb->common.framebuffer_width, tagfb->common.framebuffer_height);
        print ("framebuffer_bpp : %d, framebuffer_pitch : %d\n", tagfb->common.framebuffer_bpp, tagfb->common.framebuffer_pitch);

        for (int y = 0; y < tagfb->common.framebuffer_height; ++y)
        {
          for (int x = 0; x < tagfb->common.framebuffer_width; ++x)
          {
            switch (tagfb->common.framebuffer_bpp)
            {
              case 8:
              {
                multiboot_uint8_t* pixel = fb + tagfb->common.framebuffer_pitch * y + x;
                //*pixel                   = color;
              }
              break;
              case 15:
              case 16:
              {
                multiboot_uint16_t* pixel = fb + tagfb->common.framebuffer_pitch * y + 2 * x;
                //*pixel                    = color;
              }
              break;
              case 24:
              {
                multiboot_uint32_t* pixel = fb + tagfb->common.framebuffer_pitch * y + 3 * x;
                //*pixel                    = (color & 0xffffff) | (*pixel & 0xff000000);
              }
              break;

              case 32:
              {
                multiboot_uint32_t* pixel = fb + tagfb->common.framebuffer_pitch * y + 4 * x;
                //*pixel                    = color;
              }
              break;
            }
          }
        }

        break;
      }
      case MULTIBOOT_TAG_TYPE_EFI32:
      {
        struct multiboot_tag_efi32* efi32 = (struct multiboot_tag_efi32*)tag;

        break;
      }
      case MULTIBOOT_TAG_TYPE_EFI64:
      {
        struct multiboot_tag_efi64* efi64 = (struct multiboot_tag_efi64*)tag;

        break;
      }
      case MULTIBOOT_TAG_TYPE_EFI_MMAP:
      {
        struct multiboot_tag_efi_mmap* mmap = (struct multiboot_tag_efi_mmap*)tag;

        break;
      }
    }
  }
  tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));

  puts ("hello! welcome to my vos project.");

  init_idt ();

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((vos_uint32*)vendor)[0] = cpuid.ebx;
  ((vos_uint32*)vendor)[1] = cpuid.edx;
  ((vos_uint32*)vendor)[2] = cpuid.ecx;
  puts (vendor);
  if (memcmp (vendor, "GenuineIntel", 12) == 0)
    intel_entry ();
  else if (memcmp (vendor, "AuthenticAMD", 12) == 0)
    amd_entry ();
  else
    puts ("Unsupported CPU vendors.");

  return 0;
}
