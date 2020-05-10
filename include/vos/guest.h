//
// Created by x7cc on 2020/4/24.
//

#ifndef VOS_GUEST_H
#define VOS_GUEST_H

#include "vos/vos.h"
#include "vos/types.h"

typedef struct vos_guest_s
{
  IN vos_uint num_threads;
  IN vos_uint num_cpu_cores;
  IN vos_uint num_mem_pages;
  IN vos_uint enable_debug;
  IN vos_uint enable_physical_address_translation;
  OUT vos_uint physical_address_translation_pointer;
  OUT vos_uint pml4_HPA;
  OUT vos_uint memory_base_tmep;
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
vos_uint guest_malloc (vos_guest_t* guest, vos_uint size);

#endif //VOS_GUEST_H
