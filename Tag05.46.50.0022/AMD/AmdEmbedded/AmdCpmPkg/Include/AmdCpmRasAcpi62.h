/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_RAS_ACPI_6_2_H_
#define _AMD_CPM_RAS_ACPI_6_2_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#pragma pack (push, 1)
/// BERT Generic Processor Error Entry
typedef struct _GENERIC_PROC_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PLATFORM_PROC_ERR_SEC  ProcErrorSection;                                  ///< Platform Processor Error Section
} GENERIC_PROC_ERR_ENTRY_V2;

/// BERT Generic Memory Error Entry
typedef struct _GENERIC_MEM_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PLATFORM_MEM_ERR_SEC  MemErrorSection;                                    ///< Platform Memory Error Section
} GENERIC_MEM_ERR_ENTRY_V2;

/// Generic Nbio Error Entry
typedef struct _GENERIC_NBIO_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_NBIO_ERROR_RECORD  NbioErrorSection;                                  ///< Platform NBIO Error Section
} GENERIC_NBIO_ERR_ENTRY_V2;

/// Generic Smn Error Entry
typedef struct _GENERIC_SMN_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_SMN_ERROR_RECORD  SmnErrorSection;                                    ///< Platform NBIO Error Section
} GENERIC_SMN_ERR_ENTRY_V2;

/// Generic PcieAer Error Entry
typedef struct _GENERIC_PCIE_AER_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PCIE_ERROR_SECTION  PcieAerErrorSection;                                  ///< Platform NBIO Error Section
} GENERIC_PCIE_AER_ERR_ENTRY_V2;

/// Generic FchAlink Error Entry
typedef struct _GENERIC_FCH_ALINK_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_ALINK_ERROR_RECORD  FchALinkErrorSection;                             ///< Platform FCH Alink Error Section
} GENERIC_FCH_ALINK_ERR_ENTRY_V2;

/// Generic USB ECC/ Parity Error Entry
typedef struct _GENERIC_USB_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_USB_ERROR_RECORD  UsbErrorSection;                                    ///< Platform FCH Alink Error Section
} GENERIC_USB_ERR_ENTRY_V2;

/// Generic Sata Error Entry
typedef struct _GENERIC_SATA_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_SATA_ERROR_RECORD  SataErrorSection;                                  ///< Platform Sata Error Section
} GENERIC_SATA_ERR_ENTRY_V2;

/// Generic Ccix (S-Link) Error Entry
typedef struct _GENERIC_SLINK_ERR_ENTRY_V2 {
  EFI_ACPI_6_2_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_SLINK_ERROR_RECORD  SlinkErrorSection;                                ///< Platform Slink Error Section
} GENERIC_SLINK_ERR_ENTRY_V2;

///
/// Platform RAS configuration data structure
///
typedef struct {
  BOOLEAN       PFEHEnable;                        ///< PFEHEnable
  BOOLEAN       FchApuRasSmiSupport;               ///< FchApuRasSmiSupport
  BOOLEAN       FchSataRasSupport;                 ///< FchSataRasSupport
  BOOLEAN       AmdMemPostPackageRepair;           ///< AmdMemPostPackageRepair
  UINT16        SwSmiCmdPortAddr;                  ///< SwSmiCmdPortAddr
  UINT8         MceSwSmiData;                      ///< MceSwSmiData
  UINT8         EinjSwSmiData;                     ///< EinjSwSmiData
  BOOLEAN       McaErrThreshEn;                    ///< McaErrThreshEn
  UINT16        McaErrThreshCount;                 ///< McaErrThreshCount
  UINT32        McaPollInterval;                   ///< McaPollInterval
  UINT32        NbioPollInterval;                  ///< NbioPollInterval
  UINT32        PciePollInterval;                  ///< PciePollInterval
  BOOLEAN       NbioCorrectedErrThreshEn;          ///< NbioCorrectedErrThreshEn
  UINT16        NbioCorrectedErrThreshCount;       ///< NbioCorrectedErrThreshCount
  BOOLEAN       NbioDeferredErrThreshEn;           ///< NbioDeferredErrThreshEn
  UINT16        NbioDeferredErrThreshCount;        ///< NbioDeferredErrThreshCount
  UINT32        RasSmiThreshold;                   ///< RasSmiThreshold
  UINT32        RasSmiScale;                       ///< RasSmiScale
  UINT8         RasThresholdPeriodicSmiEn;         ///< RasThresholdPeriodicSmiCtrl
  UINT32        RasRetryCnt;                       ///< RasRetryCnt;
  UINT8         EinjTrigErrSwSmiId;                ///< EinjTrigErrSwSmiId
  BOOLEAN       GhesNotifyType;                    ///< GhesNotifyType
  BOOLEAN       PcieGhesNotifyType;                ///< PcieGhesNotifyType
  UINT32        PcieRpCorrectedErrorMask;          ///< PcieRpCorrectedErrorMask
  UINT32        PcieRpUnCorrectedErrorMask;        ///< PcieRpUnCorrectedErrorMask
  UINT32        PcieRpUnCorrectedErrorSeverity;    ///< PcieRpUnCorrectedErrorSeverity
  UINT32        PcieDevCorrectedErrorMask;         ///< PcieDevCorrectedErrorMask
  UINT32        PcieDevUnCorrectedErrorMask;       ///< PcieDevUnCorrectedErrorMask
  UINT32        PcieDevUnCorrectedErrorSeverity;   ///< PcieDevUnCorrectedErrorSeverity
  UINT32        PcieBridgeCorrectedErrorMask;      ///< PcieBridgeCorrectedErrorMask
  UINT32        PcieBridgeUnCorrectedErrorMask;    ///< PcieBridgeUnCorrectedErrorMask
  UINT32        PcieBridgeUnCorrectedErrorSeverity;///< PcieBridgeUnCorrectedErrorSeverity
  BOOLEAN       CcixGhesCorrectedNotifyType;       ///< CcixGhesCorrectedNotifyType
  BOOLEAN       CcixGhesDeferredNotifyType;        ///< CcixGhesDeferredNotifyType
  BOOLEAN       HardPprEnable;                     ///< CpmHardPprEnable
  BOOLEAN       FchAlinkRasSupport;                ///< FchAlinkRasSupport;
  UINT32        FchAlinkErrPollInterval;           ///< FchAlinkErrPollInterval;
  UINT8         RasSmiScaleUnit;                   ///< RasSmiScaleUnit;
  UINT32        RasSmiPeriod;                      ///< RasSmiPeriod;
  BOOLEAN       UsbRasSupport;                     ///< UsbRasSupport;
  UINT32        UsbErrPollInterval;                ///< UsbErrPollInterval;
  BOOLEAN       GhesUnCorrNotifyType;              ///< GhesUnCorrNotifyType
  BOOLEAN       PcieUnCorrGhesNotifyType;          ///< PcieUnCorrGhesNotifyType
  UINT8         EinjBeginInjOptSwSmiData;          ///< EinjBeginInjOptSwSmiData
  UINT8         EinjEndOptSwSmiData;               ///< EinjEndOptSwSmiData
  UINT8         PcieAerReportMechanism;            ///< PcieAerReportMechanism
  BOOLEAN       HestDMCStrucEn;                    ///< HestDMCStrucEn
} PLATFORM_RAS_POLICY_V2;

#pragma pack (pop)

typedef union {
  EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC_STRUCTURE           LocalApicEntry;         //EFI_ACPI_6_2_PROCESSOR_LOCAL_APIC           0x00
  EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC_STRUCTURE         Localx2ApicEntry;       //EFI_ACPI_6_2_PROCESSOR_LOCAL_X2APIC         0x09
  struct {
    UINT8 Type;
    UINT8 Length;
  } CommonApicEntry;
} MADT_APIC_ENTRY_PTR;

///
/// Platform APEI Private data structure
///
typedef struct _PLATFORM_APEI_PRIVATE_BUFFER_V2 {
  BOOLEAN                                       Valid;                        ///< Indicate if data valid
  BOOLEAN                                       PlatformHestValid;            ///< Indicate if Platform Hest supported
  BOOLEAN                                       PlatformBertValid;            ///< Indicate if Platform Bert supported
  BOOLEAN                                       PlatformEinjValid;            ///< Indicate if Platform Einj supported
  BOOLEAN                                       PciMapValid;                  ///< Indicate if PCI Map Data valid
  UINT8                                         MaxMcaBankCount;              ///< Max MCA bank count throught all cores;
  UINT64                                        PcieBaseAddress;              ///< Pcie Base Address
  APEI_EINJ_REGISTERS                           *EinjActMemRegAddr;           ///< EINJ Action Instruction Register buffer pointer
  UINT8                                         *TrigErrorAct;                ///< EINJ Action Table pointer
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdNbioErrBlk;               ///< Nbio Generic Correctable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdNbioUnErrBlk;             ///< Nbio Generic Uncorrectable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieAerErrBlk;            ///< PCI-E Root Port AER Generic Correctable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieAerUnErrBlk;          ///< PCI-E Root Port AER Generic Uncorrectable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieDevAerErrBlk;         ///< PCI-E Devices AER Generic Correctable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieDevAerUnErrBlk;       ///< PCI-E Devices AER Generic Uncorrectable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemMceErrBlk;             ///< Memory Generic Error Status Block for MCE
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemCmcErrBlk;             ///< Memory Generic Error Status Block for CMC
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemDmcErrBlk;             ///< Memory Generic Error Status Block for DMC
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcMceErrBlk;            ///< Processor Generic Error Status Block for MCE
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcCmcErrBlk;            ///< Processor Generic Error Status Block for CMC
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcDmcErrBlk;            ///< Processor Generic Error Status Block for DMC
  PCIE_ACTIVE_PORT_MAP                          *AmdPciePortMap;              ///< Active PCI-E port map
  PLATFORM_RAS_POLICY_V2                        PlatRasPolicy;                ///< RAS Platform configuration
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdFchAlinkErrBlk;           ///< FCH A-Link Generic Uncorrectable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdUsbErrBlk;                ///< USB ECC/Parity Generic Uncorrectable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdSataErrBlk;               ///< Sata Generic Error Status Block
  RAS_ACPI_SMM_DATA                             *RasAcpiSmmData;              ///< RAS platform ACPI to SMM data buffer pointer
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieBridgeAerErrBlk;      ///< PCI-E Bridge AER Generic Correctable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieBridgeAerUnErrBlk;    ///< PCI-E Bridge AER Generic Uncorrectable Error Status Block
  OEM_MEMORY_MAP_TABLE                          *OemMemoryMapTable;           ///< Oem platform Memory silkscreen map table
  MCA_LOCALAPICID_MAPPING_TABLE                 *McaLocalApicIdMappingTable;  ///< MCA Local APIC ID Mapping Table
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdSlinkErrBlk;              ///< S-Link Generic Correctable Error Status Block
  EFI_ACPI_6_2_GENERIC_ERROR_STATUS_STRUCTURE   *AmdSlinkUnErrBlk;            ///< S-Link Bridge AER Generic Uncorrectable Error Status Block
} PLATFORM_APEI_PRIVATE_BUFFER_V2;

#endif //_AMD_CPM_RAS_ACPI_6_2_H_

