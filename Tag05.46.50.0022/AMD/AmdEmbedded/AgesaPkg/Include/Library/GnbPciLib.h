/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Various PCI service routines.
 *
 *
  * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * 
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _GNBLIBPCI_H_
#define _GNBLIBPCI_H_

#include "Porting.h"
#include "AMD.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#pragma pack (push, 1)
/**
 * @brief common AMD vendor and device IDs
 * @details 
 */
#define AMD_PCI_VID                   0x1022    ///< Vendor ID
#define AMD_PCI_DID_ROOT_COMPLEX      0x1480    ///< Device ID for Root complex
#define AMD_PCI_DID_IOMMU             0x1481    ///< Device ID IOMMU
#define AMD_PCI_DID_DUMMY_HOSTBRIDGE  0x1482    ///< Dummy host bridge Device ID
#define AMD_PCI_DID_GPP_BRIDGE        0x1483    ///< Device ID GPP Bridge
#define AMD_PCI_DID_INT_GPP_BRIDGE    0x1484    ///< Device ID GPP Bridge
#define AMD_PCI_DID_DUMMY_FUNCTION    0x1485    ///< Dummy function Device ID
#define AMD_PCI_DID_PSP               0x1486    ///< Device ID PSP

/**
 * @brief Type 0 and 1 config space offsets
 * @details 
 */
#define PCICFG_SPACE_VID_OFFSET                   0x00    ///< Vendor ID offset-manufacturer id
#define PCICFG_SPACE_DID_OFFSET                   0x02    ///< Device ID offset-identifies particular device
#define PCICFG_SPACE_COMMAND_OFFSET               0x04    ///< Command offset
#define PCICFG_SPACE_STATUS_OFFSET                0x06    ///< Status Offset
#define PCICFG_SPACE_REV_ID_OFFSET                0x08    ///< Revision ID offset
#define PCICFG_SPACE_CLASS_CODE_OFFSET            0x09    ///< Class code offset
#define PCICFG_SPACE_CACHE_LINE_SIZE_OFFSET       0x0C    ///< Cache line size offset
#define PCICFG_SPACE_MASTER_LATENY_OFFSET         0x0D    ///< Master latency offset
#define PCICFG_SPACE_HEADER_TYPE_OFFSET           0x0E    ///< Header type offset
#define PCICFG_SPACE_BIST_OFFSET                  0x0F    ///> bist register offset
#define PCICFG_SPACE_CAPABILITIES_PTR_OFFSET      0x34    ///> capabilities pointer
#define PCICFG_SPACE_INTERRUPT_LINE_OFFSET        0x3C    ///> interrupt routing information communication
#define PCICFG_SPACE_INTERRUPT_PIN_OFFSET         0x3D    ///> identifies legacy interrupt message

/**
 * @brief Type 0 config space offsets
 * @details 
 */
#define PCICFG_SPACE_SUBSYSTEM_VID_OFFSET         0x2C      ///< vendor id offset
#define PCICFG_SPACE_SUBSYSTEM_ID_OFFSET          0x2E      ///< uniquely identify adapter or subsystem where PCIe compt resides
#define PCICFG_SPACE_EXPANSION_ROM_BAR_OFFSET     0x30      ///< handle base address and size of expansion rom

/**
 * @brief Type 1 config space offsets
 * @details 
 */
#define PCICFG_SPACE_BAR0_OFFSET                  0x10    ///< Bar address register 0
#define PCICFG_SPACE_BAR1_OFFSET                  0x14    ///< Bar address register 1
#define PCICFG_SPACE_PRIMARY_BUS_OFFSET           0x18    ///< Primary bus number-legacy software compatibility
#define PCICFG_SPACE_SECONDARY_BUS_OFFSET         0x19    ///< Secondary bus number-record bus number of PCI bus segment
#define PCICFG_SPACE_SUBORDINATE_BUS_OFFSET       0x1A    ///< Subordinate bus number-record number of highest numbered PCI bus segment
#define PCICFG_SPACE_SEC_LATENCY_TIMER_OFFSET     0x1B    ///< does not apply to PCI express.
#define PCICFG_SPACE_IO_BASE_OFFSET               0x1C    ///< Optional-defines an address range for bridge to determine I/o transactions
#define PCICFG_SPACE_IO_LIMIT_OFFSET              0x1D    ///< optional-defines an address range for bridge to determine I/o transactions
#define PCICFG_SPACE_SECONDARY_STATUS_OFFSET      0x1E    ///< provides register layout
#define PCICFG_SPACE_MEMORY_BASE_OFFSET           0x20    ///< memory mapped register address range used by bridge to determine forwarding transactions
#define PCICFG_SPACE_MEMORY_LIMIT_OFFSET          0x22    ///< memory mapped register address range used by bridge to determine forwarding transactions
#define PCICFG_SPACE_PREFETCH_BASE_LOW_OFFSET     0x24    ///< optional,must indicate 64-bits supported.
#define PCICFG_SPACE_PREFETCH_BASE_HIGH_OFFSET    0x26    ///< optional must indicate 64 bits supported
#define PCICFG_SPACE_PREFETCH_LOW_OFFSET          0x28    ///< optional
#define PCICFG_SPACE_PREFETCH_HIGH_OFFSET         0x2C    ///< optional
#define PCICFG_SPACE_IO_BASE_LOW_OFFSET           0x30    ///< optional
#define PCICFG_SPACE_IO_BASE_HIGH_OFFSET          0x32    ///< optional
#define PCICFG_SPACE_EXPANSION_ROM_BASE_OFFSET    0x38    ///< different than type 0 config header
#define PCICFG_SPACE_BRIDGE_CONTROL_OFFSET        0x3E    ///< provides extension to command register that are specific to function with header

/**
 * @brief Max functions,devices and buses
 * @details 
 */
#define PCIE_MAX_FUNCTIONS    8             ///< Maximum functions
#define PCIE_MAX_DEVICES      32            ///< Maximum devices connected
#define PCIE_MAX_BUS          256           ///< Maximum buses

/**
 * @brief As we need them, flesh out these PCIE Configuration status structures. No longer use hard codes
 * @details 
 */
typedef union {
  struct {
    UINT8     ParityErrorResponseEnable:1;
    UINT8     SerrEnable:1;
    UINT8     Reserved0:3;
    UINT8     MasterAbortMode:1;
    UINT8     SecondaryBusReset:1;
    UINT8     FastBackToBackTransactionEn:1;
    UINT8     PrimaryDiscardTimer:1;
    UINT8     SecondaryDiscardTimer:1;
    UINT8     DiscardTimerStatus:1;
    UINT8     DiscardTimerSerrEn:1;
    UINT8     Reserved1:4;
  } Field;                                      ///< field structure with different parameters
  UINT16 Value;                                 ///< value field
} PCICFG_SPACE_BRIDGE_CONTROL_STRUCT;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
#ifndef IOMMU_CAP_ID
    #define IOMMU_CAP_ID                0x0F    ///< IOMMU capability id
#endif
#ifndef ARI_CAP_ID
    #define ARI_CAP_ID                  0x0E  /***< ARI Extended Capability ID*/
#endif
#ifndef PCIE_EXT_ESM_CAP_ID
    #define PCIE_EXT_ESM_CAP_ID         0x23  /***< Designated Vendor-Specific Extended Capability ID*/
#endif
#ifndef PCIE_EXT_AER_CAP_ID
    #define PCIE_EXT_AER_CAP_ID         0x01  /***< Advanced Error Reporting Extended Capability ID*/
#endif
#ifndef PCIE_EXT_DEV_SN_ID
    #define PCIE_EXT_DEV_SN_ID          0x03  /***< Device Serial Number Extended Capability ID*/
#endif
#ifndef PCIE_EXT_RCEC_CAP_ID
    #define PCIE_EXT_RCEC_CAP_ID        0x07  /***< Root Complex Event Collector Endpoint Association Extended Capability ID*/
#endif
#ifndef DPC_EXT_CAP_ID
    #define DPC_EXT_CAP_ID              0x1D  /***< DPC Extended Capability ID*/
#endif
#ifndef PCIE_LTR_EXT_CAP_ID
    #define PCIE_LTR_EXT_CAP_ID         0x18  /***< Latency Tolerance Reporting (LTR) Capability ID*/
#endif
#ifndef PCIE_SEC_EXT_CAP_CAP_ID
    #define PCIE_SEC_EXT_CAP_CAP_ID     0x19  /***< Secondary PCI Express Extended Capability ID*/
#endif


/** @brief PCIe device type
 *  @details 
 */
typedef enum {
  PcieDeviceEndPoint,                       /***< Endpoint (Type 00h Header)*/
  PcieDeviceLegacyEndPoint,                 /***< Legacy endpointt (Type 00h Header)*/
  PcieDeviceRootComplex = 4,                /***< Root complex (Type 01h Header)*/
  PcieDeviceUpstreamPort,                   /***< Upstream port (Type 01h Header)*/
  PcieDeviceDownstreamPort,                 /***< Downstream Port (Type 01h Header)*/
  PcieDevicePcieToPcix,                     /***< PCIe to PCI/PCIx bridge (Type 01h Header)*/
  PcieDevicePcixToPcie,                     /***< PCI/PCIx to PCIe bridge (Type 01h Header)*/
  PCieDeviceRCiEP,                          /***< CXL RCiEP (Type 00h Header)*/
  PcieDeviceRcec,                           /***< Root Complex Event Collector (Type 00h Header)*/
  PcieNotPcieDevice = 0xff                  /***< unknown device*/
} PCIE_DEVICE_TYPE;

typedef  UINT32 SCAN_STATUS;    ///< scan status

/**
 * @brief Internally used controls for PCI Bus scan functions
 * @details 
 */
#define SCAN_SKIP_FUNCTIONS         0x1       ///< PCI bus scan skip functions
#define SCAN_SKIP_DEVICES           0x2       ///< PCI bus scan skip devices
#define SCAN_SKIP_BUSES             0x4       ///< PCI bus scan skip buses
#define SCAN_SUCCESS                0x0       ///< PCI bus scan success

typedef struct _GNB_PCI_SCAN_DATA GNB_PCI_SCAN_DATA;    ///< Forward declaration needed for multi-structure mutual references

/**
 *  @brief Scan supporting data
 *  @details 
 */
typedef SCAN_STATUS (*GNB_SCAN_CALLBACK) (
  IN       PCI_ADDR             Device,
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData
  );

struct _GNB_PCI_SCAN_DATA {
  GNB_SCAN_CALLBACK     GnbScanCallback;  /***< Callback for each found device*/
  AMD_CONFIG_PARAMS     *StdHeader;       /***< Standard configuration header*/

};
/**
 * @brief PCIE capabilities structure common header.
 * @details 
 */
#define PM_CAP_ID                   0x01

typedef struct {
  UINT8       CapabilityId;
  UINT8       NextCapabilityId;
} PCIE_CAPABILITY_HEADER;

/** @brief Power Management Capabilities Register (PCIE4.0 7.5.2)
 *  @details 
 */
typedef union {
  struct {
    UINT8     Version:3;
    UINT8     PmeClock:1;
    UINT8     ImRdyOnRtrn:1;
    UINT8     Dsi:1;
    UINT8     AuxCurrent:3;
    UINT8     D1Support:1;
    UINT8     D2Support:1;
    UINT8     PmeSupport:5;
  } Field;
  UINT16 Value;
} PM_CAPABILITIES_REGISTER;

typedef union {
  struct {
    UINT8     PowerState:2;
    UINT8     Reserved1:1;
    UINT8     NoSoftReset:1;
    UINT8     Reserved2:4;
    UINT8     PmeEnable:1;
    UINT8     DataSelect:4;
    UINT8     DataScale:2;
    UINT8     PmeStatus:1;
    UINT8     Reserved3:6;
    UINT8     Undefined1:1;
    UINT8     Undefined2:1;
    UINT8     Data;
  } Field;
  UINT16 Value;
} PM_STATUS_CONTROL_REGISTER;

/** @brief Power state(s) for above structure.
 *  @details
 */
typedef enum {
  PM_D0     = 0,
  PM_D1     = 1,
  PM_D2     = 2,
  PM_D3_HOT = 3
} PM_POWERSTATES;

typedef struct {
  PCIE_CAPABILITY_HEADER       PmCapabilityHeader;              ///< capability header
  PM_CAPABILITIES_REGISTER     PmManagementCapabilities;        ///< management capability
  PM_STATUS_CONTROL_REGISTER   PmStatusControl;                 ///< status control
  UINT8                        PmStatusReserved;                ///< status reserved
  UINT8                        PmData;                          ///< data
} PCIE_CAP_PM;

/**
 * @brief PCIE capabilities structure(PCIE4.0 7.5.3)
 * @details 
 */
#define   PCIE_CAP_ID                0x10   ///< Capability ID
#define   PCIE_LINK_CAP_REGISTER     0x0C   /***< Link capabilities*/
#define   PCIE_LINK_CTRL_REGISTER    0x10   /***< Controls link specific capabilities*/
#define   PCIE_DEVICE_CAP_REGISTER   0x04   /***< Device capabilities*/
#define   PCIE_DEVICE_CTRL_REGISTER  0x08   /***< Controls device specific capabilities*/
#define   PCIE_LINK_STATUS_REGISTER  0x12   ///< Status register
#define   PCIE_DEVICE_CAP2_REGISTER  0x24   /***< Device 2 capabilities*/
#define   PCIE_DEVICE_CNTL2_REGISTER 0x28   /***< Controls device specific capabilities*/
#define   PCIE_LINK_CAP2_REGISTER    0x2C   ///< Capabilitity link registers
#define   PCIE_LINK_CTRL3_REGISTER   0x04   ///< Link control registers
#define   PCIE_LINK_CNTL2_REGISTER   0x30   ///< Link control registers
#define   PCIE_ASPM_L1_SUPPORT_CAP   BIT11  /***< ASPM programming*/
#define   PCIE_SLOT_CAP_REGISTER     0x14   /***< slot specific capabilities*/

/**
 * @brief PCIE L1 PM SUB Capabilities Structure
 * @details Link cap registers give link specific capabilities.
 */
#define   PCIE_L1_PM_SUB_CAP_ID      0x1E   ///< L1 PM SUB capability ID
#define   PCIE_L1_PM_SUB_CAP         0x04   ///< L1 PM SUB capability
#define   PCIE_L1_PM_SUB_CNTL        0x08   ///< L1 PM SUB capability Control
#define   PCIE_L1_PM_SUB_CNTL2       0x0C   ///< L1 PM SUB capability

typedef union {
  struct {
    UINT8     CorrectableErrorReportingEn:1;
    UINT8     NonFatalErrorReportingEn:1;
    UINT8     FatalErrorReportingEn:1;
    UINT8     UnsupportedReqReportingEn:1;
    UINT8     UnsupportedReqSeverity:1;
    UINT8     MaxPayloadSize:3;
    UINT8     ExtendedTagFieldEn:1;
    UINT8     PhantomFunctionsEn:1;
    UINT8     AuxPowerPmEnable:1;
    UINT8     DevCtrlStop:1;
    UINT8     MaxReadReqSize:3;
    UINT8     Reserved0:1;
  } Field;
  UINT16 Value;
} PCIE_DEVICE_CTRL_REGISTER_STRUCT;

typedef union {
  struct {
    UINT8     AspmControl:2;
    UINT8     Reserved1:1;
    UINT8     ReadCompletionBoundary:1;
    UINT8     LinkDisable:1;
    UINT8     RetrainLink:1;
    UINT8     CommonClockCOnfig:1;
    UINT8     ExtendedSync:1;
    UINT8     EnableClockPm:1;
    UINT8     HardwareAutoWidthDisable:1;
    UINT8     LinkBandwidthManagementIntEn:1;
    UINT8     LinkAutoBandwidthIntEn:1;
    UINT8     Reserved2:2;
    UINT8     DrsSignalingControl:2;
  } Field;
  UINT16 Value;
} PCIE_LINK_CTRL_REGISTER_STRUCT;

typedef union {
  struct {
    UINT8       LinkSpeed:4;
    UINT8       LinkWidth:6;
    UINT8       LinkTrainingError:1;
    UINT8       LinkTraining:1;
    UINT8       SlotClockConfig:1;
    UINT8       Reserved0:3;
  } Field;
  UINT16 Value;
} PCIE_LINK_STATUS_REGISTER_STRUCT;

typedef union {
  struct {
    UINT8     Reserved1:1;
    UINT8     SupportedLinkSpeedsVector:7;
    UINT8     CrosslinkSupported:1;
    UINT8     LowerSkpOsGenSupportSpdVec:7;
    UINT8     LowerSkpOsRcpSupportedSpdVec:7;
    UINT8     Reserved2:8;
    UINT8     DRSSupported:1;
  } Field;
  UINT32 Value;
} PCIE_LINK_CAP2_REGISTER_STRUCT;

typedef union {
  struct {
    UINT16    PerformEqualization:1;
    UINT16    LinkEqualizationReqIntEn:1;
    UINT16    Reserved1:7;
    UINT16    EnableLowerSkpOsGenVec:7;
  } Field;
  UINT16 Value;
} PCIE_LINK_CTRL3_REGISTER_STRUCT;

typedef union {
  struct {
    UINT8     TargetLinkSpeed:4;
    UINT8     EnterCompliance:1;
    UINT8     HwAutonomousSpeedDis:1;
    UINT8     SelectableDeephasis:1;
    UINT8     TxMargin:3;
    UINT8     EnterModCompliance:1;
    UINT8     ComplianceSos:1;
    UINT8     CompliancePresetDeemphasis:4;
  } Field;
  UINT16 Value;
} PCIE_LINK_CTRL2_REGISTER_STRUCT;

/**
 * @brief PCIe Extended Capabilities Header
 * @details 
 */
typedef union {
  struct {
    UINT32     PcieExtCapId:16;               /***< PCI Express Extended Capability ID */
    UINT32     PcieExtCapVersion:4;           /***< Capability Version */
    UINT32     PcieExtCapNextCapOffet:12;     /***< Next Capability Offset */
  } Field;
  UINT32 Value;
} PCIE_EXTENDED_CAPABILITY_HEADER;

/**
 * @brief PCIe Uncorrectable Error Mask (PCIe4.0 7.8.4.3)
 * @details 
 */
#define PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET  0x08

typedef union {
  struct {
    UINT32                  Undefined1:1;
    UINT32                  Reserved1:3;
    UINT32                  DataLinkProtocolErrorMask:1;
    UINT32                  SurpriseDownErrorMask:1;
    UINT32                  Reserved2:6;
    UINT32                  PoisonedTLPMask:1;
    UINT32                  FlowControlProtocolErrorMask:1;
    UINT32                  CompletionTimeoutMask:1;
    UINT32                  CompleterAbortMask:1;
    UINT32                  UnexpectedCompletionMask:1;
    UINT32                  ReceiverOverflowMask:1;
    UINT32                  MalTlpMask:1;
    UINT32                  ECRCErrorMask:1;
    UINT32                  UnsupportedRequestErrorStatuser:1;
    UINT32                  AcsViolationMask:1;
    UINT32                  UncorrectableInternalErrorMask:1;
    UINT32                  McBlockedTlpMask:1;
    UINT32                  AtomicopEgressBlockedMask:1;
    UINT32                  TlpPrefixBlockedErrorMask:1;
    UINT32                  PoisonedTlpEgressBlockedMask:1;
    UINT32                  Reserved3:5;
  } Field;
  UINT32 Value;
} PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK;

/**
 * @brief PCIe Uncorrectable Error Severity (PCIe4.0 7.8.4.4)
 * @details 
 */
#define PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET  0x0C

typedef union {
  struct {
    UINT32                  Undefined1:1;
    UINT32                  Reserved1:3;
    UINT32                  DataLinkProtocolErrorSeverity:1;
    UINT32                  SurpriseDownErrorSeverity:1;
    UINT32                  Reserved2:6;
    UINT32                  PoisonedTLPSeverity:1;
    UINT32                  FlowControlProtocolErrorSeverity:1;
    UINT32                  CompletionTimeoutSeverity :1;
    UINT32                  CompleterAbortSeverity :1;
    UINT32                  UnexpectedCompletionSeverity:1;
    UINT32                  ReceiverOverflowErrorSeverity:1;
    UINT32                  MalTlpSeverity:1;
    UINT32                  ECRCErrorSeverity:1;
    UINT32                  UnsupportedRequestErrorSeverity:1;
    UINT32                  AcsViolationSeverity:1;
    UINT32                  UncorrectableInternalErrorSeverity:1;
    UINT32                  McBlockedTlpSeverity:1;
    UINT32                  AtomicOpEgressBlockedSeverity:1;
    UINT32                  TlpPrefixBlockedErrorSeverity:1;
    UINT32                  PoisonedTlpEgressBlockedESeverity:1;
    UINT32                  Reserved3:5;
  } Field;
  UINT32 Value;
} PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY;

/**
 * @brief Correctable Error Mask (PCIe4.0 7.8.4.6)
 * @details 
 */
#define PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET  0x14

typedef union {
  struct {
    UINT32                  ReceiverErrorMask:1;
    UINT32                  Reserved1:5;
    UINT32                  BadTLPMask:1;
    UINT32                  BadDLLPMask:1;
    UINT32                  ReplayNumberRolloverMask:1;
    UINT32                  Reserved2:3;
    UINT32                  ReplayTimerTimeoutMask:1;
    UINT32                  AdvisoryNonFatalErrorMask:1;
    UINT32                  CorrectedInternalErrorMask:1;
    UINT32                  HeaderLogOverflowMask:1;
    UINT32                  Reserved3:16;
  } Field;
  UINT32 Value;
} PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK;

/**
 * @brief Advanced Error capabilities and Control Register (PCIe4.0 7.8.4.7)
 * @details 
 */
#define PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET  0x18

typedef union {
  struct {
    UINT32                          FirstErrorPointer:5 ;
    UINT32                          EcrcGenerationCapable:1 ;
    UINT32                          EcrcGenerationEnable:1 ;
    UINT32                          EcrcCheckCapable:1 ;
    UINT32                          EcrcCheckEnable:1 ;
    UINT32                          MultipleHeaderRecordingCapable:1 ;
    UINT32                          MultipleHeaderRecordingEnable:1 ;
    UINT32                          TlpPrefixLogPresent:1 ;
    UINT32                          CompletionTimeoutPrefixHeaderLogCapable:1 ;
    UINT32                          Reserved_31_13:19;
  } Field;
  UINT32 Value;
} PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL;

/**
 * @brief CCIX PROTOCOL LAYER DVSEC EXTENDED CAPABILITY  (ESM)
 * @details Vendor specific (AMD Only) ESM Capabilities Structure
 */
typedef union {
  struct {
  UINT32      EsmVendorId:16;         /***< This MUST be validated to be 0x1022*/
  UINT32      EsmCapabilityRev:4;
  UINT32      EsmCapabilityLen:12;
  } Field;
  UINT32 Value;
} PCIE_EXTCAP_ESM_VENDOR_SPECIFIC_HEADER_1;

typedef union {
  struct {
    UINT16      EsmGen3DataRate:7;
    UINT16      Reserved1:1;
    UINT16      EsmGen4DataRate:7;
    UINT16      EsmEnable:1;
  } Field;
  UINT16 Value;
} PCIE_EXTCAP_ESM_CONTROL;

typedef union {
  struct {
    UINT8         Esm8_0GTs:1;
    UINT8         Esm8_1GTs:1;
    UINT8         Esm8_2GTs:1;
    UINT8         Esm8_3GTs:1;
    UINT8         Esm8_4GTs:1;
    UINT8         Esm8_5GTs:1;
    UINT8         Esm8_6GTs:1;
    UINT8         Esm8_7GTs:1;
    UINT8         Esm8_8GTs:1;
    UINT8         Esm8_9GTs:1;
    UINT8         Esm9_0GTs:1;
    UINT8         Esm9_1GTs:1;
    UINT8         Esm9_2GTs:1;
    UINT8         Esm9_3GTs:1;
    UINT8         Esm9_4GTs:1;
    UINT8         Esm9_5GTs:1;
    UINT8         Esm9_6GTs:1;
    UINT8         Esm9_7GTs:1;
    UINT8         Esm9_8GTs:1;
    UINT8         Esm9_9GTs:1;
    UINT8         Esm10_0GTs:1;
    UINT8         Esm10_1GTs:1;
    UINT8         Esm10_2GTs:1;
    UINT8         Esm10_3GTs:1;
    UINT8         Esm10_4GTs:1;
    UINT8         Esm10_5GTs:1;
    UINT8         Esm10_6GTs:1;
    UINT8         Esm10_7GTs:1;
    UINT8         Esm10_8GTs:1;
    UINT8         Esm10_9GTs:1;
    UINT8         Reserved:2;
  } Field;
  UINT32 Value;
} PCIE_CAPABLESPEEDS_1;

typedef union {
  struct {
    UINT8         Esm11_0GTs:1;
    UINT8         Esm11_1GTs:1;
    UINT8         Esm11_2GTs:1;
    UINT8         Esm11_3GTs:1;
    UINT8         Esm11_4GTs:1;
    UINT8         Esm11_5GTs:1;
    UINT8         Esm11_6GTs:1;
    UINT8         Esm11_7GTs:1;
    UINT8         Esm11_8GTs:1;
    UINT8         Esm11_9GTs:1;
    UINT8         Esm12_0GTs:1;
    UINT8         Esm12_1GTs:1;
    UINT8         Esm12_2GTs:1;
    UINT8         Esm12_3GTs:1;
    UINT8         Esm12_4GTs:1;
    UINT8         Esm12_5GTs:1;
    UINT8         Esm12_6GTs:1;
    UINT8         Esm12_7GTs:1;
    UINT8         Esm12_8GTs:1;
    UINT8         Esm12_9GTs:1;
    UINT8         Esm13_0GTs:1;
    UINT8         Esm13_1GTs:1;
    UINT8         Esm13_2GTs:1;
    UINT8         Esm13_3GTs:1;
    UINT8         Esm13_4GTs:1;
    UINT8         Esm13_5GTs:1;
    UINT8         Esm13_6GTs:1;
    UINT8         Esm13_7GTs:1;
    UINT8         Esm13_8GTs:1;
    UINT8         Esm13_9GTs:1;
    UINT8         Reserved:2;
  } Field;
  UINT32 Value;
} PCIE_CAPABLESPEEDS_2;

typedef union {
  struct {
    UINT8         Esm14_0GTs:1;
    UINT8         Esm14_1GTs:1;
    UINT8         Esm14_2GTs:1;
    UINT8         Esm14_3GTs:1;
    UINT8         Esm14_4GTs:1;
    UINT8         Esm14_5GTs:1;
    UINT8         Esm14_6GTs:1;
    UINT8         Esm14_7GTs:1;
    UINT8         Esm14_8GTs:1;
    UINT8         Esm14_9GTs:1;
    UINT8         Esm15_0GTs:1;
    UINT8         Esm15_1GTs:1;
    UINT8         Esm15_2GTs:1;
    UINT8         Esm15_3GTs:1;
    UINT8         Esm15_4GTs:1;
    UINT8         Esm15_5GTs:1;
    UINT8         Esm15_6GTs:1;
    UINT8         Esm15_7GTs:1;
    UINT8         Esm15_8GTs:1;
    UINT8         Esm15_9GTs:1;
    UINT8         Reserved1:4;
    UINT8         Reserved2;
  } Field;
  UINT32 Value;
} PCIE_CAPABLESPEEDS_3;

typedef union {
  struct {
    UINT8         Esm16_0GTs:1;
    UINT8         Esm16_1GTs:1;
    UINT8         Esm16_2GTs:1;
    UINT8         Esm16_3GTs:1;
    UINT8         Esm16_4GTs:1;
    UINT8         Esm16_5GTs:1;
    UINT8         Esm16_6GTs:1;
    UINT8         Esm16_7GTs:1;
    UINT8         Esm16_8GTs:1;
    UINT8         Esm16_9GTs:1;
    UINT8         Esm17_0GTs:1;
    UINT8         Esm17_1GTs:1;
    UINT8         Esm17_2GTs:1;
    UINT8         Esm17_3GTs:1;
    UINT8         Esm17_4GTs:1;
    UINT8         Esm17_5GTs:1;
    UINT8         Esm17_6GTs:1;
    UINT8         Esm17_7GTs:1;
    UINT8         Esm17_8GTs:1;
    UINT8         Esm17_9GTs:1;
    UINT8         Esm18_0GTs:1;
    UINT8         Esm18_1GTs:1;
    UINT8         Esm18_2GTs:1;
    UINT8         Esm18_3GTs:1;
    UINT8         Esm18_4GTs:1;
    UINT8         Esm18_5GTs:1;
    UINT8         Esm18_6GTs:1;
    UINT8         Esm18_7GTs:1;
    UINT8         Esm18_8GTs:1;
    UINT8         Esm18_9GTs:1;
    UINT8         Reserved:2;
  } Field;
  UINT32 Value;
} PCIE_CAPABLESPEEDS_4;

typedef union {
  struct {
    UINT8         Esm19_0GTs:1;
    UINT8         Esm19_1GTs:1;
    UINT8         Esm19_2GTs:1;
    UINT8         Esm19_3GTs:1;
    UINT8         Esm19_4GTs:1;
    UINT8         Esm19_5GTs:1;
    UINT8         Esm19_6GTs:1;
    UINT8         Esm19_7GTs:1;
    UINT8         Esm19_8GTs:1;
    UINT8         Esm19_9GTs:1;
    UINT8         Esm20_0GTs:1;
    UINT8         Esm20_1GTs:1;
    UINT8         Esm20_2GTs:1;
    UINT8         Esm20_3GTs:1;
    UINT8         Esm20_4GTs:1;
    UINT8         Esm20_5GTs:1;
    UINT8         Esm20_6GTs:1;
    UINT8         Esm20_7GTs:1;
    UINT8         Esm20_8GTs:1;
    UINT8         Esm20_9GTs:1;
    UINT8         Esm21_0GTs:1;
    UINT8         Esm21_1GTs:1;
    UINT8         Esm21_2GTs:1;
    UINT8         Esm21_3GTs:1;
    UINT8         Esm21_4GTs:1;
    UINT8         Esm21_5GTs:1;
    UINT8         Esm21_6GTs:1;
    UINT8         Esm21_7GTs:1;
    UINT8         Esm21_8GTs:1;
    UINT8         Esm21_9GTs:1;
    UINT8         Reserved:2;
  } Field;
  UINT32 Value;
} PCIE_CAPABLESPEEDS_5;

/**
 * @brief This calculates the value of EsmGen3DataRate
 * @details 
 */
#define ESM_DATA_RATE_CALC_GEN3(SPEED)           ((100*SPEED - 640)/10)
/**
 * @brief This calculates the value of EsmGen4DataRate
 * @details 
 */
#define ESM_DATA_RATE_CALC_GEN4(SPEED)           ((100*SPEED - 1280)/10)
typedef struct {
  PCIE_EXTENDED_CAPABILITY_HEADER            EsmExtCapHeader;                       ///< PCI SIG defined ID number
  PCIE_EXTCAP_ESM_VENDOR_SPECIFIC_HEADER_1   EsmExtCapVendorSpHeader1;              ///< vendor based
  UINT16                                     EsmExtCapVendorSpHeader2;              ///< vendor based id
  UINT16                                     EsmExtCapStatus;                       ///< Capability status
  PCIE_EXTCAP_ESM_CONTROL                    EsmExtCapControl;                      ///< control status
  UINT16                                     EsmExtCapReserved;                     ///< reserved
  PCIE_CAPABLESPEEDS_1                       EsmExtCapSupportedSpeeds1;             ///< supported speeds
  PCIE_CAPABLESPEEDS_2                       EsmExtCapSupportedSpeeds2;             ///< supported speeds
  PCIE_CAPABLESPEEDS_3                       EsmExtCapSupportedSpeeds3;             ///< supported speeds
  PCIE_CAPABLESPEEDS_4                       EsmExtCapSupportedSpeeds4;             ///< supported speeds
  PCIE_CAPABLESPEEDS_5                       EsmExtCapSupportedSpeeds5;             ///< supported speeds
  UINT32                                     EsmExtCapSupportedSpeeds6;             ///< supported speeds
  UINT32                                     EsmExtCapSupportedSpeeds7;             ///< supported speeds
} PCIE_EXTCAP_ESM_CAPABILITIES;

/**
 * @brief CCIX TRANSPORT DVSEC EXTENDED CAPABILITY
 * @details Vendor specific (AMD Only) CCIX Capabilities Structure
 */
typedef union {
  struct {
  UINT32      DvsecVendorId:16;         /***< This MUST be validated to be 0x1022*/
  UINT32      DvsecCapabilityRev:4;     ///< DVSEC structure revision
  UINT32      DvsecCapabilityLen:12;    ///< DVSEC capability structure length
  } Field;                              ///< structure
  UINT32 Value;                         ///< union value
} PCIE_EXTCAP_DVSEC_VENDOR_SPECIFIC_HEADER_1;

typedef union {
  struct {
    UINT16                            ESM_MODE_SUPPORTED:1;
    UINT16                            ESM_PHY_REACH_LENGTH_CAPABILITY:2;
    UINT16                            ESM_RECALIBRATION_NEEDED_ON_ESM_DATA_RATE_UPDATE:1;
    UINT16                            ESM_CALIBRATION_TIME:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            ESM_QUICK_EQUALIZATION_TIMEOUT:3;
    UINT16                            Reserved_15_11:5;
  } Field;
  UINT16 Value;
} PCIE_CCIX_CAP;

typedef union {
  struct {
    UINT32                            ESM_SUPPORT_2P5GT:1;
    UINT32                            ESM_SUPPORT_5GT:1;
    UINT32                            ESM_SUPPORT_8GT:1;
    UINT32                            Reserved_4_3:2;
    UINT32                            ESM_SUPPORT_16GT:1;
    UINT32                            Reserved_8_6:3;
    UINT32                            ESM_SUPPORT_20GT:1;
    UINT32                            Reserved_13_10:4;
    UINT32                            ESM_SUPPORT_25GT:1;
    UINT32                            Reserved_31_15:17;
  } Field;                            ///< structure
  UINT32 Value;                       ///< union value
} PCIE_CCIX_ESM_REQD_CAP;

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;                            ///< structure
  UINT32 Value;                       ///< union value
} PCIE_CCIX_ESM_OPTL_CAP;

typedef union {
  struct {
    UINT32                            ESM_CURRENT_DATA_RATE:7;
    UINT32                            ESM_CALIBRATION_COMPLETE:1;
    UINT32                            Reserved_31_8:24;
  } Field;                            ///< structure
  UINT32 Value;                       ///< value of the union
} PCIE_CCIX_ESM_STATUS;

typedef union {
  struct {
    UINT32                            ESM_DATA_RATE0:7;
    UINT32                            ESM_PERFORM_CALIBRATION:1;
    UINT32                            ESM_DATA_RATE1:7;
    UINT32                            ESM_ENABLE:1;
    UINT32                            ESM_EXTENDED_EQUALIZATION_PHASE2_TIMEOUT:3;
    UINT32                            ESM_COMPLIANCE:1;
    UINT32                            ESM_EXTENDED_EQUALIZATION_PHASE3_TIMEOUT:3;
    UINT32                            Reserved_23_23:1;
    UINT32                            LINK_REACH_TARGET:1;
    UINT32                            RETIMER_PRESENT:1;
    UINT32                            ESM_QUICK_EQUALIZATION_TIMEOUT_SELECT:3;
    UINT32                            Reserved_31_29:3;
  } Field;                            ///< structure
  UINT32 Value;                       ///< value of the union
} PCIE_CCIX_ESM_CNTL;

typedef union {
  struct {
    UINT32                            CCIX_OPTIMIZED_TLP_FORMAT_ENABLE:1;
    UINT32                            CCIX_PCIE_COMPATIBLE_TLP_ENABLE:1;
    UINT32                            Reserved_31_2:30;
  } Field;                            ///< structure
  UINT32 Value;                       ///< value of the union
} PCIE_CCIX_TRANS_CNTL;

typedef union {
  struct {
    UINT32                            CCIX_OPTIMIZED_TLP_FORMAT_SUPPORT:1;
    UINT32                            Reserved_31_1:31;                         ///< reserved for future
  } Field;                            ///< Field structure
  UINT32 Value;                       ///< union value
} PCIE_CCIX_TRANS_CAP;

typedef struct {
  PCIE_EXTENDED_CAPABILITY_HEADER            CcixExtCapHeader;                    ///< Extended capabilitity header
  PCIE_EXTCAP_DVSEC_VENDOR_SPECIFIC_HEADER_1 CcixExtCapVendorSpHeader1;           ///< Ccix Extended capabilitity vendor specific header
  UINT16                                     CcixExtCapVendorSpHeader2;           ///< Ccix Extended capabilitity vendor specific header
  PCIE_CCIX_CAP                              CcixCap;                             ///< Ccix Capabilitity
  PCIE_CCIX_ESM_REQD_CAP                     CcixEsmReqdCap;                      ///< Ccix ESM requirement capability
  PCIE_CCIX_ESM_OPTL_CAP                     CcixEsmOptlCap;                      ///< Optional capabilitity
  PCIE_CCIX_ESM_STATUS                       CcixEsmStatus;                       ///< ESM status
  PCIE_CCIX_ESM_CNTL                         CcixEsmControl;                      ///< ESM control
  UINT32                                     RegisterESMLane3_0_EqCtrl20;         ///< register lanes
  UINT32                                     RegisterESMLane7_4_EqCtrl20;         ///< register lanes
  UINT32                                     RegisterESMLane11_8_EqCtrl20;        ///< register lanes
  UINT32                                     RegisterESMLane15_12_EqCtrl20;       ///< register lanes
  UINT32                                     RegisterESMLane3_0_EqCtrl25;         ///< register lanes
  UINT32                                     RegisterESMLane7_4_EqCtrl25;         ///< register lanes
  UINT32                                     RegisterESMLane11_8_EqCtrl25;        ///< register lanes
  UINT32                                     RegisterESMLane15_12_EqCtrl25;       ///< register lanes
  PCIE_CCIX_TRANS_CAP                        CcixTransCap;                        ///< transfer capability
  PCIE_CCIX_TRANS_CNTL                       CcixTransCntl;                       ///< transfer control
} PCIE_EXTCAP_CCIX_CAPABILITIES;

#define DVSEC_HEADER_1_OFFSET    4    ///< header 1 offset
#define DVSEC_HEADER_2_OFFSET    8    ///< header 2 offset

/**
 * @brief Common configuration values
 * @details 
 */
#define MAX_PAYLOAD_128             0x0     /***< Max allowed payload size 128 bytes*/
#define MAX_PAYLOAD_256             0x1     /***< Max allowed payload size 256 bytes*/
#define MAX_PAYLOAD_512             0x2     /***< Max allowed payload size 512 bytes*/
#define MAX_PAYLOAD_1024            0x3     /***< Max allowed payload size 1024 bytes*/
#define MAX_PAYLOAD_2048            0x4     /***< Max allowed payload size 2048 bytes*/
#define MAX_PAYLOAD_4096            0x5     /***< Max allowed payload size 4096 bytes*/
#define MAX_PAYLOAD                 0x5     /***< Max allowed payload size according to spec is 101b (4096 bytes)*/

#define MAX_READREQUESTSIZE_128     0x0     /***< Max allowed read request size 128 bytes*/
#define MAX_READREQUESTSIZE_256     0x1     /***< Max allowed read request size 256 bytes*/
#define MAX_READREQUESTSIZE_512     0x2     /***< Max allowed read request size 512 bytes*/
#define MAX_READREQUESTSIZE_1024    0x3     /***< Max allowed read request size 1024 bytes*/
#define MAX_READREQUESTSIZE_2048    0x4     /***< Max allowed read request size 2048 bytes*/
#define MAX_READREQUESTSIZE_4096    0x5     /***< Max allowed read request size 4096 bytes*/

#pragma pack (pop)

 /*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 * PROTOTYPES FOR COMMON AMD AGESA PCI LIBRARY FUNCTIONS
 */

BOOLEAN
GnbLibPciIsDevicePresent (
  IN      UINT32              Address,                ///< check device presence
  IN      AMD_CONFIG_PARAMS   *StdHeader              ///< check presence
 );

BOOLEAN
GnbLibPciIsBridgeDevice (
  IN      UINT32              Address,                ///< Check if bridge device
  IN      AMD_CONFIG_PARAMS   *StdHeader              ///< Check if bridge device
 );

BOOLEAN
GnbLibPciIsMultiFunctionDevice (
  IN      UINT32              Address,                ///< Check if multiple device function
  IN      AMD_CONFIG_PARAMS   *StdHeader              ///< Check if multiple device function
 );

BOOLEAN
GnbLibPciIsPcieDevice (
  IN      UINT32              Address,                ///< Check if device is pcie
  IN      AMD_CONFIG_PARAMS   *StdHeader              ///< Check if device is pcie
 );

UINT8
GnbLibFindPciCapability (
  IN      UINT32              Address,                ///< Check to find pci capability
  IN      UINT8               CapabilityId,           ///< Check to find pci capability
  IN      AMD_CONFIG_PARAMS   *StdHeader
  );

UINT16
GnbLibFindPcieExtendedCapability (
  IN      UINT32              Address,                    ///< Check if pcie capability extended
  IN      UINT16              ExtendedCapabilityId,       ///< Check if pcie capability extended
  IN      AMD_CONFIG_PARAMS   *StdHeader                  ///< PCI standard header ptr
  );

VOID
GnbLibPciScan (
  IN      PCI_ADDR            Start,                  ///< PCI scan
  IN      PCI_ADDR            End,                    ///< PCI scan
  IN      GNB_PCI_SCAN_DATA   *ScanData               ///< pci scan data ptr
 );

VOID
GnbLibPciScanSecondaryBus (
  IN       PCI_ADDR             Bridge,                 ///< check sec bus
  IN OUT   GNB_PCI_SCAN_DATA    *ScanData               ///< check scan data
  );

PCIE_DEVICE_TYPE
GnbLibGetPcieDeviceType (
  IN      PCI_ADDR            Device,                   ///< get pcie device type
  IN      AMD_CONFIG_PARAMS   *StdHeader                ///< get pcie device type
  );

VOID
GnbLibS3SaveConfigSpace (
  IN      UINT32              Address,                      ///< save config space
  IN      UINT16              StartRegisterAddress,         ///< save config space
  IN      UINT16              EndRegisterAddress,           ///< save config space
  IN      ACCESS_WIDTH        Width,                        ///< save config space width
  IN      AMD_CONFIG_PARAMS   *StdHeader                    ///< save std header
  );

#endif /* _GNBLIBPCI_H_ */


