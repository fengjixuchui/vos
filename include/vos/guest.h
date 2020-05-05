//
// Created by x7cc on 2020/4/24.
//

#ifndef VOS_GUEST_H
#define VOS_GUEST_H

#include "vos/vos.h"
#include "vos/types.h"
#include "vmx.h"

typedef struct vos_guest
{
  struct vos_guest* next;
  IN uint mem_page_count;
  IN uint enable_debug;
  IN uint enable_physical_address_translation;
  OUT uint physical_address_translation_pointer;
  OUT uint pml4_HPA;
  OUT uint memory_base;
  union
  {
    IN void* host_vmcs;
    IN void* host_vmcb;
  };
  union
  {
    IN void* guest_vmcs;
    IN void* guest_vmcb;
  };
} vos_guest_t;

/// 在vmm中给guest申请一块内存.
/// \param guest
/// \param size
/// \return         Guest Physical Address
uint guest_malloc (vos_guest_t* guest, uint size);

#endif //VOS_GUEST_H
