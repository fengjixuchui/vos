//
// Created by x7cc on 2020/4/15.
//

#ifndef VOS2_X86_H
#define VOS2_X86_H

// clang-format off

/** @name Machine Specific Registers
 * @{
 */
/** Machine check address register (P5). */
#define MSR_P5_MC_ADDR                      (uint32)(0x00000000)
/** Machine check type register (P5). */
#define MSR_P5_MC_TYPE                      (uint32)(0x00000001)
/** Time Stamp Counter. */
#define MSR_IA32_TSC                        0x10
#define MSR_IA32_CESR                       (uint32)(0x00000011)
#define MSR_IA32_CTR0                       (uint32)(0x00000012)
#define MSR_IA32_CTR1                       (uint32)(0x00000013)

#define MSR_IA32_PLATFORM_ID                0x17

#ifndef MSR_IA32_APICBASE /* qemu cpu.h kludge */
# define MSR_IA32_APICBASE                  0x1b
/** Local APIC enabled. */
# define MSR_IA32_APICBASE_EN               RT_BIT_64(11)
/** X2APIC enabled (requires the EN bit to be set). */
# define MSR_IA32_APICBASE_EXTD             RT_BIT_64(10)
/** The processor is the boot strap processor (BSP). */
# define MSR_IA32_APICBASE_BSP              RT_BIT_64(8)
/** Minimum base address mask, consult CPUID leaf 0x80000008 for the actual
 *  width. */
# define MSR_IA32_APICBASE_BASE_MIN         (uint64)(0x0000000ffffff000)
/** The default physical base address of the APIC. */
# define MSR_IA32_APICBASE_ADDR             (uint64)(0x00000000fee00000)
/** Gets the physical base address from the MSR. */
# define MSR_IA32_APICBASE_GET_ADDR(a_Msr)  ((a_Msr) & X86_PAGE_4K_BASE_MASK)
#endif

/** Undocumented intel MSR for reporting thread and core counts.
 * Judging from the XNU sources, it seems to be introduced in Nehalem. The
 * first 16 bits is the thread count. The next 16 bits the core count, except
 * on Westmere where it seems it's only the next 4 bits for some reason. */
#define MSR_CORE_THREAD_COUNT               0x35

/** CPU Feature control. */
#define MSR_IA32_FEATURE_CONTROL                      0x3A
/** Feature control - Lock MSR from writes (R/W0). */
#define MSR_IA32_FEATURE_CONTROL_LOCK                 RT_BIT_64(0)
/** Feature control - Enable VMX inside SMX operation (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_SMX_VMXON            RT_BIT_64(1)
/** Feature control - Enable VMX outside SMX operation (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_VMXON                RT_BIT_64(2)
/** Feature control - SENTER local functions enable (R/WL).  */
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_0    RT_BIT_64(8)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_1    RT_BIT_64(9)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_2    RT_BIT_64(10)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_3    RT_BIT_64(11)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_4    RT_BIT_64(12)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_5    RT_BIT_64(13)
#define MSR_IA32_FEATURE_CONTROL_SENTER_LOCAL_FN_6    RT_BIT_64(14)
/** Feature control - SENTER global enable (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_SENTER_GLOBAL_EN     RT_BIT_64(15)
/** Feature control - SGX launch control enable (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_SGX_LAUNCH_EN        RT_BIT_64(17)
/** Feature control - SGX global enable (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_SGX_GLOBAL_EN        RT_BIT_64(18)
/** Feature control - LMCE on (R/WL). */
#define MSR_IA32_FEATURE_CONTROL_LMCE                 RT_BIT_64(20)

/** Per-processor TSC adjust MSR. */
#define MSR_IA32_TSC_ADJUST                 0x3B

/** Spectre control register.
 * Logical processor scope. Reset value 0, unaffected by SIPI & INIT. */
#define MSR_IA32_SPEC_CTRL                  0x48
/** IBRS - Indirect branch restricted speculation. */
#define MSR_IA32_SPEC_CTRL_F_IBRS           RT_BIT_32(0)
/** STIBP - Single thread indirect branch predictors. */
#define MSR_IA32_SPEC_CTRL_F_STIBP          RT_BIT_32(1)

/** Prediction command register.
 * Write only, logical processor scope, no state since write only. */
#define MSR_IA32_PRED_CMD                   0x49
/** IBPB - Indirect branch prediction barrie when written as 1. */
#define MSR_IA32_PRED_CMD_F_IBPB            RT_BIT_32(0)

/** BIOS update trigger (microcode update). */
#define MSR_IA32_BIOS_UPDT_TRIG             0x79

/** BIOS update signature (microcode). */
#define MSR_IA32_BIOS_SIGN_ID               0x8B

/** SMM monitor control. */
#define MSR_IA32_SMM_MONITOR_CTL            0x9B
/** SMM control - Valid. */
#define MSR_IA32_SMM_MONITOR_VALID                  RT_BIT_64(0)
/** SMM control - VMXOFF unblocks SMI. */
#define MSR_IA32_SMM_MONITOR_VMXOFF_UNBLOCK_SMI     RT_BIT_64(2)
/** SMM control - MSEG base physical address. */
#define MSR_IA32_SMM_MONITOR_MSGEG_PHYSADDR(a)      (((a) >> 12) & (uint64)(0xfffff))

/** SMBASE - Base address of SMRANGE image (Read-only, SMM only). */
#define MSR_IA32_SMBASE                     0x9E

/** General performance counter no. 0. */
#define MSR_IA32_PMC0                       0xC1
/** General performance counter no. 1. */
#define MSR_IA32_PMC1                       0xC2
/** General performance counter no. 2. */
#define MSR_IA32_PMC2                       0xC3
/** General performance counter no. 3. */
#define MSR_IA32_PMC3                       0xC4
/** General performance counter no. 4. */
#define MSR_IA32_PMC4                       0xC5
/** General performance counter no. 5. */
#define MSR_IA32_PMC5                       0xC6
/** General performance counter no. 6. */
#define MSR_IA32_PMC6                       0xC7
/** General performance counter no. 7. */
#define MSR_IA32_PMC7                       0xC8

/** Nehalem power control. */
#define MSR_IA32_PLATFORM_INFO              0xCE

/** Get FSB clock status (Intel-specific). */
#define MSR_IA32_FSB_CLOCK_STS              0xCD

/** C-State configuration control. Intel specific: Nehalem, Sandy Bridge. */
#define MSR_PKG_CST_CONFIG_CONTROL          (uint32)(0x000000e2)

/** C0 Maximum Frequency Clock Count */
#define MSR_IA32_MPERF                      0xE7
/** C0 Actual Frequency Clock Count */
#define MSR_IA32_APERF                      0xE8

/** MTRR Capabilities. */
#define MSR_IA32_MTRR_CAP                   0xFE

/** Architecture capabilities (bugfixes). */
#define MSR_IA32_ARCH_CAPABILITIES          (uint32)(0x10a)
/** CPU is no subject to meltdown problems. */
#define MSR_IA32_ARCH_CAP_F_RDCL_NO         RT_BIT_32(0)
/** CPU has better IBRS and you can leave it on all the time. */
#define MSR_IA32_ARCH_CAP_F_IBRS_ALL        RT_BIT_32(1)
/** CPU has return stack buffer (RSB) override. */
#define MSR_IA32_ARCH_CAP_F_RSBO            RT_BIT_32(2)
/** Virtual machine monitors need not flush the level 1 data cache on VM entry.
 * This is also the case when MSR_IA32_ARCH_CAP_F_RDCL_NO is set. */
#define MSR_IA32_ARCH_CAP_F_VMM_NEED_NOT_FLUSH_L1D RT_BIT_32(3)
/** CPU does not suffer from MDS issues. */
#define MSR_IA32_ARCH_CAP_F_MDS_NO          RT_BIT_32(4)

/** Flush command register. */
#define MSR_IA32_FLUSH_CMD                  (uint32)(0x10b)
/** Flush the level 1 data cache when this bit is written. */
#define MSR_IA32_FLUSH_CMD_F_L1D            RT_BIT_32(0)

/** Cache control/info. */
#define MSR_BBL_CR_CTL3                     (uint32)(0x11e)

#ifndef MSR_IA32_SYSENTER_CS /* qemu cpu.h kludge */
/** SYSENTER_CS - the R0 CS, indirectly giving R0 SS, R3 CS and R3 DS.
 * R0 SS == CS + 8
 * R3 CS == CS + 16
 * R3 SS == CS + 24
 */
#define MSR_IA32_SYSENTER_CS                0x174
/** SYSENTER_ESP - the R0 ESP. */
#define MSR_IA32_SYSENTER_ESP               0x175
/** SYSENTER_EIP - the R0 EIP. */
#define MSR_IA32_SYSENTER_EIP               0x176
#endif

/** Machine Check Global Capabilities Register. */
#define MSR_IA32_MCG_CAP                    0x179
/** Machine Check Global Status Register. */
#define MSR_IA32_MCG_STATUS                 0x17A
/** Machine Check Global Control Register. */
#define MSR_IA32_MCG_CTRL                   0x17B

/** Page Attribute Table. */
#define MSR_IA32_CR_PAT                     0x277
/** Default PAT MSR value on processor powerup / reset (see Intel spec. 11.12.4
 *  "Programming the PAT", AMD spec. 7.8.2 "PAT Indexing") */
#define MSR_IA32_CR_PAT_INIT_VAL            (uint64)(0x0007040600070406)

/** Performance event select MSRs. (Intel only) */
#define MSR_IA32_PERFEVTSEL0                0x186
#define MSR_IA32_PERFEVTSEL1                0x187
#define MSR_IA32_PERFEVTSEL2                0x188
#define MSR_IA32_PERFEVTSEL3                0x189

/** Flexible ratio, seems to be undocumented, used by XNU (tsc.c).
 * The 16th bit whether flex ratio is being used, in which case bits 15:8
 * holds a ratio that Apple takes for TSC granularity.
 *
 * @note This MSR conflicts the P4 MSR_MCG_R12 register. */
#define MSR_FLEX_RATIO                      0x194
/** Performance state value and starting with Intel core more.
 * Apple uses the >=core features to determine TSC granularity on older CPUs. */
#define MSR_IA32_PERF_STATUS                0x198
#define MSR_IA32_PERF_CTL                   0x199
#define MSR_IA32_THERM_STATUS               0x19c

/** Offcore response event select registers. */
#define MSR_OFFCORE_RSP_0                   0x1a6
#define MSR_OFFCORE_RSP_1                   0x1a7

/** Enable misc. processor features (R/W). */
#define MSR_IA32_MISC_ENABLE                   0x1A0
/** Enable fast-strings feature (for REP MOVS and REP STORS). */
#define MSR_IA32_MISC_ENABLE_FAST_STRINGS      RT_BIT_64(0)
/** Automatic Thermal Control Circuit Enable (R/W). */
#define MSR_IA32_MISC_ENABLE_TCC               RT_BIT_64(3)
/** Performance Monitoring Available (R). */
#define MSR_IA32_MISC_ENABLE_PERF_MON          RT_BIT_64(7)
/** Branch Trace Storage Unavailable (R/O). */
#define MSR_IA32_MISC_ENABLE_BTS_UNAVAIL       RT_BIT_64(11)
/** Precise Event Based Sampling (PEBS) Unavailable (R/O). */
#define MSR_IA32_MISC_ENABLE_PEBS_UNAVAIL      RT_BIT_64(12)
/** Enhanced Intel SpeedStep Technology Enable (R/W). */
#define MSR_IA32_MISC_ENABLE_SST_ENABLE        RT_BIT_64(16)
/** If MONITOR/MWAIT is supported (R/W). */
#define MSR_IA32_MISC_ENABLE_MONITOR           RT_BIT_64(18)
/** Limit CPUID Maxval to 3 leafs (R/W). */
#define MSR_IA32_MISC_ENABLE_LIMIT_CPUID       RT_BIT_64(22)
/** When set to 1, xTPR messages are disabled (R/W). */
#define MSR_IA32_MISC_ENABLE_XTPR_MSG_DISABLE  RT_BIT_64(23)
/** When set to 1, the Execute Disable Bit feature (XD Bit) is disabled (R/W). */
#define MSR_IA32_MISC_ENABLE_XD_DISABLE        RT_BIT_64(34)

/** Trace/Profile Resource Control (R/W) */
#define MSR_IA32_DEBUGCTL                   (uint32)(0x000001d9)
/** Last branch record. */
#define MSR_IA32_DEBUGCTL_LBR               RT_BIT_64(0)
/** Branch trace flag (single step on branches). */
#define MSR_IA32_DEBUGCTL_BTF               RT_BIT_64(1)
/** Performance monitoring pin control (AMD only). */
#define MSR_IA32_DEBUGCTL_PB0               RT_BIT_64(2)
#define MSR_IA32_DEBUGCTL_PB1               RT_BIT_64(3)
#define MSR_IA32_DEBUGCTL_PB2               RT_BIT_64(4)
#define MSR_IA32_DEBUGCTL_PB3               RT_BIT_64(5)
/** Trace message enable (Intel only). */
#define MSR_IA32_DEBUGCTL_TR                RT_BIT_64(6)
/** Branch trace store (Intel only). */
#define MSR_IA32_DEBUGCTL_BTS               RT_BIT_64(7)
/** Branch trace interrupt (Intel only). */
#define MSR_IA32_DEBUGCTL_BTINT             RT_BIT_64(8)
/** Branch trace off in privileged code (Intel only). */
#define MSR_IA32_DEBUGCTL_BTS_OFF_OS        RT_BIT_64(9)
/** Branch trace off in user code (Intel only). */
#define MSR_IA32_DEBUGCTL_BTS_OFF_USER      RT_BIT_64(10)
/** Freeze LBR on PMI flag (Intel only). */
#define MSR_IA32_DEBUGCTL_FREEZE_LBR_ON_PMI       RT_BIT_64(11)
/** Freeze PERFMON on PMI flag (Intel only). */
#define MSR_IA32_DEBUGCTL_FREEZE_PERFMON_ON_PMI   RT_BIT_64(12)
/** Freeze while SMM enabled (Intel only). */
#define MSR_IA32_DEBUGCTL_FREEZE_WHILE_SMM_EM     RT_BIT_64(14)
/** Advanced debugging of RTM regions (Intel only). */
#define MSR_IA32_DEBUGCTL_RTM               RT_BIT_64(15)
/** Debug control MSR valid bits (Intel only). */
#define MSR_IA32_DEBUGCTL_VALID_MASK_INTEL  (  MSR_IA32_DEBUGCTL_LBR | MSR_IA32_DEBUGCTL_BTF   | MSR_IA32_DEBUGCTL_TR  \
                                             | MSR_IA32_DEBUGCTL_BTS | MSR_IA32_DEBUGCTL_BTINT | MSR_IA32_DEBUGCTL_BTS_OFF_OS \
                                             | MSR_IA32_DEBUGCTL_BTS_OFF_USER | MSR_IA32_DEBUGCTL_FREEZE_LBR_ON_PMI \
                                             | MSR_IA32_DEBUGCTL_FREEZE_PERFMON_ON_PMI | MSR_IA32_DEBUGCTL_FREEZE_WHILE_SMM_EM \
                                             | MSR_IA32_DEBUGCTL_RTM)

/** @name Last branch registers for P4 and Xeon, models 0 thru 2.
 * @{ */
#define MSR_P4_LASTBRANCH_0                 0x1db
#define MSR_P4_LASTBRANCH_1                 0x1dc
#define MSR_P4_LASTBRANCH_2                 0x1dd
#define MSR_P4_LASTBRANCH_3                 0x1de

/** LBR Top-of-stack MSR (index to most recent record). */
#define MSR_P4_LASTBRANCH_TOS               0x1da
/** @} */

/** @name Last branch registers for Core 2 and related Xeons.
 * @{ */
#define MSR_CORE2_LASTBRANCH_0_FROM_IP      0x40
#define MSR_CORE2_LASTBRANCH_1_FROM_IP      0x41
#define MSR_CORE2_LASTBRANCH_2_FROM_IP      0x42
#define MSR_CORE2_LASTBRANCH_3_FROM_IP      0x43

#define MSR_CORE2_LASTBRANCH_0_TO_IP        0x60
#define MSR_CORE2_LASTBRANCH_1_TO_IP        0x61
#define MSR_CORE2_LASTBRANCH_2_TO_IP        0x62
#define MSR_CORE2_LASTBRANCH_3_TO_IP        0x63

/** LBR Top-of-stack MSR (index to most recent record). */
#define MSR_CORE2_LASTBRANCH_TOS            0x1c9
/** @} */

/** @name Last branch registers.
 * @{ */
#define MSR_LASTBRANCH_0_FROM_IP            0x680
#define MSR_LASTBRANCH_1_FROM_IP            0x681
#define MSR_LASTBRANCH_2_FROM_IP            0x682
#define MSR_LASTBRANCH_3_FROM_IP            0x683
#define MSR_LASTBRANCH_4_FROM_IP            0x684
#define MSR_LASTBRANCH_5_FROM_IP            0x685
#define MSR_LASTBRANCH_6_FROM_IP            0x686
#define MSR_LASTBRANCH_7_FROM_IP            0x687
#define MSR_LASTBRANCH_8_FROM_IP            0x688
#define MSR_LASTBRANCH_9_FROM_IP            0x689
#define MSR_LASTBRANCH_10_FROM_IP           0x68a
#define MSR_LASTBRANCH_11_FROM_IP           0x68b
#define MSR_LASTBRANCH_12_FROM_IP           0x68c
#define MSR_LASTBRANCH_13_FROM_IP           0x68d
#define MSR_LASTBRANCH_14_FROM_IP           0x68e
#define MSR_LASTBRANCH_15_FROM_IP           0x68f
#define MSR_LASTBRANCH_16_FROM_IP           0x690
#define MSR_LASTBRANCH_17_FROM_IP           0x691
#define MSR_LASTBRANCH_18_FROM_IP           0x692
#define MSR_LASTBRANCH_19_FROM_IP           0x693
#define MSR_LASTBRANCH_20_FROM_IP           0x694
#define MSR_LASTBRANCH_21_FROM_IP           0x695
#define MSR_LASTBRANCH_22_FROM_IP           0x696
#define MSR_LASTBRANCH_23_FROM_IP           0x697
#define MSR_LASTBRANCH_24_FROM_IP           0x698
#define MSR_LASTBRANCH_25_FROM_IP           0x699
#define MSR_LASTBRANCH_26_FROM_IP           0x69a
#define MSR_LASTBRANCH_27_FROM_IP           0x69b
#define MSR_LASTBRANCH_28_FROM_IP           0x69c
#define MSR_LASTBRANCH_29_FROM_IP           0x69d
#define MSR_LASTBRANCH_30_FROM_IP           0x69e
#define MSR_LASTBRANCH_31_FROM_IP           0x69f

#define MSR_LASTBRANCH_0_TO_IP              0x6c0
#define MSR_LASTBRANCH_1_TO_IP              0x6c1
#define MSR_LASTBRANCH_2_TO_IP              0x6c2
#define MSR_LASTBRANCH_3_TO_IP              0x6c3
#define MSR_LASTBRANCH_4_TO_IP              0x6c4
#define MSR_LASTBRANCH_5_TO_IP              0x6c5
#define MSR_LASTBRANCH_6_TO_IP              0x6c6
#define MSR_LASTBRANCH_7_TO_IP              0x6c7
#define MSR_LASTBRANCH_8_TO_IP              0x6c8
#define MSR_LASTBRANCH_9_TO_IP              0x6c9
#define MSR_LASTBRANCH_10_TO_IP             0x6ca
#define MSR_LASTBRANCH_11_TO_IP             0x6cb
#define MSR_LASTBRANCH_12_TO_IP             0x6cc
#define MSR_LASTBRANCH_13_TO_IP             0x6cd
#define MSR_LASTBRANCH_14_TO_IP             0x6ce
#define MSR_LASTBRANCH_15_TO_IP             0x6cf
#define MSR_LASTBRANCH_16_TO_IP             0x6d0
#define MSR_LASTBRANCH_17_TO_IP             0x6d1
#define MSR_LASTBRANCH_18_TO_IP             0x6d2
#define MSR_LASTBRANCH_19_TO_IP             0x6d3
#define MSR_LASTBRANCH_20_TO_IP             0x6d4
#define MSR_LASTBRANCH_21_TO_IP             0x6d5
#define MSR_LASTBRANCH_22_TO_IP             0x6d6
#define MSR_LASTBRANCH_23_TO_IP             0x6d7
#define MSR_LASTBRANCH_24_TO_IP             0x6d8
#define MSR_LASTBRANCH_25_TO_IP             0x6d9
#define MSR_LASTBRANCH_26_TO_IP             0x6da
#define MSR_LASTBRANCH_27_TO_IP             0x6db
#define MSR_LASTBRANCH_28_TO_IP             0x6dc
#define MSR_LASTBRANCH_29_TO_IP             0x6dd
#define MSR_LASTBRANCH_30_TO_IP             0x6de
#define MSR_LASTBRANCH_31_TO_IP             0x6df

/** LBR Top-of-stack MSR (index to most recent record). */
#define MSR_LASTBRANCH_TOS                  0x1c9
/** @} */

/** Intel TSX (Transactional Synchronization Extensions) control MSR. */
#define MSR_IA32_TSX_CTRL                   0x122

/** Variable range MTRRs.
 * @{ */
#define MSR_IA32_MTRR_PHYSBASE0             0x200
#define MSR_IA32_MTRR_PHYSMASK0             0x201
#define MSR_IA32_MTRR_PHYSBASE1             0x202
#define MSR_IA32_MTRR_PHYSMASK1             0x203
#define MSR_IA32_MTRR_PHYSBASE2             0x204
#define MSR_IA32_MTRR_PHYSMASK2             0x205
#define MSR_IA32_MTRR_PHYSBASE3             0x206
#define MSR_IA32_MTRR_PHYSMASK3             0x207
#define MSR_IA32_MTRR_PHYSBASE4             0x208
#define MSR_IA32_MTRR_PHYSMASK4             0x209
#define MSR_IA32_MTRR_PHYSBASE5             0x20a
#define MSR_IA32_MTRR_PHYSMASK5             0x20b
#define MSR_IA32_MTRR_PHYSBASE6             0x20c
#define MSR_IA32_MTRR_PHYSMASK6             0x20d
#define MSR_IA32_MTRR_PHYSBASE7             0x20e
#define MSR_IA32_MTRR_PHYSMASK7             0x20f
#define MSR_IA32_MTRR_PHYSBASE8             0x210
#define MSR_IA32_MTRR_PHYSMASK8             0x211
#define MSR_IA32_MTRR_PHYSBASE9             0x212
#define MSR_IA32_MTRR_PHYSMASK9             0x213
/** @} */

/** Fixed range MTRRs.
 * @{ */
#define MSR_IA32_MTRR_FIX64K_00000          0x250
#define MSR_IA32_MTRR_FIX16K_80000          0x258
#define MSR_IA32_MTRR_FIX16K_A0000          0x259
#define MSR_IA32_MTRR_FIX4K_C0000           0x268
#define MSR_IA32_MTRR_FIX4K_C8000           0x269
#define MSR_IA32_MTRR_FIX4K_D0000           0x26a
#define MSR_IA32_MTRR_FIX4K_D8000           0x26b
#define MSR_IA32_MTRR_FIX4K_E0000           0x26c
#define MSR_IA32_MTRR_FIX4K_E8000           0x26d
#define MSR_IA32_MTRR_FIX4K_F0000           0x26e
#define MSR_IA32_MTRR_FIX4K_F8000           0x26f
/** @} */

/** MTRR Default Range. */
#define MSR_IA32_MTRR_DEF_TYPE              0x2FF

/** Global performance counter control facilities (Intel only). */
#define MSR_IA32_PERF_GLOBAL_STATUS         0x38E
#define MSR_IA32_PERF_GLOBAL_CTRL           0x38F
#define MSR_IA32_PERF_GLOBAL_OVF_CTRL       0x390

/** Precise Event Based sampling (Intel only). */
#define MSR_IA32_PEBS_ENABLE                0x3F1

#define MSR_IA32_MC0_CTL                    0x400
#define MSR_IA32_MC0_STATUS                 0x401

/** Basic VMX information. */
#define MSR_IA32_VMX_BASIC                  0x480
/** Allowed settings for pin-based VM execution controls. */
#define MSR_IA32_VMX_PINBASED_CTLS          0x481
/** Allowed settings for proc-based VM execution controls. */
#define MSR_IA32_VMX_PROCBASED_CTLS         0x482
/** Allowed settings for the VM-exit controls. */
#define MSR_IA32_VMX_EXIT_CTLS              0x483
/** Allowed settings for the VM-entry controls. */
#define MSR_IA32_VMX_ENTRY_CTLS             0x484
/** Misc VMX info. */
#define MSR_IA32_VMX_MISC                   0x485
/** Fixed cleared bits in CR0. */
#define MSR_IA32_VMX_CR0_FIXED0             0x486
/** Fixed set bits in CR0. */
#define MSR_IA32_VMX_CR0_FIXED1             0x487
/** Fixed cleared bits in CR4. */
#define MSR_IA32_VMX_CR4_FIXED0             0x488
/** Fixed set bits in CR4. */
#define MSR_IA32_VMX_CR4_FIXED1             0x489
/** Information for enumerating fields in the VMCS. */
#define MSR_IA32_VMX_VMCS_ENUM              0x48A
/** Allowed settings for secondary proc-based VM execution controls */
#define MSR_IA32_VMX_PROCBASED_CTLS2        0x48B
/** EPT capabilities. */
#define MSR_IA32_VMX_EPT_VPID_CAP           0x48C
/** Allowed settings of all pin-based VM execution controls. */
#define MSR_IA32_VMX_TRUE_PINBASED_CTLS     0x48D
/** Allowed settings of all proc-based VM execution controls. */
#define MSR_IA32_VMX_TRUE_PROCBASED_CTLS    0x48E
/** Allowed settings of all VMX exit controls. */
#define MSR_IA32_VMX_TRUE_EXIT_CTLS         0x48F
/** Allowed settings of all VMX entry controls. */
#define MSR_IA32_VMX_TRUE_ENTRY_CTLS        0x490
/** Allowed settings for the VM-function controls. */
#define MSR_IA32_VMX_VMFUNC                 0x491

/** Intel PT - Enable and control for trace packet generation. */
#define MSR_IA32_RTIT_CTL                   0x570

/** DS Save Area (R/W). */
#define MSR_IA32_DS_AREA                    0x600
/** Running Average Power Limit (RAPL) power units. */
#define MSR_RAPL_POWER_UNIT                 0x606
/** Package C3 Interrupt Response Limit. */
#define MSR_PKGC3_IRTL                      0x60a
/** Package C6/C7S Interrupt Response Limit 1. */
#define MSR_PKGC_IRTL1                      0x60b
/** Package C6/C7S Interrupt Response Limit 2. */
#define MSR_PKGC_IRTL2                      0x60c
/** Package C2 Residency Counter. */
#define MSR_PKG_C2_RESIDENCY                0x60d
/** PKG RAPL Power Limit Control. */
#define MSR_PKG_POWER_LIMIT                 0x610
/** PKG Energy Status. */
#define MSR_PKG_ENERGY_STATUS               0x611
/** PKG Perf Status. */
#define MSR_PKG_PERF_STATUS                 0x613
/** PKG RAPL Parameters. */
#define MSR_PKG_POWER_INFO                  0x614
/** DRAM RAPL Power Limit Control. */
#define MSR_DRAM_POWER_LIMIT                0x618
/** DRAM Energy Status. */
#define MSR_DRAM_ENERGY_STATUS              0x619
/** DRAM Performance Throttling Status. */
#define MSR_DRAM_PERF_STATUS                0x61b
/** DRAM RAPL Parameters. */
#define MSR_DRAM_POWER_INFO                 0x61c
/** Package C10 Residency Counter. */
#define MSR_PKG_C10_RESIDENCY               0x632
/** PP0 Energy Status. */
#define MSR_PP0_ENERGY_STATUS               0x639
/** PP1 Energy Status. */
#define MSR_PP1_ENERGY_STATUS               0x641
/** Turbo Activation Ratio. */
#define MSR_TURBO_ACTIVATION_RATIO          0x64c
/** Core Performance Limit Reasons. */
#define MSR_CORE_PERF_LIMIT_REASONS         0x64f

/** X2APIC MSR range start. */
#define MSR_IA32_X2APIC_START               0x800
/** X2APIC MSR -  APIC ID Register. */
#define MSR_IA32_X2APIC_ID                  0x802
/** X2APIC MSR -  APIC Version Register. */
#define MSR_IA32_X2APIC_VERSION             0x803
/** X2APIC MSR -  Task Priority Register. */
#define MSR_IA32_X2APIC_TPR                 0x808
/** X2APIC MSR -  Processor Priority register. */
#define MSR_IA32_X2APIC_PPR                 0x80A
/** X2APIC MSR -  End Of Interrupt register. */
#define MSR_IA32_X2APIC_EOI                 0x80B
/** X2APIC MSR -  Logical Destination Register. */
#define MSR_IA32_X2APIC_LDR                 0x80D
/** X2APIC MSR -  Spurious Interrupt Vector Register. */
#define MSR_IA32_X2APIC_SVR                 0x80F
/** X2APIC MSR -  In-service Register (bits 31:0). */
#define MSR_IA32_X2APIC_ISR0                0x810
/** X2APIC MSR -  In-service Register (bits 63:32). */
#define MSR_IA32_X2APIC_ISR1                0x811
/** X2APIC MSR -  In-service Register (bits 95:64). */
#define MSR_IA32_X2APIC_ISR2                0x812
/** X2APIC MSR -  In-service Register (bits 127:96). */
#define MSR_IA32_X2APIC_ISR3                0x813
/** X2APIC MSR -  In-service Register (bits 159:128). */
#define MSR_IA32_X2APIC_ISR4                0x814
/** X2APIC MSR -  In-service Register (bits 191:160). */
#define MSR_IA32_X2APIC_ISR5                0x815
/** X2APIC MSR -  In-service Register (bits 223:192). */
#define MSR_IA32_X2APIC_ISR6                0x816
/** X2APIC MSR -  In-service Register (bits 255:224). */
#define MSR_IA32_X2APIC_ISR7                0x817
/** X2APIC MSR -  Trigger Mode Register (bits 31:0). */
#define MSR_IA32_X2APIC_TMR0                0x818
/** X2APIC MSR -  Trigger Mode Register (bits 63:32). */
#define MSR_IA32_X2APIC_TMR1                0x819
/** X2APIC MSR -  Trigger Mode Register (bits 95:64). */
#define MSR_IA32_X2APIC_TMR2                0x81A
/** X2APIC MSR -  Trigger Mode Register (bits 127:96). */
#define MSR_IA32_X2APIC_TMR3                0x81B
/** X2APIC MSR -  Trigger Mode Register (bits 159:128). */
#define MSR_IA32_X2APIC_TMR4                0x81C
/** X2APIC MSR -  Trigger Mode Register (bits 191:160). */
#define MSR_IA32_X2APIC_TMR5                0x81D
/** X2APIC MSR -  Trigger Mode Register (bits 223:192). */
#define MSR_IA32_X2APIC_TMR6                0x81E
/** X2APIC MSR -  Trigger Mode Register (bits 255:224). */
#define MSR_IA32_X2APIC_TMR7                0x81F
/** X2APIC MSR -  Interrupt Request Register (bits 31:0). */
#define MSR_IA32_X2APIC_IRR0                0x820
/** X2APIC MSR -  Interrupt Request Register (bits 63:32). */
#define MSR_IA32_X2APIC_IRR1                0x821
/** X2APIC MSR -  Interrupt Request Register (bits 95:64). */
#define MSR_IA32_X2APIC_IRR2                0x822
/** X2APIC MSR -  Interrupt Request Register (bits 127:96). */
#define MSR_IA32_X2APIC_IRR3                0x823
/** X2APIC MSR -  Interrupt Request Register (bits 159:128). */
#define MSR_IA32_X2APIC_IRR4                0x824
/** X2APIC MSR -  Interrupt Request Register (bits 191:160). */
#define MSR_IA32_X2APIC_IRR5                0x825
/** X2APIC MSR -  Interrupt Request Register (bits 223:192). */
#define MSR_IA32_X2APIC_IRR6                0x826
/** X2APIC MSR -  Interrupt Request Register (bits 255:224). */
#define MSR_IA32_X2APIC_IRR7                0x827
/** X2APIC MSR -  Error Status Register. */
#define MSR_IA32_X2APIC_ESR                 0x828
/** X2APIC MSR - LVT CMCI Register. */
#define MSR_IA32_X2APIC_LVT_CMCI            0x82F
/** X2APIC MSR -  Interrupt Command Register. */
#define MSR_IA32_X2APIC_ICR                 0x830
/** X2APIC MSR -  LVT Timer Register. */
#define MSR_IA32_X2APIC_LVT_TIMER           0x832
/** X2APIC MSR -  LVT Thermal Sensor Register. */
#define MSR_IA32_X2APIC_LVT_THERMAL         0x833
/** X2APIC MSR -  LVT Performance Counter Register. */
#define MSR_IA32_X2APIC_LVT_PERF            0x834
/** X2APIC MSR -  LVT LINT0 Register. */
#define MSR_IA32_X2APIC_LVT_LINT0           0x835
/** X2APIC MSR -  LVT LINT1 Register. */
#define MSR_IA32_X2APIC_LVT_LINT1           0x836
/** X2APIC MSR -  LVT Error Register . */
#define MSR_IA32_X2APIC_LVT_ERROR           0x837
/** X2APIC MSR -  Timer Initial Count Register. */
#define MSR_IA32_X2APIC_TIMER_ICR           0x838
/** X2APIC MSR -  Timer Current Count Register. */
#define MSR_IA32_X2APIC_TIMER_CCR           0x839
/** X2APIC MSR -  Timer Divide Configuration Register. */
#define MSR_IA32_X2APIC_TIMER_DCR           0x83E
/** X2APIC MSR - Self IPI. */
#define MSR_IA32_X2APIC_SELF_IPI            0x83F
/** X2APIC MSR range end. */
#define MSR_IA32_X2APIC_END                 0x8FF
/** X2APIC MSR - LVT start range. */
#define MSR_IA32_X2APIC_LVT_START           MSR_IA32_X2APIC_LVT_TIMER
/** X2APIC MSR - LVT end range (inclusive). */
#define MSR_IA32_X2APIC_LVT_END             MSR_IA32_X2APIC_LVT_ERROR

/** K6 EFER - Extended Feature Enable Register. */
#define MSR_K6_EFER                         (uint32)(0xc0000080)
/** @todo document EFER */
/** Bit 0 - SCE - System call extensions (SYSCALL / SYSRET). (R/W) */
#define  MSR_K6_EFER_SCE                     RT_BIT_32(0)
/** Bit 8 - LME - Long mode enabled. (R/W) */
#define  MSR_K6_EFER_LME                     RT_BIT_32(8)
#define  MSR_K6_EFER_BIT_LME                 8 /**< Bit number of MSR_K6_EFER_LME */
/** Bit 10 - LMA - Long mode active. (R) */
#define  MSR_K6_EFER_LMA                     RT_BIT_32(10)
#define  MSR_K6_EFER_BIT_LMA                 10 /**< Bit number of MSR_K6_EFER_LMA */
/** Bit 11 - NXE - No-Execute Page Protection Enabled. (R/W) */
#define  MSR_K6_EFER_NXE                     RT_BIT_32(11)
#define  MSR_K6_EFER_BIT_NXE                 11 /**< Bit number of MSR_K6_EFER_NXE */
/** Bit 12 - SVME - Secure VM Extension Enabled. (R/W) */
#define  MSR_K6_EFER_SVME                    RT_BIT_32(12)
/** Bit 13 - LMSLE - Long Mode Segment Limit Enable. (R/W?) */
#define  MSR_K6_EFER_LMSLE                   RT_BIT_32(13)
/** Bit 14 - FFXSR - Fast FXSAVE / FXRSTOR (skip XMM*). (R/W) */
#define  MSR_K6_EFER_FFXSR                   RT_BIT_32(14)
/** Bit 15 - TCE - Translation Cache Extension. (R/W) */
#define  MSR_K6_EFER_TCE                     RT_BIT_32(15)
/** Bit 17 - MCOMMIT - Commit Stores to memory. (R/W) */
#define  MSR_K6_EFER_MCOMMIT                 RT_BIT_32(17)

/** K6 STAR - SYSCALL/RET targets. */
#define MSR_K6_STAR                         (uint32)(0xc0000081)
/** Shift value for getting the SYSRET CS and SS value. */
#define  MSR_K6_STAR_SYSRET_CS_SS_SHIFT     48
/** Shift value for getting the SYSCALL CS and SS value. */
#define  MSR_K6_STAR_SYSCALL_CS_SS_SHIFT    32
/** Selector mask for use after shifting. */
#define  MSR_K6_STAR_SEL_MASK               (uint32)(0xffff)
/** The mask which give the SYSCALL EIP. */
#define  MSR_K6_STAR_SYSCALL_EIP_MASK       (uint32)(0xffffffff)
/** K6 WHCR - Write Handling Control Register. */
#define MSR_K6_WHCR                         (uint32)(0xc0000082)
/** K6 UWCCR - UC/WC Cacheability Control Register. */
#define MSR_K6_UWCCR                        (uint32)(0xc0000085)
/** K6 PSOR - Processor State Observability Register. */
#define MSR_K6_PSOR                         (uint32)(0xc0000087)
/** K6 PFIR - Page Flush/Invalidate Register. */
#define MSR_K6_PFIR                         (uint32)(0xc0000088)

/** Performance counter MSRs. (AMD only) */
#define MSR_K7_EVNTSEL0                     (uint32)(0xc0010000)
#define MSR_K7_EVNTSEL1                     (uint32)(0xc0010001)
#define MSR_K7_EVNTSEL2                     (uint32)(0xc0010002)
#define MSR_K7_EVNTSEL3                     (uint32)(0xc0010003)
#define MSR_K7_PERFCTR0                     (uint32)(0xc0010004)
#define MSR_K7_PERFCTR1                     (uint32)(0xc0010005)
#define MSR_K7_PERFCTR2                     (uint32)(0xc0010006)
#define MSR_K7_PERFCTR3                     (uint32)(0xc0010007)

/** K8 LSTAR - Long mode SYSCALL target (RIP). */
#define MSR_K8_LSTAR                        (uint32)(0xc0000082)
/** K8 CSTAR - Compatibility mode SYSCALL target (RIP). */
#define MSR_K8_CSTAR                        (uint32)(0xc0000083)
/** K8 SF_MASK - SYSCALL flag mask. (aka SFMASK) */
#define MSR_K8_SF_MASK                      (uint32)(0xc0000084)
/** K8 FS.base - The 64-bit base FS register. */
#define MSR_K8_FS_BASE                      (uint32)(0xc0000100)
/** K8 GS.base - The 64-bit base GS register. */
#define MSR_K8_GS_BASE                      (uint32)(0xc0000101)
/** K8 KernelGSbase - Used with SWAPGS. */
#define MSR_K8_KERNEL_GS_BASE               (uint32)(0xc0000102)
/** K8 TSC_AUX - Used with RDTSCP. */
#define MSR_K8_TSC_AUX                      (uint32)(0xc0000103)
#define MSR_K8_SYSCFG                       (uint32)(0xc0010010)
#define MSR_K8_HWCR                         (uint32)(0xc0010015)
#define MSR_K8_IORRBASE0                    (uint32)(0xc0010016)
#define MSR_K8_IORRMASK0                    (uint32)(0xc0010017)
#define MSR_K8_IORRBASE1                    (uint32)(0xc0010018)
#define MSR_K8_IORRMASK1                    (uint32)(0xc0010019)
#define MSR_K8_TOP_MEM1                     (uint32)(0xc001001a)
#define MSR_K8_TOP_MEM2                     (uint32)(0xc001001d)
/** North bridge config? See BIOS & Kernel dev guides for
 * details. */
#define MSR_K8_NB_CFG                       (uint32)(0xc001001f)

/** Hypertransport interrupt pending register.
 * "BIOS and Kernel Developer's Guide for AMD NPT Family 0Fh Processors" */
#define MSR_K8_INT_PENDING                  (uint32)(0xc0010055)

/** SVM Control. */
#define MSR_K8_VM_CR                        (uint32)(0xc0010114)
/** Disables HDT (Hardware Debug Tool) and certain internal debug
 *  features. */
#define MSR_K8_VM_CR_DPD                    RT_BIT_32(0)
/** If set, non-intercepted INIT signals are converted to \#SX
 *  exceptions. */
#define MSR_K8_VM_CR_R_INIT                 RT_BIT_32(1)
/** Disables A20 masking.  */
#define MSR_K8_VM_CR_DIS_A20M               RT_BIT_32(2)
/** Lock bit for this MSR controlling bits 3 (LOCK) and 4 (SVMDIS). */
#define MSR_K8_VM_CR_LOCK                   RT_BIT_32(3)
/** SVM disable. When set, writes to EFER.SVME are treated as MBZ. When
 *  clear, EFER.SVME can be written normally. */
#define MSR_K8_VM_CR_SVM_DISABLE            RT_BIT_32(4)

#define MSR_K8_IGNNE                        (uint32)(0xc0010115)
#define MSR_K8_SMM_CTL                      (uint32)(0xc0010116)
/** SVM - VM_HSAVE_PA - Physical address for saving and restoring
 *                      host state during world switch. */
#define MSR_K8_VM_HSAVE_PA                  (uint32)(0xc0010117)

/** @} */
// clang-format on

#endif //VOS2_X86_H
