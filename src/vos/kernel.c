#include "vos/types.h"
#include "bochs/bochs.h"
#include "vos/vos.h"
#include "vos/cpu.h"
#include "vos/intel/intel.h"
#include "vos/amd/amd.h"
#include "vos/stdio.h"
#include "vos/asm.h"
#include "vos/memory.h"
#include "grub/multiboot2.h"

int x86_64_main (unsigned long magic, unsigned long addr)
{
  char vendor[13] = {0};

  cls ();

  puts ("hello! welcome to my vos project.");
  print ("magic : 0x%x, addr : 0x%x\n", magic, addr);
  struct multiboot_tag* tag;
  unsigned              size;

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

        for (mmap = ((struct multiboot_tag_mmap*)tag)->entries;
             (multiboot_uint8_t*)mmap < (multiboot_uint8_t*)tag + tag->size;
             mmap = (multiboot_memory_map_t*)((unsigned long)mmap + ((struct multiboot_tag_mmap*)tag)->entry_size))
          print (" base_addr = 0x%x%x,"
                 " length = 0x%x%x, type = 0x%x\n",
                 (unsigned)(mmap->addr >> 32),
                 (unsigned)(mmap->addr & 0xffffffff),
                 (unsigned)(mmap->len >> 32),
                 (unsigned)(mmap->len & 0xffffffff),
                 (unsigned)mmap->type);
      }
      break;
      case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
      {
        multiboot_uint32_t                color;
        unsigned                          i;
        struct multiboot_tag_framebuffer* tagfb
          = (struct multiboot_tag_framebuffer*)tag;
        void* fb = (void*)(unsigned long)tagfb->common.framebuffer_addr;

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
            for (int x = 0; x < tagfb->common.framebuffer_width; ++x)
            {
              for (int y = 0; y < tagfb->common.framebuffer_height; ++y)
              {
                uint8* pixel = fb + (y * tagfb->common.framebuffer_pitch) + (x * 4);
                pixel[0]     = 0xF0; // B
                pixel[1]     = 0xF0; // G
                pixel[2]     = 0xF0; // R?
                pixel[3]     = 255;
              }
            }

            break;

          case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
            color = '\\' | 0x0100;
            break;

          default:
            color = 0xffffffff;
            break;
        }

        print ("framebuffer_addr : %x, framebuffer_type : %d\n", tagfb->common.framebuffer_addr, tagfb->common.framebuffer_type);
        print ("framebuffer_width : %d, framebuffer_height : %d\n", tagfb->common.framebuffer_width, tagfb->common.framebuffer_height);
        print ("framebuffer_bpp : %d, framebuffer_pitch : %d\n", tagfb->common.framebuffer_bpp, tagfb->common.framebuffer_pitch);
        // bochs_break ();
        //        for (i = 0; i < tagfb->common.framebuffer_width && i < tagfb->common.framebuffer_height; i++)
        //        {
        //          switch (tagfb->common.framebuffer_bpp)
        //          {
        //            case 8:
        //            {
        //              multiboot_uint8_t* pixel = fb + tagfb->common.framebuffer_pitch * i + i;
        //              //*pixel                   = color;
        //            }
        //            break;
        //            case 15:
        //            case 16:
        //            {
        //              multiboot_uint16_t* pixel = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
        //              //*pixel                    = color;
        //            }
        //            break;
        //            case 24:
        //            {
        //              multiboot_uint32_t* pixel = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
        //              //*pixel                    = (color & 0xffffff) | (*pixel & 0xff000000);
        //            }
        //            break;
        //
        //            case 32:
        //            {
        //              bochs_break();
        //              multiboot_uint32_t* pixel = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
        //              //*pixel                    = color;
        //            }
        //            break;
        //          }
        //        }

        break;
      }
    }
  }
  tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));

  cpuid_t cpuid;
  __cpuid (&cpuid, 0);

  ((uint32*)vendor)[0] = cpuid.ebx;
  ((uint32*)vendor)[1] = cpuid.edx;
  ((uint32*)vendor)[2] = cpuid.ecx;
  puts (vendor);
  if (memcmp (vendor, "GenuineIntel", 12) == 0)
    intel_entry ();
  else if (memcmp (vendor, "AuthenticAMD", 12) == 0)
    amd_entry ();
  else
    puts ("Unsupported CPU vendors.");

  while (1)
    ;

  return 0;
}
