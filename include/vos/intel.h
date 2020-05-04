//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_INTEL_H
#define VOS_INTEL_H

// clang-format off

#define CR0_PE_MASK         (1 <<  0)   // Protection Enable (bit 0 of CR0)
#define CR0_MP_MASK         (1 <<  1)   // Monitor Coprocessor (bit 1 of CR0)
#define CR0_EM_MASK         (1 <<  2)   // Emulation (bit 2 of CR0)
#define CR0_TS_MASK         (1 <<  3)   // Task Switched (bit 3 of CR0)
#define CR0_ET_MASK         (1 <<  4)   // Extension Type (bit 4 of CR0)
#define CR0_NE_MASK         (1 <<  5)   // Numeric Error (bit 5 of CR0)
#define CR0_WP_MASK         (1 << 16)   // Write Protect (bit 16 of CR0)
#define CR0_AM_MASK         (1 << 18)   // Alignment Mask (bit 18 of CR0)
#define CR0_NW_MASK         (1 << 29)   // Not Write-through (bit 29 of CR0)
#define CR0_CD_MASK         (1 << 30)   // Cache Disable (bit 30 of CR0)
#define CR0_PG_MASK         (1 << 31)   // Paging (bit 31 of CR0)

#define CR3_PWT_MASK        (1 << 3)    // Page-level Write-Through (bit 3 of CR3)
#define CR3_PCD_MASK        (1 << 4)    // Page-level Cache Disable (bit 4 of CR3)

#define CR4_VME_MASK        (1 << 0)    // Virtual-8086 Mode Extensions (bit 0 of CR4)
#define CR4_PVI_MASK        (1 << 1)    // Protected-Mode Virtual Interrupts (bit 1 of CR4)
#define CR4_TSD_MASK        (1 << 2)    // Time Stamp Disable (bit 2 of CR4)
#define CR4_DE_MASK         (1 << 3)    // Debugging Extensions (bit 3 of CR4)
#define CR4_PSE_MASK        (1 << 4)    // Page Size Extensions (bit 4 of CR4)
#define CR4_PAE_MASK        (1 << 5)    // Physical Address Extension (bit 5 of CR4)
#define CR4_MCE_MASK        (1 << 6)    // Machine-Check Enable (bit 6 of CR4)
#define CR4_PGE_MASK        (1 << 7)    // Page Global Enable (bit 7 of CR4)
#define CR4_PCE_MASK        (1 << 8)    // Performance-Monitoring Counter Enable (bit 8 of CR4)
#define CR4_OSFXSR_MASK     (1 << 9)    // Operating System Support for FXSAVE and FXRSTOR instructions (bit 9 of CR4)
#define CR4_OSXMMEXCPT_MASK (1 << 10)   // Operating System Support for Unmasked SIMD Floating-Point Exceptions (bit 10 of CR4)
#define CR4_UMIP_MASK       (1 << 11)   // User-Mode Instruction Prevention (bit 11 of CR4)
#define CR4_VMXE_MASK       (1 << 13)   // VMX-Enable Bit (bit 13 of CR4)
#define CR4_SMXE_MASK       (1 << 14)   // SMX-Enable Bit (bit 14 of CR4)
#define CR4_FSGSBASE_MASK   (1 << 16)   // FSGSBASE-Enable Bit (bit 16 of CR4)
#define CR4_PCIDE_MASK      (1 << 17)   // PCID-Enable Bit (bit 17 of CR4)
#define CR4_OSXSAVE_MASK    (1 << 18)   // XSAVE and Processor Extended States-Enable Bit (bit 18 of CR4)
#define CR4_SMEP_MASK       (1 << 20)   // SMEP-Enable Bit (bit 20 of CR4)
#define CR4_SMAP_MASK       (1 << 21)   // SMAP-Enable Bit (bit 21 of CR4)
#define CR4_PKE_MASK        (1 << 22)   // Protection-Key-Enable Bit (bit 22 of CR4)
#define CR4_CET_MASK        (1 << 23)   //

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
#define IA32_FEATURE_CONTROL_VMXON_MASK    (1 << 2)

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
