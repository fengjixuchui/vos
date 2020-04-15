//
// Created by x7cc on 2020/4/14.
//

#ifndef VOS_VMX_H
#define VOS_VMX_H

// clang-format off

// =============
//  VMCS fields
// =============

/* VMCS 16-bit control fields */
/* binary 0000_00xx_xxxx_xxx0 */
#define VMCS_16BIT_CONTROL_VPID                            0x00000000 /* VPID */
#define VMCS_16BIT_CONTROL_POSTED_INTERRUPT_VECTOR         0x00000002 /* Posted Interrupts */
#define VMCS_16BIT_CONTROL_EPTP_INDEX                      0x00000004 /* #VE Exception */

/* VMCS 16-bit guest-state fields */
/* binary 0000_10xx_xxxx_xxx0 */
#define VMCS_16BIT_GUEST_ES_SELECTOR                       0x00000800
#define VMCS_16BIT_GUEST_CS_SELECTOR                       0x00000802
#define VMCS_16BIT_GUEST_SS_SELECTOR                       0x00000804
#define VMCS_16BIT_GUEST_DS_SELECTOR                       0x00000806
#define VMCS_16BIT_GUEST_FS_SELECTOR                       0x00000808
#define VMCS_16BIT_GUEST_GS_SELECTOR                       0x0000080A
#define VMCS_16BIT_GUEST_LDTR_SELECTOR                     0x0000080C
#define VMCS_16BIT_GUEST_TR_SELECTOR                       0x0000080E
#define VMCS_16BIT_GUEST_INTERRUPT_STATUS                  0x00000810 /* Virtual Interrupt Delivery */
#define VMCS_16BIT_GUEST_PML_INDEX                         0x00000812 /* Page Modification Logging */

/* VMCS 16-bit host-state fields */
/* binary 0000_11xx_xxxx_xxx0 */
#define VMCS_16BIT_HOST_ES_SELECTOR                        0x00000C00
#define VMCS_16BIT_HOST_CS_SELECTOR                        0x00000C02
#define VMCS_16BIT_HOST_SS_SELECTOR                        0x00000C04
#define VMCS_16BIT_HOST_DS_SELECTOR                        0x00000C06
#define VMCS_16BIT_HOST_FS_SELECTOR                        0x00000C08
#define VMCS_16BIT_HOST_GS_SELECTOR                        0x00000C0A
#define VMCS_16BIT_HOST_TR_SELECTOR                        0x00000C0C

/* VMCS 64-bit control fields */
/* binary 0010_00xx_xxxx_xxx0 */
#define VMCS_64BIT_CONTROL_IO_BITMAP_A                     0x00002000
#define VMCS_64BIT_CONTROL_IO_BITMAP_A_HI                  0x00002001
#define VMCS_64BIT_CONTROL_IO_BITMAP_B                     0x00002002
#define VMCS_64BIT_CONTROL_IO_BITMAP_B_HI                  0x00002003
#define VMCS_64BIT_CONTROL_MSR_BITMAPS                     0x00002004
#define VMCS_64BIT_CONTROL_MSR_BITMAPS_HI                  0x00002005
#define VMCS_64BIT_CONTROL_VMEXIT_MSR_STORE_ADDR           0x00002006
#define VMCS_64BIT_CONTROL_VMEXIT_MSR_STORE_ADDR_HI        0x00002007
#define VMCS_64BIT_CONTROL_VMEXIT_MSR_LOAD_ADDR            0x00002008
#define VMCS_64BIT_CONTROL_VMEXIT_MSR_LOAD_ADDR_HI         0x00002009
#define VMCS_64BIT_CONTROL_VMENTRY_MSR_LOAD_ADDR           0x0000200A
#define VMCS_64BIT_CONTROL_VMENTRY_MSR_LOAD_ADDR_HI        0x0000200B
#define VMCS_64BIT_CONTROL_EXECUTIVE_VMCS_PTR              0x0000200C
#define VMCS_64BIT_CONTROL_EXECUTIVE_VMCS_PTR_HI           0x0000200D
#define VMCS_64BIT_CONTROL_PML_ADDRESS                     0x0000200E /* Page Modification Logging */
#define VMCS_64BIT_CONTROL_PML_ADDRESS_HI                  0x0000200F
#define VMCS_64BIT_CONTROL_TSC_OFFSET                      0x00002010
#define VMCS_64BIT_CONTROL_TSC_OFFSET_HI                   0x00002011
#define VMCS_64BIT_CONTROL_VIRTUAL_APIC_PAGE_ADDR          0x00002012 /* TPR shadow */
#define VMCS_64BIT_CONTROL_VIRTUAL_APIC_PAGE_ADDR_HI       0x00002013
#define VMCS_64BIT_CONTROL_APIC_ACCESS_ADDR                0x00002014 /* APIC virtualization */
#define VMCS_64BIT_CONTROL_APIC_ACCESS_ADDR_HI             0x00002015
#define VMCS_64BIT_CONTROL_POSTED_INTERRUPT_DESC_ADDR      0x00002016 /* Posted Interrupts */
#define VMCS_64BIT_CONTROL_POSTED_INTERRUPT_DESC_ADDR_HI   0x00002017
#define VMCS_64BIT_CONTROL_VMFUNC_CTRLS                    0x00002018 /* VM Functions */
#define VMCS_64BIT_CONTROL_VMFUNC_CTRLS_HI                 0x00002019
#define VMCS_64BIT_CONTROL_EPTPTR                          0x0000201A /* EPT */
#define VMCS_64BIT_CONTROL_EPTPTR_HI                       0x0000201B
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP0                0x0000201C /* Virtual Interrupt Delivery */
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP0_HI             0x0000201D
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP1                0x0000201E
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP1_HI             0x0000201F
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP2                0x00002020
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP2_HI             0x00002021
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP3                0x00002022
#define VMCS_64BIT_CONTROL_EOI_EXIT_BITMAP3_HI             0x00002023
#define VMCS_64BIT_CONTROL_EPTP_LIST_ADDRESS               0x00002024 /* VM Functions - EPTP switching */
#define VMCS_64BIT_CONTROL_EPTP_LIST_ADDRESS_HI            0x00002025
#define VMCS_64BIT_CONTROL_VMREAD_BITMAP_ADDR              0x00002026 /* VMCS Shadowing */
#define VMCS_64BIT_CONTROL_VMREAD_BITMAP_ADDR_HI           0x00002027
#define VMCS_64BIT_CONTROL_VMWRITE_BITMAP_ADDR             0x00002028 /* VMCS Shadowing */
#define VMCS_64BIT_CONTROL_VMWRITE_BITMAP_ADDR_HI          0x00002029
#define VMCS_64BIT_CONTROL_VE_EXCEPTION_INFO_ADDR          0x0000202A /* #VE Exception */
#define VMCS_64BIT_CONTROL_VE_EXCEPTION_INFO_ADDR_HI       0x0000202B
#define VMCS_64BIT_CONTROL_XSS_EXITING_BITMAP              0x0000202C /* XSAVES */
#define VMCS_64BIT_CONTROL_XSS_EXITING_BITMAP_HI           0x0000202D
#define VMCS_64BIT_CONTROL_ENCLS_EXITING_BITMAP            0x0000202E /* ENCLS/SGX */
#define VMCS_64BIT_CONTROL_ENCLS_EXITING_BITMAP_HI         0x0000202F
#define VMCS_64BIT_CONTROL_SPPTP                           0x00002030 /* Sup-Page Write Protection */
#define VMCS_64BIT_CONTROL_SPPTP_HI                        0x00002031
#define VMCS_64BIT_CONTROL_TSC_MULTIPLIER                  0x00002032 /* TSC Scaling */
#define VMCS_64BIT_CONTROL_TSC_MULTIPLIER_HI               0x00002033

/* VMCS 64-bit read only data fields */
/* binary 0010_01xx_xxxx_xxx0 */
#define VMCS_64BIT_GUEST_PHYSICAL_ADDR                     0x00002400 /* EPT */
#define VMCS_64BIT_GUEST_PHYSICAL_ADDR_HI                  0x00002401

/* VMCS 64-bit guest state fields */
/* binary 0010_10xx_xxxx_xxx0 */
#define VMCS_64BIT_GUEST_LINK_POINTER                      0x00002800
#define VMCS_64BIT_GUEST_LINK_POINTER_HI                   0x00002801
#define VMCS_64BIT_GUEST_IA32_DEBUGCTL                     0x00002802
#define VMCS_64BIT_GUEST_IA32_DEBUGCTL_HI                  0x00002803
#define VMCS_64BIT_GUEST_IA32_PAT                          0x00002804 /* PAT */
#define VMCS_64BIT_GUEST_IA32_PAT_HI                       0x00002805
#define VMCS_64BIT_GUEST_IA32_EFER                         0x00002806 /* EFER */
#define VMCS_64BIT_GUEST_IA32_EFER_HI                      0x00002807
#define VMCS_64BIT_GUEST_IA32_PERF_GLOBAL_CTRL             0x00002808 /* Perf Global Ctrl */
#define VMCS_64BIT_GUEST_IA32_PERF_GLOBAL_CTRL_HI          0x00002809
#define VMCS_64BIT_GUEST_IA32_PDPTE0                       0x0000280A /* EPT */
#define VMCS_64BIT_GUEST_IA32_PDPTE0_HI                    0x0000280B
#define VMCS_64BIT_GUEST_IA32_PDPTE1                       0x0000280C
#define VMCS_64BIT_GUEST_IA32_PDPTE1_HI                    0x0000280D
#define VMCS_64BIT_GUEST_IA32_PDPTE2                       0x0000280E
#define VMCS_64BIT_GUEST_IA32_PDPTE2_HI                    0x0000280F
#define VMCS_64BIT_GUEST_IA32_PDPTE3                       0x00002810
#define VMCS_64BIT_GUEST_IA32_PDPTE3_HI                    0x00002811
#define VMCS_64BIT_GUEST_IA32_BNDCFGS                      0x00002812 /* MPX */
#define VMCS_64BIT_GUEST_IA32_BNDCFGS_HI                   0x00002813

/* VMCS 64-bit host state fields */
/* binary 0010_11xx_xxxx_xxx0 */
#define VMCS_64BIT_HOST_IA32_PAT                           0x00002C00 /* PAT */
#define VMCS_64BIT_HOST_IA32_PAT_HI                        0x00002C01
#define VMCS_64BIT_HOST_IA32_EFER                          0x00002C02 /* EFER */
#define VMCS_64BIT_HOST_IA32_EFER_HI                       0x00002C03
#define VMCS_64BIT_HOST_IA32_PERF_GLOBAL_CTRL              0x00002C04 /* Perf Global Ctrl */
#define VMCS_64BIT_HOST_IA32_PERF_GLOBAL_CTRL_HI           0x00002C05

/* VMCS 32_bit control fields */
/* binary 0100_00xx_xxxx_xxx0 */
#define VMCS_32BIT_CONTROL_PIN_BASED_EXEC_CONTROLS         0x00004000
#define VMCS_32BIT_CONTROL_PROCESSOR_BASED_VMEXEC_CONTROLS 0x00004002
#define VMCS_32BIT_CONTROL_EXECUTION_BITMAP                0x00004004
#define VMCS_32BIT_CONTROL_PAGE_FAULT_ERR_CODE_MASK        0x00004006
#define VMCS_32BIT_CONTROL_PAGE_FAULT_ERR_CODE_MATCH       0x00004008
#define VMCS_32BIT_CONTROL_CR3_TARGET_COUNT                0x0000400A
#define VMCS_32BIT_CONTROL_VMEXIT_CONTROLS                 0x0000400C
#define VMCS_32BIT_CONTROL_VMEXIT_MSR_STORE_COUNT          0x0000400E
#define VMCS_32BIT_CONTROL_VMEXIT_MSR_LOAD_COUNT           0x00004010
#define VMCS_32BIT_CONTROL_VMENTRY_CONTROLS                0x00004012
#define VMCS_32BIT_CONTROL_VMENTRY_MSR_LOAD_COUNT          0x00004014
#define VMCS_32BIT_CONTROL_VMENTRY_INTERRUPTION_INFO       0x00004016
#define VMCS_32BIT_CONTROL_VMENTRY_EXCEPTION_ERR_CODE      0x00004018
#define VMCS_32BIT_CONTROL_VMENTRY_INSTRUCTION_LENGTH      0x0000401A
#define VMCS_32BIT_CONTROL_TPR_THRESHOLD                   0x0000401C /* TPR shadow */
#define VMCS_32BIT_CONTROL_SECONDARY_VMEXEC_CONTROLS       0x0000401E
#define VMCS_32BIT_CONTROL_PAUSE_LOOP_EXITING_GAP          0x00004020 /* PAUSE loop exiting */
#define VMCS_32BIT_CONTROL_PAUSE_LOOP_EXITING_WINDOW       0x00004022 /* PAUSE loop exiting */

/* VMCS 32-bit read only data fields */
/* binary 0100_01xx_xxxx_xxx0 */
#define VMCS_32BIT_INSTRUCTION_ERROR                       0x00004400
#define VMCS_32BIT_VMEXIT_REASON                           0x00004402
#define VMCS_32BIT_VMEXIT_INTERRUPTION_INFO                0x00004404
#define VMCS_32BIT_VMEXIT_INTERRUPTION_ERR_CODE            0x00004406
#define VMCS_32BIT_IDT_VECTORING_INFO                      0x00004408
#define VMCS_32BIT_IDT_VECTORING_ERR_CODE                  0x0000440A
#define VMCS_32BIT_VMEXIT_INSTRUCTION_LENGTH               0x0000440C
#define VMCS_32BIT_VMEXIT_INSTRUCTION_INFO                 0x0000440E

/* VMCS 32-bit guest-state fields */
/* binary 0100_10xx_xxxx_xxx0 */
#define VMCS_32BIT_GUEST_ES_LIMIT                          0x00004800
#define VMCS_32BIT_GUEST_CS_LIMIT                          0x00004802
#define VMCS_32BIT_GUEST_SS_LIMIT                          0x00004804
#define VMCS_32BIT_GUEST_DS_LIMIT                          0x00004806
#define VMCS_32BIT_GUEST_FS_LIMIT                          0x00004808
#define VMCS_32BIT_GUEST_GS_LIMIT                          0x0000480A
#define VMCS_32BIT_GUEST_LDTR_LIMIT                        0x0000480C
#define VMCS_32BIT_GUEST_TR_LIMIT                          0x0000480E
#define VMCS_32BIT_GUEST_GDTR_LIMIT                        0x00004810
#define VMCS_32BIT_GUEST_IDTR_LIMIT                        0x00004812
#define VMCS_32BIT_GUEST_ES_ACCESS_RIGHTS                  0x00004814
#define VMCS_32BIT_GUEST_CS_ACCESS_RIGHTS                  0x00004816
#define VMCS_32BIT_GUEST_SS_ACCESS_RIGHTS                  0x00004818
#define VMCS_32BIT_GUEST_DS_ACCESS_RIGHTS                  0x0000481A
#define VMCS_32BIT_GUEST_FS_ACCESS_RIGHTS                  0x0000481C
#define VMCS_32BIT_GUEST_GS_ACCESS_RIGHTS                  0x0000481E
#define VMCS_32BIT_GUEST_LDTR_ACCESS_RIGHTS                0x00004820
#define VMCS_32BIT_GUEST_TR_ACCESS_RIGHTS                  0x00004822
#define VMCS_32BIT_GUEST_INTERRUPTIBILITY_STATE            0x00004824
#define VMCS_32BIT_GUEST_ACTIVITY_STATE                    0x00004826
#define VMCS_32BIT_GUEST_SMBASE                            0x00004828
#define VMCS_32BIT_GUEST_IA32_SYSENTER_CS_MSR              0x0000482A
#define VMCS_32BIT_GUEST_PREEMPTION_TIMER_VALUE            0x0000482E /* VMX preemption timer */

/* VMCS 32-bit host-state fields */
/* binary 0100_11xx_xxxx_xxx0 */
#define VMCS_32BIT_HOST_IA32_SYSENTER_CS_MSR               0x00004C00

/* VMCS natural width control fields */
/* binary 0110_00xx_xxxx_xxx0 */
#define VMCS_CONTROL_CR0_GUEST_HOST_MASK                   0x00006000
#define VMCS_CONTROL_CR4_GUEST_HOST_MASK                   0x00006002
#define VMCS_CONTROL_CR0_READ_SHADOW                       0x00006004
#define VMCS_CONTROL_CR4_READ_SHADOW                       0x00006006
#define VMCS_CR3_TARGET0                                   0x00006008
#define VMCS_CR3_TARGET1                                   0x0000600A
#define VMCS_CR3_TARGET2                                   0x0000600C
#define VMCS_CR3_TARGET3                                   0x0000600E

/* VMCS natural width read only data fields */
/* binary 0110_01xx_xxxx_xxx0 */
#define VMCS_VMEXIT_QUALIFICATION                          0x00006400
#define VMCS_IO_RCX                                        0x00006402
#define VMCS_IO_RSI                                        0x00006404
#define VMCS_IO_RDI                                        0x00006406
#define VMCS_IO_RIP                                        0x00006408
#define VMCS_GUEST_LINEAR_ADDR                             0x0000640A

/* VMCS natural width guest state fields */
/* binary 0110_10xx_xxxx_xxx0 */
#define VMCS_GUEST_CR0                                     0x00006800
#define VMCS_GUEST_CR3                                     0x00006802
#define VMCS_GUEST_CR4                                     0x00006804
#define VMCS_GUEST_ES_BASE                                 0x00006806
#define VMCS_GUEST_CS_BASE                                 0x00006808
#define VMCS_GUEST_SS_BASE                                 0x0000680A
#define VMCS_GUEST_DS_BASE                                 0x0000680C
#define VMCS_GUEST_FS_BASE                                 0x0000680E
#define VMCS_GUEST_GS_BASE                                 0x00006810
#define VMCS_GUEST_LDTR_BASE                               0x00006812
#define VMCS_GUEST_TR_BASE                                 0x00006814
#define VMCS_GUEST_GDTR_BASE                               0x00006816
#define VMCS_GUEST_IDTR_BASE                               0x00006818
#define VMCS_GUEST_DR7                                     0x0000681A
#define VMCS_GUEST_RSP                                     0x0000681C
#define VMCS_GUEST_RIP                                     0x0000681E
#define VMCS_GUEST_RFLAGS                                  0x00006820
#define VMCS_GUEST_PENDING_DBG_EXCEPTIONS                  0x00006822
#define VMCS_GUEST_IA32_SYSENTER_ESP_MSR                   0x00006824
#define VMCS_GUEST_IA32_SYSENTER_EIP_MSR                   0x00006826
#define VMCS_GUEST_IA32_S_CET                              0x00006828
#define VMCS_GUEST_SSP                                     0x0000682A
#define VMCS_GUEST_INTERRUPT_SSP_TABLE_ADDR                0x0000682C

/* VMCS natural width host state fields */
/* binary 0110_11xx_xxxx_xxx0 */
#define VMCS_HOST_CR0                                      0x00006C00
#define VMCS_HOST_CR3                                      0x00006C02
#define VMCS_HOST_CR4                                      0x00006C04
#define VMCS_HOST_FS_BASE                                  0x00006C06
#define VMCS_HOST_GS_BASE                                  0x00006C08
#define VMCS_HOST_TR_BASE                                  0x00006C0A
#define VMCS_HOST_GDTR_BASE                                0x00006C0C
#define VMCS_HOST_IDTR_BASE                                0x00006C0E
#define VMCS_HOST_IA32_SYSENTER_ESP_MSR                    0x00006C10
#define VMCS_HOST_IA32_SYSENTER_EIP_MSR                    0x00006C12
#define VMCS_HOST_RSP                                      0x00006C14
#define VMCS_HOST_RIP                                      0x00006C16
#define VMCS_HOST_IA32_S_CET                               0x00006C18
#define VMCS_HOST_SSP                                      0x00006C1A
#define VMCS_HOST_INTERRUPT_SSP_TABLE_ADDR                 0x00006C1C

#define VMX_HIGHEST_VMCS_ENCODING   (0x34)

#define VMX_VMEXIT_CTRL1_SAVE_DBG_CTRLS             (1 <<  2) /* legacy must be '1 */
#define VMX_VMEXIT_CTRL1_HOST_ADDR_SPACE_SIZE       (1 <<  9)
#define VMX_VMEXIT_CTRL1_LOAD_PERF_GLOBAL_CTRL_MSR  (1 << 12) /* Perf Global Control */
#define VMX_VMEXIT_CTRL1_INTA_ON_VMEXIT             (1 << 15)
#define VMX_VMEXIT_CTRL1_STORE_PAT_MSR              (1 << 18) /* PAT */
#define VMX_VMEXIT_CTRL1_LOAD_PAT_MSR               (1 << 19) /* PAT */
#define VMX_VMEXIT_CTRL1_STORE_EFER_MSR             (1 << 20) /* EFER */
#define VMX_VMEXIT_CTRL1_LOAD_EFER_MSR              (1 << 21) /* EFER */
#define VMX_VMEXIT_CTRL1_STORE_VMX_PREEMPTION_TIMER (1 << 22) /* VMX preemption timer */
#define VMX_VMEXIT_CTRL1_CLEAR_BNDCFGS              (1 << 23) /* MPX */
#define VMX_VMEXIT_CTRL1_SUPPRESS_VMX_PACKETS       (1 << 24) /* Processor Trace */
#define VMX_VMEXIT_CTRL1_LOAD_HOST_CET_STATE        (1 << 28) /* CET */

// clang-format on

// See: 30.4 VM INSTRUCTION ERROR NUMBERS
#define VM_INSTRUCTION_ERROR_NUMBERS(XX)                                                                                                       \
  XX (0, "")                                                                                                                                   \
  XX (1, "VMCALL executed in VMX root operation")                                                                                              \
  XX (2, "VMCLEAR with invalid physical address")                                                                                              \
  XX (3, "VMCLEAR with VMXON pointer")                                                                                                         \
  XX (4, "VMLAUNCH with non-clear VMCS")                                                                                                       \
  XX (5, "VMRESUME with non-launched VMCS")                                                                                                    \
  XX (6, "VMRESUME after VMXOFF (VMXOFF and VMXON between VMLAUNCH and VMRESUME)")                                                             \
  XX (7, "VM entry with invalid control field(s)")                                                                                             \
  XX (8, "VM entry with invalid host-state field(s)")                                                                                          \
  XX (9, "VMPTRLD with invalid physical address")                                                                                              \
  XX (10, "VMPTRLD with VMXON pointer")                                                                                                        \
  XX (11, "VMPTRLD with incorrect VMCS revision identifier")                                                                                   \
  XX (12, "VMREAD/VMWRITE from/to unsupported VMCS component")                                                                                 \
  XX (13, "VMWRITE to read-only VMCS component")                                                                                               \
  XX (15, "VMXON executed in VMX root operation")                                                                                              \
  XX (16, "VM entry with invalid executive-VMCS pointer2")                                                                                     \
  XX (17, "VM entry with non-launched executive VMCS2")                                                                                        \
  XX (18, "VM entry with executive-VMCS pointer not VMXON pointer (when attempting to deactivate the dual-monitor treatment of SMIs and SMM)") \
  XX (19, "VMCALL with non-clear VMCS (when attempting to activate the dual-monitor treatment of SMIs and SMM)")                               \
  XX (20, "VMCALL with invalid VM-exit control fields")                                                                                        \
  XX (22, "VMCALL with incorrect MSEG revision identifier (when attempting to activate the dual-monitor treatment of SMIs and SMM)")           \
  XX (23, "VMXOFF under dual-monitor treatment of SMIs and SMM")                                                                               \
  XX (24, "VMCALL with invalid SMM-monitor features (when attempting to activate the dual-monitor treatment of SMIs and SMM)")                 \
  XX (25, "VM entry with invalid VM-execution control fields in executive VMCS (when attempting to return from SMM)")                          \
  XX (26, "VM entry with events blocked by MOV SS.")                                                                                           \
  XX (28, "Invalid operand to INVEPT/INVVPID.")

const char* VM_INSTRUCTION_ERROR_STRING (int num);

#endif //VOS_VMX_H
