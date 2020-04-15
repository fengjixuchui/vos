//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_INTEL_H
#define VOS_INTEL_H

// clang-format off

#define CR0_PE_MASK         (1 <<  0)
#define CR0_MP_MASK         (1 <<  1)
#define CR0_EM_MASK         (1 <<  2)
#define CR0_TS_MASK         (1 <<  3)
#define CR0_ET_MASK         (1 <<  4)
#define CR0_NE_MASK         (1 <<  5)
#define CR0_WP_MASK         (1 << 16)
#define CR0_AM_MASK         (1 << 18)
#define CR0_NW_MASK         (1 << 29)
#define CR0_CD_MASK         (1 << 30)
#define CR0_PG_MASK         (1 << 31)

#define CR4_VME_MASK        (1 << 0)
#define CR4_PVI_MASK        (1 << 1)
#define CR4_TSD_MASK        (1 << 2)
#define CR4_DE_MASK         (1 << 3)
#define CR4_PSE_MASK        (1 << 4)
#define CR4_PAE_MASK        (1 << 5)
#define CR4_MCE_MASK        (1 << 6)
#define CR4_PGE_MASK        (1 << 7)
#define CR4_PCE_MASK        (1 << 8)
#define CR4_OSFXSR_MASK     (1 << 9)
#define CR4_OSXMMEXCPT_MASK (1 << 10)
#define CR4_UMIP_MASK       (1 << 11)
#define CR4_VMXE_MASK       (1 << 13)
#define CR4_SMXE_MASK       (1 << 14)
#define CR4_FSGSBASE_MASK   (1 << 16)
#define CR4_PCIDE_MASK      (1 << 17)
#define CR4_OSXSAVE_MASK    (1 << 18)
#define CR4_SMEP_MASK       (1 << 20)
#define CR4_SMAP_MASK       (1 << 21)
#define CR4_PKE_MASK        (1 << 22)
#define CR4_CET_MASK        (1 << 23)

#define IA32_FEATURE_CONTROL        0x3A
#define IA32_SYSENTER_CS            0x174
#define IA32_SYSENTER_ESP           0x175
#define IA32_SYSENTER_EIP           0x176
#define IA32_VMX_BASIC              0x480
#define IA32_VMX_PINBASEDCTLS       0x481
#define IA32_VMX_PROCBASEDCTLS      0x482
#define IA32_VMX_EXITCTLS           0x483
#define IA32_VMX_ENTRYCTLS          0x484
#define IA32_VMX_MISC               0x485
#define IA32_VMX_CR0FIXED0          0x486
#define IA32_VMX_CR0FIXED1          0x487
#define IA32_VMX_CR4FIXED0          0x488
#define IA32_VMX_CR4FIXED1          0x489
#define IA32_VMX_VMCSENUM           0x48A
#define IA32_VMX_PROCBASEDCTLS2     0x48B
#define IA32_VMX_EPTVPIDCAP         0x48C
#define IA32_VMX_TRUEPINBASEDCTLS   0x48D
#define IA32_VMX_TRUEPROCBASEDCTLS  0x48E
#define IA32_VMX_TRUEEXITCTLS       0x48F
#define IA32_VMX_TRUEENTRYCTLS      0x490
#define IA32_VMX_VMFUNC             0x491

#define IA32_FEATURE_CONTROL_LOCK_MASK   (1 << 0)
#define IA32_FEATURE_CONTROL_VMX_MASK    (1 << 2)

#define IA32_VMX_BASIC_VMCS_SIZE(v)              ((v >> 32) & 0b1111111111111)
#define IA32_VMX_BASIC_VMCS_REVISION_IDENTIFIER_MASK  (0b1111111111111111111111111111111)

#define EFER_SCE_MASK       (1 <<  0)
#define EFER_LME_MASK       (1 <<  8)
#define EFER_LMA_MASK       (1 << 10)
#define EFER_NXE_MASK       (1 << 11)
#define EFER_SVME_MASK      (1 << 12)
#define EFER_LMSLE_MASK     (1 << 13)
#define EFER_FFXSR_MASK     (1 << 14)
#define EFER_TCE_MASK       (1 << 15)

#define FLAGS_CF_MASK       (1 <<  0)
#define FLAGS_PF_MASK       (1 <<  2)
#define FLAGS_AF_MASK       (1 <<  4)
#define FLAGS_ZF_MASK       (1 <<  6)
#define FLAGS_SF_MASK       (1 <<  7)
#define FLAGS_TF_MASK       (1 <<  8)
#define FLAGS_IF_MASK       (1 <<  9)
#define FLAGS_DF_MASK       (1 << 10)
#define FLAGS_OF_MASK       (1 << 11)

// clang-format on

void intel_entry ();

#endif //VOS_INTEL_H
