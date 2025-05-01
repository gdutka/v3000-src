/*****************************************************************************
 *
 * Copyright (C) 2016-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_RAS_RMB_H_
#define _AMD_CPM_RAS_RMB_H_

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
typedef struct _PROC_GENERIC_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PROC_GENERIC_ERR_SEC  ProcGenErrorSection;                                ///< Generic Processor Error Section
} PROC_GENERIC_ERR_ENTRY_GN;

/// BERT Specific Processor Error Entry
typedef struct _GENERIC_PROC_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PLATFORM_PROC_ERR_SEC  ProcErrorSection;                                  ///< Platform Processor Error Section
} GENERIC_PROC_ERR_ENTRY_GN;

/// BERT Generic Memory Error Entry
typedef struct _GENERIC_MEM_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PLATFORM_MEM_ERR_SEC  MemErrorSection;                                    ///< Platform Memory Error Section
} GENERIC_MEM_ERR_ENTRY_GN;

/// Generic Nbio Error Entry
typedef struct _GENERIC_NBIO_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_NBIO_ERROR_RECORD  NbioErrorSection;                                  ///< Platform NBIO Error Section
} GENERIC_NBIO_ERR_ENTRY_GN;

/// Generic Smn Error Entry
typedef struct _GENERIC_SMN_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  AMD_SMN_ERROR_RECORD  SmnErrorSection;                                    ///< Platform NBIO Error Section
} GENERIC_SMN_ERR_ENTRY_GN;

/// Generic PcieAer Error Entry
typedef struct _GENERIC_PCIE_AER_ERR_ENTRY_GN {
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenErrorDataEntry;     ///< Generic Error Data Entry
  PCIE_ERROR_SECTION  PcieAerErrorSection;                                  ///< Platform NBIO Error Section
} GENERIC_PCIE_AER_ERR_ENTRY_GN;

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
  BOOLEAN       CpmGhesAssistEnable;               ///< CpmGhesAssistEnable
  UINT8         OpnFuseConfig;                     ///< OpnFuseConfig
} PLATFORM_RAS_POLICY_RMB;


///
/// Asynchronous NVME Hot Plug - Error Disconnect Recovery (EDR) ACPI to SMM data buffer pointer
///
#define EDR_DSM_OPERATION_REGION_SIGNATURE  SIGNATURE_32 ('E', 'D', 'R', 'D')    ///< 0x44524445
#define EDR_OST_OPERATION_REGION_SIGNATURE  SIGNATURE_32 ('E', 'D', 'R', 'O')    ///< 0x4F524445

typedef struct _EDR_DSM_ACPI_SMM_DATA {
  UINT8   EdrDsmSwSmiCmd;                 // EDR_DSM_SW_SMI_CMD
  UINT8   RpBus;                          // PCIe Root Port Bus Number
  UINT32  RpDevFnc;                       // PCIe Root port Device and Function Number
  UINT8   FunctionIndex;                  // Function Index 0x0C, 0x0D
  UINT8   InputData;                      // Input data for the function
  UINT16  OutputData;                     // Output data
  UINT32  RpAslDevName;                   // Roor Port ASL Device Name
} EDR_DSM_ACPI_SMM_DATA;

typedef struct _EDR_OST_ACPI_SMM_DATA {
  UINT8   EdrOstSwSmiCmd;                 // EDR_OST_SW_SMI_CMD
  UINT8   RpBus;                          // PCIe Root Port Bus Number
  UINT32  RpDevFnc;                       // PCIe Root port Device and Function Number
  UINT32  StatusCode;                     // _OST Input data: Arg1 - Status Code
} EDR_OST_ACPI_SMM_DATA;

//
// PSP EINJ
//
typedef enum _PSP_RAS_EINJ_CMD_ID {
  CMD_ID_GET_CMD_BUF_ADDR_BITS_00_15 = 0,
  CMD_ID_GET_CMD_BUF_ADDR_BITS_16_31,
  CMD_ID_GET_CMD_BUF_ADDR_BITS_32_47,
  CMD_ID_GET_CMD_BUF_ADDR_BITS_48_63,
  CMD_ID_EXECUTE_COMMAND_DIRECT,
  CMD_ID_EXECUTE_COMMAND_BUFFER
} PSP_RAS_EINJ_CMD_ID;

typedef struct _PSP_RAS_EINJ_CMD {
  PSP_RAS_EINJ_CMD_ID  CmdId;
  UINT16               StatOrDta;
  UINT16               AltStat;
} PSP_RAS_EINJ_CMD;

typedef struct _DRAM_ERROR_LOCATION_STRUCTURE {
  UINT8    Socket;
  UINT8    Channel;
  UINT8    Reserved[2];
} DRAM_ERROR_LOCATION_STRUCTURE;

typedef struct _NBIO_ERROR_LOCATION_STRUCTURE {
  UINT8    GroupId;
  UINT8    StructID;
  UINT8    Reserved[2];
} NBIO_ERROR_LOCATION_STRUCTURE;

typedef struct _MPX_ERROR_LOCATION_STRUCTURE {
  UINT8    Socket;
  UINT8    CcdNumber;
  UINT8    SourceIndex;
  UINT8    Reserved;
} MPX_ERROR_LOCATION_STRUCTURE;

typedef struct _GMI_ERROR_LOCATION_STRUCTURE {
  UINT8    ErrorSource;
  UINT8    LinkNumber;
  UINT8    Reserved[2];
} GMI_ERROR_LOCATION_STRUCTURE;

typedef struct _XGMI_ERROR_LOCATION_STRUCTURE {
  UINT8    ErrorSource;
  UINT8    LinkNumber;
  UINT8    Reserved[2];
} XGMI_ERROR_LOCATION_STRUCTURE;

typedef struct _TWIX_ERROR_LOCATION_STRUCTURE {
  UINT8    ErrorSource;
  UINT8    LinkNumber;
  UINT8    Reserved[2];
} TWIX_ERROR_LOCATION_STRUCTURE;

typedef struct _WAFL_ERROR_LOCATION_STRUCTURE {
  UINT8    ErrorSource;
  UINT8    LinkNumber;
  UINT8    Reserved[2];
} WAFL_ERROR_LOCATION_STRUCTURE;

typedef struct _USB_ERROR_LOCATION_STRUCTURE {
  UINT8    Socket;
  UINT8    Controller;
  UINT8    Reserved[2];
} USB_ERROR_LOCATION_STRUCTURE;

typedef struct _SATA_ERROR_LOCATION_STRUCTURE {
  UINT8    ErrorSource;
  UINT8    Socket;
  UINT8    Controller;
  UINT8    Port;
} SATA_ERROR_LOCATION_STRUCTURE;

typedef union {
  DRAM_ERROR_LOCATION_STRUCTURE    DramErrloc;
  NBIO_ERROR_LOCATION_STRUCTURE    NbioErrloc;
  MPX_ERROR_LOCATION_STRUCTURE     MpxErrloc;
  GMI_ERROR_LOCATION_STRUCTURE     GmiErrloc;
  XGMI_ERROR_LOCATION_STRUCTURE    XgmiErrloc;
  TWIX_ERROR_LOCATION_STRUCTURE    TwixErrloc;
  WAFL_ERROR_LOCATION_STRUCTURE    WaflErrloc;
  USB_ERROR_LOCATION_STRUCTURE     UsbErrloc;
  SATA_ERROR_LOCATION_STRUCTURE    SataErrloc;
  UINT8                            Data[4];
} ERROR_LOCATION_STRUCTURE;

typedef struct _OEM_DEFINED_STRUCTURE {
  UINT64                      SupportedErrSrc;
  UINT64                      ErrSrcToInj;
  UINT8                       Severity;
  UINT8                       Reserved[3];
  UINT32                      InjCtrl;
  ERROR_LOCATION_STRUCTURE    Location;
} OEM_DEFINED_STRUCTURE;

//Table 18-411 in the ACPI spec, v6.3
typedef struct _VENDOR_ERROR_TYPE_EXTENSION_STRUCTURE {
  UINT32                   Length;
  UINT32                   Sbgd;
  UINT16                   VendorId;
  UINT16                   DeviceId;
  UINT8                    RevId;
  UINT8                    Reserved[3];
  OEM_DEFINED_STRUCTURE    OemDefStruct;
} VENDOR_ERROR_TYPE_EXTENSION_STRUCTURE;

typedef union _PSP_COMMAND_BUFFER_OFFSET_16 {
  struct {
    UINT32  ChkBsyStatRgn                   : 1;  ///< bit 0    CHECK_BUSY_STATUS
    UINT32  GetCmdStatRgn                   : 8;  ///< bit 1:8  GET_COMMAND_STATUS
    UINT32                                  : 23; ///< bit 9:31 Reserved.
  } Field;
  UINT32    Value;                                ///< Raw data
} PSP_COMMAND_BUFFER_OFFSET_16;

typedef struct _PSP_COMMAND_BUFFER_STRUCTURE {
  UINT32                                        GetErrTypRgn;
  UINT32                                        Reserved01;
  UINT32                                        SetErrTypRgn;
  UINT32                                        Reserved02;
  PSP_COMMAND_BUFFER_OFFSET_16                  PspCmdBuffOffset16;
  UINT32                                        Reserved03[3];
  UINT64                                        GetTrigActTblRgn;
  UINT32                                        TrigErrRgn;
  UINT32                                        EndOpnRgn;
  SET_ERR_TYPE_WITH_ADDR                        SetErrTypWtAddrRgn;
  VENDOR_ERROR_TYPE_EXTENSION_STRUCTURE         VenErrTypExStrucRgn;
  TRIG_ERROR_ACTION                             TrigActTbl;
} PSP_COMMAND_BUFFER_STRUCTURE;

#pragma pack (pop)

typedef union {
  EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_STRUCTURE           LocalApicEntry;         //EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC           0x00
  EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC_STRUCTURE         Localx2ApicEntry;       //EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC         0x09
  struct {
    UINT8 Type;
    UINT8 Length;
  } CommonApicEntry;
} MADT_APIC_ENTRY_PTR_RMB;

#define MSR_MCA_GPR_PR_CNTRL (0xC0010125ul)  // McaGprPrCntrl
#define MSR_MCA_GPR_PR_DATA  (0xC0010126ul)  // McaGprPrData

// McaGprPrCntrl                             // Value    Name     Description
#define AMD_GPR_THRD         (0x00)          // 00h      GPR0     {61'b0,2'b{thread_id},1'b{Valid}}
#define AMD_GPR_RAX          (0x01)          // 01h      GPR1     RAX
#define AMD_GPR_RBX          (0x02)          // 02h      GPR2     RBX
#define AMD_GPR_RCX          (0x03)          // 03h      GPR3     RCX
#define AMD_GPR_RDX          (0x04)          // 04h      GPR4     RDX
#define AMD_GPR_RSP          (0x05)          // 05h      GPR5     RSP
#define AMD_GPR_RBP          (0x06)          // 06h      GPR6     RBP
#define AMD_GPR_RSI          (0x07)          // 07h      GPR7     RSI
#define AMD_GPR_RDI          (0x08)          // 08h      GPR8     RDI
#define AMD_GPR_R8           (0x09)          // 09h      GPR9     R8
#define AMD_GPR_R9           (0x0A)          // 0Ah      GPR10    R9
#define AMD_GPR_R10          (0x0B)          // 0Bh      GPR11    R10
#define AMD_GPR_R11          (0x0C)          // 0Ch      GPR12    R11
#define AMD_GPR_R12          (0x0D)          // 0Dh      GPR13    R12
#define AMD_GPR_R13          (0x0E)          // 0Eh      GPR14    R13
#define AMD_GPR_R14          (0x0F)          // 0Fh      GPR15    R14
#define AMD_GPR_R15          (0x10)          // 10h      GPR16    R15
#define AMD_GPR_RFLAGS       (0x11)          // 11h      GPR17    RFLAGS

/// Generic Error Data Entry [offset 0x44] - FRU Text
#define FRU_TEXT_MAX_LENGTH  (20)

///
/// Platform APEI Private data structure
///
typedef struct _PLATFORM_APEI_PRIVATE_BUFFER_RMB {
  BOOLEAN                                       Valid;                        ///< Indicate if data valid
  BOOLEAN                                       PlatformHestValid;            ///< Indicate if Platform Hest supported
  BOOLEAN                                       PlatformBertValid;            ///< Indicate if Platform Bert supported
  BOOLEAN                                       PlatformEinjValid;            ///< Indicate if Platform Einj supported
  BOOLEAN                                       PciMapValid;                  ///< Indicate if PCI Map Data valid
  UINT8                                         MaxMcaBankCount;              ///< Max MCA bank count throught all cores;
  UINT64                                        PcieBaseAddress;              ///< Pcie Base Address
  APEI_EINJ_REGISTERS                           *EinjActMemRegAddr;           ///< EINJ Action Instruction Register buffer pointer
  UINT8                                         *TrigErrorAct;                ///< EINJ Action Table pointer
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdNbioErrBlk;               ///< Nbio Generic Correctable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdNbioUnErrBlk;             ///< Nbio Generic Uncorrectable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieAerErrBlk;            ///< PCI-E Root Port AER Generic Correctable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieAerUnErrBlk;          ///< PCI-E Root Port AER Generic Uncorrectable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieDevAerErrBlk;         ///< PCI-E Devices AER Generic Correctable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieDevAerUnErrBlk;       ///< PCI-E Devices AER Generic Uncorrectable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemMceErrBlk;             ///< Memory Generic Error Status Block for MCE
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemCmcErrBlk;             ///< Memory Generic Error Status Block for CMC
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdMemDmcErrBlk;             ///< Memory Generic Error Status Block for DMC
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcMceErrBlk;            ///< Processor Generic Error Status Block for MCE
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcCmcErrBlk;            ///< Processor Generic Error Status Block for CMC
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdProcDmcErrBlk;            ///< Processor Generic Error Status Block for DMC
  PCIE_ACTIVE_PORT_MAP                          *AmdPciePortMap;              ///< Active PCI-E port map
  PLATFORM_RAS_POLICY_RMB                        PlatRasPolicy;                ///< RAS Platform configuration
  RAS_ACPI_SMM_DATA                             *RasAcpiSmmData;              ///< RAS platform ACPI to SMM data buffer pointer
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieBridgeAerErrBlk;      ///< PCI-E Bridge AER Generic Correctable Error Status Block
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE   *AmdPcieBridgeAerUnErrBlk;    ///< PCI-E Bridge AER Generic Uncorrectable Error Status Block
  OEM_MEMORY_MAP_TABLE                          *OemMemoryMapTable;           ///< Oem platform Memory silkscreen map table
  MCA_LOCALAPICID_MAPPING_TABLE                 *McaLocalApicIdMappingTable;  ///< MCA Local APIC ID Mapping Table
  EDR_DSM_ACPI_SMM_DATA                         *EdrDsmAcpiSmmData;           ///< Asynchronous NVME Hot Plug - Error Disconnect Recovery (EDR) _DSM ACPI to SMM data buffer pointer
  EDR_OST_ACPI_SMM_DATA                         *EdrOstAcpiSmmData;           ///< Asynchronous NVME Hot Plug - Error Disconnect Recovery (EDR) _OST ACPI to SMM data buffer pointer
  UINT64                                        FchAcpiMmioBase[8];           ///< FchAcpiMmioBase[0] for Master FCH, FchAcpiMmioBase[1] for Slave FCH. (Align with FCH_MULITI_FCH_DATA_BLOCK)
} PLATFORM_APEI_PRIVATE_BUFFER_RMB;

#endif //_AMD_CPM_RAS_RMB_H_

