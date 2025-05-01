/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_RAS_COMMON_H_
#define _AMD_CPM_RAS_COMMON_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <IndustryStandard/Acpi.h>
#include <Base.h>
#include <AmdRas.h>
#include <AmdCpmSlinkRas.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define AMD_CREATOR_ID SIGNATURE_32('A', 'M', 'D', ' ')

#define AMD_HEST_TABLE_ID SIGNATURE_64('A', 'M', 'D', ' ', 'H', 'E', 'S', 'T')
#define AMD_BERT_TABLE_ID SIGNATURE_64('A', 'M', 'D', ' ', 'B', 'E', 'R', 'T')
#define AMD_EINJ_TABLE_ID SIGNATURE_64('A', 'M', 'D', ' ', 'E', 'I', 'N', 'J')

#define ACPI_REV1                   (0x01)
#define MAX_ERROR_BLOCK_SIZE        (0x1000)
#define MAX_MCA_ERROR_BLOCK_SIZE    (0x200)

///HEST
#define MCE_SOURCE_ID0                  (1)
#define CMC_SOURCE_ID0                  (2)
#define NBIO_SOURCE_ID                  (3)
#define PCIE_SOURCE_ID                  (4)
#define DMC_SOURCE_ID0                  (5)
#define FCH_ALINK_SOURCE_ID             (6)
#define USB_SOURCE_ID                   (7)
#define SATA_SOURCE_ID                  (8)
#define PCIE_ROOT_PORT_SRC_ID           (0x100)
#define PCIE_DEVICE_SOURCE_ID           (0x101)
#define PCIE_BRIDGE_SOURCE_ID           (0x102)
#define PCIE_RP_GHES_CORR_SRC_ID        (0x200)
#define PCIE_RP_GHES_UNCORR_SRC_ID      (0x201)
#define PCIE_DEV_GHES_CORR_SRC_ID       (0x202)
#define PCIE_DEV_GHES_UNCORR_SRC_ID     (0x203)
#define PCIE_BRIDGE_GHES_CORR_SRC_ID    (0x204)
#define PCIE_BRIDGE_GHES_UNCORR_SRC_ID  (0x205)
#define NBIO_UNCORR_SOURCE_ID           (0x300)
#define NBIO_CORR_SOURCE_ID             (0x301)
#define MEM_MCE_SOURCE_ID               (0x400)
#define MEM_CMC_SOURCE_ID               (0x401)
#define MEM_DMC_SOURCE_ID               (0x402)
#define SLINK_UNCORR_SOURCE_ID          (0x500)
#define SLINK_CORR_SOURCE_ID            (0x501)
#define PROC_MCE_SOURCE_ID              (0x1000)
#define PROC_CMC_SOURCE_ID              (0x5000)
#define PROC_DMC_SOURCE_ID              (0x9000)
#define RECORDS_PREALLOCATE             (0x01)
#define SECTIONS_PER_RECORD             (0x01)

typedef enum _GHES_TYPE_NUM {
  NBIO_GHES = 1,
  PCIE_GHES,
  SATA_GHES,
  USB_GHES
} GHES_TYPE_NUM;

#define HEST_STRUC_TYPE_MCE         (0x01)

///
/// Hardware Error Notification types. All other values are reserved
///
#define HARDWARE_ERROR_NOTIFICATION_POLLED                0x00
#define HARDWARE_ERROR_NOTIFICATION_EXTERNAL_INTERRUPT    0x01
#define HARDWARE_ERROR_NOTIFICATION_LOCAL_INTERRUPT       0x02
#define HARDWARE_ERROR_NOTIFICATION_SCI                   0x03
#define HARDWARE_ERROR_NOTIFICATION_NMI                   0x04
#define HARDWARE_ERROR_NOTIFICATION_CMCI                  0x05
#define HARDWARE_ERROR_NOTIFICATION_MCE                   0x06
#define HARDWARE_ERROR_NOTIFICATION_GPIO                  0x07

/// Memory Error Types UEFI 2.6 Table 275
typedef enum _MEM_ERROR_TYPE_NUM {
  UNKNOWN = 0,                    ///< 0
  NO_ERROR,                       ///< 1
  SINGLE_BIT_ECC,                 ///< 2
  MULTI_BIT_ECC,                  ///< 3
  SINGLE_SYM_CHIP_KILL_ECC,       ///< 4
  MULTI_SYM_CHIP_KILL_ECC,        ///< 5
  MASTER_ABORT,                   ///< 6
  TARGET_ABORT,                   ///< 7
  PARITY_ERROR,                   ///< 8
  WATCHDOG_TIMEOUT,               ///< 9
  INVALID_ADDRESS,                ///< 10
  MIRROR_BROKEN,                  ///< 11
  MEMORY_SPARING,                 ///< 12
  SCRUB_CORR_ERROR,               ///< 13
  SCRUB_UNCORR_ERROR,             ///< 14
  PHYSICAL_MEM_MAPOUT             ///< 15
} MEM_ERROR_TYPE_NUM;

/// UEFI 2.6 Section N Table 275
/// Bit[15]-Rank Num Valid, Bit[14]-Memory Error Type Valid, Bit[9]-Column Valid, Bit[8]-Row Valid, Bit[6]-Bank Valid, Bit[5]-Module Valid, Bit[4]-Card Valid, Bit[3]-Node Valid, Bit[2]-Error Address Valid, Bit[0]-ErrSts Valid
#define MEM_VALID_BIT_MAP ((1 << 15) | (1 << 9) | (1 << 8) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 1) | (1 << 0))
#define MEM_PARITY_VALID_BIT_MAP ((1 << 14) | (1 << 4) | (1 << 3) | (1 << 0))
#define MEM_TEST_VALID_BIT_MAP ((1 << 14) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 0))
#define NBIO_VALID_BIT_MAP ((1 << 7) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))
#define NBIF_VALID_BIT_MAP ((1 << 6) | (1 << 5) | (1 << 2) | (1 << 1) | (1 << 0))
#define SMN_VALID_BIT_MAP ((1 << 2) | (1 << 1) | (1 << 0))
#define DXIO_VALID_BIT_MAP ((1 << 5) | (1 << 2) | (1 << 1) | (1 << 0))

/// UEFI 2.6 Section N Table 249 Error Section Descriptor GUID Definitions
/// Processor Generic Error Section GUID
#define PROCESSOR_GENERIC_SECT_GUID \
  { 0x9876CCAD, 0x47B4, 0x4BDB, 0xB6, 0x5E, 0x16, 0xF1, 0x93, 0xC4, 0xF3, 0xDB }
  // 9876CCAD-47B4-4bdb-B65E16F193C4F3DB

//Processor Specific (IA32/X64) Error Section GUID
#define PROCESSOR_SPECIFIC_X86_SECT_GUID \
  { 0xDC3EA0B0, 0xA144, 0x4797, 0xB9, 0x5B, 0x53, 0xFA, 0x24, 0x2B, 0x6E, 0x1D }
  // DC3EA0B0-A144-4797-B95B53FA242B6E1D

#define IA32_X64_CACHE_CHECK_STRUC_GUID \
  { 0xA55701F5, 0xE3EF, 0x43de, 0xAC, 0x72, 0x24, 0x9B, 0x57, 0x3F, 0xAD, 0x2C }
  // A55701F5-E3EF-43DE-AC72249B573FAD2C

#define IA32_X64_TLB_CHECK_STRUC_GUID \
  { 0xFC06B535, 0x5E1F, 0x4562, 0x9F, 0x25, 0x0A, 0x3B, 0x9A, 0xDB, 0x63, 0xC3 }
  // FC06B535-5E1F-4562-9F250A3B9ADB63C3

#define IA32_X64_BUS_CHECK_STRUC_GUID \
  { 0x1CF3F8B3, 0xC5B1, 0x49a2, 0xAA, 0x59, 0x5E, 0xEF, 0x92, 0xFF, 0xA6, 0x3C }
  // 1CF3F8B3-C5B1-49a2-AA595EEF92FFA63C

#define IA32_X64_MS_CHECK_FIELD_DESC_GUID \
  { 0x48AB7F57, 0xDC34, 0x4F6C, 0xA7, 0xD3, 0xB0, 0xB5, 0xB0, 0xA7, 0x43, 0x14 }
  // 48AB7F57-DC34-4F6C-A7D3B0B5B0A74314

/// Platform Memory Error Section GUID
#define PLATFORM_MEMORY_SECT_GUID \
  { 0xA5BC1114, 0x6F64, 0x4EDE, 0xB8, 0x63, 0x3E, 0x83, 0xED, 0x7C, 0x83, 0xB1 }
  // A5BC1114-6F64-4EDE-B8633E83ED7C83B1

/// PCI Express Error Section GUID
#define PCIE_SECT_GUID \
  { 0xD995E954, 0xBBC1, 0x430F, 0xAD, 0x91, 0xB4, 0x4D, 0xCB, 0x3C, 0x6F, 0x35 }
  //D995E954-BBC1-430F-AD91B44DCB3C6F35

/// Firmware Error Record Section GUID
#define FIRMWARE_ERROR_RECORD_SECT_GUID \
  { 0x81212A96, 0x09ED, 0x4996, 0x94, 0x71, 0x8D, 0x72, 0x9C, 0x8E, 0x69, 0xED }
  //81212A96-09ED-4996-94718D729C869ED

/// PCI/PCIX Error Section GUID
#define PCI_PCIX_SECT_GUID \
  { 0xC5753963, 0x3B84, 0x4095, 0xBF, 0x78, 0xED, 0xDA, 0xD3, 0xF9, 0xC9, 0xDD }
  //C5753963-3B84-4095-BF78EDDAD3F9C9DD

/// PCI Component Device GUID
#define PCI_COMPONENT_DEVICE_SECT_GUID \
  { 0xEB5E4685, 0xCA66, 0x4769, 0xB6, 0xA2, 0x26, 0x06, 0x8B, 0x00, 0x13, 0x26 }
  //EB5E4685-CA66-4769-B6A226068B001326

/// DMAr Generic Section GUID
#define DMAR_GENERIC_SECT_GUID \
  { 0x5B51FEF7, 0xC79D, 0x4434, 0x8F, 0x1B, 0xAA, 0x62, 0xDE, 0x3E, 0x2C, 0x64 }
  //5B51FEF7-C79D-4434-8F1BAA62DE3E2C64

/// IOMMU Specific DMAr Section GUID
#define IOMMU_SPECIFIC_DMAR_SECT_GUID \
  { 0x036F84E1, 0x7F37, 0x428c, 0xA7, 0x9E, 0x57, 0x5F, 0xDF, 0xAA, 0x84, 0xEC }
  //036F84E1-7F37-428C-A79E575FDFAA84EC

/// MCA Deferred Error Section GUID
#define MCA_DEFERRED_ERROR_SECT_GUID \
  { 0xAFDA558D, 0x51C9, 0x4256, 0xBB, 0xD0, 0x22, 0x4E, 0x07, 0x84, 0x46, 0xF1 }
  //AFDA558D-51C9-4256-BBD0224E078446F1

/// S-Link Interface Error Section GUID
#define SLINK_ERROR_SECT_GUID \
  { 0x91335EF6, 0xEBFB, 0x4478, 0xA6, 0xA6, 0x88, 0xB7, 0x28, 0xCF, 0x75, 0xD7 }
  //91335EF6-EBFB-4478-A6A6-88B728CF75D7

/// EINJ Instruction definition
#define EINJ_SET_ERROR_TYPE_WITH_ADDRESS        (0x08)

/// Register bit width
#define REG_BIT_WIDTH_BYTE        (8)
#define REG_BIT_WIDTH_WORD        (16)
#define REG_BIT_WIDTH_DWORD       (32)
#define REG_BIT_WIDTH_QWORD       (64)

/// Register bit offset
#define REG_BIT_OFFSET0           (0)
#define REG_BIT_OFFSET1           (1)

/// FLAGS
#define EINJ_CLEAR_FLAG           (0)
#define EINJ_SET_FLAG             (1)

/// Reserved
#define EINJ_RESERVED             (0)

/// Mask
#define EINJ_BYTE_MASK            (0xFF)
#define EINJ_WORD_MASK            (0xFFFF)
#define EINJ_DWORD_MASK           (0xFFFFFFFFul)
#define EINJ_QWORD_MASK           (0xFFFFFFFFFFFFFFFFull)
#define EINJ_CHK_BUSY_STS_MASK    (0x01)
#define EINJ_GET_CMD_STS_MASK     (0x03)

/// Injection Instructions
#define EINJ_READ_REGISTER            (0x00)  // Returns a register value
#define EINJ_READ_REGISTER_VALUE      (0x01)  // Reads register value and compares results to value in VALUE field
#define EINJ_WRITE_REGISTER           (0x02)  // Write register (ignores value in register field)
#define EINJ_WRITE_REGISTER_VALUE     (0x03)  // Write register using data in value field
#define EINJ_NOOP                     (0x04)  // No operation

///Busy bits
#define APEI_RESET_BUSY_BIT           (0X00)
#define APEI_SET_BUSY_BIT             (0x01)

#define SMIPORT                       (0xB2)
#define EINJ_BEGIN_INJ_CMD            (0x81)

#define EINJ_INSTR_ENTRIES            (9)
#define EINJ_TRIG_INSTR_ENTRIES       (1)

#define EINJ_ERROR_VECTOR_CORR        (0x0001)
#define EINJ_ERROR_VECTOR_UNCORR      (0x0002)

#define EINJ_DFLT_INJECT_ADDRESS      (0x100000000)
//#define EINJ_DFLT_INJECT_ADDRESS      (0xC41FE71C0)
//#define EINJ_DFLT_INJECT_ADDRESS      (0x2041E9F4C0)

#pragma pack (push, 1)
/// Initial data of AMD_MC_BANK
typedef struct _AMD_MC_BANK_INIT_DATA {
  UINT64 CtrlInitData;            ///< Initial data of CtrlInitDataLSD
  UINT32 CtrlRegMSRAddr;          ///< Initial data of CtrlRegMSRAddr
  UINT32 StatRegMSRAddr;          ///< Initial data of StatRegMSRAddr
  UINT32 AddrRegMSRAddr;          ///< Initial data of AddrRegMSRAddr
  UINT32 MiscRegMSRAddr;          ///< Initial data of MiscRegMSRAddr
} AMD_MC_BANK_INIT_DATA;

/// Initial data of Machine Check
typedef struct _AMD_MC_INIT_DATA {
  UINT64 GlobCapInitData;         ///< Check global capability register
  UINT64 GlobCtrlInitData;        ///< Check global control register
  UINT8  ClrStatusOnInit;         ///< Indicates if the status information in this machine check
                                  ///< bank is to be cleared during system initialization
  UINT8  StatusDataFormat;        ///< Indicates the format of the data in the status register
  UINT8  ConfWriteEn;             ///< This field indicates whether configuration parameters may be
                                  ///< modified by the OS. If the bit for the associated parameter is
                                  ///< set, the parameter is writable by the OS.
  UINT8  McBankNum;               ///< Number of MCA Bank
  AMD_MC_BANK_INIT_DATA *McBankInitData;  ///< Pointer to Initial data of Mc Bank
} AMD_MC_INIT_DATA;

/// MSR179 Global Machine Check Capabilities data struct
typedef struct _MSR_MCG_CAP_STRUCT {
  UINT64 Count:8;                 ///< Indicates the number of
                                  ///< error-reporting banks visible to each core
  UINT64 McgCtlP:1;               ///< 1=The machine check control registers
  UINT64 Rsvd:55;                 ///< reserved
} MSR_MCG_CAP_STRUCT;

/// DPC Capability Register
typedef union _DPC_CAP_REG {
  struct {                                 ///< DPC_CAP_REG
    UINT16  DpcInterruptMessageNumber    :  5;  ///< bit 4:0   DPC Interrupt Message Number
    UINT16  RpExtensionsForDpc           :  1;  ///< bit 5     RP Extensions for DPC
    UINT16  PoisonedTLPEgressBlk         :  1;  ///< bit 6     Poison TLP Egress Blocking Supported
    UINT16  DpcSoftwareTrig              :  1;  ///< bit 7     DPC Software Triggering Supported
    UINT16  RpPioLogSize                 :  4;  ///< bit 11:8  RP PIO Log Size
    UINT16  DlActiveErrCorSignaling      :  1;  ///< bit 12    DL_Active ERR_COR Signaling Supported
    UINT16  Reserved                     :  3;  ///< bit 15:13 Reserved.
  } Field;
  UINT16    Value;                         ///< Raw data
} DPC_CAP_REG;

/// DPC Control Register
///   BXXD01F[7:1]x386;
///   BXXD02F1x386
///   BXXD03F[7:1]x386
///   BXXD04F1x386
typedef union _DPC_CNTL_REG {
  struct {                                 ///< PCIE_DPC_CNTL_REG
    UINT16  DpcTriggerEnable        :  2;  ///< bit 1:0  DPC Trigger Enable
    UINT16  DpcCompletionControl    :  1;  ///< bit 2    DPC Completion Control
    UINT16  DpcInterruptEnable      :  1;  ///< bit 3    DPC Interrupt Enable
    UINT16  DpcErrCorEnable         :  1;  ///< bit 4    DPC ERR_COR Enable
    UINT16  PoisonedTLPEgressBlkEn  :  1;  ///< bit 5    Poisoned TLP Egress Blocking Enable
    UINT16  DpcSoftwareTrigger      :  1;  ///< bit 6    DPC Software Trigger
    UINT16  DlActiveErrCorEnable    :  1;  ///< bit 7    DL_Active ERR_COR Enable
    UINT16  Reserved                :  8;  ///< bit 15:8 Reserved.
  } Field;
  UINT16    Value;                         ///< Raw data
} DPC_CNTL_REG;

/// DPC Status Register
typedef union _DPC_STS_REG {
  struct {                                 ///< DPC_STS_REG
    UINT16  DpcTriggerStatus             :  1;  ///< bit 0     DPC Trigger Status
    UINT16  DpcTriggerReason             :  2;  ///< bit 2:1   DPC Trigger Reason
    UINT16  DpcInterruptStatus           :  1;  ///< bit 3     DPC Interrupt Status
    UINT16  DpcRpBusy                    :  1;  ///< bit 4     DPC RP Busy
    UINT16  DpcTriggerReasonExt          :  2;  ///< bit 6:5   DPC Trigger Reason Extension
    UINT16  Reserved1                    :  1;  ///< bit 7     Reserved.
    UINT16  RpPioFirstErrorPointer       :  5;  ///< bit 12:8  RP PIO First Error Pointer
    UINT16  Reserved2                    :  3;  ///< bit 15:13 Reserved.
  } Field;
  UINT16    Value;                         ///< Raw data
} DPC_STS_REG;

#define ROOT_ERROR_STATUS_MASK          (0x7F)

/// Root Error Status Register
typedef union _ROOT_ERR_STS_REG {
  struct {                                    ///< ROOT_ERR_STS_REG
    UINT32  ErrCorReceived                  :  1;  ///< bit 0 ERR_COR Received
    UINT32  MultiErrCorReceived             :  1;  ///< bit 1 Multiple ERR_COR Received
    UINT32  ErrFatalNonFatalReceived        :  1;  ///< bit 2 ERR_FATAL/NONFATAL Received
    UINT32  MultiErrFatalNonFatalReceived   :  1;  ///< bit 3 Multiple ERR_FATAL/NONFATAL Received
    UINT32  FirstUncorrectableFatal         :  1;  ///< bit 4 First Uncorrectable Fatal
    UINT32  NonFatalErrMesgReceived         :  1;  ///< bit 5 Non-Fatal Error Messages Received
    UINT32  FatalErrMesgReceived            :  1;  ///< bit 6 Fatal Error Messages Received
    UINT32                                  :  20; ///< bit 7:26 Reserved.
    UINT32  AdvancedErrIntMesgNum           :  5;  ///< bit 27:31 Advanced Error Interrupt Message Number
  } Field;
  UINT32    Value;                         ///< Raw data
} ROOT_ERR_STS_REG;

/// ACPI 6.2 Table 6-194 Platform-Wide _OSC Capabilities DWORD 2
/// ACPI 6.3 Table 6-200 Platform-Wide _OSC Capabilities DWORD 2
typedef union {
  struct {                                      ///< PLATFORM_WIDE_OSC_CAP
    UINT32       ProcessorAggregatorDev:1;      ///< Processor Aggregator Device Support
    UINT32       PpcOstProcessing:1;            ///< _PPC _OST Processing Support
    UINT32       PR3:1;                         ///< _PR3 Support
    UINT32       InsEjectOstProcessing:1;       ///< Insertion / Ejection _OST Processing Support
    UINT32       Apei:1;                        ///< APEI Support
    UINT32       Cppc:1;                        ///< CPPC Support
    UINT32       Cppc2:1;                       ///< CPPC 2 Support
    UINT32       PlatCoordinatedLowPowerIdle:1; ///< Platform Coordinated Low Power Idle Support
    UINT32       OSInitiatedLowPowerIdle:1;     ///< OS Initiated Low Power Idle Support
    UINT32       FastThermalSampling:1;         ///< Fast Thermal Sampling support
    UINT32       GreaterThan16Pstate:1;         ///< Greater Than 16 p-state support
    UINT32       GenEventDevice:1;              ///< Generic Event Device support
    UINT32       DivCppcHighestOptimization:1;  ///< Diverse CPPC Highest Optimization Support
    UINT32       InterruptResourceSource:1;     ///< Interrupt ResourceSource support
    UINT32       FlexAddrSpaceforCppcReg:1;     ///< Flexible Address Space for CPPC Registers
    UINT32       GhesAssist:1;                  ///< GHES_ASSIST Support
    UINT32       MultiPccChannelforCppc:1;      ///< Multi PCC channel support for CPPC
    UINT32       GenericInitiatorSupport:1;     ///< Generic Initiator Support
    UINT32       :14;                           ///< [31:18] Reserved
  } Field;
  UINT32  Value;
} PLATFORM_WIDE_OSC_CAP;

/// _OSC Interface for PCI Host Bridge Devices
/// ACPI 6.2 Table 6-195 Interpretation _OSC Support Field, DWORD 2
//
/// _OSC Interface for PCI Host Bridge Devices (ToDo: 6.2.11.3 {deprecated}?)
/// ACPI 6.3 Table Interpretation of _OSC Support Field, DWORD 2
typedef union {
  struct {                                      ///< PCIE_OSC_SUPP
    UINT32       ExtendPciConfigSupp:1;         ///< Extended PCI Config operation regions supported
    UINT32       ASPMSupp:1;                    ///< Active State Power Management supported
    UINT32       CPMCSupp:1;                    ///< Clock Power Management Capability supported
    UINT32       PciSegmentGroupSupp:1;         ///< PCI Segment Groups supported
    UINT32       MSISupp:1;                     ///< MSI supported
    UINT32       :27;                           ///< [31:5] Reserved
  } Field;
  UINT32  Value;
} PCIE_OSC_SUPP;

/// ACPI 6.2 Table 6-195 Interpretation _OSC Control Field, DWORD 3
/// ACPI 6.3 Table: Interpretation of _OSC Control Field, Passed in via Arg3, DWORD 3
typedef union {
  struct {                                      ///< PCIE_OSC_CTRL
    UINT32       PcieNativeHotPlugCtrl:1;       ///< PCI Express Native Hot Plug control
    UINT32       SHPCNativeHotPlugCtrl:1;       ///< SHPC Native Hot Plug control
    UINT32       PcieNativePwrMgmtCtrl:1;       ///< PCI Express Native Power Management Events control
    UINT32       PcieAerCtrl:1;                 ///< PCI Express Advanced Error Reporting (AER) control
    UINT32       PcieCapStructCtrl:1;           ///< PCI Express Capability Structure control
    UINT32       Reserved:2;                    ///< [6:5] Reserved
    UINT32       PcieDpcCtrl:1;                 ///< PCI Express Downstream Port Containment control
    UINT32       :24;                           ///< [31:8] Reserved
  } Field;
  UINT32  Value;
} PCIE_OSC_CTRL;

/// UEFI 2.6 Appendix Table 250. Processor Generic Error Section (Ref: MdePkg\Include\Guid\Cper.h)
///
/// The validation bit mask indicates whether or not each of the following fields are
/// valid in processor Generic Error section.
///@{
#define GENERIC_ERROR_PROC_TYPE_VALID            (BIT0)
#define GENERIC_ERROR_PROC_ISA_VALID             (BIT1)
#define GENERIC_ERROR_PROC_ERROR_TYPE_VALID      (BIT2)
#define GENERIC_ERROR_PROC_OPERATION_VALID       (BIT3)
#define GENERIC_ERROR_PROC_FLAGS_VALID           (BIT4)
#define GENERIC_ERROR_PROC_LEVEL_VALID           (BIT5)
#define GENERIC_ERROR_PROC_VERSION_VALID         (BIT6)
#define GENERIC_ERROR_PROC_BRAND_VALID           (BIT7)
#define GENERIC_ERROR_PROC_ID_VALID              (BIT8)
#define GENERIC_ERROR_PROC_TARGET_ADDR_VALID     (BIT9)
#define GENERIC_ERROR_PROC_REQUESTER_ID_VALID    (BIT10)
#define GENERIC_ERROR_PROC_RESPONDER_ID_VALID    (BIT11)
#define GENERIC_ERROR_PROC_INST_IP_VALID         (BIT12)
///@}

///
/// The type of the processor architecture in processor Generic Error section.
///@{
#define GENERIC_ERROR_PROC_TYPE_IA32_X64         (0x00)
#define GENERIC_ERROR_PROC_TYPE_IA64             (0x01)
#define GENERIC_ERROR_PROC_TYPE_ARM              (0x02)
///@}

///
/// The type of the instruction set executing when the error occurred in processor
/// Generic Error section.
///@{
#define GENERIC_ERROR_PROC_ISA_IA32              (0x00)
#define GENERIC_ERROR_PROC_ISA_IA64              (0x01)
#define GENERIC_ERROR_PROC_ISA_X64               (0x02)
#define GENERIC_ERROR_PROC_ISA_ARM_A32_T32       (0x03)
#define GENERIC_ERROR_PROC_ISA_ARM_A64           (0x04)
///@}

///
/// The type of error that occurred in processor Generic Error section.
///@{
#define GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN    (0x00)
#define GENERIC_ERROR_PROC_ERROR_TYPE_CACHE      (0x01)
#define GENERIC_ERROR_PROC_ERROR_TYPE_TLB        (0x02)
#define GENERIC_ERROR_PROC_ERROR_TYPE_BUS        (0x04)
#define GENERIC_ERROR_PROC_ERROR_TYPE_MICRO_ARCH (0x08)
///@}

///
/// The type of operation in processor Generic Error section.
///@{
#define GENERIC_ERROR_PROC_OPERATION_GENERIC               (0x00)
#define GENERIC_ERROR_PROC_OPERATION_DATA_READ             (0x01)
#define GENERIC_ERROR_PROC_OPERATION_DATA_WRITE            (0x02)
#define GENERIC_ERROR_PROC_OPERATION_INSTRUCTION_EXEC      (0x03)
///@}

///
/// Flags bit mask indicates additional information about the error in processor Generic
/// Error section
///@{
#define GENERIC_ERROR_PROC_FLAGS_RESTARTABLE     (BIT0)
#define GENERIC_ERROR_PROC_FLAGS_PRECISE_IP      (BIT1)
#define GENERIC_ERROR_PROC_FLAGS_OVERFLOW        (BIT2)
#define GENERIC_ERROR_PROC_FLAGS_CORRECTED       (BIT3)
///@}

typedef struct _PROC_GENERIC_ERR_SEC {
  UINT64             ValidFields;               ///< The validation bit mask indicates whether or not each of the following fields is valid in this section.
  UINT8              Type;                      ///< The type of the processor architecture
  UINT8              Isa;                       ///< The type of the instruction set executing when the error occurred
  UINT8              ErrorType;                 ///< The type of error that occurred
  UINT8              Operation;                 ///< The type of operation
  UINT8              Flags;                     ///< Additional information about the error
  UINT8              Level;                     ///< Level of the structure where the error occurred
  UINT16             Resv1;                     ///< Must be zero
  UINT64             VersionInfo;               ///< The CPU Version Information and returns Family, Model, and stepping information
  CHAR8              BrandString[128];          ///< The null-terminated ASCII Processor Brand String
  UINT64             ApicId;                    ///< Uniquely identifies the logical processor
  UINT64             TargetAddr;                ///< The target address associated with the error
  UINT64             RequestorId;               ///< The requestor associated with the error
  UINT64             ResponderId;               ///< The responder associated with the error
  UINT64             InstructionIP;             ///< The instruction pointer when the error occurred.
} PROC_GENERIC_ERR_SEC;

/// UEFI 2.6 Appendix N Table 253. Cache Check Structure

#define CACHE_CHECK_TRANSTYPE              (BIT0)
#define CACHE_CHECK_OPERATION              (BIT1)
#define CACHE_CHECK_LEVEL                  (BIT2)
#define CACHE_CHECK_PROC_CONEXT_CORRUPT    (BIT3)
#define CACHE_CHECK_UNCORRECTED            (BIT4)
#define CACHE_CHECK_PRECISE_IP             (BIT5)
#define CACHE_CHECK_RESTARTABLE            (BIT6)
#define CACHE_CHECK_OVERFLOW               (BIT7)

typedef union {
  struct {                                     ///< CACHE_CHECK_STRUC
    UINT64       ValidationBits:16;            ///< [15:0] Validation Bits
    UINT64       TranscationType:2;            ///< [17:16] Transcation Type
    UINT64       Operation:4;                  ///< [21:18] Operation
    UINT64       Level:3;                      ///< [24:22] Cache Level
    UINT64       ProcessorContextCorrupt:1;    ///< [25] Processor Context Corrupt
    UINT64       Uncorrected:1;                ///< [26] Uncorrected
    UINT64       PreciseIp:1;                  ///< [27] Precise IP
    UINT64       RestartableIp:1;              ///< [28] Restartable IP
    UINT64       Overflow:1;                   ///< [29] Overflow
    UINT64       :34;                          ///< [63:30] Reserved
  } Field;
  UINT64  Value;
} CACHE_CHECK_STRUC;

/// UEFI 2.6 Appendix N Table 254. TLB Check Structure

#define TLB_CHECK_TRANSTYPE              (BIT0)
#define TLB_CHECK_OPERATION              (BIT1)
#define TLB_CHECK_LEVEL                  (BIT2)
#define TLB_CHECK_PROC_CONEXT_CORRUPT    (BIT3)
#define TLB_CHECK_UNCORRECTED            (BIT4)
#define TLB_CHECK_PRECISE_IP             (BIT5)
#define TLB_CHECK_RESTARTABLE            (BIT6)
#define TLB_CHECK_OVERFLOW               (BIT7)

typedef union {
  struct {                                     ///< TLB_CHECK_STRUC
    UINT64       ValidationBits:16;            ///< [15:0] Validation Bits
    UINT64       TranscationType:2;            ///< [17:16] Transcation Type
    UINT64       Operation:4;                  ///< [21:18] Operation
    UINT64       Level:3;                      ///< [24:22] Cache Level
    UINT64       ProcessorContextCorrupt:1;    ///< [25] Processor Context Corrupt
    UINT64       Uncorrected:1;                ///< [26] Uncorrected
    UINT64       PreciseIp:1;                  ///< [27] Precise IP
    UINT64       RestartableIp:1;              ///< [28] Restartable IP
    UINT64       Overflow:1;                   ///< [29] Overflow
    UINT64       :34;                          ///< [63:30] Reserved
  } Field;
  UINT64  Value;
} TLB_CHECK_STRUC;

/// UEFI 2.6 Appendix N Table 255. Bus Check Structure

#define BUS_CHECK_TRANSTYPE              (BIT0)
#define BUS_CHECK_OPERATION              (BIT1)
#define BUS_CHECK_LEVEL                  (BIT2)
#define BUS_CHECK_PROC_CONEXT_CORRUPT    (BIT3)
#define BUS_CHECK_UNCORRECTED            (BIT4)
#define BUS_CHECK_PRECISE_IP             (BIT5)
#define BUS_CHECK_RESTARTABLE            (BIT6)
#define BUS_CHECK_OVERFLOW               (BIT7)
#define BUS_CHECK_PARTICIPTION           (BIT8)
#define BUS_CHECK_TIME_OUT               (BIT9)
#define BUS_CHECK_ADDRESS_SPACE          (BIT10)

typedef union {
  struct {                                     ///< BUS_CHECK_STRUC
    UINT64       ValidationBits:16;            ///< [15:0] Validation Bits
    UINT64       TranscationType:2;            ///< [17:16] Transcation Type
    UINT64       Operation:4;                  ///< [21:18] Operation
    UINT64       Level:3;                      ///< [24:22] Cache Level
    UINT64       ProcessorContextCorrupt:1;    ///< [25] Processor Context Corrupt
    UINT64       Uncorrected:1;                ///< [26] Uncorrected
    UINT64       PreciseIp:1;                  ///< [27] Precise IP
    UINT64       RestartableIp:1;              ///< [28] Restartable IP
    UINT64       Overflow:1;                   ///< [29] Overflow
    UINT64       ParticitionType:2;            ///< [31:30] Particition Type
    UINT64       TimeOut:1;                    ///< [32] Time Out
    UINT64       AddressSpace:2;               ///< [43:33] Address Space
    UINT64       :29;                          ///< [63:35] Reserved
  } Field;
  UINT64  Value;
} BUS_CHECK_STRUC;

/// UEFI 2.6 Appendix N Table 256. MS Check Field Description
#define MS_CHECK_ERRORTYPE              (BIT0)
#define MS_CHECK_PROC_CONEXT_CORRUPT    (BIT1)
#define MS_CHECK_UNCORRECTED            (BIT2)
#define MS_CHECK_PRECISE_IP             (BIT3)
#define MS_CHECK_RESTARTABLE            (BIT4)
#define MS_CHECK_OVERFLOW               (BIT5)

#define MSCHK_ERRTYPE_NO_ERROR                    (0)
#define MSCHK_ERRTYPE_UNCLASSIFIED                (1)
#define MSCHK_ERRTYPE_MICROCODE_ROM_PARITY_ERR    (2)
#define MSCHK_ERRTYPE_EXTERNAL_ERROR              (3)
#define MSCHK_ERRTYPE_FRC_ERROR                   (4)
#define MSCHK_ERRTYPE_INTERNAL_UNCLASSIFIED       (5)

typedef union {
  struct {                                     ///< MS_CHECK_FIELD
    UINT64       ValidationBits:16;            ///< [15:0] Validation Bits
    UINT64       ErrorType:3;                  ///< [18:16] Error Type
    UINT64       ProcessorContextCorrupt:1;    ///< [19] Processor Context Corrupt
    UINT64       Uncorrected:1;                ///< [20] Uncorrected
    UINT64       PreciseIp:1;                  ///< [21] PreciseIp
    UINT64       RestartableIp:1;              ///< [22] RestartableIp
    UINT64       Overflow:1;                   ///< [23] Overflow
    UINT64       :40;                          ///< [63:24] Reserved
  } Field;
  UINT64  Value;
} MS_CHECK_FIELD;

/// UEFI 2.6 Appendix N Table 252. IA32/X64 Processor Error Information Structure
typedef union {
  struct {                                     ///< PROC_ERR_INFO_VALID_BIT
    UINT64       CheckInfoValid:1;             ///< [0] Check Info Valid
    UINT64       TargetAddrIdentifierValid:1;  ///< [1] Target Address Identifier Valid
    UINT64       RequestorIdentifierValid:1;   ///< [2] Requestor Identifier Valid
    UINT64       ResponderIdentifierValid:1;   ///< [3] Responder Identifier Valid
    UINT64       InstructionPointerValid:1;    ///< [4] Instruction Pointer Valid
    UINT64       :59;                          ///< [63:5] Reserved
  } Field;
  UINT64  Value;
} PROC_ERR_INFO_VALID_BIT;

typedef struct _PROC_ERR_INFO_STRUC {
  UINT8   ErrorStructureType[16];             ///< Error Structure Type
  PROC_ERR_INFO_VALID_BIT   ValidBits;        ///< Validation Bits
  UINT64  CheckInformation;                   ///< Check Information
  UINT64  TargetIdentifier;                   ///< Target Identifier
  UINT64  RequestorIdentifier;                ///< Requestor Identifier
  UINT64  ResponderIdentifier;                ///< Responder Identifier
  UINT64  InstructionPointer;                 ///< Instruction Pointer
} PROC_ERR_INFO_STRUC;

/// UEFI 2.6 Appendix N Table 259.
typedef struct _X64_REGISTER_STATE {
  UINT64 RAX;
  UINT64 RBX;
  UINT64 RCX;
  UINT64 RDX;
  UINT64 RSI;
  UINT64 RDI;
  UINT64 RBP;
  UINT64 RSP;
  UINT64 R8;
  UINT64 R9;
  UINT64 R10;
  UINT64 R11;
  UINT64 R12;
  UINT64 R13;
  UINT64 R14;
  UINT64 R15;
  UINT16 CS;
  UINT16 DS;
  UINT16 SS;
  UINT16 ES;
  UINT16 FS;
  UINT16 GS;
  UINT32 Reserved;
  UINT64 RFLAGS;
  UINT64 EIP;
  UINT64 CR0;
  UINT64 CR1;
  UINT64 CR2;
  UINT64 CR3;
  UINT64 CR4;
  UINT64 CR8;
  UINT64 GDTR_LO;
  UINT64 GDTR_HI;
  UINT64 IDTR_LO;
  UINT64 IDTR_HI;
  UINT16 LDTR;
  UINT16 TR;
} X64_REGISTER_STATE;

typedef union {
  struct {
    UINT64  Valid:1;
    UINT64  ThreadId:2;
    UINT64  Reserved:61;
  } Field;
  UINT64  Value;
} AMD_X64_REGISTER_GPR0_STATE;

typedef struct _AMD_X64_REGISTER_STATE {
  X64_REGISTER_STATE           *Uefix64RegState;
  AMD_X64_REGISTER_GPR0_STATE  Amdx64RegGpr0State;
} AMD_X64_REGISTER_STATE;

/// UEFI 2.6 Appendix N Table 257. IA32/X64 Processor Context Information
#define UNCLASSIFIED_DATA                             (0)
#define MSR_REGISTERS                                 (1)
#define MODE_EXECUTION_CONTEXT_32BIT                  (2)
#define MODE_EXECUTION_CONTEXT_64BIT                  (3)
#define FXSAVE_CONTEXT                                (4)
#define DEBUG_REGISTERS_32BIT_MODE                    (5)
#define DEBUG_REGISTERS_64BIT_MODE                    (6)
#define MEMORY_MAPPED_REGISTERS                       (7)

typedef struct _PROC_CONEXT_INFO_STRUC {
  UINT16  RegisterContextType;                ///< Register Context Type
  UINT16  RegisterArraySize;                  ///< Register Array Size
  UINT32  MSRAddress;                         ///< MSR Address
  UINT64  MMRegisterAddress;                  ///< Memory Map Register Address
  UINT64  RegisterArray[];                    ///< Register Array
} PROC_CONEXT_INFO_STRUC;

//Each processor context information structure is padded with zeros if the size is not a multiple of 16 bytes. (16=2^4)
#define PROC_CONEXT_INFO_STRUC_ALIGNMENT              (4)  // 2^(4) = 16

/// UEFI 2.6 Appendix N Table 251. IA32/X64 Processor Error Section

/// IA32/X64 Processor Error Section Structure
typedef union {
  struct {                                      ///< PROC_ERR_INFO_VALID_BIT
    UINT64       LocalApicIDValid:1;            ///< [0] Check Info Valid
    UINT64       CpuIdInfoValid:1;              ///< [1] Target Address Identifier Valid
    UINT64       ProcErrInfoStrucNum:6;         ///< [7:2] Number of Processor Error Information Structure
    UINT64       ProcContextInfoStrucNum:6;     ///< [13:8] Number of Processor Context Information Structure
    UINT64       :50;                           ///< [63:14] Reserved
  } Field;
  UINT64  Value;
} PROC_ERR_VALID_BIT;

typedef struct _PLATFORM_PROC_ERR_SEC {
  PROC_ERR_VALID_BIT   ValidBits;               ///< Validation Bits
  UINT64  LocalApicID;                          ///< Processor APIC ID
  UINT64  CpuIdInfo_EAX;                        ///< CPUID Information output value from EAX
  UINT64  CpuIdInfo_EBX;                        ///< CPUID Information output value from EBX
  UINT64  CpuIdInfo_ECX;                        ///< CPUID Information output value from ECX
  UINT64  CpuIdInfo_EDX;                        ///< CPUID Information output value from EDX
  UINT64  CpuIdInfo_PD1;                        ///< CPUID Information Padding 1
  UINT64  CpuIdInfo_PD2;                        ///< CPUID Information Padding 2
} PLATFORM_PROC_ERR_SEC;

typedef union {
  struct {                                     ///< MCA_ERROR_CODE_TYPES
    UINT16       CacheLevel:2;                 ///< [1:0] Cache Level
    UINT16       TransactionType:2;            ///< [3:2] Transaction Type
    UINT16       TlbErr:12;                    ///< [15:4] TLB error check bit
  } Field;
  UINT16  Value;
} MCA_TLB_ERROR;

typedef union {
  struct {                                     ///< MCA_ERROR_CODE_TYPES
    UINT16       CacheLevel:2;                 ///< [1:0] Cache Level
    UINT16       TransactionType:2;            ///< [3:2] Transaction Type
    UINT16       MemTransactionType:4;         ///< [7:4] Memory Transaction Type
    UINT16       MemErr:8;                     ///< [8] Memmory type error check bit
  } Field;
  UINT16  Value;
} MCA_MEM_ERROR;

typedef union {
  struct {                                     ///< MCA_ERROR_CODE_TYPES
    UINT16       CacheLevel:2;                 ///< [1:0] Cache Level
    UINT16       TransactionType:2;            ///< [3:2] Transaction Type
    UINT16       MemTransactionType:4;         ///< [7:4] Memory Transaction Type
    UINT16       Timeout:1;                    ///< [8] Timeout
    UINT16       ParticipationProcessor:2;     ///< [10:9] Participation Processor
    UINT16       BusErr:5;                     ///< [15:11] Bus Error Type check bit
  } Field;
  UINT16  Value;
} MCA_PP_ERROR;

typedef union {
  struct {                                     ///< MCA_ERROR_CODE_TYPES
    UINT16       Zero1:8;                      ///< [7:0] zero bits
    UINT16       InternalErrorType:2;          ///< [9:8] Internal Error Type
    UINT16       InternalErr:6;                ///< [15:10] Internal Error check bit
  } Field;
  UINT16  Value;
} MCA_INT_ERROR;

#define TLB_ERROR_MASK          (0xFFF0)
#define TLB_ERROR_CHK_SHIFT     (4)
#define MEM_ERROR_MASK          (0xFF00)
#define MEM_ERROR_CHK_SHIFT     (8)
#define PP_ERROR_MASK           (0xF800)
#define PP_ERROR_CHK_SHIFT      (11)
#define INT_ERROR_MASK          (0xFC00)
#define INT_ERROR_CHK_SHIFT     (10)

/// UEFI 2.6 Appendix N Table 275 Spec platform memory error

/// Memory Error Section Valid bit fields
typedef union {
  struct {                                     ///< MEM_ERR_VALID_BIT
    UINT64       ErrStatusValid:1;             ///< [0] Error Status Valid
    UINT64       PhysicalAddrValid:1;          ///< [1] Physical Address Valid
    UINT64       PhysicalAddrMaskValid:1;      ///< [2] Physical Address Mask Valid
    UINT64       NodeValid:1;                  ///< [3] Node Valid
    UINT64       CardValid:1;                  ///< [4] Card Valid
    UINT64       ModuleValid:1;                ///< [5] Module Valid
    UINT64       BankValid:1;                  ///< [6] Bank Valid
    UINT64       DeviceValid:1;                ///< [7] Device Valid
    UINT64       RowValid:1;                   ///< [8] Row Valid
    UINT64       ColumnValid:1;                ///< [9] Column Valid
    UINT64       BitPositionValid:1;           ///< [10] Bit Position Valid
    UINT64       PltfrmReqIdValid:1;           ///< [11] Platform Requestor Id Valid
    UINT64       PltfrmRespIdValid:1;          ///< [12] Platform Responder Id Valid
    UINT64       MemPltfrmTargetValid:1;       ///< [13] Memory Platform Target Valid
    UINT64       MemoryErrTypeValid:1;         ///< [14] Memory Error Type Valid
    UINT64       RankNumberValid:1;            ///< [15] Rank Number Valid
    UINT64       CardHandleValid:1;            ///< [16] Card Handle Valid
    UINT64       ModuleHandleValid:1;          ///< [17] Module Handle Valid
    UINT64       ExtendRow1617Valid:1;         ///< [18] Extend Row bit 16 and 17 Valid
    UINT64       BankGroupValid:1;             ///< [19] Bank Group Valid
    UINT64       BankAddressValid:1;           ///< [20] Bank Address Valid
    UINT64       ChipIdValid:1;                ///< [21] Chip Identification Valid
    UINT64       :42;                          ///< [63:22] Reserved
  } Field;
  UINT64  Value;
} MEM_ERR_VALID_BIT;

/// UEFI 2.6 Appendix N Table 285 Spec. Error Type
#define CPER_ERROR_TYPE_ERR_INTERNAL            (1)
#define CPER_ERROR_TYPE_ERR_MEM                 (4)
#define CPER_ERROR_TYPE_ERR_TLB                 (5)
#define CPER_ERROR_TYPE_ERR_CACHE               (6)
#define CPER_ERROR_TYPE_ERR_FUNCTION            (7)
#define CPER_ERROR_TYPE_ERR_SELFTEST            (8)
#define CPER_ERROR_TYPE_ERR_FLOW                (9)
#define CPER_ERROR_TYPE_ERR_BUS                 (16)
#define CPER_ERROR_TYPE_ERR_MAP Virtual         (17)
#define CPER_ERROR_TYPE_ERR_IMPROPER            (18)
#define CPER_ERROR_TYPE_ERR_UNIMPL              (19)
#define CPER_ERROR_TYPE_ERR_LOL                 (20)
#define CPER_ERROR_TYPE_ERR_RESPONSE            (21)
#define CPER_ERROR_TYPE_ERR_PARITY              (22)
#define CPER_ERROR_TYPE_ERR_PROTOCOL            (23)
#define CPER_ERROR_TYPE_ERR_ERROR               (24)
#define CPER_ERROR_TYPE_ERR_TIMEOUT             (25)
#define CPER_ERROR_TYPE_ERR_POISONED            (26)

/// UEFI 2.6 Appendix N Table 284 Spec. Error Status
typedef union {
  struct {
     UINT64       :8;                           ///< Reserved
     UINT64       ErrorType:8;                  ///< Encoded value for the Error_Type
     UINT64       Address:1;                    ///< Error was detected on the address signals or on the address portion of the transaction.
     UINT64       Control:1;                    ///< Error was detected on the control signals or in the control portion of the transaction.
     UINT64       Data:1;                       ///< Error was detected on the data signals or in the data portion of the transaction.
     UINT64       Responder:1;                  ///< Error was detected by the responder of the transaction.
     UINT64       Requester:1;                  ///< Error was detected by the requester of the transaction.
     UINT64       FirstError:1;                 ///< If multiple errors are logged for a section type, this is the first error in the chronological sequence.
     UINT64       Overflow:1;                   ///< Additional errors occurred and were not logged due to lack of logging resources.
     UINT64       :41;                          ///<
  } Field;
  UINT64  Value;
} CPER_ERROR_STATUS;

/// ACPI 6.2 Table 18-381 Generic Error Data Entry. Flags field
/// ACPI 6.3 Table 18-392 Generic Error Data Entry. Flags field
#define CPER_SECTION_FLAG_PRIMARY                   BIT0
#define CPER_SECTION_FLAG_CONTAINMENTWARNING        BIT1
#define CPER_SECTION_FLAG_RESET                     BIT2
#define CPER_SECTION_FLAG_ERRORTHRESHOLDEXCEEDED    BIT3
#define CPER_SECTION_FLAG_RESOURCENOTACCESSIBLE     BIT4
#define CPER_SECTION_FLAG_LATENTERROR               BIT5
#define CPER_SECTION_FLAG_PROPAGATED                BIT6
#define CPER_SECTION_FLAG_OVERFLOW                  BIT7

typedef union {
  struct {
     UINT8       Primary:1;                     ///< If set, identifies the section as the section to be associated with the error condition.
     UINT8       ContainmentWarning:1;          ///< If set, the error was not contained within the processor or memory hierarchy
     UINT8       Reset:1;                       ///< If set, the component has been reset and must be re-initialized or re-enabled by the OS.
     UINT8       ErrorThresholdExceeded:1;      ///< If set, OS may choose to discontinue use of this resource.
     UINT8       ResourceNotAccessible:1;       ///< If set, the resource could not be queried for error information due to conflicts with other system software or resources.
     UINT8       LatentError:1;                 ///< If set, action has been taken to ensure error containment
     UINT8       Propagated:1;                  ///< If set, the section is to be associated with an error that has been propagated due to hardware poisoning.
     UINT8       Overflow:1;                   ///< If set, the firmware has detected an overflow of buffers/queues.
  } Field;
  UINT8  Value;
} CPER_SECTION_FLAG;

/// UEFI 2.6 Appendix N Table 275 Spec.
typedef struct _PLATFORM_MEM_ERR_SEC {
  MEM_ERR_VALID_BIT   ValidBits;    ///< Valid bits Bitmp
  UINT64  ErrStatus;                ///< Error Status
  UINT64  PhyAddr;                  ///< Physical memory address of detected error
  UINT64  PhyAddrMask;              ///< Physical Error Address mask
  UINT16  Node;                     ///< Node Number
  UINT16  Card;                     ///< Card Number
  UINT16  Module;                   ///< Module Number
  UINT16  Bank;                     ///< Bank Number
  UINT16  Device;                   ///< Device Number
  UINT16  Row;                      ///< Row Number
  UINT16  Column;                   ///< Column Number
  UINT16  BitPosition;              ///< Bit Position
  UINT64  RequestorID;              ///< Requestor ID
  UINT64  ResponderID;              ///< Responder ID
  UINT64  TargetID;                 ///< Target ID
  UINT8   MemErrType;               ///< Memory Error Type
  UINT8   Extended;                 ///< Extended
  UINT16  RankNumber;               ///< Rank Number
  UINT16  CardHandle;               ///< Card Number
  UINT16  ModuleHandle;             ///< Module Number
} PLATFORM_MEM_ERR_SEC;

/// UEFI 2.6 Appendix N Table 277 Spec.
///
/// PCIE Error Section DEVICE_ID
///
typedef struct {
  UINT16                     VendorId;                               ///< Vendor ID
  UINT16                     DeviceId;                               ///< Device ID
  UINT8                      ClassCode[3];                           ///< Class Code
  UINT8                      Function;                               ///< Function
  UINT8                      Device;                                 ///< Device
  UINT16                     Segment;                                ///< Segment
  UINT8                      PrimaryBus;                             ///< Primary Bus
  UINT8                      SecondaryBus;                           ///< Secondary Bus
  UINT16                     Slot;                                   ///< Slot
  UINT8                      Reserved;                               ///< Reserved
} DEVICE_ID;

///
/// PCIE Error Section CAP_STRUCTURE
///
typedef struct {
  UINT32                     CapabilityData[15];                     ///< CapabilityData
} CAP_STRUCTURE;

///
/// PCIE Error Section AER_INFO
///
typedef struct {
  UINT32                     AerInfoData[24];                        ///< AerInfoData
} AER_INFO;

/// DRAM Limit Address Register
typedef union {
  struct {                                      ///< PCIE_ERR_VALID_BIT
    UINT64       PortTypeValid:1;              ///< [0] Port Type Valid
    UINT64       VersionValid:1;               ///< [1] Version Valid
    UINT64       CommandStatusValid:1;         ///< [2] Command Status Valid
    UINT64       DeviceIdValid:1;              ///< [3] Device ID Valid
    UINT64       DeviceSerialNumValid:1;       ///< [4] Device Serial Number Valid
    UINT64       BridgeCntlStsValid:1;         ///< [5] Bridge Control Status Valid
    UINT64       CapStructStsValid:1;          ///< [6] Capability Structure Status Valid
    UINT64       AERInfoValid:1;               ///< [7] AER Info Valid
    UINT64       :56;                          ///< [63:8] Reserved
  } Field;
  UINT64  Value;
} PCIE_ERR_VALID_BIT;

///
/// PCIE Error Section
///
typedef struct {
  PCIE_ERR_VALID_BIT         Validation;                             ///< Validation Bits
  UINT32                     PortType;                               ///< Port Type
  UINT32                     Revision;                               ///< Revision
  UINT32                     CommandStatus;                          ///< Command Status
  UINT32                     Reserved;                               ///< Reserved
  DEVICE_ID                  DeviceId;                               ///< Device Id
  UINT8                      SerialNum[8];                           ///< Serial Num
  UINT32                     BridgeCtrlStatus;                       ///< Bridge Control Status
  CAP_STRUCTURE              CapabilityStructure;                    ///< Capability Structure
  AER_INFO                   AerInfo;                                ///< AER Info
} PCIE_ERROR_SECTION;


/*
 * SET_ERROR_TYPE_WITH_ADDRESS action command structure.
 */

typedef struct _SET_ERR_TYPE_WITH_ADDR {
  UINT32  ErrorType;
  UINT32  VendorExtension;
  UINT32  Flags;
  UINT32  Apicid;
  UINT64  MemoryAddress;
  UINT64  MemoryAddressRange;
  UINT32  PcieSbdf;
} SET_ERR_TYPE_WITH_ADDR;

typedef enum _SET_WITH_ADDR_NUM {
  SETWA_FLAGS_APICID = 1,
  SETWA_FLAGS_MEM = 2,
  SETWA_FLAGS_PCIE_SBDF = 4,
} SET_WITH_ADDR_NUM;

/*
 * Vendor extensions for platform specific operations
 */
typedef struct _VENDOR_ERR_TYPE_EXTEN {
  UINT32  Length;
  UINT32  PcieSbdf;
  UINT16  VendorId;
  UINT16  DeviceId;
  UINT8   RevId;
  UINT8   Reserved[3];
} VENDOR_ERR_TYPE_EXTEN;

/// EINJ Correctable Memory Trigger Action Table
typedef struct _TRIG_ERROR_ACTION {
  EFI_ACPI_6_0_EINJ_TRIGGER_ACTION_TABLE  TrigErrActionHeader;                                  ///< Trigger Error Action Header
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY TrigErrorInstruction[EINJ_TRIG_INSTR_ENTRIES];  ///< Trigger Error Instruction Array
} TRIG_ERROR_ACTION;

/// APEI EINJ Registers
typedef struct _APEI_EINJ_REGISTERS {
  UINT64  EinjBeginInjReg;              ///< EINJ Begin Injection Register
  UINT64  EinjGetTriActTblReg;          ///< EINJ Get Trigger Action Table Register
  UINT64  EinjSetErrTypeReg;            ///< EINJ Set Error Type Register
  UINT64  EinjGetErrTypeReg;            ///< EINJ Get Error Type Register
  UINT64  EinjEndOperReg;               ///< EINJ End Operation Register
  UINT64  EinjChkBusyStsReg;            ///< EINJ Check Busy Status Register
  UINT64  EinjCmdStsReg;                ///< EINJ Command Status Register
  UINT64  EinjSetErrTypeWithAddrReg;    ///< EINJ Set Error Type with Address Register
} APEI_EINJ_REGISTERS;

/// EINJ ACPI Table
typedef struct _APEI_EINJ_ACPI_TABLE {
  EFI_ACPI_6_0_ERROR_INJECTION_TABLE_HEADER       InjectionHeader;                          ///< APEI EINJ Table Header
  EFI_ACPI_6_0_EINJ_INJECTION_INSTRUCTION_ENTRY   InjectionInstEntry[EINJ_INSTR_ENTRIES];   ///< APEI EINJ Injection Intrustion Instance Entry
} APEI_EINJ_ACPI_TABLE;

///
/// Platform RAS ACPI to SMM data buffer
///
#define RAS_OPERATION_REGION_SIGNATURE  SIGNATURE_32 ('P', 'R', 'A', 'S')        ///< 0x53415250
#define RAS_ACPI_SMM_DATA_BUFFER_SIGNATURE  SIGNATURE_32 ('P', 'D', 'A', 'T')    ///< 0x54414450

typedef struct _RAS_ACPI_SMM_DATA {
  UINT32                    Sig;                    ///< RAS ACPI to SMMI data buffer signature.
  PLATFORM_WIDE_OSC_CAP     AcpiPlatformCap;        ///< Platform-Wide _OCS Capabilities DWORD 2
  PCIE_OSC_SUPP             PcieOscSupp;            ///< PCI-E Interpretation _OSC Support Field, DWORD 2 Mask
  PCIE_OSC_CTRL             PcieOscCtrl;            ///< PCI-E Interpretation _OSC Support Field, DWORD 3 Mask
} RAS_ACPI_SMM_DATA;

/// Pcie Port Profile
typedef struct {
  UINT8         NbioDieNum;             ///< NbioDieNum
  UINT8         NbioBusNum;             ///< NbioBusNum
  UINT8         PciPortNumber;          ///< PciPortNumber
  UINT8         EndPointDevCnt;         ///< Device counter under the root port.
  UINT32        RpPciAddr;              ///< Root Port PCI configuration Address
  LIST_ENTRY    PciLinkList;            ///< Head of link list for PCI topology under root port.
} PCIE_PORT_PROFILE;

/// Pcie Device Entry
typedef struct {
  LIST_ENTRY    ListEntry;              ///< Link list for PCI topology under root port.
  UINT32        DevAddr;                ///< PCI configuration space address
  UINT8         DevType;                ///< PCIe Device Type
} PCIE_DEV_ENTRY;

/// Pcie Active Port Map
typedef struct {
  UINT16              PortCount;        ///< PortCount
  PCIE_PORT_PROFILE   PciPortNumber[1]; ///< PciPortNumber
} PCIE_ACTIVE_PORT_MAP;

/// Pcie Port List
typedef struct {
  UINT8              Device;            ///< Device
  UINT8              Function;          ///< Function
} PCIE_PORT_LIST;

/// Pcie Error List Entry
typedef struct {
  BOOLEAN       EntryValid;             ///< Entry is Valid
  UINT32        DevAddr;                ///< PCI configuration space address
  UINT8         DevType;                ///< PCIe Device Type
  UINT32        RootErrSts;             ///< Root Port AER Root Error Status
} PCIE_ERR_ENTRY;

/// Nbio Parity Error Table
typedef struct {
  UINT32        GroupType;       ///< Correctable/Uncorrectable
  UINT32        GroupId;         ///< Group ID
  UINT32        StructId;        ///< Structure ID
  UINT32        ErrGenCmd;       ///< Error command
} NBIO_PARITY_ERROR_TABLE;

#define MCA_INSTANCE_ID_END          (0xFFFF)

/// MCA Instance ID List
typedef struct {
  UINT32        InstanceID;         ///< MCA IPID Instance ID
  UINT8         NumberOfInstance;   ///< The number of block instance
} MCA_INSTANCE_ID_ENTRY;

#define MAX_PCIEMAP_BLOCK_SIZE      (0x800)

/// MCA Instance ID List
typedef struct {
  UINT16        CurrentStatOffset;
  UINT16        CurrentCntlOffset;
  UINT16        ErrLogOffset;
  UINT16        CmnStatOffset;
  UINT16        PortStatOffset;
} CCIX_ERROR_LOG_DATA;

#pragma pack (pop)

#define GENERIC_ERROR_DATA_ENTRY_FRUTEXT_SIZE      (20)
#define MEM_ERROR_MXA_PREFIX_FRUTEXT_SIZE          (14)
#define MEM_ERROR_MXA_SILKSCREEN_TEXT_SIZE         (6)

/// Platform memory DIMM locater map
typedef struct {
  UINT16                Node;               ///< Socket (For SSP, only one IOD per socket)
  UINT16                Card;               ///< Channel ID
  UINT16                Module;             ///< Module 0 (CS=0,1):Module 1 (CS=2,3)
  CHAR8                 FruText[MEM_ERROR_MXA_SILKSCREEN_TEXT_SIZE];         ///< DIMM locator, silkscreen text. (max 6 bytes)
} OEM_MEMORY_MAP_TABLE_ENTRY;

typedef struct {
  UINT32                     TableSize;                     ///< Table Entries total size, not include header size
  UINT32                     TableEntryNum;                 ///< Table Entries number
  OEM_MEMORY_MAP_TABLE_ENTRY *MemoryMapTableEntry;          ///< Oem memory map table entry.
} OEM_MEMORY_MAP_TABLE;

typedef struct {
  UINT32 ApicId;
  UINT32 ProcessorUid;
} MCA_LOCALAPICID_MAPPING_STRUCTURE;

typedef struct {
  UINT32                             TableEntryNum;
  MCA_LOCALAPICID_MAPPING_STRUCTURE  McaLocalApicIdMappingStructure[1];
} MCA_LOCALAPICID_MAPPING_TABLE;

#endif //_AMD_CPM_RAS_COMMON_H_

