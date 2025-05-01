/*****************************************************************************
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdRasRegistersRmb.h
 *
 * @brief  SOC specific RAS definition
 *
 * @details Provide SOC specific structure and definition.
 *
 */

#ifndef _AMD_RAS_REGISTERS_RMB_H_
#define _AMD_RAS_REGISTERS_RMB_H_

#include "AmdRas.h"

#undef BIT0
#undef BIT1
#undef BIT2
#undef BIT3
#undef BIT4
#undef BIT5
#undef BIT6
#undef BIT7
#undef BIT8
#undef BIT9
#undef BIT10
#undef BIT10
#undef BIT11
#undef BIT12
#undef BIT13
#undef BIT14
#undef BIT15
#undef BIT16
#undef BIT17
#undef BIT18
#undef BIT19
#undef BIT20
#undef BIT21
#undef BIT22
#undef BIT23
#undef BIT24
#undef BIT25
#undef BIT26
#undef BIT27
#undef BIT28
#undef BIT29
#undef BIT30
#undef BIT31
#undef BIT32
#undef BIT33
#undef BIT34
#undef BIT35
#undef BIT36
#undef BIT37
#undef BIT38
#undef BIT39
#undef BIT40
#undef BIT41
#undef BIT42
#undef BIT43
#undef BIT44
#undef BIT45
#undef BIT46
#undef BIT47
#undef BIT48
#undef BIT49
#undef BIT40
#undef BIT41
#undef BIT42
#undef BIT43
#undef BIT44
#undef BIT45
#undef BIT46
#undef BIT47
#undef BIT48
#undef BIT49
#undef BIT50
#undef BIT51
#undef BIT52
#undef BIT53
#undef BIT54
#undef BIT55
#undef BIT56
#undef BIT57
#undef BIT58
#undef BIT59
#undef BIT60
#undef BIT61
#undef BIT62
#undef BIT63
#undef AMD_VENDOR_ID

#define BIT0        (0x0000000000000001ull)
#define BIT1        (0x0000000000000002ull)
#define BIT2        (0x0000000000000004ull)
#define BIT3        (0x0000000000000008ull)
#define BIT4        (0x0000000000000010ull)
#define BIT5        (0x0000000000000020ull)
#define BIT6        (0x0000000000000040ull)
#define BIT7        (0x0000000000000080ull)
#define BIT8        (0x0000000000000100ull)
#define BIT9        (0x0000000000000200ull)
#define BIT10       (0x0000000000000400ull)
#define BIT11       (0x0000000000000800ull)
#define BIT12       (0x0000000000001000ull)
#define BIT13       (0x0000000000002000ull)
#define BIT14       (0x0000000000004000ull)
#define BIT15       (0x0000000000008000ull)
#define BIT16       (0x0000000000010000ull)
#define BIT17       (0x0000000000020000ull)
#define BIT18       (0x0000000000040000ull)
#define BIT19       (0x0000000000080000ull)
#define BIT20       (0x0000000000100000ull)
#define BIT21       (0x0000000000200000ull)
#define BIT22       (0x0000000000400000ull)
#define BIT23       (0x0000000000800000ull)
#define BIT24       (0x0000000001000000ull)
#define BIT25       (0x0000000002000000ull)
#define BIT26       (0x0000000004000000ull)
#define BIT27       (0x0000000008000000ull)
#define BIT28       (0x0000000010000000ull)
#define BIT29       (0x0000000020000000ull)
#define BIT30       (0x0000000040000000ull)
#define BIT31       (0x0000000080000000ull)
#define BIT32       (0x0000000100000000ull)
#define BIT33       (0x0000000200000000ull)
#define BIT34       (0x0000000400000000ull)
#define BIT35       (0x0000000800000000ull)
#define BIT36       (0x0000001000000000ull)
#define BIT37       (0x0000002000000000ull)
#define BIT38       (0x0000004000000000ull)
#define BIT39       (0x0000008000000000ull)
#define BIT40       (0x0000010000000000ull)
#define BIT41       (0x0000020000000000ull)
#define BIT42       (0x0000040000000000ull)
#define BIT43       (0x0000080000000000ull)
#define BIT44       (0x0000100000000000ull)
#define BIT45       (0x0000200000000000ull)
#define BIT46       (0x0000400000000000ull)
#define BIT47       (0x0000800000000000ull)
#define BIT48       (0x0001000000000000ull)
#define BIT49       (0x0002000000000000ull)
#define BIT50       (0x0004000000000000ull)
#define BIT51       (0x0008000000000000ull)
#define BIT52       (0x0010000000000000ull)
#define BIT53       (0x0020000000000000ull)
#define BIT54       (0x0040000000000000ull)
#define BIT55       (0x0080000000000000ull)
#define BIT56       (0x0100000000000000ull)
#define BIT57       (0x0200000000000000ull)
#define BIT58       (0x0400000000000000ull)
#define BIT59       (0x0800000000000000ull)
#define BIT60       (0x1000000000000000ull)
#define BIT61       (0x2000000000000000ull)
#define BIT62       (0x4000000000000000ull)
#define BIT63       (0x8000000000000000ull)

///MSR
#define MSR_APIC_BAR                    (0x0000001BUL)
#define LAPIC_BASE_ADDR_MASK            (0x0000FFFFFFFFF000ULL)

#define APIC_ID_REG                     (0x20)
#define APIC20_ApicId_Offset            (24)

#define MSR_SYSENTER_EIP                (0x00000176UL)
#define MSR_MCG_CAP                     (0x00000179UL)
#define MSR_MCG_STAT                    (0x0000017AUL)
  #define MSR_MCG_EIPV                  BIT1
#define MSR_HWCR                        (0xC0010015UL)
  #define MCA_STS_WREN_BIT              BIT18
#define MSR_MCEXCEPREDIR                (0xC0010022UL)
#define MSR_SMITRIGIOCYCLE              (0xC0010056UL)
#define MSR_MMIO_CFG_BASE               (0xC0010058UL)
#define MSR_PSTATE_0                    (0xC0010064UL)
#define MSR_SMM_BASE                    (0xC0010111UL)
#define MSR_LOCAL_SMI_STATUS            (0xC001011AUL)
#define MSR_PFEH_CFG                    (0xC0010120UL)
  #define GENERATE_DEFERREDLVT_ON_EXIT      BIT1
  #define GENERATE_THRESHOLDLVT_ON_EXIT     BIT2
  #define GENERATE_MCE_ON_EXIT              BIT3
  #define SMI_EXITTYPE_MASK             (GENERATE_THRESHOLDLVT_ON_EXIT + GENERATE_DEFERREDLVT_ON_EXIT +GENERATE_MCE_ON_EXIT)

#define MSR_PFEH_CLOAK_CFG              (0xC0010121UL)
#define MSR_PFEH_DEF_INT_MASK           (0xC0010122UL)

#define MCA_LEGACY_BASE                 (0x00000400UL)
#define MCA_LEGACY_MAX_BANK             (32)
#define MCA_LEGACY_REG_PER_BANK         (1<<2)
#define MCA_LEGACY_TOP_ADDR             (MCA_LEGACY_BASE + (MCA_LEGACY_MAX_BANK * MCA_LEGACY_REG_PER_BANK))
#define MCA_REG_OFFSET_MASK             (0x00000003)

#define MCA_CTL_MASK_BASE               (0xC0010400UL)
#define MCA_EXTENSION_BASE              (0xC0002000UL)
#define SMCA_REG_PER_BANK               (1<<4)

#define SMCA_REG_OFFSET_MASK            (0x0000000F)
#define MCA_CTL_OFFSET                  (0x00)
#define MCA_STATUS_OFFSET               (0x01)
#define MCA_ADDR_OFFSET                 (0x02)
#define MCA_MISC0_OFFSET                (0x03)
#define MCA_CONFIG_OFFSET               (0x04)
#define MCA_IPID_OFFSET                 (0x05)
#define MCA_SYND_OFFSET                 (0x06)
#define MCA_DESTAT_OFFSET               (0x08)
#define MCA_DEADDR_OFFSET               (0x09)
#define MCA_MISC1_OFFSET                (0x0A)

#define MCA_EMPTY0_BANK                 (4)

//MCA Hardware ID
#define MCA_CPU_CORE_ID                 (0x0B0)
  #define LS_MCA_TYPE                   (0x0010)
  #define IF_MCA_TYPE                   (0x0001)
  #define L2_MCA_TYPE                   (0x0002)
  #define DE_MCA_TYPE                   (0x0003)
  #define EX_MCA_TYPE                   (0x0005)
  #define FP_MCA_TYPE                   (0x0006)
  #define L3_MCA_TYPE                   (0x0007)

#define MCA_DATA_FABRIC_ID              (0x02E)
  #define CS_MCA_TYPE                   (0x0002)
  #define PIE_MCA_TYPE                  (0x0001)

#define MCA_UMC_ID                      (0x096)
  #define UMC_MCA_TYPE                  (0x0000)
  #define UMC0_MCA_INS_ID               (0x050F00)
  #define UMC1_MCA_INS_ID               (0x150F00)
  #define UMC2_MCA_INS_ID               (0x250F00)
  #define UMC3_MCA_INS_ID               (0x350F00)
  #define UMC4_MCA_INS_ID               (0x450F00)
  #define UMC5_MCA_INS_ID               (0x550F00)
  #define UMC6_MCA_INS_ID               (0x650F00)
  #define UMC7_MCA_INS_ID               (0x750F00)

#define MCA_PARAMETER_BLOCK_ID          (0x005)
  #define PB_MCA_TYPE                   (0x0000)
  #define PB0_MCA_INS_ID                (0x30082900)
  #define PB1_MCA_INS_ID                (0x32082900)
  #define PB2_MCA_INS_ID                (0x34082900)
  #define PB3_MCA_INS_ID                (0x36082900)
  #define PB4_MCA_INS_ID                (0x38082900)
  #define PB5_MCA_INS_ID                (0x3A082900)
  #define PB6_MCA_INS_ID                (0x3C082900)
  #define PB7_MCA_INS_ID                (0x3E082900)

#define MCA_PSP_ID                      (0x0FF)
  #define PSP_MCA_TYPE                  (0x0001)

#define MCA_SMU_ID                      (0x001)
  #define SMU_MCA_TYPE                  (0x0001)
  #define MP5_MCA_TYPE                  (0x0002)
  #define MP50_MCA_INS_ID               (0x30430400)
  #define MP51_MCA_INS_ID               (0x32430400)
  #define MP52_MCA_INS_ID               (0x34430400)
  #define MP53_MCA_INS_ID               (0x36430400)
  #define MP54_MCA_INS_ID               (0x38430400)
  #define MP55_MCA_INS_ID               (0x3A430400)
  #define MP56_MCA_INS_ID               (0x3C430400)
  #define MP57_MCA_INS_ID               (0x3E430400)

#define MCA_NBIO_ID                     (0x0018)
  #define NBIO_MCA_TYPE                 (0x0000)
  #define NBIO0_MCA_INS_ID              (0x13B17000)
  #define NBIO1_MCA_INS_ID              (0x13C17000)
  #define NBIO2_MCA_INS_ID              (0x13D17000)
  #define NBIO3_MCA_INS_ID              (0x13E17000)

#define MCA_PCIE_ID                     (0x0046)
  #define PCIE_MCA_TYPE                 (0x0000)
  #define PCIE0_MCA_INS_ID              (0x115C0000)
  #define PCIE1_MCA_INS_ID              (0x116C0000)
  #define PCIE2_MCA_INS_ID              (0x117C0000)
  #define PCIE3_MCA_INS_ID              (0x118C0000)

//This is old x86 MCA address
#define LMCA_ADDR_REG                   (0x00)
#define LMCA_STATUS_REG                 (0x01)

#define SMM_SAVE_STATE_OFFSET           (0xFE00)

#define DEVICE_WIDTH_x4                 (4)
#define DEVICE_WIDTH_x8                 (8)
#define ECC_SYMBOL_SIZE_x4              (0)
#define ECC_SYMBOL_SIZE_x8              (1)
#define ECC_SYMBOL_SIZE_x16             (2)
#define ECC_BIT_INTERLEAVING_DISABLED   (0)
#define ECC_BIT_INTERLEAVING_ENABLED    (1)

/// ACPI define
/// ACPI 6.2, 6.3 Generic Error Data Entry - Revision
#define GENERIC_ERROR_REVISION          (0x0300)
#define AMD_VENDOR_ID                   (0x1022)

///NBIO
#define IOHC_NB_SMN_INDEX_2_BIOS        (0x00B8)
#define IOHC_NB_SMN_DATA_2_BIOS         (0x00BC)

#define MAX_SOCKET_SUPPORT                  (1)
#define MAX_NBIO_PER_DIE                    (1)
#define MAX_NBIO_SUPPORT                    (1)             //Max availiable on SSP Platform
#define MCA_PCIE_CORE0_PORT_SUPPORT         (1)             //Max PCI-E port support for Core 0.
#define MCA_PCIE_CORE1_PORT_SUPPORT         (1)             //Max PCI-E port support for Core 1.
#define MCA_PCIE_CORE2_PORT_SUPPORT         (1)             //Max PCI-E port support for Core 2.
#define MAX_PCIE_PORT_SUPPORT               (11)            //Max PCI-E port support per NBIO.
#define MAX_NBIF_PER_NBIO                   (1)
#define MAX_LEAF_PER_NBIF                   (5)
#define MAX_SYSHUB_PER_NBIO                 (1)
#define MAX_GDCL_LEAF_PER_SYSHUB            (3)
#define MAX_GDCSOC_LEAF_PER_SYSHUB          (2)
#define MAX_GDCSHUB_LEAF_PER_SYSHUB         (2)
#define MAX_SHUB_MPX_LAST_XREQ_LOG          (3)
#define MAX_USB_PER_DIE                     (2)

#define MAX_UNCORR_GRP_NUM                  (25)
#define MAX_CORR_GRP_NUM                    (23)
#define MAX_UCP_GRP_NUM                     (17)

#define NBIO_SMN_ADDR_OFFSET                (0x100000)
#define IOHC_INTERRUPT_EOI                  (0x13B10120)
#define RAS_PARITY_CONTROL_0                (0x13B20000)
#define RAS_PARITY_CONTROL_1                (0x13B20004)
#define RAS_GLOBAL_STATUS_LO                (0x13B20020)
#define RAS_GLOBAL_STATUS_HI                (0x13B20024)
#define RAS_GLOBAL_STATUS_NBIO_ERR_MASK     (0x0000000F)
#define RAS_GLOBAL_STATUS_LO_MASK           (0x000001FF)
#define RAS_GLOBAL_STATUS_HI_MASK           (0x0003FFFF)

#define PARITY_ERROR_STATUS_UNCORR_GRP      (0x13B20028)
#define PARITY_ERROR_STATUS_CORR_GRP        (0x13B2008C)
#define PARITY_COUNTER_CORR_GRP             (0x13B200E8)
#define PARITY_ERROR_STATUS_UCP_GRP         (0x13B20144)
#define PARITY_COUNTER_UCP_GRP              (0x13B20188)
#define PCIE_PORT_ACTION_CONTROL_BASE       (0x13B201F0)
#define PCIE_ACTION_CONTROL_OFFSET          (0x20)
#define NBIF0_ACTION_CONTROL_BASE           (0x13B20430)
#define NBIF1_ACTION_CONTROL_BASE           (0x13B20450)

#define NBIF0_BIFL_RAS_CENTRAL_STATUS       (0x10139040)
#define NBIF0_BIFL_RAS_LEAF0_CTRL           (0x10139080)
#define NBIF0_BIFL_RAS_LEAF1_CTRL           (0x10139084)
#define NBIF0_BIFL_RAS_LEAF2_CTRL           (0x10139088)
#define NBIF2_BIFL_RAS_LEAF3_CTRL           (0x1093908C)
#define NBIF2_BIFL_RAS_LEAF4_CTRL           (0x10939090)
#define NBIF0_BIFL_RAS_LEAF0_STATUS         (0x101390C0)
#define NBIF0_BIFL_RAS_LEAF1_STATUS         (0x101390C4)
#define NBIF0_BIFL_RAS_LEAF2_STATUS         (0x101390C8)
#define NBIF2_BIFL_RAS_LEAF3_STATUS         (0x109390CC)
#define NBIF2_BIFL_RAS_LEAF4_STATUS         (0x109390D0)

#define GDCL_RAS_CENTRAL_STATUS             (0x0141F840)
#define GDCL_RAS_LEAF0_CTRL                 (0x0141F880)
#define GDCL_RAS_LEAF1_CTRL                 (0x0141F884)
#define GDCL_RAS_LEAF2_CTRL                 (0x0141F888)
#define GDCL_RAS_LEAF0_STATUS               (0x0141F8C0)
#define GDCL_RAS_LEAF1_STATUS               (0x0141F8C4)
#define GDCL_RAS_LEAF2_STATUS               (0x0141F8C8)
#define GDCSOC_RAS_CENTRAL_STATUS           (0x0141FA40)
#define GDCSOC_RAS_LEAF0_CTRL               (0x0141FA80)
#define GDCSOC_RAS_LEAF1_CTRL               (0x0141FA84)
#define GDCSOC_RAS_LEAF0_STATUS             (0x0141FAC0)
#define GDCSOC_RAS_LEAF1_STATUS             (0x0141FAC4)
#define GDCSHUB_RAS_CENTRAL_STATUS          (0x0141FC40)
#define GDCSHUB_RAS_LEAF0_STATUS            (0x0181FCC0)
#define GDCSHUB_RAS_LEAF1_STATUS            (0x0181FCC4)
#define NBIF_SHUB_TODET_CLIENT_STATUS       (0x1013A268)

#define NBIO2NBIF1_RCC_DEV0_EPF3_STRAP0     (0x10734600)
#define NBIO3NBIF1_RCC_DEV0_EPF3_STRAP0     (0x10834600)
#define NBIO2NBIF1_MEM_ECC_STATUS_CNTR0     (0x16D8013C)  //USB0
#define NBIO3NBIF1_MEM_ECC_STATUS_CNTR0     (0x16F8013C)  //USB1
#define NBIO2NBIF1_MEM_ECC_SMI_STATUS_CNTR0 (0x16D80140)  //USB0
#define NBIO3NBIF1_MEM_ECC_SMI_STATUS_CNTR0 (0x16F80140)  //USB1

#define NBIO0_EGRESS_POISON_STATUS_LO       (0x13B20820)
#define NBIO1_EGRESS_POISON_STATUS_LO       (0x13C20820)
#define NBIO2_EGRESS_POISON_STATUS_LO       (0x13D20820)
#define NBIO3_EGRESS_POISON_STATUS_LO       (0x13E20820)
#define NBIO0_EGRESS_POISON_STATUS_HI       (0x13B20824)
#define NBIO1_EGRESS_POISON_STATUS_HI       (0x13C20824)
#define NBIO2_EGRESS_POISON_STATUS_HI       (0x13D20824)
#define NBIO3_EGRESS_POISON_STATUS_HI       (0x13E20824)

///MSR
/// P-state MSR
typedef union {
  struct {                             ///< Bitfields of P-state MSR
    UINT64 CpuFid_7_0:8;               ///< CpuFid[7:0]
    UINT64 CpuDfsId:6;                 ///< CpuDfsId
    UINT64 CpuVid:8;                   ///< CpuVid
    UINT64 IddValue:8;                 ///< IddValue
    UINT64 IddDiv:2;                   ///< IddDiv
    UINT64 :31;                        ///< Reserved
    UINT64 PstateEn:1;                 ///< Pstate Enable
  } Field;
  UINT64  Value;
} PSTATE_MSR;

typedef union {
  /// Bitfields of MCA_SYND_UMC DramEccErr error type error information decode.
  struct {
    UINT32 ChipSelect:3;                ///< Chip Select
    UINT32 :1;                          ///< Reserved
    UINT32 Cid:3;                       ///< Specifies the rank multiply ID for supported DIMMs
    UINT32 :1;                          ///< Reserved
    UINT32 Symbol:6;                    ///< Only contains valid information on a corrected error.
    UINT32 HwHistoryErr:1;              ///< Hardware history error
    UINT32 SwManagedBadSymbolIdErr:1;   ///< Software-Managed Bad Symbol ID Error
    UINT32 :16;                         ///< Reserved
  } Fields;
  UINT32  Value;
} UMC_SYND_ECC_ERR_INFO;

///NBIO
typedef enum _NBIO_ERROR_CODE_EXT_NUM {
  NBIO_PARITY = 0,                          ///< 0
  NBIO_PCIE_SIDEBAND,                       ///< 1
  NBIO_ERREVENT,                            ///< 2
  NBIO_EGRESS_POISON,                       ///< 3
  NBIO_IOHC_INTERNAL_POISON                 ///< 4
} NBIO_ERROR_CODE_EXT_NUM;

typedef union {
  /// Bitfields of IOHCRAS Parity Control 0 Register
  struct {
    UINT32 ParityCorrThreshold:16;          ///< The number of ucp parity errors logged before an action is triggered.
    UINT32 ParityUCPThreshold:16;           ///< The number of correctable parity errors logged before an action is triggered.
  } Fields;
  UINT32  Value;
} PARITY_CONTROL_0_REG;

///NBIO IOHCRAS Parity Control 1 Register
typedef union {
  /// Bitfields of IOHCRAS Parity Control 1 Register
  struct {
    UINT32 ParityErrGenGroupSel:8;          ///< Controls the parity error group that is the target of error injection or masking.
    UINT32 ParityErrGenGroupTypeSel:1;      ///< 0- Uncorrectable Group Type, 1- - Correctable Group Type.
    UINT32 :2;                              ///< Reserved.
    UINT32 ParityErrGenIdSel:5;             ///< Selects a structure within the target parity group for error injection or masking.
    UINT32 ParityErrGenCmd:4;               ///< 0000 - Reserved
                                            ///< 0001 - Error Injection on Write Side
                                            ///< 0010 - Error Injection on Read Side
                                            ///< 0011 - Reserved
                                            ///< 0100 - Disable Error Reporting
                                            ///< 0101 - Enable Error Reporting
                                            ///< 0110 - Reserved
                                            ///< 0111-1111 - Reserved
                                            ///< Encoding for ECC Parity Wrappers:
                                            ///< 0000 - Disable ECC Error Detection and Correction
                                            ///< 0001 - Enable ECC Error Detection and Correction
                                            ///< 0010 - SEC Error Injection
                                            ///< 0011 - Disable SEC Error Reporting
                                            ///< 0100 - Enable SEC Error Reporting
                                            ///< 0101 - DED Error Injection
                                            ///< 0110 - Disable DED Error Reporting
                                            ///< 0111 - Enable DED Error Reporting
                                            ///< 1000 - Disable Conversion of DED Errors to Poisoned Data
                                            ///< 1001 - Enable Conversion of DED Errors to Poisoned Data
                                            ///< 1010 - UCP Error Injection
                                            ///< 1011 - Disable UCP Error Reporting
                                            ///< 1100 - Enable UCP Error Reporting
    UINT32 :10;                             ///< Reserved.
    UINT32 ParityErrGenTrigger:1;           ///< Writing this bit to 1 triggers the command defined by the other ParityErrGen* fields in this register.
    UINT32 ParityErrGenInjectAllow:1;       ///< Software must poll this bit and make sure this bit is asserted before updating the rest of the fields in this register.
  } Fields;
  UINT32  Value;
} PARITY_CONTROL_1_REG;

typedef union {
  /// Bitfields of IOHCRAS PCIE0/1PortA/H*_ACTION_CONTROL Register
  struct {
    UINT32 ApmlErrEn:1;                     ///< 1 causes an interrupt to SMU/MP1 to be triggered when the associated error is logged.
    UINT32 IntrGenSel:2;                    ///< A non-zero value causes an interrupt to be sent to the CPU complex when the associated error is logged.
    UINT32 LinkDisEn:1;                     ///< 1 causes PCIe links to be disabled when the associated error is logged.
    UINT32 SyncFloodEn:1;                   ///< 1 causes an SDP ErrEvent to be triggered when the associated error is logged.
    UINT32 :27;                             ///< Reserved.
  } Fields;
  UINT32  Value;
} ERR_ACT_CTRL_REG;

typedef union {
  /// Bitfields of RAS_GLOBAL_STS_HI Register
  struct {
    UINT32 Pcie0PortA:1;                    ///< The status of errors from PCIE0 PortA
    UINT32 Pcie0PortB:1;                    ///< The status of errors from PCIE0 PortB
    UINT32 Pcie0PortC:1;                    ///< The status of errors from PCIE0 PortC
    UINT32 Pcie0PortD:1;                    ///< The status of errors from PCIE0 PortD
    UINT32 Pcie0PortE:1;                    ///< The status of errors from PCIE0 PortE
    UINT32 Pcie0PortF:1;                    ///< The status of errors from PCIE0 PortF
    UINT32 Pcie0PortG:1;                    ///< The status of errors from PCIE0 PortG
    UINT32 Pcie0PortH:1;                    ///< The status of errors from PCIE0 PortH
    UINT32 Pcie1PortA:1;                    ///< The status of errors from PCIE1 PortA
    UINT32 Pcie1PortB:1;                    ///< The status of errors from PCIE1 PortB
    UINT32 Pcie1PortC:1;                    ///< The status of errors from PCIE1 PortC
    UINT32 Pcie1PortD:1;                    ///< The status of errors from PCIE1 PortD
    UINT32 Pcie1PortE:1;                    ///< The status of errors from PCIE1 PortE
    UINT32 Pcie1PortF:1;                    ///< The status of errors from PCIE1 PortF
    UINT32 Pcie1PortG:1;                    ///< The status of errors from PCIE1 PortG
    UINT32 Pcie1PortH:1;                    ///< The status of errors from PCIE1 PortH
    UINT32 Pcie2PortA:1;                    ///< The status of errors from PCIE2 PortA
    UINT32 Pcie2PortB:1;                    ///< The status of errors from PCIE2 PortB
    UINT32 Nbif0PortA:1;                    ///< The status of errors from NBIF0 PortA
    UINT32 Nbif1PortA:1;                    ///< The status of errors from NBIF1 PortA
    UINT32 Nbif1PortB:1;                    ///< The status of errors from NBIF1 PortB
    UINT32 Nbif1PortC:1;                    ///< The status of errors from NBIF1 PortC
    UINT32 :10;                             ///< Reserved
  } Fields;
  UINT32  Value;
} RAS_GLOBAL_STS_HI_REG;

typedef union {
  struct {
    UINT32 AthubTimeoutDetected:1;
    UINT32 :1; ///< Reserved
    UINT32 Mp1TimeoutDetected  :1; 
    UINT32 :2; ///< Reserved
    UINT32 Mp0TimeoutDetected  :1;
    UINT32 :26; ///< Reserved
  } Field;
  UINT32 Value;
} NBIF_SHUB_TODET_CLIENT_STATUS_REG;

///DXIO

typedef union {
  /// Bitfields of PHY_IVC_40_ECC_MEMORY_CNTL1 Register
  struct {
    UINT32 ForceSecOnDed:1;                 ///< FORCE_SEC_ON_DED
    UINT32 :7;                              ///<
    UINT32 DED:4;                           ///< Double Error Detect
    UINT32 SEC:4;                           ///< Single Error Detect
  } Fields;
  UINT16  Value;
}PHY_IVC_40_ECC_MEMORY_CNTL1;

// USB
typedef union {
  struct {
    UINT32 ECC_DedErrSMI:1 ; ///<
    UINT32              :31; ///<
  } Field;
  UINT32 Value;
} MEM_ECC_SMI_STATUS_CNTR0_REG;

#define PHY_IVC_40_ECC_MEMORY_CNTL1_REG     (0x10072)

#define PAPCS0_OFFSET                       (0x00000)
#define PAPCS1_OFFSET                       (0x20000)
#define PAPCS2_OFFSET                       (0x40000)
#define PAPCS3_OFFSET                       (0x60000)
#define PAPCS_APERTURE_SIZE                 (0x20000)

#define PCS_RANGE                           (4)
#define SATA_PCS_RANGE                      (2)

#define PCIE_SCAN_RANGE                     (8)
#define SATA_SCAN_RANGE                     (4)
#define XGMI_SCAN_RANGE                     (6)
#define WALF_SCAN_RANGE                     (2)

#define APERTURE_SIZE                       (0x100000)

#define PCS_PCIE0P0_BASE                    (0x12200000)
#define PCS_PCIE1P0_BASE                    (0x12300000)
#define PCS_PCIE2P0_BASE                    (0x12400000)
#define PCS_PCIE3P0_BASE                    (0x12500000)
#define PCS_PCIE4P0_BASE                    (0x12600000)
#define PCS_PCIE5P0_BASE                    (0x12700000)
#define PCS_PCIE6P0_BASE                    (0x12800000)
#define PCS_PCIE7P0_BASE                    (0x12900000)

#define PCS_SATA0P0_BASE                    (0x12A00000)
#define PCS_SATA1P0_BASE                    (0x12B00000)
#define PCS_SATA2P0_BASE                    (0x12C00000)
#define PCS_SATA3P0_BASE                    (0x12D00000)

#define PCS_XGMI0P0_BASE                    (0x12E00000)
#define PCS_XGMI1P0_BASE                    (0x12F00000)
#define PCS_XGMI2P0_BASE                    (0x13000000)
#define PCS_XGMI3P0_BASE                    (0x13100000)
#define PCS_XGMI4P0_BASE                    (0x13200000)
#define PCS_XGMI5P0_BASE                    (0x13300000)

#define PCS_WAFL0P0_BASE                    (0x13400000)
#define PCS_WAFL1P0_BASE                    (0x13500000)

#define PCS_WAFLPCIE1P0_BASE                (0x18000000)

///FCH
#define ACPIMMIO32(x) (*(volatile UINT32*)(UINTN)(x))
#define ACPIMMIO16(x) (*(volatile UINT16*)(UINTN)(x))
#define ACPIMMIO8(x) (*(volatile UINT8*)(UINTN)(x))
#define ACPI_MMIO_BASE                  (0xFED80000ul)
#define SMI_BASE                        (0x200)         // DWORD
#define FCH_SMI_REG18                   (0x18)
#define FCH_SMI_REG1C                   (0x1C)
#define FCH_SMI_REG96                   (0x96)
#define FCH_SMI_REG98                   (0x98)
#define FCH_SMI_REGC0                   (0xC0)
#define FCH_SMI_REGC3                   (0xC3)
#define PMIO_BASE                       (0x300)         // DWORD
#define FCH_PMIOA_REG10                 (0x10)          // FCH::PM::PWRRSTCFG
  #define PM_TOGGLEALLPWRGOODONC        BIT1            
#define FCH_PMIOA_REG6A                 (0x6A)          // FCH::PM::ACPISMICMD
#define FCH_PMIOA_REGBE                 (0xBE)          // FCH::PM::RESETCONTROL1
  #define PM_RSTTOCPUPWRGDEN            BIT7
#define FCH_PMIOA_REGC0                 (0xC0)          // FCH::PM::S5_RESET_STATUS
#define FCH_PMIOA_REG504                (0x504)         // FCH::PM::PMCONTROL

#define SMI_TIMER_ENABLE                BIT15

/// PMIO BreakEvent Register for SMN errors
#define PMIO_SMN_BREAK_EVENT                (0x02D01380)
#define FCH_PMIOA_REG80                     (0x80)      // FCH::PM::BreakEvent

#define SMN_CATEGORY_SMN_PARITY_TIMEOUT_PSP_SMU_PARITY_ECC (1 << 0)
#define SMN_CATEGORY_PSP_PARITY_ECC         (1 << 2)
#define SMN_CATEGORY_SMN_TIMEOUT_SMU        (1 << 3)
#define SMN_CATEGORY_SMN_LINK_PACKET_CRC_WITH_RETRY (1 << 4)
#define SMN_CATEGORY_MASK                   (SMN_CATEGORY_SMN_PARITY_TIMEOUT_PSP_SMU_PARITY_ECC | SMN_CATEGORY_PSP_PARITY_ECC | SMN_CATEGORY_SMN_TIMEOUT_SMU | SMN_CATEGORY_SMN_LINK_PACKET_CRC_WITH_RETRY)

///SATA
#define FCH_SMN_SATA_CONTROL_BAR5           (0x03101000ul)
#define FCH_SMN_SATA_CONTROL_SLOR           (0x03101800ul)
#define FCH_SMN_SATA_STEP                   (0x100000ul)
#define FCH_SATA_SLOR_REG88                 (0x0088)
#define FCH_SATA_BAR5_REG110                (0x0110)
#define FCH_SATA_BAR5_REG130                (0x0130)

///FCH A-Link RAS
#define FCH_SMI_REG84                       (0x84)
#define FCH_SMBUS_BUS                       (0x00)
#define FCH_SMBUS_DEV                       (0x14)
#define FCH_SMBUS_FUNC                      (0x00)
#define FCH_LPC_BUS                         (0x00)
#define FCH_LPC_DEV                         (0x14)
#define FCH_LPC_FUNC                        (0X03)
#define PCI_STATUS                          (0X06)

#define HUASHAN_SATA_PORT_NUM               (8)

#define SATA_PAR_ERR_CNXT_STS               BIT10
#define SATA_PAR_ERR_H2D_STS                BIT9
#define SATA_PAR_ERR_D2H2D_STS              BIT8
#define SATA_PAR_ERR_STS                    (SATA_PAR_ERR_CNXT_STS + SATA_PAR_ERR_H2D_STS + SATA_PAR_ERR_D2H2D_STS)

///FCH::PM::S5_RESET_STATUS
typedef union {
  struct {
    UINT32                                             thermaltrip:1 ; ///<
    UINT32                                           pwrbtn4second:1 ; ///<
    UINT32                                                shutdown:1 ; ///<
    UINT32                                     thermaltripfromtemp:1 ; ///<
    UINT32                                  remotepowerdownfromasf:1 ; ///<
    UINT32                                            shutdownfan0:1 ; ///<
    UINT32                                            shutdownfan1:1 ; ///<
    UINT32                                            shutdownfan2:1 ; ///<
    UINT32                                            shutdownfan3:1 ; ///<
    UINT32                                          intthermaltrip:1 ; ///<
    UINT32                                     pwrgddwnbeforeslps3:1 ; ///<
    UINT32                                       slps3toldtpwrgden:1 ; ///<
    UINT32                                       disableldtpwrgood:1 ; ///<
    UINT32                                             dissbtonbpg:1 ; ///<
    UINT32                                          pmeturnofftime:2 ; ///<
    UINT32                                                 userrst:1 ; ///<
    UINT32                                             soft_pcirst:1 ; ///<
    UINT32                                              do_k8_init:1 ; ///<
    UINT32                                             do_k8_reset:1 ; ///<
    UINT32                                          Reserved_20_20:1 ; ///<
    UINT32                                              sleepreset:1 ; ///<
    UINT32                                                kb_reset:1 ; ///<
    UINT32                                            shutdown_msg:1 ; ///<
    UINT32                                             failbootrst:1 ; ///<
    UINT32                                      watchdogissuereset:1 ; ///<
    UINT32                                      remoteresetfromasf:1 ; ///<
    UINT32                                              sync_flood:1 ; ///<
    UINT32                                              hang_reset:1 ; ///<
    UINT32                                              mp1_wdtout:1 ; ///<
    UINT32                                               reserved0:2 ; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} S5_RESET_STATUS_REG;

#define PM_S5_RESET_STATUS_WARM_RESET_FLAG_MASK (0x07E70000)
#define PM_S5_RESET_STATUS_RESET_FLAG_MASK (0x3FFF0000)

typedef union {
  struct {
    UINT16                                                  sci_en:1 ; ///<
    UINT16                                                   bmrld:1 ; ///<
    UINT16                                                 gbl_rls:1 ; ///<
    UINT16                                            Reserved_9_3:7 ; ///<
    UINT16                                                 slp_typ:3 ; ///<
    UINT16                                                  slp_en:1 ; ///<
    UINT16                                                reserved:2 ; ///<
  } Field;                                                             ///<
  UINT16 Value;                                                        ///<
} PMCONTROL_REG;

///UMC
/**
 * @brief The total number of UMC
 *
 * @details Four Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define MAX_UMC_CHANNEL_PER_DIE             (4)

 /**
 * @brief The SMN address of UMC00CHx00000000 of UMC Controller
 *
 * @details This is the address of [DRAM CS Base Address] (UMC::BaseAddr)@ UMC0
 */
#define UMC0_CH_REG_BASE                    (0x00050000)

/**
 * @brief The SMN address offset of UMC_x_CH
 *
 * @details The SMN address offset of UMC_x_CH is 0x100000, based on the previous UMC_(x-1)_CH SMN address
 *          where x = 1, 2, 3, 4
 *          e.g The SMN address of UMC00CH = 0x00050000
 *                             =>  UMC01CH = UMC00CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00150000
 *                             =>  UMC02CH = UMC01CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00250000
 *                             =>  UMC03CH = UMC02CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00350000
 */
#define UMC_SMN_ADDR_OFFSET                 (0x100000)

 /**
 * @brief The SMN address of UMC01CHx00000000 of UMC Controller
 *
 * @details This is the address of [DRAM CS Base Address] (UMC::BaseAddr)@ UMC1
 */
#define UMC1_CH_REG_BASE                    (0x00150000)

 /**
 * @brief The SMN address offset of [DRAM Address Configuration] (UMC::AddrCfg), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [DRAM Address Configuration] (UMC::AddrCfg). e.g. UMC::AddrCfg@UMC0, CS0 = 0x00050040
 */
#define UMC_ADDR_CFG                        (0x00000040)

 /**
 * @brief The SMN address offset of [DRAM ECC Control] (UMC::EccCtrl), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [DRAM ECC Control] (UMC::EccCtrl). e.g. UMC::EccCtrl@UMC0 = 0x0005014C
 */
#define UMC_ECC_CTRL                        (0x0000014C)

 /**
 * @brief The SMN address offset of [UMC Miscellaneous Configuration] (UMC::MiscCfg), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [UMC Miscellaneous Configuration] (UMC::MiscCfg). e.g. UMC::EccCtrl@UMC0 = 0x000501E0
 */
#define UMC_MISC_CFG                        (0x000001E0)

 /**
 * @brief The SMN address offset of [DRAM ECC Error Inject Control] (UMC::EccErrInjCtrl), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [DRAM ECC Error Inject Control] (UMC::EccErrInjCtrl). e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050D8C
 */
#define UMC_ECC_ERR_INJ_CTRL                (0x00000D8C)

 /**
 * @brief The EccErrAddrEn bit of [DRAM ECC Error Inject Control] (UMC::EccErrInjCtrl), based on UMC::BaseAddr
 *
 * @details Allow address-based injection from the DF CS.
 */
  #define ECC_ERR_ADDR_EN                   BIT2

 /**
 * @brief The SMN address offset of [DRAM Error Inject] (UMC::EccErrInj), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [DRAM Error Inject] (UMC::EccErrInj). e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050D80
 */
#define UMC_ECC_ERR_INJ                     (0x00000DB0)

 /**
 * @brief The SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0)@Dimm0, based on UMC::BaseAddr@UNC00
 *
 * @details This is the SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0). e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050C80
 */
#define UMC_MCAFRUTEXTDW0_DIMM0             (0xC80)
 /**
 * @brief The SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0)@Dimm1, based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0). e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050C90
 */
#define UMC_MCAFRUTEXTDW0_DIMM1             (0xC90)  //Todo

 /**
 * @brief The SMN address offset of [UMC Capabilities High] (UMC::UmcCapHi), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [UMC Capabilities High] (UMC::UmcCapHi). e.g. UMC::UmcCapHi@UMC0 = 0x00050DF4
 */
#define UMC_CAPABILITIES_HIGH               (0xDF4)

//
// EccErrCntSel Register Bitfields:
//

/// Bitfield Description : Index
#define EccErrCntSel_Index_OFFSET                             0       ///< Bit offset
#define EccErrCntSel_Index_WIDTH                              6       ///< Width
#define EccErrCntSel_Index_MASK                               0x3fUL  ///< Bit mask

/// Bitfield Description : Reserved
//#define EccErrCntSel_Reserved_30_6_OFFSET                     6
//#define EccErrCntSel_Reserved_30_6_WIDTH                      25
//#define EccErrCntSel_Reserved_30_6_MASK                       0x7fffffc0UL

/// Bitfield Description : AutoInc
#define EccErrCntSel_AutoInc_OFFSET                            31           ///< Bit offset
#define EccErrCntSel_AutoInc_WIDTH                             1            ///< Width
#define EccErrCntSel_AutoInc_MASK                              0x80000000UL ///< Bit mask

/**
 * @brief Structure of DRAM ECC Error Count Select Register (UMC::EccErrCntSel).
 *
 * @details Specifies the index into the EccErrCnt register.
 */
typedef union {
  struct {
    UINT32                                                   Index: 6; ///< ECC Error Counter Select Index.
    UINT32                                           Reserved_30_6:25; ///< Reserved.
    UINT32                                                 AutoInc: 1; ///< When enabled, the index register will auto increment in hardware after a RD or WR access.
  } Field;
  UINT32 Value;
} ECC_ERR_CNT_SEL_REG;


///
/// Register Name EccErrCntSel
///
#define SMN_UMCWPHY0UMC0_EccErrCntSel_ADDRESS                  0x50d80UL
//#define SMN_UMCWPHY0UMC0_EccErrCntSel_OFFSET                   0xd80UL    //Unused

#define Subchannelel_Max                                       0x2 // DDR5 DIMM Subchannel (0=A,1=B),
#define Chipselect_Max                                         0x4 // Chipselect 3:0,
#define RankMul_Max                                            0x8 // RankMul 7:0 (LRDIMM/3DS).


///
/// EccErrCnt Register Bitfields:
///

/// Bitfield Description :
//#define EccErrCnt_EccErrCnt_OFFSET                             0          //Unused
//#define EccErrCnt_EccErrCnt_WIDTH                              16         //Unused
/**
 * @brief The EccErrCnt mask for the UMC::EccErrCnt register
 *
 * @details This mask is used to get/clear ECC error counter value.
 *          ECC error counter value selected by EccErrCntSel index which maps to SubCh/CS/Plr.
 */
#define EccErrCnt_EccErrCnt_MASK                               0xffffUL

/// Bitfield Description :
//#define EccErrCnt_EccErrThresh_OFFSET                          16         //Unused
//#define EccErrCnt_EccErrThresh_WIDTH                           16         //Unused
/**
 * @brief The EccErrThresh mask for the UMC::EccErrCnt register
 *
 * @details This mask is used to get/clear Threshold value.
 *          Threshold value for the selected by EccErrCntSel index which maps to SubCh/CS/Plr.
 */
#define EccErrCnt_EccErrThresh_MASK                            0xffff0000UL

/**
 * @brief Structure of DRAM ECC Error Count Register (UMC::EccErrCnt).
 *
 * @details The ECC error count value as selected by EccErrCntSel which maps to SubCh/CS/Plr.
 */
typedef union {
  struct {
    UINT32                                               EccErrCnt:16; ///< ECC Threshold value
    UINT32                                            EccErrThresh:16; ///< ECC Threshold value
  } Field;
  UINT32 Value;
} UMC_ECC_ERR_CNT_REG;

//
// Register Name EccErrCnt
//
 /**
 * @brief The address of UMC00CHx00000D84 in the UMC Controller Registers space
 *
 * @details This is the SMN address of [DRAM ECC Error Count] (UMC::EccErrCnt)@UMC0.
 */
#define SMN_UMC_EccErrCnt_ADDRESS                     0x50d84UL
//#define SMN_UMC_EccErrCnt_OFFSET                      0xd84UL              //Unused


//
// EccErrCntCtrl Register Bitfields:
//

/// Bitfield Description : EccErrCntEn
//#define EccErrCntCtrl_EccErrCntEn_OFFSET                        0         //Unused
//#define EccErrCntCtrl_EccErrCntEn_WIDTH                         1         //Unused
/**
 * @brief The EccErrCntEn mask bit for UMC::EccErrCntCtrl register
 *
 * @details This mask bit is used for EccErrCntCtrl[0]. The EccErrCntEn bit is used to enables the per-chipselect ECC error counters.
 */
#define EccErrCntCtrl_EccErrCntEn_MASK                          0x01UL

/// Bitfield Description : Enable Leaky Bucket mode for ECC Error Counters
//#define EccErrCntCtrl_EccErrLeakEn_OFFSET                       1         //Unused
//#define EccErrCntCtrl_EccErrLeakEn_WIDTH                        1         //Unused
/**
 * @brief The EccErrLeakEn mask bit for UMC::EccErrCntCtrl register
 *
 * @details This mask bit is used for EccErrCntCtrl[1]. The EccErrLeakEn bit is used to enable Leaky Bucket mode for ECC Error Counters.
 */
#define EccErrCntCtrl_EccErrLeakEn_MASK                         0x02UL

/// Bitfield Description : Reserved.
//#define EccErrCntCtrl_Reserved_3_2_OFFSET                       2         //Unused
//#define EccErrCntCtrl_Reserved_3_2_WIDTH                        2         //Unused
//#define EccErrCntCtrl_Reserved_3_2_MASK                         0x0CUL    //Unused

/// Bitfield Description : Specifies the type of interrupt generated when any EccErrCnt field transitions to FFFFh.
//#define EccErrCntCtrl_EccErrInt_OFFSET                          4         //Unused
//#define EccErrCntCtrl_EccErrInt_WIDTH                           2         //Unused

/**
 * @brief The EccErrInt mask bit for UMC::EccErrCntCtrl register
 *
 * @details This mask is used for EccErrCntCtrl[5:4]. The EccErrInt is used to
 *          specifie the type of interrupt generated when any EccErrCnt field transitions to FFFFh.
 */
#define EccErrCntCtrl_EccErrInt_MASK                            0x30UL

/// Bitfield Description :Reserved.
//#define EccErrCntCtrl_Reserved_7_6_OFFSET                       6         //Unused
//#define EccErrCntCtrl_Reserved_7_6_WIDTH                        2         //Unused
//#define EccErrCntCtrl_Reserved_7_6_MASK                         0xC0UL    //Unused

/// Bitfield Description : Leak Rate.
//#define EccErrCntCtrl_EccErrLeakRate_OFFSET                     8         //Unused
//#define EccErrCntCtrl_EccErrLeakRate_WIDTH                      5         //Unused
/**
 * @brief The EccErrInt mask bit for UMC::EccErrCntCtrl register
 *
 * @details This mask is used for EccErrCntCtrl[12:8]. The EccErrLeakRate is used to
 *          specifie the Leak Rate assuming 100MHz REFCLK.
 */
#define EccErrCntCtrl_EccErrLeakRate_MASK                       0x1F00UL

/// Bitfield Description : Reserved.
//#define EccErrCntCtrl_Reserved_31_13_OFFSET                     13              //Unused
//#define EccErrCntCtrl_Reserved_31_13_WIDTH                      19              //Unused
//#define EccErrCntCtrl_Reserved_31_13_MASK                       0xffffe000UL    //Unused

/**
 * @brief Structure of DRAM ECC Error Count Control (UMC::EccErrCntCtrl).
 * @details
 */
typedef union {
  struct {
    UINT32                                             EccErrCntEn:1 ; ///< Enables the per-chipselect ECC error counters
    UINT32                                            EccErrLeakEn:1 ; ///< Enable Leaky Bucket mode for ECC Error Counters
    UINT32                                            Reserved_3_2:2 ; ///< Reserved
    UINT32                                               EccErrInt:2 ; ///< Specifies the type of interrupt generated
    UINT32                                            Reserved_7_6:2 ; ///< Reserved
    UINT32                                          EccErrLeakRate:5 ; ///< Leak Rate
    UINT32                                          Reserved_31_13:19; ///< Reserved
  } Field;
  UINT32 Value;
} UMC_ECC_ERR_CNT_CTRL_REG;

/**
 * @brief Structure of DRAM Address Configuration (UMC::AddrCfg).
 * @details
 */
typedef union {
  struct {
    UINT32                                            Reserved_1_0:2 ; ///< Reserved
    UINT32                                           NumBankGroups:2 ; ///< Specifies the number of BG bits
    UINT32                                                   NumRM:3 ; ///< Specifies the number of RM bits
    UINT32                                              Reserved_7:1 ; ///< Reserved
    UINT32                                                  NumRow:4 ; ///< Specifies the number of row address bits
    UINT32                                          Reserved_15_12:4 ; ///< Reserved
    UINT32                                                  NumCol:4 ; ///< Specifies the number of column address bits
    UINT32                                                NumBanks:2 ; ///< Specifies the number of total bank address bits, including NumBankGroups
    UINT32                                          Reserved_29_22:8 ; ///< Reserved
    UINT32                                                   CSXor:2 ; ///< Specifies XOR function bits to remap the CS decoder
  } Field;
  UINT32 Value;
} UMC_ADDRCFG_REG;

//
// Register Name EccErrCntCtrl
//
 /**
 * @brief The address of UMC00CHx00000D88 in the UMC Controller Registers space
 *
 * @details This is the SMN address of [DRAM ECC Error Count Control] (UMC::EccErrCntCtrl)@UMC0.
 */
#define SMN_UMC_EccErrCntCtrl_ADDRESS                  0x50d88UL
//#define SMN_UMC_EccErrCntCtrl_OFFSET                   0xd88UL    //Unused

///DF
// Note that these must be consecutive.
 /**
 * @brief The Data Fabric revision UNKNOWN.
 *
 * @details Can only be used for broadcast accesses to determine the DF type
 */
#define DF_TYPE_UNKNOWN    99 // Can only be used for broadcast accesses to determine the DF type.

 /**
 * @brief The Data Fabric revision 2.
 *
 * @details Read F1x208. In DF3, this is the ComponentIdMask and that must be non-zero.
 *          In DF2, F1x208[7:0] was reserved. If F1x208[7:0] is non-zero, this is DF3 Else it is DF2.
 *          All of the accesses here must be done as "broadcast accesses"
 */
#define DF_TYPE_DF2         1

 /**
 * @brief The Data Fabric revision 3.
 *
 * @details Read F1x208. In DF3, this is the ComponentIdMask and that must be non-zero.
 *          In DF2, F1x208[7:0] was reserved. If F1x208[7:0] is non-zero, this is DF3.
 *          All of the accesses here must be done as "broadcast accesses"
 */
#define DF_TYPE_DF3         2

 /**
 * @brief The Data Fabric revision 3.5.
 *
 * @details  Read F1x150. F1x150[15:0] is the ComponentIdMask and at least some LSB bits must be non-zero.
 *           Prior to DF 3.5, F1x150 was reserved. If F1x150[7:0] != 0, then this is "DF3.5"
 *          All of the accesses here must be done as "broadcast accesses"
 */
#define DF_TYPE_DF3POINT5   3 // RMB, VG, Trento and MI200 (Trento+MI200 is heterogeneous support only)

 /**
 * @brief The Data Fabric revision 4.
 *
 * @details Read F0x40 (FabricBlockInstanceCount). Bits 27:24 specify the MajorRevision,
 *          but this was added in DF4. Prior to DF4, this field is always zero.
 */
#define DF_TYPE_DF4         4

 /**
 * @brief The PCI Function number of Data Fabric Function 0
 *
 * @details Data Fabric Function 0 is located at Bus 0x00/ Device 0x18/ Function 0x00 with Vendor ID 1022h and Device ID 14ADh
 */
#define DF_CFGADDRESSCNTL_FUNC              (0x00)

 /**
 * @brief The PCI address offset of [Config Address Control] (DF::CfgAddressCntl) in DF Function 0's PCI configuration space.
 *
 * @details DF::CfgAddressCntl[7:0] SecBusNum: Specifies the configuration-space bus number of the IO Link.
 */
#define DF_CFGADDRESSCNTL_OFFSET            (0xC04)  //D18F0xC04

 /**
 * @brief The SMN address of DF Function 6.
 *
 * @details DF_DFF6_REG_BASE points to the DF Function 6's PCI configuration spaces register 0x00 (DF::DeviceVendorId6)
 */
#define DF_DFF6_REG_BASE                    (0x49006000)  //D18F6x000. Genesis: (0x0001D800)  //Todo

 /**
 * @brief The SMN address of DF Function 2.
 *
 * @details DF_DFF2_REG_BASE points to the DF Function 2's PCI configuration spaces register 0x00 (DF::DeviceVendorId2)
 */
#define DF_DFF2_REG_BASE                    (0x49002000)  //D18F2x000

 /**
 * @brief The SMN address offset of DF::DramScrubErrAddrLo in DF Function 2.
 *
 * @details D18F2x8F0 [DRAM Scrubber Error Address Low] (DF::DramScrubErrAddrLo):
 *          DRAM Scrubber Error Injection Address Low.
 */
#define DRAM_SCRUBBER_ERRORADDR_LO          (0x000008F0)  //DF::DramScrubErrAddrLo

 /**
 * @brief The ERR_INJ_EN bit in DF::DramScrubErrAddrLo.
 *
 * @details DF::DramScrubErrAddrLo[0]: ErrInjEn
 *          A new command to inject error is triggered by writing a 1.
 */
  #define ERR_INJ_EN                        BIT0

 /**
 * @brief The SMN address offset of DF::DramScrubErrAddrHi in DF Function 2.
 *
 * @details D18F2x8F4 [DRAM Scrubber Error Address High] (DF::DramScrubErrAddrHi):
 *          DRAM Scrubber Error Injection Address High.
 */
#define DRAM_SCRUBBER_ERRORADDR_HI          (0x000008F4)  //(DF::DramScrubErrAddrHi)

 /**
 * @brief The SMN address offset of UMC::DramScrubCtrl) in UMC Controller register space.
 *
 * @details UMC[00...11]CHx00000174 [DRAM Scrub Control] (UMC::DramScrubCtrl)
 *          Control the properties of the DRAM Scrubber.
 */
#define DRAM_SCRUBBER_CONTROL_REG          (0x00000174)  //DramScrubCtrl)

 /**
 * @brief The SMN address offset of UMC::DramScrubBaseAddr in UMC Controller register space.
 *
 * @details UMC[00...11]CHx00000178 [DRAM Scrub Base Address] (UMC::DramScrubBaseAddr)
 *          Specifies the normalized base address of the DRAM region for memory clear and periodic scrub.
 */
#define DRAM_SCRUBBER_BASEADDR_REG          (0x00000178)  //DramScrubBaseAddr

 /**
 * @brief The SMN address offset of UMC::DramScrubLimitAddr in UMC Controller register space.
 *
 * @details UMC[00...11]CHx0000017C [DRAM Scrub Limit Address] (UMC::DramScrubLimitAddr)
 *          Specifies the normalized limit address of the DRAM region for memory clear and periodic scrub.
 */
#define DRAM_SCRUBBER_LIMIT_ADDR            (0x0000017C)  //DramScrubLimitAddr

 /**
 * @brief The SMN address offset of (UMC::DramScrubAddrLo in UMC Controller register space.
 *
 * @details UMC[00...11]CHx00000180 [DRAM Scrub Addr Low] (UMC::DramScrubAddrLo)
 *          Patrol Scrubber current position address low
 */
#define DRAM_SCRUBBER_ADDR_LO               (0x00000180)  //DramScrubAddrLo

 /**
 * @brief The SMN address offset of (UMC::DramScrubAddrHi in UMC Controller register space.
 *
 * @details UMC[00...11]CHx00000184 [DRAM Scrub Addr High] (UMC::DramScrubAddrHi)
 *          Patrol Scrubber current position address high.
 */
#define DRAM_SCRUBBER_ADDR_HI               (0x00000184)  //DramScrubAddrHi

 /**
 * @brief The mask for DF::DramScrubErrAddrLo
 *
 * @details D18F2x8F0 [DRAM Scrubber Error Address Low] (DF::DramScrubErrAddrLo) Bit 5:0
 *          are not used to specify the ErrInjAddrLo. Therefore, use this mask to mask Bit 5:0.
 */
#define DRAM_SCRUBBER_ERROR_ADDRESS_LOW_MASK (0xFFFFFFFFFFFFFFC0)

// These are designed to map 1:1 to the hardware encodings, but
// any special case overlaps are moved to >0x20 and handled
// specifically in decodeIntLvNumChan()
 /**
 * @brief Interleave mode - 1 channel (no interleave)
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_NONE                  0x00
 /**
 * @brief Interleave mode - 2 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_2CHAN_NOHASH          0x01
 /**
 * @brief Interleave mode - 4 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_4CHAN_NOHASH          0x03
 /**
 * @brief Interleave mode - 8 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_8CHAN_NOHASH          0x05
 /**
 * @brief Interleave mode - 6 channels
 *
 * @details Genesis SoC - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 5:2
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in Genesis PPR.
 */
#define INTERLEAVE_MODE_DF3_6CHAN             0x06
 /**
 * @brief Interleave mode - 16 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_16CHAN_NOHASH         0x07
 /**
 * @brief Interleave mode - 32 channels
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_32CHAN_NOHASH         0x08
 /**
 * @brief Interleave mode - COD-4 2 channel hash
 *
 * @details Genesis SoC - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 5:2
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in Genesis PPR.
 */
#define INTERLEAVE_MODE_DF3_COD4_2CHAN_HASH   0x0C
 /**
 * @brief Interleave mode - COD-2 4 channel hash
 *
 * @details Genesis SoC - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 5:2
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in Genesis PPR.
 */
#define INTERLEAVE_MODE_DF3_COD2_4CHAN_HASH   0x0D
 /**
 * @brief Interleave mode - COD-1 8 channel hash
 *
 * @details Genesis SoC - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 5:2
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in Genesis PPR.
 */
#define INTERLEAVE_MODE_DF3_COD1_8CHAN_HASH   0x0E
 /**
 * @brief Interleave mode - NPS-4 2 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS4_2CHAN_HASH   0x10
 /**
 * @brief Interleave mode - NPS-2 4 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS2_4CHAN_HASH   0x11
 /**
 * @brief Interleave mode - NPS-1 8 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS1_8CHAN_HASH   0x12
 /**
 * @brief Interleave mode - NPS-4 3 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS4_3CHAN_HASH   0x13
 /**
 * @brief Interleave mode - NPS-2 6 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS2_6CHAN_HASH   0x14
 /**
 * @brief Interleave mode - NPS-1 12 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS1_12CHAN_HASH  0x15
 /**
 * @brief Interleave mode - NPS-2 5 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS2_5CHAN_HASH   0x16
 /**
 * @brief Interleave mode - NPS-1 10 channel hash
 *
 * @details D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Map Initialization] section in RPR.
 */
#define INTERLEAVE_MODE_DF4_NPS1_10CHAN_HASH  0x17
 /**
 * @brief Interleave mode - 8 channel hash
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define INTERLEAVE_MODE_MI_HASH_8CHAN         0x1C
 /**
 * @brief Interleave mode - 16 channel hash
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define INTERLEAVE_MODE_MI_HASH_16CHAN        0x1D
 /**
 * @brief Interleave mode - 32 channel hash
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define INTERLEAVE_MODE_MI_HASH_32CHAN        0x1E
 /**
 * @brief Interleave mode - 2 channel hash
 *
 * @details Genesis SoC - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 7:4
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved.
 *          For details, please refer to the [DRAM Address Maps] section in Zeppelin PPR.
 */
#define INTERLEAVE_MODE_ZP_2CHAN_HASH         0x21 // This really uses IntLvNumChan=8, but gets a different enum here


// Defines for register bit positions in DF
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 28
 *
 * @details Zeppelin - DF::SystemFabricIdMask[31:28] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the
 *          SocketId by to align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF__SOCKET_ID_SHIFT_BITPOS_LO_DF2       28
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 31
 *
 * @details Zeppelin - DF::SystemFabricIdMask[31:28] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the
 *          SocketId by to align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF__SOCKET_ID_SHIFT_BITPOS_HI_DF2       31
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 16
 *
 * @details Zeppelin - DF::SystemFabricIdMask[23:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_LO_DF2        16
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 23
 *
 * @details Zeppelin - DF::SystemFabricIdMask[23:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_HI_DF2        23
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 24
 *
 * @details Zeppelin - DF::SystemFabricIdMask[27:24] DieIdShft.
 *          DieId shift amount for the system. Tells how many bits to shift the DieId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the DieId.
 */
#define DF__DIE_ID_SHIFT_BITPOS_LO_DF2          24
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 27
 *
 * @details Zeppelin - DF::SystemFabricIdMask[27:24] DieIdShft.
 *          DieId shift amount for the system. Tells how many bits to shift the DieId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the DieId.
 */
#define DF__DIE_ID_SHIFT_BITPOS_HI_DF2          27
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 8
 *
 * @details Zeppelin - DF::SystemFabricIdMask[15:8] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          FabricId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_LO_DF2            8
 /**
 * @brief Zeppelin - D18F1x208 [System Fabric ID Mask] (DF::SystemFabricIdMask) Bit 15
 *
 * @details Zeppelin - DF::SystemFabricIdMask[15:8] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          FabricId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_HI_DF2           15
 /**
 * @brief Rmb - D18F4x1B4 [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 8
 *
 * @details Rmb - DF::SystemFabricIdMask1[11:8] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the SocketId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF__SOCKET_ID_SHIFT_BITPOS_LO_DF3        8
 /**
 * @brief Rmb - D18F4x1B4 [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 11
 *
 * @details Rmb - DF::SystemFabricIdMask1[11:8] SocketIdShft.
 *          SocketId shift amount for the system. Tells how many bits to shift the SocketId by to
 *          align it to bit 0. In effect, this field identifies the LSB of the SocketId.
 */
#define DF__SOCKET_ID_SHIFT_BITPOS_HI_DF3       11 // not all bits are present, but they are reserved in DF3 and used in DF4
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 24
 *
 * @details Genesis - DF::SystemFabricIdMask1[26:24] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding bit in
 *          the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_LO_DF3        24
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 26
 *
 * @details Genesis - DF::SystemFabricIdMask1[26:24] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding bit in
 *          the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_HI_DF3        26
 /**
 * @brief Rmb - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 0
 *
 * @details Rmb - DF::SystemFabricIdMask2[15:0] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_LO_DF4            0
 /**
 * @brief Rmb - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 15
 *
 * @details Rmb - DF::SystemFabricIdMask2[15:0] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_HI_DF4           15

 /**
 * @brief Rmb - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 16
 *
 * @details Rmb - DF::SystemFabricIdMask2[31:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding
 *          bit in the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_LO_DF4        16
 /**
 * @brief Rmb - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2) Bit 31
 *
 * @details Rmb - DF::SystemFabricIdMask2[31:16] SocketIdMask.
 *          SocketId Mask for the system. If a bit in this mask is set, then the corresponding
 *          bit in the NodeId is used to identify a socket. SocketId bits must be contiguous.
 */
#define DF__SOCKET_ID_MASK_BITPOS_HI_DF4        31

// Note: no DF__DIE_ID_SHIFT_BITPOS_LO_DF3 and DF__DIE_ID_SHIFT_BITPOS_HI_DF3.
// Use the DF__NODE_ID_SHIFT_BITPOS_ to determine the LSB of NodeID.
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 16
 *
 * @details Genesis - DF::SystemFabricIdMask1[18:16] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_LO_DF3           16
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 18
 *
 * @details Genesis - DF::SystemFabricIdMask1[18:16] DieIdMask.
 *          DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the
 *          NodeId is used to identify a die. DieId bits must be contiguous.
 */
#define DF__DIE_ID_MASK_BITPOS_HI_DF3           18
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 0
 *
 * @details Genesis - DF::SystemFabricIdMask1[3:0] NodeIdShft.
 *          NodeId shift amount for the system. Tells how many bits to shift the NodeId by to align it
 *          to bit 0. In effect, this field identifies the LSB of the NodeId.
 */
#define DF__NODE_ID_SHIFT_BITPOS_LO_DF3          0
 /**
 * @brief Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1) Bit 3
 *
 * @details Genesis - DF::SystemFabricIdMask1[3:0] NodeIdShft.
 *          NodeId shift amount for the system. Tells how many bits to shift the NodeId by to align it
 *          to bit 0. In effect, this field identifies the LSB of the NodeId.
 */
#define DF__NODE_ID_SHIFT_BITPOS_HI_DF3          3
 /**
 * @brief Rmb - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 16
 *
 * @details Rmb - DF::SystemFabricIdMask0[31:16] NodeIdMask.
 *          NodeId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a node. NodeId bits must be contiguous.
 */
#define DF__NODE_ID_MASK_BITPOS_LO_DF3          16
 /**
 * @brief Rmb - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 31
 *
 * @details Rmb - DF::SystemFabricIdMask0[31:16] NodeIdMask.
 *          NodeId Mask for the system. If a bit in this mask is set, then the corresponding bit
 *          in the FabricId is used to identify a node. NodeId bits must be contiguous.
 */
#define DF__NODE_ID_MASK_BITPOS_HI_DF3          31 // not all bits are present, but they are reserved in DF3 and used in DF4
 /**
 * @brief Rmb - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 0
 *
 * @details Rmb - DF::SystemFabricIdMask0[15:0] CompIdMask.
 *          ComponentId Mask for the system. If a bit in this mask is set, then the
 *          corresponding bit in the FabricId is used to identify a component. ComponentId bits must be contiguous.
 */
#define DF__COMPONENT_ID_MASK_BITPOS_LO_DF3      0
 /**
 * @brief Rmb - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0) Bit 15
 *
 * @details Rmb - DF::SystemFabricIdMask0[15:0] CompIdMask.
 *          ComponentId Mask for the system. If a bit in this mask is set, then the
 *          corresponding bit in the FabricId is used to identify a component. ComponentId bits must be contiguous.
 */
#define DF__COMPONENT_ID_MASK_BITPOS_HI_DF3     15 // not all bits are present, but they are reserved in DF3 and used in DF4
 /**
 * @brief Rmb - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 0
 *
 * @details Rmb - DF::FabricBlockInstanceCount[7:0] BlkInstCount.
 *          Provides the total number of instances of all the blocks in the Data Fabric.
 */
#define DF__BLOCK_INSTANCE_COUNT_BITPOS_LO       0
 /**
 * @brief Rmb - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 7
 *
 * @details Rmb - DF::FabricBlockInstanceCount[7:0] BlkInstCount.
 *          Provides the total number of instances of all the blocks in the Data Fabric.
 */
#define DF__BLOCK_INSTANCE_COUNT_BITPOS_HI       7
 /**
 * @brief Rmb - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 24
 *
 * @details Rmb - DF::FabricBlockInstanceCount[27:24] MajorRevision.
 *          Specifies the major revision of the Data Fabric hardware, MajorRevision.MinorRevision.
 */
#define DF__MAJOR_REVISION_BITPOS_LO            24
 /**
 * @brief Rmb - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount) Bit 27
 *
 * @details Rmb - DF::FabricBlockInstanceCount[27:24] MajorRevision.
 *          Specifies the major revision of the Data Fabric hardware, MajorRevision.MinorRevision.
 */
#define DF__MAJOR_REVISION_BITPOS_HI            27
 /**
 * @brief Rmb - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) Bit 0
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          Instance type register.
 */
#define DF__INSTANCE_TYPE_BITPOS_LO              0
 /**
 * @brief Rmb - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) Bit 3
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          Instance type register.
 */
#define DF__INSTANCE_TYPE_BITPOS_HI              3
 /**
 * @brief Rmb - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM) Bit 8
 *
 * @details Rmb - DF::FabricBlockInstanceInformation3_CSNCSPIEALLM[19:8] BlockFabricID.
 *          This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are
 *          no pins allocated on the package to identify the socket and die. Allocation of bits in this field for
 *          socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
 */
#define DF__BLOCK_FABRICID_BITPOS_LO             8
 /**
 * @brief Rmb - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM) Bit 19
 *
 * @details Rmb - DF::FabricBlockInstanceInformation3_CSNCSPIEALLM[19:8] BlockFabricID.
 *          This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are
 *          no pins allocated on the package to identify the socket and die. Allocation of bits in this field for
 *          socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
 */
#define DF__BLOCK_FABRICID_BITPOS_HI            19 // not all bits are present, but they are reserved in DF3 and used in DF4
#define DF__INSTANCE_SUBTYPE_BITPOS_LO          24
#define DF__INSTANCE_SUBTYPE_BITPOS_HI          26

 /**
 * @brief Zeppelin - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 0
 *        Genesis  - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 0
 *        Rmb      - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 0
 *
 * @details Zeppelin, Genesis,  Rmb - DF::DramOffset[0] HiAddrOffsetEn.
 *          The offset specified by HiAddrOffset is added when forming the normalized address.
 *          Control addition of HiAddrOffset when forming normalized address. This field must be set to one when
 *          HiAddrOffset is non-zero.
 */
#define DF__HI_ADDR_OFFSET_EN_BITPOS             0
 /**
 * @brief Zeppelin - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 20
 *
 * @details Zeppelin - DF::DramOffset[31:20] HiAddrOffset.
 *          Offset address[39:28] for CS DRAM Address range 1. Specifies the normalized address at the base of the associated range.
 *          HiAddrOffset is always the normalized address for DF::DramBaseAddress[DramBaseAddr].
 */
#define DF__HI_ADDR_OFFSET_BITPOS_LO_DF2        20
 /**
 * @brief Zeppelin - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 31
 *
 * @details Zeppelin - DF::DramOffset[31:20] HiAddrOffset.
 *          Offset address[39:28] for CS DRAM Address range 1. Specifies the normalized address at the base of the associated range.
 *          HiAddrOffset is always the normalized address for DF::DramBaseAddress[DramBaseAddr].
 */
#define DF__HI_ADDR_OFFSET_BITPOS_HI_DF2        31
 /**
 * @brief Genesis - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 12
 *
 * @details Genesis - DF::DramOffset[31:12] HiAddrOffset.
 *          Offset address[47:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of the
 *          associated range.
 */
#define DF__HI_ADDR_OFFSET_BITPOS_LO_DF3        12
 /**
 * @brief Genesis - D18F0x1B4 [DRAM Offset] (DF::DramOffset) Bit 31
 *
 * @details Genesis - DF::DramOffset[31:12] HiAddrOffset.
 *          Offset address[47:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of the
 *          associated range.
 */
#define DF__HI_ADDR_OFFSET_BITPOS_HI_DF3        31
 /**
 * @brief Rmb - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 1
 *
 * @details Rmb - DF::DramOffset[24:1] HiAddrOffset.
 *          Offset address[51:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of the
 *          associated range.
 */
#define DF__HI_ADDR_OFFSET_BITPOS_LO_DF4         1
 /**
 * @brief Rmb - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset) Bit 24
 *
 * @details Rmb - DF::DramOffset[24:1] HiAddrOffset.
 *          Offset address[51:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of the
 *          associated range.
 */
#define DF__HI_ADDR_OFFSET_BITPOS_HI_DF4        24

 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 0
 *        Genesis  - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 0
 *        Rmb      - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 0
 *
 * @details Zeppelin - DF::DramBaseAddress[0] AddrRngVal.
 *          Genesis  - DF::DramBaseAddress[0] AddrRngVal.
 *          Rmb      - DF::DramAddressCtl[0] AddrRngVal.
 *          Address range 0 valid (enabled/disabled)
 */
#define DF__ADDR_RANGE_VALID_BITPOS              0
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 12
 *
 * @details Genesis - DF::DramBaseAddress[31:12] DramBaseAddress.
 *          Dram Base address[47:28]
 */
#define DF__DRAM_BASE_ADDR_BITPOS_LO            12
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 31
 *
 * @details Genesis - DF::DramBaseAddress[31:12] DramBaseAddress.
 *          Dram Base address[47:28]
 */
#define DF__DRAM_BASE_ADDR_BITPOS_HI            31
 /**
 * @brief Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress) Bit 12
 *
 * @details Genesis - DF::DramLimitAddress[31:12] DramLimitAddr.
 *          DRAM limit address[47:28]
 */
#define DF__DRAM_LIMIT_ADDR_BITPOS_LO           12
 /**
 * @brief Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress) Bit 31
 *
 * @details Genesis - DF::DramLimitAddress[31:12] DramLimitAddr.
 *          DRAM limit address[47:28]
 */
#define DF__DRAM_LIMIT_ADDR_BITPOS_HI           31
 /**
 * @brief Rmb - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress) Bit 0
 *
 * @details Rmb - DF::DramBaseAddress[27:0] DramBaseAddr.
 *          Dram Base address[55:28]
 */
#define DF__DRAM_BASE_ADDR_BITPOS_LO_DF4         0
 /**
 * @brief Rmb - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress) Bit 27
 *
 * @details Rmb - DF::DramBaseAddress[27:0] DramBaseAddr.
 *          Dram Base address[55:28]
 */
#define DF__DRAM_BASE_ADDR_BITPOS_HI_DF4        27
 /**
 * @brief Rmb - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress) Bit 0
 *
 * @details Rmb - DF::DramLimitAddress[27:0] DramLimitAddr.
 *          DRAM limit address[55:28]
 */
#define DF__DRAM_LIMIT_ADDR_BITPOS_LO_DF4        0
 /**
 * @brief Rmb - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress) Bit 27
 *
 * @details Rmb - DF::DramLimitAddress[27:0] DramLimitAddr.
 *          DRAM limit address[55:28]
 */
#define DF__DRAM_LIMIT_ADDR_BITPOS_HI_DF4       27
 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 1
 *        Genesis  - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 1
 *        Rmb      - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 1
 *
 * @details Zeppelin - DF::DramBaseAddress[1] LgcyMmioHoleEn..
 *          Genesis  - DF::DramBaseAddress[1] LgcyMmioHoleEn.
 *          Rmb      - DF::DramAddressCtl[1] LgcyMmioHoleEn.
 *          0=Memory hoisting is not enabled. 1=Enable memory hoisting for this address range.
 */
#define DF__LEGACY_MMIO_HOLE_EN_BITPOS           1
 /**
 * @brief Rmb - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 24
 *
 * @details Rmb - DF::DramHoleControl[31:24] DramHoleBase.
 *          Dram Hole Base[31:24]. Specifies the base address of the IO hole below the 4GB address level
 *          for legacy 32-bit devices. MMIO hole cannot spam multiple DRAM ranges; it must lie within a
 *          single DRAM address range. DRAM range which spans the IO hole needs to be hoisted (added) to
 *          compensate for the hole.
 */
#define DF__DRAM_HOLE_BASE_ADDR_BITPOS_LO       24
 /**
 * @brief Rmb - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 31
 *
 * @details Rmb - DF::DramHoleControl[31:24] DramHoleBase.
 *          Dram Hole Base[31:24]. Specifies the base address of the IO hole below the 4GB address level
 *          for legacy 32-bit devices. MMIO hole cannot spam multiple DRAM ranges; it must lie within a
 *          single DRAM address range. DRAM range which spans the IO hole needs to be hoisted (added) to
 *          compensate for the hole.
 */
#define DF__DRAM_HOLE_BASE_ADDR_BITPOS_HI       31
 /**
 * @brief Zeppelin - D18F0x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 0
 *        Genesis - D18F0x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 0
 *        Rmb     - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl) Bit 0
 *
 * @details Zeppelin - DF::DramHoleControl[0] DramHoleValid.
 *          Genesis - DF::DramHoleControl[0] DramHoleValid.
 *          Rmb - DF::DramHoleControl[0] DramHoleValid.
 *          Dram Hole Valid.
 */
#define DF__DRAM_HOLE_VALID_BITPOS               0
 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 8
 *
 * @details Zeppelin - DF::DramBaseAddress[10:8] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved.
 */
#define DF__INTLV_ADDR_SEL_BITPOS_LO_DF2         8
 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 10
 *
 * @details Zeppelin - DF::DramBaseAddress[10:8] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved.
 */
#define DF__INTLV_ADDR_SEL_BITPOS_HI_DF2        10
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 9
 *
 * @details Genesis - DF::DramBaseAddress[11:9] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF__INTLV_ADDR_SEL_BITPOS_LO_DF3         9
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 11
 *
 * @details Genesis - DF::DramBaseAddress[11:9] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF__INTLV_ADDR_SEL_BITPOS_HI_DF3        11
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 0
 *
 * @details Rmb - DF::DramAddressIntlv[2:0] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF__INTLV_ADDR_SEL_BITPOS_LO_DF4         0
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 2
 *
 * @details Rmb - DF::DramAddressIntlv[2:0] IntLvAddrSel.
 *          Encoded value for this address range which specifies the starting address bit used for interleaving.
 *          The number of address bits used for interleaving depends on the number of channels across
 *          which they are interleaved. Values not listed are RESERVED
 */
#define DF__INTLV_ADDR_SEL_BITPOS_HI_DF4         2
 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 4
 *
 * @details Zeppelin - DF::DramBaseAddress[7:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Supported Channel interleave: none, 2, 4, 8, or 16 channels; 2 channel hash.
 *          When using 2 channel hash: IntLvAddrSel must select address bit 8 or 9.
 *          Die and socket interleave must be turned off. Rule: (DF::DramBaseAddress_instCS0[IntLvNumChan] != 1). Rule:
 *          (DF::DramBaseAddress_instCS1[IntLvNumChan] != 1). This product does not support tri-channel interleave.
 */
#define DF__INTLV_NUM_CHAN_BITPOS_LO_DF2         4
 /**
 * @brief Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress) Bit 7
 *
 * @details Zeppelin - DF::DramBaseAddress[7:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Supported Channel interleave: none, 2, 4, 8, or 16 channels; 2 channel hash.
 *          When using 2 channel hash: IntLvAddrSel must select address bit 8 or 9.
 *          Die and socket interleave must be turned off. Rule: (DF::DramBaseAddress_instCS0[IntLvNumChan] != 1). Rule:
 *          (DF::DramBaseAddress_instCS1[IntLvNumChan] != 1). This product does not support tri-channel interleave.
 */
#define DF__INTLV_NUM_CHAN_BITPOS_HI_DF2         7
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 2
 *
 * @details Genesis - DF::DramBaseAddress[5:2] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Supported Channel interleave: none, 2, 4, or 8 channels.
 *          If 6-channel is interleaved, then IntLvAddrSel MUST be 3 or 4 (2KB or 4KB).
 *          This product does not support tri-channel interleave.
 */
#define DF__INTLV_NUM_CHAN_BITPOS_LO_DF3         2
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 5
 *
 * @details Genesis - DF::DramBaseAddress[5:2] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Supported Channel interleave: none, 2, 4, or 8 channels.
 *          If 6-channel is interleaved, then IntLvAddrSel MUST be 3 or 4 (2KB or 4KB).
 *          This product does not support tri-channel interleave.
 */
#define DF__INTLV_NUM_CHAN_BITPOS_HI_DF3         5
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 4
 *
 * @details Rmb - DF::DramAddressIntlv[8:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Hash interleaves must program IntLvAddrSel to select address bit [8].
 */
#define DF__INTLV_NUM_CHAN_BITPOS_LO_DF4         4
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 8
 *
 * @details Rmb - DF::DramAddressIntlv[8:4] IntLvNumChan.
 *          Encoded value for this address range which specifies the number of coherent slaves across which
 *          addresses are interleaved. Hash interleaves must program IntLvAddrSel to select address bit [8].
 */
#define DF__INTLV_NUM_CHAN_BITPOS_HI_DF4         8
 /**
 * @brief  On Rmb SoC, this is Reserved.
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define DF__INTLV_NUM_CHAN_BITPOS_HI_DF3POINT5   6
/**
 * @brief Zeppelin - D18F0x1[14...8C] [DRAM Limit Address] (DF::DramLimitAddress) Bit 10
 *
 * @details Zeppelin - DF::DramLimitAddress[11:10] IntLvNumDies.
 *          Encoded value for this address range which specifies the number of dies across which addresses are
 *          interleaved. Supported Die Interleave Configurations. Interleave on 2 or 4 Dies => interleave
 *          starting address bit 8, 9, 10, or 11. No die interleave. Interleave specification bits must contiguous
 *          beginning in the starting address bit in this arrangement: {socket; die; channel}.
 */
#define DF__INTLV_NUM_DIES_BITPOS_LO_DF2        10
/**
 * @brief Zeppelin - D18F0x1[14...8C] [DRAM Limit Address] (DF::DramLimitAddress) Bit 11
 *
 * @details Zeppelin - DF::DramLimitAddress[11:10] IntLvNumDies.
 *          Encoded value for this address range which specifies the number of dies across which addresses are
 *          interleaved. Supported Die Interleave Configurations. Interleave on 2 or 4 Dies => interleave
 *          starting address bit 8, 9, 10, or 11. No die interleave. Interleave specification bits must contiguous
 *          beginning in the starting address bit in this arrangement: {socket; die; channel}.
 */
#define DF__INTLV_NUM_DIES_BITPOS_HI_DF2        11
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 6
 *
 * @details Genesis - DF::DramBaseAddress[7:6] IntLvNumDies.
 *          Encoded value for this address range which specifies the number of dies across which addresses are
 *          interleaved. Supported Die Interleave Configurations Interleave on 2 or 4 Dies => interleave
 *          starting address bit 8, 9, 10, or 11 No die interleave Interleave specification bits must contiguous
 *          beginning in the starting address bit in this arrangement: {socket; die; channel}.
 */
#define DF__INTLV_NUM_DIES_BITPOS_LO_DF3         6
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 7
 *
 * @details Genesis - DF::DramBaseAddress[7:6] IntLvNumDies.
 *          Encoded value for this address range which specifies the number of dies across which addresses are
 *          interleaved. Supported Die Interleave Configurations Interleave on 2 or 4 Dies => interleave
 *          starting address bit 8, 9, 10, or 11 No die interleave Interleave specification bits must contiguous
 *          beginning in the starting address bit in this arrangement: {socket; die; channel}.
 */
#define DF__INTLV_NUM_DIES_BITPOS_HI_DF3         7
 /**
 * @brief  On Rmb SoC, this is Reserved.
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define DF__INTLV_NUM_DIES_BITPOS_DF3POINT5      7
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 12
 *
 * @details Rmb - DF::DramAddressIntlv[13:12] IntLvNumDies.
 *          Encoded value which specifies the number of dies across which addresses are interleaved.
 */
#define DF__INTLV_NUM_DIES_BITPOS_LO_DF4        12
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 13
 *
 * @details Rmb - DF::DramAddressIntlv[13:12] IntLvNumDies.
 *          Encoded value which specifies the number of dies across which addresses are interleaved.
 */
#define DF__INTLV_NUM_DIES_BITPOS_HI_DF4        13
 /**
 * @brief Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress) Bit 8
 *
 * @details Genesis - DF::DramBaseAddress[8] IntLvNumSockets.
 *          0=1 socket (no interleave). 1=2 sockets. Specifies the number of sockets across which
 *          addresses are interleaved. Socket interleaving is not supported when COD interleaving
 *          is enabled. If IntLvNumChan=12, IntLvNumSockets must be 0. If If IntLvNumChan=13,
 *          IntLvNumSockets must be 0. If IntLvNumChan=14, IntLvNumSockets must be 0.
 */
#define DF__INTLV_NUM_SOCKETS_BITPOS             8
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 18
 *
 * @details Rmb - DF::DramAddressIntlv[18] IntLvNumSockets.
 *          0=No socket interleave. 1=2 sockets. Specifies the number of sockets across which addresses
 *          are interleaved.
 */
#define DF__INTLV_NUM_SOCKETS_BITPOS_DF4        18
/**
 * @brief Zeppelin - D18F0x1[14...8C] [DRAM Limit Address] (DF::DramLimitAddress) Bit 0
 *
 * @details Zeppelin - DF::DramLimitAddress[7:0] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramBaseAddress[IntLvNumChan]) this field
 *          provides the target CS FabricId for this address map.
 */
#define DF__DSTFABRICID_BITPOS_LO_DF2            0
/**
 * @brief Zeppelin - D18F0x1[14...8C] [DRAM Limit Address] (DF::DramLimitAddress) Bit 7
 *
 * @details Zeppelin - DF::DramLimitAddress[7:0] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramBaseAddress[IntLvNumChan]) this field
 *          provides the target CS FabricId for this address map.
 */
#define DF__DSTFABRICID_BITPOS_HI_DF2            7
 /**
 * @brief Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress) Bit 0
 *
 * @details Genesis - DF::DramLimitAddress[9:0] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramBaseAddress[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF__DSTFABRICID_BITPOS_LO_DF3            0
 /**
 * @brief Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress) Bit 9
 *
 * @details Genesis - DF::DramLimitAddress[9:0] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramBaseAddress[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF__DSTFABRICID_BITPOS_HI_DF3            9
 /**
 * @brief  On Rmb SoC, this is Reserved.
 *
 * @details On Rmb SoC, this is Reserved.
 */
#define DF__DSTFABRICID_BITPOS_HI_DF3POINT5     11
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 16
 *
 * @details Rmb - DF::DramAddressCtl[27:16] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramAddressIntlv[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF__DSTFABRICID_BITPOS_LO_DF4           16
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 27
 *
 * @details Rmb - DF::DramAddressCtl[27:16] DstFabricID.
 *          When CS interleaving is disabled (see DF::DramAddressIntlv[IntLvNumChan]) this field provides
 *          the target CS FabricId for this address map. When CS interleaving is enabled, this field
 *          provides the CS Fabric ID where interleave starts.
 */
#define DF__DSTFABRICID_BITPOS_HI_DF4           27
 /**
 * @brief Genesis - D18F0x3F8 [DF Global Control] (DF::DfGlobalCtrl) Bit 20
 *
 * @details Genesis - DF::DfGlobalCtrl[20] GlbHashIntlvCtl64K.
 *          Enables inclusion of address bits on the 64KB boundary (bits 16, 17, 18) in the
 *          COD hashed interleave computation.
 */
#define DF__HASH_INTLV_CTL_64K_BITPOS_DF3       20
 /**
 * @brief Genesis - D18F0x3F8 [DF Global Control] (DF::DfGlobalCtrl) Bit 21
 *
 * @details Genesis - DF::DfGlobalCtrl[20] GlbHashIntlvCtl2M.
 *          Enables inclusion of address bits on the 2MB boundary (bits 21, 22, 23) in the
 *          COD hashed interleave computation.
 */
#define DF__HASH_INTLV_CTL_2M_BITPOS_DF3        21
 /**
 * @brief Genesis - D18F0x3F8 [DF Global Control] (DF::DfGlobalCtrl) Bit 22
 *
 * @details Genesis - DF::DfGlobalCtrl[20] GlbHashIntlvCtl1G.
 *          Enables inclusion of address bits on the 1GB boundary (bits 30, 31, 32) in the
 *          COD hashed interleave computation.
 */
#define DF__HASH_INTLV_CTL_1G_BITPOS_DF3        22
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 8
 *
 * @details Rmb - DF::DramAddressCtl[8] HashIntlvCtl64K.
 *          Enables inclusion of address bits on the 64KB boundary (bits 16, 17, 18) in a hashed
 *          interleave computation.
 */
#define DF__HASH_INTLV_CTL_64K_BITPOS_DF4        8
 /**
 * @brief Genesis - D18F2x090 [Non-power-of-2 channel Configuration Register for CS DRAM Address Maps] (DF::Np2ChannelConfig) Bit 0
 *
 * @details Genesis - DF::Np2ChannelConfig[5:0] Log2Addr64KSpace0.
 *          Program based on the size of the address map rounded up to the next power of two. For example, a 6-channel configuration
 *          takes the per-channel size times 8. Note: Ignore, or do not care, if the address map is a power-of-2.
 */
#define DF__LOG2_ADDR_SPACE_BITPOS_LO            0
 /**
 * @brief Genesis - D18F2x090 [Non-power-of-2 channel Configuration Register for CS DRAM Address Maps] (DF::Np2ChannelConfig) Bit 5
 *
 * @details Genesis - DF::Np2ChannelConfig[5:0] Log2Addr64KSpace0.
 *          Program based on the size of the address map rounded up to the next power of two. For example, a 6-channel configuration
 *          takes the per-channel size times 8. Note: Ignore, or do not care, if the address map is a power-of-2.
 */
#define DF__LOG2_ADDR_SPACE_BITPOS_HI            5
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 24
 *
 * @details Rmb - DF::DramAddressIntlv[24]
 */
#define DF__LOG2_ADDR_SPACE_BITPOS_LO_DF4       24
 /**
 * @brief Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv) Bit 29
 *
 * @details Rmb - DF::DramAddressIntlv[29]
 */
#define DF__LOG2_ADDR_SPACE_BITPOS_HI_DF4       29
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl) Bit 4
 *
 * @details Rmb - DF::DramAddressCtl[4] RemapEn.
 *          Remap Enable
 */
#define DF__REMAP_EN_BITPOS_DF4                  4
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::RemapSel) Bit 5
 *
 * @details Rmb - DF::DramAddressCtl[7:5] RemapSel.
 *          Remap Select
 */
#define DF__REMAP_SEL_BITPOS_LO_DF4              5
 /**
 * @brief Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::RemapSel) Bit 7
 *
 * @details Rmb - DF::DramAddressCtl[7:5] RemapSel.
 *          Remap Select
 */
#define DF__REMAP_SEL_BITPOS_HI_DF4              7


 /**
 * @brief Dram Base address LSB
 *
 * @details Zeppelin - D18F0x1[10...88] [DRAM Base Address] (DF::DramBaseAddress)[31:12] DramBaseAddr.
 *                     => Dram Base address[47:28] => MSB: 47, LSB: 28
 *          Genesis  - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress)[31:12] DramBaseAddr.
 *                     => Dram Base address[47:28] => MSB: 47, LSB: 28
 *          Rmb      - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)[27:0] DramBaseAddr.
 *                     => Dram Base address[55:28] => MSB: 55, LSB: 28
 */
#define DF__LSB_ADDR_BIT_IN_DRAM_MAPS           28
 /**
 * @brief HiAddrOffset LSB
 *
 * @details Zeppelin - D18F0x1B4 [DRAM Offset] (DF::DramOffset)[31:20] HiAddrOffset.
 *                     => Offset address[39:28] for CS DRAM Address range 1 => MSB: 39, LSB: 28
 *          Genesis  - D18F0x1B4 [DRAM Offset] (DF::DramOffset)[31:12] HiAddrOffset.
 *                     => Offset address[47:28] for CS DRAM Address range 1 => MSB: 47, LSB: 28
 *          Rmb      - D18F7x140...D18F7x148 [DRAM Offset] (DF::DramOffset)[24:1] HiAddrOffset.
 *                     => Offset address[51:28] for CS DRAM Address range 1 => MSB: 51, LSB: 28
 */
#define DF__LSB_ADDR_BIT_IN_DRAM_OFFSET         28
 /**
 * @brief Number of address map registers in a single map
 *
 * @details In DF4, we include the remap register as part of the map
 *          BASE+LIMIT+CTL+INTLV+REMAPHI+REMAPLO
 *          In DF3, we include the NP2_CHANNEL_CONFIG and the remap as part of the map
 *          BASE+LIMIT+{zeros}+{zeros}+NP2+REMAPLO
 */
#define ADDR_MAP_ARRAYSIZE                       6

 /**
 * @brief DF::DramBaseAddress register in single map[0]
 *
 * @details Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress)
 *           Rmb    - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)
 */
#define ADDR_MAP_ARRAY_BASE_OFFSET               0
 /**
 * @brief DF::DramLimitAddress register in single map[1]
 *
 * @details Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress)
 *           Rmb    - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress)
 */
#define ADDR_MAP_ARRAY_LIMIT_OFFSET              1
 /**
 * @brief DF::DramAddressCtl register in single map[2]
 *
 * @details Genesis - N/A
 *          Rmb     - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl)
 */
#define ADDR_MAP_ARRAY_CTL_OFFSET                2
 /**
 * @brief DF::DramAddressIntlv register in single map[3]
 *
 * @details Genesis - N/A
 *          Rmb     - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv)
 */
#define ADDR_MAP_ARRAY_INTLV_OFFSET              3
 /**
 * @brief DF::Np2ChannelConfig register in single map[4]
 *
 * @details Genesis - D18F2x090 [Non-power-of-2 channel Configuration Register for CS DRAM Address Maps] (DF::Np2ChannelConfig)
 */
#define ADDR_MAP_ARRAY_NP2_OFFSET                4 // DF3 only
 /**
 * @brief DF::CsTargetRemap0B register in single map[4]
 *
 * @details Rmb - D18F7x184 [CS Target Remap 0 Register B] (DF::CsTargetRemap0B)
 */
#define ADDR_MAP_ARRAY_REMAPHI_OFFSET            4 // DF4 only
 /**
 * @brief DF::CsTargetRemap0A register in single map[5]
 *
 * @details Genesis - D18F0x060 [Socket 0 CS Target Remap Register 0] (DF::Skt0CsTargetRemap0)
 *                    Or D18F0x068 [Socket 1 CS Target Remap Register 0] (DF::Skt1CsTargetRemap0)
 *          Rmb - D18F7x180 [CS Target Remap 0 Register A] (DF::CsTargetRemap0A)
 */
#define ADDR_MAP_ARRAY_REMAPLO_OFFSET            5
 /**
 * @brief The number of address maps in the hardware address space of the pre DF4 processor
 *
 * @details The number of address maps in the hardware address space of the pre DF4 processor: 16
 */
#define DF__NUM_DRAM_MAPS_AVAILABLE_PRE_DF4     16
 /**
 * @brief The number of address maps in the hardware address space of the DF4 processor
 *
 * @details The number of address maps in the hardware address space of the DF4 processor: 32
 */
#define DF__NUM_DRAM_MAPS_AVAILABLE_DF4         32
 /**
 * @brief Rmb - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) CCM instance type
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          0h = CCM
 */
#define DF__CCM_INSTANCE_TYPE_VALUE              0
#define DF__GCM_INSTANCE_TYPE_VALUE              1
 /**
 * @brief Rmb - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0) CS instance type
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          4h = CS
 */
#define DF__CS_INSTANCE_TYPE_VALUE               4
#define DF__NCS_INSTANCE_TYPE_VALUE              5
#define DF__CNLI_INSTANCE_TYPE_VALUE            13
#define DF__CSCMP_INSTANCE_SUBTYPE_VALUE         2
#define DF__IOS_INSTANCE_SUBTYPE_VALUE           2

 /**
 * @brief The number of address maps in the hardware address space of the processor
 *
 * @details The number of address maps in the hardware address space of the processor: 20
 */
#define NUM_DRAM_MAPS           (20)

 /**
 * @brief Rmb - CCM instance type value
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          0h = CCM
 */
#define CCMINSTANCETYPE         (0)
 /**
 * @brief Rmb - CS instance type value
 *
 * @details Rmb - DF::FabricBlockInstanceInformation0[3:0] InstanceType.
 *          4h = CS
 */
#define CSINSTANCETYPE          (4)
 /**
 * @brief Rmb - DF Configuration Register Broadcast access instance ID.
 *
 * @details For details, please refer to RPR - "DF Configuration Register Broadcast and Instance Mode Access" Section.
 */
#define BROADCAST_ACCESS        (0xffffffff)
 /**
 * @brief Rmb - Number of IOD.
 *
 * @details Rmb microprocessor contains multiple core/cache complex dies (CCD) plus an I/O die (IOD).
 */
#define DIE_PER_SOCKET          (1)
 /**
 * @brief The total number of UMC
 *
 * @details Rmb - Four Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define UMC_PER_DIE             (4)
/**
 * @brief Rmb - The number of UMC channel on an UMC
 *
 * @details Rmb - Twelve Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define CHANNEL_PER_UMC         (1)
/**
 * @brief Zeppelin, Genesis - PCI function number and register offset of DF::SystemFabricIdMask0 register
 *
 * @details Zeppelin, Genesis - D18F1x208 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0)
 */
#define DF__SYSFABIDMASK0_REGADDR_DF2_AND_DF3 ((1<<12)|(0x208))
/**
 * @brief PCI function number and register offset of DF::SystemFabricIdMask0 register
 *
 * @details Reserved in Rmb
 */
#define DF__SYSFABIDMASK0_REGADDR_DF3POINT5   ((1<<12)|(0x150))
/**
 * @brief Rmb - PCI function number and register offset of DF::SystemFabricIdMask0 register
 *
 * @details Rmb - D18F4x1B0 [System Fabric ID Mask 0] (DF::SystemFabricIdMask0)
 */
#define DF__SYSFABIDMASK0_REGADDR_DF4         ((4<<12)|(0x1B0))
/**
 * @brief Genesis - PCI function number and register offset of DF::SystemFabricIdMask1 register
 *
 * @details Genesis - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1)
 */
#define DF__SYSFABIDMASK1_REGADDR_DF3         ((1<<12)|(0x20C))
/**
 * @brief PCI function number and register offset of DF::SystemFabricIdMask1 register
 *
 * @details Reserved in Rmb
 */
#define DF__SYSFABIDMASK1_REGADDR_DF3POINT5   ((1<<12)|(0x154))
/**
 * @brief Rmb - PCI function number and register offset of DF::SystemFabricIdMask1 register
 *
 * @details Rmb - D18F1x20C [System Fabric ID Mask 1] (DF::SystemFabricIdMask1)
 */
#define DF__SYSFABIDMASK1_REGADDR_DF4         ((4<<12)|(0x1B4))
/**
 * @brief PCI function number and register offset of DF::SystemFabricIdMask2 register
 *
 * @details Reserved in Rmb
 */
#define DF__SYSFABIDMASK2_REGADDR_DF3POINT5   ((1<<12)|(0x158))
/**
 * @brief Rmb - PCI function number and register offset of DF::SystemFabricIdMask2 register
 *
 * @details Rmb - D18F4x1B8 [System Fabric ID Mask 2] (DF::SystemFabricIdMask2)
 */
#define DF__SYSFABIDMASK2_REGADDR_DF4         ((4<<12)|(0x1B8))
/**
 * @brief Rmb - PCI function number and register offset of DF::FabricBlockInstanceCount register
 *
 * @details Rmb - D18F0x040 [Fabric Block Instance Count] (DF::FabricBlockInstanceCount)
 */
#define DF__FABBLKINSTCNT_REGADDR             ((0<<12)|(0x040))
/**
 * @brief Rmb - PCI function number and register offset of DF::FabricBlockInstanceInformation0 register
 *
 * @details Rmb - D18F0x044 [Fabric Block Instance Information 0] (DF::FabricBlockInstanceInformation0)
 */
#define DF__FABBLKINFO0_REGADDR               ((0<<12)|(0x044))
/**
 * @brief Rmb - PCI function number and register offset of DF::FabricBlockInstanceInformation3_CSNCSPIEALLM register
 *
 * @details Rmb - D18F0x050 [Fabric Block Instance Information 3] (DF::FabricBlockInstanceInformation3_CSNCSPIEALLM)
 */
#define DF__FABBLKINFO3_REGADDR               ((0<<12)|(0x050))
/**
 * @brief Genesis - PCI function number and register offset of DF::DramBaseAddress register
 *
 * @details Genesis - D18F0x110...D18F0x188 [DRAM Base Address] (DF::DramBaseAddress)
 */
#define DF__DRAMBASE0_REGADDR_PRE_DF4         ((0<<12)|(0x110))
/**
 * @brief Genesis - PCI function number and register offset of DF::DramLimitAddress register
 *
 * @details Genesis - D18F0x114...D18F0x18C [DRAM Limit Address] (DF::DramLimitAddress)
 */
#define DF__DRAMLIMIT0_REGADDR_PRE_DF4        ((0<<12)|(0x114))
/**
 * @brief Genesis -  PCI function number and register offset of DF::DramOffset0
 *
 * @details Genesis - DramOffset0 doesnt technically exist (always zero), DramOffset1 starts at 1B4, so we pretend DramOffset0 is at 1B0
 */
#define DF__DRAMOFFSET0_REGADDR_PRE_DF4       ((0<<12)|(0x1B0))
/**
 * @brief Rmb - PCI function number and register offset of DF::DramBaseAddress register
 *
 * @details Rmb - D18F7xE00...D18F7xF30 [DRAM Base Address] (DF::DramBaseAddress)
 */
#define DF__DRAMBASE0_REGADDR_DF4             ((7<<12)|(0xE00))
/**
 * @brief Rmb - PCI function number and register offset of DF::DramLimitAddress register
 *
 * @details Rmb - D18F7xE04...D18F7xF34 [DRAM Limit Address] (DF::DramLimitAddress)
 */
#define DF__DRAMLIMIT0_REGADDR_DF4            ((7<<12)|(0xE04))
/**
 * @brief Rmb - PCI function number and register offset of DF::DramAddressCtl register
 *
 * @details Rmb - D18F7xE08...D18F7xF38 [DRAM Address Control] (DF::DramAddressCtl)
 */
#define DF__DRAMCTL0_REGADDR_DF4              ((7<<12)|(0xE08))
/**
 * @brief Rmb - PCI function number and register offset of DF::DramAddressIntlv register
 *
 * @details Rmb - D18F7xE0C...D18F7xF3C [DRAM Address Interleave] (DF::DramAddressIntlv)
 */
#define DF__DRAMINTLV0_REGADDR_DF4            ((7<<12)|(0xE0C))
/**
 * @brief Rmb -  PCI function number and register offset of DF::DramOffset0
 *
 * @details Rmb - DramOffset0 doesnt technically exist (always zero), DramOffset1 starts at 140, so we pretend DramOffset0 is at 13C
 */
#define DF__DRAMOFFSET0_REGADDR_DF4           ((7<<12)|(0x13C))
/**
 * @brief Genesis - PCI function number and register offset of DF::DramHoleControl register
 *
 * @details Genesis - D18F0x104 [DRAM Hole Control] (DF::DramHoleControl)
 */
#define DF__DRAMHOLECTL_REGADDR_PRE_DF4       ((0<<12)|(0x104))
/**
 * @brief Rmb -  PCI function number and register offset of DF::DramHoleControl register
 *
 * @details Rmb - D18F7x104 [DRAM Hole Control] (DF::DramHoleControl)
 */
#define DF__DRAMHOLECTL_REGADDR_DF4           ((7<<12)|(0x104))
/**
 * @brief Genesis - PCI function number and register offset of DF::Np2ChannelConfig register
 *
 * @details Genesis - D18F2x090 [Non-power-of-2 channel Configuration Register for CS DRAM Address Maps] (DF::Np2ChannelConfig)
 */
#define DF__NP2CHANNELCONFIG_REGADDR          ((2<<12)|(0x90))
/**
 * @brief Genesis - PCI function number and register offset of DF::Skt0CsTargetRemap0 register
 *
 * @details Genesis - D18F0x060 [Socket 0 CS Target Remap Register 0] (DF::Skt0CsTargetRemap0)
 */
#define DF__SKT0CSTARGETREMAP0_REGADDR        ((0<<12)|(0x60))
/**
 * @brief Genesis - PCI function number and register offset of DF::Skt1CsTargetRemap0 register
 *
 * @details Genesis - D18F0x068 [Socket 1 CS Target Remap Register 0] (DF::Skt1CsTargetRemap0)
 */
#define DF__SKT1CSTARGETREMAP0_REGADDR        ((0<<12)|(0x68))
/**
 * @brief Rmb - PCI function number and register offset of DF::DfGlobalCtrl register
 *
 * @details Rmb - D18F3x044 [DF Global Control] (DF::DfGlobalCtrl)
 */
#define DF__DFGLOBALCTRL_REGADDR              ((3<<12)|(0x044))
/**
 * @brief Rmb -  PCI function number and register offset of DF::CsTargetRemap0A register
 *
 * @details Rmb - D18F7x180 [CS Target Remap 0 Register A] (DF::CsTargetRemap0A)
 */
#define DF__CSTARGETREMAP0A_REGADDR_DF4       ((7<<12)|(0x180))
#define DF__DRAMMEGABASE_REGADDR_PRE_DF4      ((0<<12)|(0x190))
#define DF__DRAMMEGALIMIT_REGADDR_PRE_DF4     ((0<<12)|(0x194))
#define DF__DRAMMEGACONTROL_REGADDR_PRE_DF4   ((0<<12)|(0x198))
/**
 * @brief Genoa A0 - PCI function number and register offset of DF::RSPQWDTIoTransLogLow register
 *
 * @details Genoa A0 - D18F3x900 [RSPQ Watchdog Timer Log Register Low] (DF::RSPQWDTIoTransLogLow)
 */
#define RSPQWDTIOTRANSLOGLOW                  ((3<<12)|(0x900))
/**
 * @brief Genoa A0- PCI function number and register offset of DF::RSPQWDTIoTransLogHi register
 *
 * @details Genoa A0- D18F3x904 [RSPQ Watchdog Timer Status Hi] (DF::RSPQWDTIoTransLogHi)
 */
#define RSPQWDTIOTRANSLOGHIGH                 ((3<<12)|(0x904))
/**
 * @brief Genoa B0 - PCI function number and register offset of DF::OrigWdtAddrLogStat
 *
 * @details Genoa B0 - D18F3x910 [Originator Watchdog Timer Status Register] (DF::OrigWdtAddrLogStat)
 */
#define ORIGWDTADDRLOGSTAT                    ((3<<12)|(0x910))
/**
 * @brief Genoa B0- PCI function number and register offset of DF::OrigWdtAddrLogLo register
 *
 * @details Genoa B0- D18F3x914 [Originator Watchdog Timer Address Register] (DF::OrigWdtAddrLogLo)
 */
#define ORIGWDTADDRLOGLO                      ((3<<12)|(0x914))
/**
 * @brief Genoa B0- PCI function number and register offset of DF::OrigWdtAddrLogHi register
 *
 * @details Genoa B0- D18F3x918 [Originator Watchdog Timer Address Register] (DF::OrigWdtAddrLogHi)
 */
#define ORIGWDTADDRLOGHI                      ((3<<12)|(0x918))
/**
 * @brief Rmb - PCI function number and register offset of DF::HardwareAssertStatusLow register
 *
 * @details Rmb - D18F3x8F0 [Hardware Assert Status Low] (DF::HardwareAssertStatusLow)
 */
#define DF_HARDWAREASSERTSTATUSLOW_ADDR       ((3<<12)|(0x8F0))
/**
 * @brief Rmb - PCI function number and register offset of DF::HardwareAssertStatusHigh register
 *
 * @details Rmb - D18F3x8F4 [Hardware Assert Status High] (DF::HardwareAssertStatusHigh)
 */
#define DF_HARDWAREASSERTSTATUSHIGH_ADDR      ((3<<12)|(0x8F4))
/**
 * @brief Rmb - PCI function number and register offset of DF::CfgAddressCntl register
 *
 * @details Rmb - D18F0xC04 [Config Address Control] (DF::CfgAddressCntl)
 */
#define DF_CFGADDRESSCNTL                     ((DF_CFGADDRESSCNTL_FUNC<<12)|(DF_CFGADDRESSCNTL_OFFSET))
/**
 * @brief Genesis - PCI function number and register offset of DF::Np2ChannelConfig register
 *
 * @details Genesis - D18F2x090 [Non-power-of-2 channel Configuration Register for CS DRAM Address Maps] (DF::Np2ChannelConfig)
 */
#define DF_NP2CHANNELCONFIG_ADDR              ((2<<12)|(0x90))
/**
 * @brief Genesis - PCI function number and register offset of DF::Skt0CsTargetRemap0 register
 *
 * @details Genesis - D18F0x060 [Socket 0 CS Target Remap Register 0] (DF::Skt0CsTargetRemap0)
 */
#define DF_SKT0CSTARGETREMAP0_ADDR            ((0<<12)|(0x60))
/**
 * @brief Genesis - PCI function number and register offset of DF::Skt1CsTargetRemap0 register
 *
 * @details Genesis - D18F0x068 [Socket 1 CS Target Remap Register 0] (DF::Skt1CsTargetRemap0)
 */
#define DF_SKT1CSTARGETREMAP0_ADDR            ((0<<12)|(0x68))
/**
 * @brief Genesis - PCI function number and register offset of DF::DfGlobalCtrl register
 *
 * @details Genesis - D18F0x3F8 [DF Global Control] (DF::DfGlobalCtrl)
 */
#define DF_DFGLOBALCTRL_ADDR                  ((0<<12)|(0x3F8))
/**
 * @brief Genesis - The CCM WDT Error mask of DF::HardwareAssertStatusLow register
 *
 * @details Genesis - Data Fabric Hardware Asserts logged in MCA - Component CCM
 *          DF::HardwareAssertStatusLow[HWAssert5] Response Queue (RSPQ) watchdog timeout
 *          DF::HardwareAssertStatusLow[HWAssert4] Request Queue (REQQ) watchdog timeout
 */
#define DF_CCM_HARDWAREASSERTSTATUSLOW_MASK     (BIT4 | BIT5)
#define DF_CCM_HARDWAREASSERTSTATUSHIGH_MASK    (BIT25 | BIT26)

#define RMB_MAX_CCM_PER_DIE                (1)
#define RMB_CCM0_INSTANCE_ID               8

#define RMB_IOMS0_INSTANCE_ID              0xD

#define FABRIC_REG_ACC_BC       (0xFF)

/* Fabric Indirect Config Access Address 3 Register */
#define FICAA3_FUNC             (0x4)
#define FICAA3_REG              (0x05C)

/* Fabric Indirect Config Access Data 3 Low Register */
#define FICAD3_LO_FUNC          (0x4)
#define FICAD3_LO_REG           (0x98)

/* Fabric Indirect Config Access Data 3 High Register */
#define FICAD3_HI_FUNC          (0x4)
#define FICAD3_HI_REG           (0x9C)

/// Fabric Indirect Config Access Address 3 Register
typedef union {
  struct {                             ///< Bitfields of Fabric Indirect Config Access Address 3 Register
    UINT32 CfgRegInstAccEn:1;          ///< CfgRegInstAccEn
    UINT32 :1;                         ///< Reserved
    UINT32 IndCfgAccRegNum:9;          ///< IndCfgAccRegNum
    UINT32 IndCfgAccFuncNum:3;         ///< IndCfgAccFuncNum
    UINT32 SixtyFourBitRegEn:1;        ///< SixtyFourBitRegEn
    UINT32 :1;                         ///< Reserved
    UINT32 CfgRegInstID:8;             ///< CfgRegInstID
    UINT32 :8;                         ///< Reserved
  } Field;
  UINT32  Value;
} FABRIC_IND_CFG_ACCESS_ADDR_REGISTER;

/// EINJ PSP
#define IOHC_MSIC0_SMN_BASE                 (0x13B10000ul)
#define IOHC_MSICX_PSP_BASE_ADDR_LO_OFFSET  (0x2E0)
#define IOHC_MSICX_PSP_BASE_ADDR_HI_OFFSET  (0x2E4)

#define C2PMSG_0_BASE                       (0x10500ul) ///< BIOS to PSP mailbox 0  Register (MP0_C2PMSG_0).  It is at offset 0 relative to the PSP_BASE_ADDR.
#define RAS_PSP_MAILBOX_OFFSET              (31 * 4)    ///< BIOS to PSP mailbox 31 Register (MP0_C2PMSG_31). It is at offset 124 (31 * 4) relative to the MP0_C2PMSG_0.

///
/// X86 to PSP Buffer which start mapping from MP0_C2PMSG_31 (PSP Mailbox for RAS).
///
typedef volatile struct {
  UINT32                    StatOrDta:16;   ///< Status/Data
  UINT32                    CommandId:8;    ///< Command ID
  UINT32                    AltStat:4;      ///< Alternate_Status
  UINT32                    Reserved:3;     ///< Reserved. Must be zero.
  UINT32                    CmdOrRspns:1;   ///< Command = 0 (Written by OS); Response = 1 (Written by PSP upon completion).
} PSP_MBOX_RAS_CMD_EXT;

typedef volatile union {
  IN  UINT32                Value;          ///< Cmd register value
  IN  PSP_MBOX_RAS_CMD_EXT  Field;          ///< Extended Cmd register with field definition
} PSP_RAS_MBOX_REG;

#pragma pack (push, 1)

///
/// The ADDR_DATA structure is initialized by reading UMC related register and is used for the address translation function.
///
typedef struct {
  IN       UINT8        TOTAL_NUM_RANKS_PER_UMCCH_ADDR_TRANS[16*4];///< Total number of ranks per UMC channel
  IN       UINT16       RANK_ENABLE_PER_UMCCH_ADDR_TRANS[16*4];    ///< CSEnable of DRAM CS Base Address
  IN       UINT64       RANK_SIZE_PER_UMCCH_ADDR_TRANS[16*4][8];   ///< Not used
  IN       UINT32       ADDRHASHBANK0[16*4];                       ///< Address Hash Bank 0
  IN       UINT32       ADDRHASHBANK1[16*4];                       ///< Address Hash Bank 1
  IN       UINT32       ADDRHASHBANK2[16*4];                       ///< Address Hash Bank 2
  IN       UINT32       ADDRHASHBANK3[16*4];                       ///< Address Hash Bank 3
  IN       UINT32       ADDRHASHBANK4[16*4];                       ///< Address Hash Bank 4
  IN       UINT32       ADDRHASHNORMADDR[16*4][3];                 ///< Address Hash CS
  IN       UINT32       ADDRHASHRMADDR[16*4][4];                   ///< Address Hash RM
  IN       UINT32       ADDRHASHPC[16*4];                          ///< Address Hash PC
  IN       UINT32       ADDRHASHPC2[16*4];                         ///< Address Hash PC 2
  IN       UINT8        EXT_ADDRHASHNORMADDR[16*4][3];             ///< Address Hash CS Extended
  IN       UINT8        EXT_ADDRHASHRMADDR[16*4][4];               ///< Address Hash RM Extended
  IN       UINT32       CSBASE[16*4][8];                           ///< DRAM CS Base Address
  IN       UINT32       CSBASESEC[16*4][8];                        ///< DRAM CS Base Secondary Address
  IN       UINT32       CSMASK[16*4][8];                           ///< DRAM CS Mask Address
  IN       UINT32       CSMASKSEC[16*4][8];                        ///< DRAM CS Mask Secondary Address
  IN       UINT32       EXT_CSBASE[16*4][8];                       ///< DRAM CS Base Address Extended
  IN       UINT32       EXT_CSBASESEC[16*4][8];                    ///< DRAM CS Base Secondary Address Extended
  IN       UINT32       EXT_CSMASK[16*4][8];                       ///< DRAM CS Mask Address Extended
  IN       UINT32       EXT_CSMASKSEC[16*4][8];                    ///< DRAM CS Mask Secondary Address Extended
  IN       UINT32       CONFIGDIMM[16*4][4];                       ///< DRAM Address Configuration
  IN       UINT32       BANKSELDIMM[16*4][4];                      ///< Bank of DRAM Bank Address Select
  IN       UINT32       ROWSELDIMM[16*4][4];                       ///< Row of DRAM Bank Address Select
  IN       UINT32       COL0SELDIMM[16*4][4];                      ///< DRAM Column Address Select Low
  IN       UINT32       COL1SELDIMM[16*4][4];                      ///< DRAM Column Address Select High
  IN       UINT32       RMSELDIMM[16*4][4];                        ///< DRAM Rank Multiply Address Select
  IN       UINT32       CSSIZEDIMM[16*4][8];                       ///< Not used
  IN       UINT32       CHANSIZEDIMM[16*4];                        ///< Not used
  IN       UINT8        VALIDHI[16*4];                             ///< use for self checking
  IN       UINT8        CSPOSHI[16*4][4];                          ///< Not used
  IN       UINT8        CSPOSLO[16*4][4];                          ///< Not used
  IN       UINT32       CTRLREG[16*4][4];                          ///< Configuration of (UMC::DDramTiming1, UMC::DDynPprCtrl1 and UMC::DAeCtrl5)
  IN       BOOLEAN      addrhash[6];                               ///< The addrhash array use to get Bank
  IN       BOOLEAN      DimmPresent[MAX_SOCKET_SUPPORT][MAX_UMC_CHANNEL_PER_DIE * DIE_PER_SOCKET]; ///< Identifies if a DIMM is present at particular channel for a given socket
} ADDR_DATA;

#pragma pack (pop)

///
/// Platform RAS configuration data structure
///
typedef struct {
    BOOLEAN       PFEHEnable;
    UINT16        SwSmiCmdPortAddr;
    UINT8         MceSwSmiData;
    BOOLEAN       McaErrThreshEn;
    UINT16        McaErrThreshCount;
    BOOLEAN       NbioCorrectedErrThreshEn;
    UINT16        NbioCorrectedErrThreshCount;
    BOOLEAN       NbioDeferredErrThreshEn;
    UINT16        NbioDeferredErrThreshCount;
    UINTN         TotalNumberOfProcessors;
    CPU_INFO      *RasCpuMap;
    ADDR_DATA     *AddrData;
} AMD_RAS_POLICY;

#endif  // _AMD_RAS_REGISTERS_RMB_H_

