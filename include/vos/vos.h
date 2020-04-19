//
// Created by x7cc on 2020/4/13.
//

#ifndef VOS_VOS_H
#define VOS_VOS_H

// clang-format off
#define VOS_HOST_PML4E_PA  ((uint32)0x01000000)
#define VOS_GUEST_PML4E_PA ((uint32)0x02000000)
#define VOS_GDT64_PA       ((uint32)0x00000010)
#define VOS_GDT_PTR_PA     ((uint32)0x00000000)
#define VOS_VMX_HOST_PA    ((uint64)0x00A00000)
#define VOS_VMX_VMCS_PA    ((uint64)0x00A01000)
// clang-format on

#endif //VOS_VOS_H
