/*****************************************************************************
 *
 * Copyright (C) 2018-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "PiDxe.h"
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/MpService.h>
#include <Protocol/AmdRasApeiRmbProtocol.h>
#include <Protocol/AmdCoreTopologyProtocol.h>
#include <Protocol/AmdNbioPcieAerProtocol.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/RasOemDimmMapProtocol.h>
#include <Protocol/AmdErrorLogServiceProtocol.h>
#include "AmdApeiErst.h"
#include "AmdApeiEinj.h"
#include "AmdApeiHest.h"
#include "AmdApeiBert.h"
#include "AmdPlatformRasRmbDxe.h"
#include "AmdCpmRasInfoHob.h"
#include <Library/UefiLib.h>
#include <Protocol/AmdErrorLogServiceProtocol.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define EFI_ACPI_TABLE_VERSION_X        (EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0 | EFI_ACPI_TABLE_VERSION_4_0 | EFI_ACPI_TABLE_VERSION_5_0)
#define ABL_CCD_BIST_FAILURE            0x4065

#undef MAX_SEC_PROC_CONTEXT_INFO_ARRY_NUM
#define MAX_SEC_PROC_CONTEXT_INFO_ARRY_NUM ((RMB_MAX_CCM_PER_DIE * 2) * 2)  //(RMB_MAX_CCM_PER_DIE * 2 Elements) * 2 type of errors

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
extern EFI_GUID gEfiPciIoProtocolGuid;

PLATFORM_APEI_PRIVATE_BUFFER_RMB  *mPlatformApeiPrivate;
EFI_ACPI_TABLE_PROTOCOL           *AcpiTableProtocol;
AMD_RAS_APEIRMB_PROTOCOL          *AmdRasApeiRmbProtocol;

// RAS_PCI_IO_PROT_EARLY_PROTOCOL RasPciIoProtEarlyProtocol = {
//   BldPcieMapAndBootErrDet
// };

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
PlatformConfigInit ( VOID );

EFI_STATUS
ParsingRasHobInfo ( VOID );

EFI_STATUS
CheckMemTestErr ( VOID );

EFI_STATUS
McaErrorDetection ( VOID );

EFI_STATUS
NbioErrorDetection ( VOID );

EFI_STATUS
SmnErrorDetection ( VOID );

EFI_STATUS
PciePortDetect ( VOID );

EFI_STATUS
PcieAerErrorConfig ( VOID );

EFI_STATUS
PcieLegacyRasErrorDection ( VOID );

EFI_STATUS
AmdErrorLogDetection ( VOID );

EFI_STATUS
NbioErrorScan (
  RAS_NBIO_ERROR_INFO   *RasNbioErrorInfo
  );

EFI_STATUS
NbifErrorScan (
  UINT8     Nbio,
  UINT8     NbioBusNum
  );

EFI_STATUS
PcieErrorScanDxe (
  UINT8     NbioBusNum
  );

EFI_STATUS
McaStatusClear (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex
  );

VOID
RasMidBootInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
);

VOID
RasLateBootInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
);

EFI_STATUS
NbifSysHubWDLogErr(
  IN UINT8   DieBusNum
  );

EFI_STATUS
NbioEgressPoisonErrLog (
  IN UINT8    Nbio,
  IN UINT8    NbioBusNum,
  IN UINT32   NbioEgressPoisonStatusLo,
  IN UINT32   NbioEgressPoisonStatusHi
  );

EFI_STATUS
NbioEgressPoisonErrorScan (
  IN UINT8  Nbio,
  IN UINT8  NbioBusNum
  );

EFI_STATUS
UpdateCpmRasSsdt (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER *Table
  );

EFI_STATUS
RasMiscAslInstall ( VOID );

EFI_STATUS
CreateMcaLookupTable ( VOID );

EFI_STATUS
PcieStsClr (
  IN       PCIE_PORT_PROFILE  *PciePortProfileInstance,
  IN       UINT32             RetryCnt
);

EFI_STATUS
UpdateDsdt ( VOID );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  EFI_MP_SERVICES_PROTOCOL      *MpServices;
  UINT8                         *MpMcaBankCntArrPtr;
} MP_MCA_BANK_INFO;

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern  EFI_BOOT_SERVICES       *gBS;

/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gGenProcErrSrc = {
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR,                            // Type
  PROC_MCE_SOURCE_ID,                                             // SourceId
  0xffff,                                                         // RelatedSourceId
  0x00,                                                           // Flags
  0x01,                                                           // Enabled
  0x00000001,                                                     // NumberofRecordsToPreAllocate
  0x00000010,                                                     // MaxSectionsPerRecord
  MAX_ERROR_BLOCK_SIZE,                                           // MaxRawDataLength
  {0x00, 0x40, 0x00, 0x04, 0x00},                                 // ErrorStatusAddress. Address will be filled in runtime
  {
    HARDWARE_ERROR_NOTIFICATION_POLLED,                           // UINT8  NotifiyType = Polled
    sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),  // UINT8  ErrNotifyLen
    {0,0,0,0,0,0,0},                                              // UINT16 ConfigWrite
    5000,                                                         // UINT32 PollInterval = 5000 ms
    0,                                                            // UINT32 Interrupt Vector
    0,                                                            // UINT32 SwitchPollingThreshVal
    0,                                                            // UINT32 SwitchPollingThreshWindow
    0,                                                            // UINT32 ErrThresholdVal;
    0                                                             // UINT32 ErrThresholdWindow
  },
  MAX_ERROR_BLOCK_SIZE                                            // ErrorStatusSize
};

// Generic Processor Error entries - Processor Generic Error Section
PROC_GENERIC_ERR_ENTRY_GN gProcGenErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UEFI 2.6 Section N 2.4.1 Processor Generic Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (PROC_GENERIC_ERR_SEC),                                // UINT32 Generic Error Data Block Length, fixed length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'P','r','o','c','e','s','s','o','r','E','r','r','o','r'}     // UINT8[20] FRU Text[20] - UEFI 2.6 Section N
  },
};

// Generic Processor Error entries - Processor Specific Error Section
GENERIC_PROC_ERR_ENTRY_GN gGenProcErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UEFI 2.6 Section N 2.4.2 Processor Specific Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    0,                                                            // UINT32 Generic Error Data Block Length, update in runtime
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'P','r','o','c','e','s','s','o','r','E','r','r','o','r'}     // UINT8[20] FRU Text[20] - UEFI 2.6 Section N
  },
  {
    {0},                                                          // UEFI 2.6 Section N Table 251 -Processor Specific Error Type Valid bit
    0,                                                            // Processor APIC ID
    0                                                             // CPUID Information
  }
};

EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gGenMemErrSrc = {
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR,                            // Type
  MEM_MCE_SOURCE_ID,                                              // SourceId
  0xffff,                                                         // RelatedSourceId
  0x00,                                                           // Flags
  0x01,                                                           // Enabled
  0x00000001,                                                     // NumberofRecordsToPreAllocate
  0x00000010,                                                     // MaxSectionsPerRecord
  MAX_ERROR_BLOCK_SIZE,                                           // MaxRawDataLength
  {0x00, 0x40, 0x00, 0x04, 0x00},                                 // ErrorStatusAddress. Address will be filled in runtime
  {
    HARDWARE_ERROR_NOTIFICATION_POLLED,                           // UINT8  NotifiyType = Polled
    sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),  // UINT8  ErrNotifyLen
    {0,0,0,0,0,0,0},                                              // UINT16 ConfigWrite
    5000,                                                         // UINT32 PollInterval = 5000 ms
    0,                                                            // UINT32 Interrupt Vector
    0,                                                            // UINT32 SwitchPollingThreshVal
    0,                                                            // UINT32 SwitchPollingThreshWindow
    0,                                                            // UINT32 ErrThresholdVal;
    0                                                             // UINT32 ErrThresholdWindow
  },
  MAX_ERROR_BLOCK_SIZE                                            // ErrorStatusSize
};

// Generic Memory Error entries
GENERIC_MEM_ERR_ENTRY_GN gGenMemErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UEFI 2.6 Section N 2.5 Memory Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (PLATFORM_MEM_ERR_SEC),                                // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'U','n','c','o','r','r','e','c','t','e','d','E','r','r'}     // UINT8[20] FRU Text[20] - UEFI 2.6 Section N
  },
  {
    {0},                                                          // 0x4009 UEFI 2.6 Section N Table 245 Bit[14]-Memory Error Type Valid, Bit[3]-Node Valid, Bit[0]-ErrSts Valid (0xC34D)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                  // Card,Node, Module, Bank, etc.
    UNKNOWN                                                       // Memory Error Type
  }
};

// Generic Memory CRC/Parity Error entries
GENERIC_MEM_ERR_ENTRY_GN gGenMemParityErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UEFI 2.6 Section N 2.5 Memory Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (PLATFORM_MEM_ERR_SEC),                                // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'C','R','C','/','P','a','r','i','t','y','E','r','r'}         // UINT8[20] FRU Text[20] - UEFI 2.6 Section N
  },
  {
    {0},                                                          // 0x4009 UEFI 2.6 Section N Table 245 Bit[14]-Memory Error Type Valid, Bit[3]-Node Valid, Bit[0]-ErrSts Valid (0xC34D)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                  // Card,Node, Module, Bank, etc.
    PARITY_ERROR                                                  // Memory Error Type
  }
};

// Generic Memory Tester Error entries
GENERIC_MEM_ERR_ENTRY_GN gGenMemTestErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UEFI 2.6 Section N 2.5 Memory Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (PLATFORM_MEM_ERR_SEC),                                // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {0}                                                           // UINT8[20] FRU Text[20] - UEFI 2.6 Section N, this field will update in runtime.
  },
  {
    {0},                                                          // 0x4009 UEFI 2.6 Section N Table 245 Bit[14]-Memory Error Type Valid, Bit[3]-Node Valid, Bit[0]-ErrSts Valid (0xC34D)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                  // Card,Node, Module, Bank, etc.
    PHYSICAL_MEM_MAPOUT                                           // Memory test error type
  }
};

GENERIC_NBIO_ERR_ENTRY_GN gGenNbioErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // AMD NBIO Error Section Type 16-byte GUID, fill the GUID in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (AMD_NBIO_ERROR_RECORD),                               // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'N','b','i','o','E','r','r','o','r'}                         // UINT8 [20] FRU Text[20] - UEFI 2.6 Section N
  },
  {
    {0},
    0,
    {0},
    {0},
    0,
    0
  }
};

GENERIC_SMN_ERR_ENTRY_GN gGenSmnErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // AMD SMN Error Section Type 16-byte GUID
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (AMD_SMN_ERROR_RECORD),                                // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'S','m','n','E','r','r','o','r'}                             // UINT8 [20] FRU Text[20] - UEFI 2.6 Section N
  },
  {
    {0},
    0,
    0,
    {0},
    0
  }
};

EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gNbioErrSrc = {
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR,                            // Type
  NBIO_SOURCE_ID,                                                 // SourceId
  0xffff,                                                         // RelatedSourceId
  0x00,                                                           // Flags
  0x01,                                                           // Enabled
  0x00000001,                                                     // NumberofRecordsToPreAllocate
  0x00000010,                                                     // MaxSectionsPerRecord
  MAX_ERROR_BLOCK_SIZE,                                           // MaxRawDataLength
  {0x00, 0x40, 0x00, 0x04, 0x00},                                 // ErrorStatusAddress. Address will be filled in runtime
  {
    HARDWARE_ERROR_NOTIFICATION_POLLED,                           // UINT8  NotifiyType = Polled
    sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),  // UINT8  ErrNotifyLen
    {0,0,0,0,0,0,0},                                              // UINT16 ConfigWrite
    5000,                                                         // UINT32 PollInterval = 5000 ms
    0,                                                            // UINT32 Interrupt Vector
    0,                                                            // UINT32 SwitchPollingThreshVal
    0,                                                            // UINT32 SwitchPollingThreshWindow
    0,                                                            // UINT32 ErrThresholdVal;
    0                                                             // UINT32 ErrThresholdWindow
  },
  MAX_ERROR_BLOCK_SIZE                                            // ErrorStatusSize
};

GENERIC_PCIE_AER_ERR_ENTRY_GN gGenPcieErrEntry = {
  {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // AMD PCIE Error Section Type 16-byte GUID, update in runtime.
    ERROR_SEVERITY_FATAL,                                         // UINT32 ACPI 6.2 Table 18-380 Generic Error Data Entry
    GENERIC_ERROR_REVISION,                                       // UINT16 UEFI 2.6 Section N Revision Field
    FRU_STRING_VALID,                                             // UINT8 UEFI 2.6 Section N Section Descriptor Validation Bits field
    0x1,                                                          // UINT8 UEFI 2.6 Section N Section Descriptor Flags
    sizeof (PCIE_ERROR_SECTION),                                  // UINT32 Generic Error Data Block Length
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},                            // UINT8[16] FRU ID - UEFI 2.6 Section N
    {'P','c','i','e','E','r','r','o','r'}                         // UINT8 [20] FRU Text[20] - UEFI 2.6 Section N
  }
};

EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gPcieGenErrSrc = {
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR,                            // Type
  0,                                                              // PCIE GHES SourceId, update in runtime
  0xffff,                                                         // RelatedSourceId, update in runtime
  0x00,                                                           // Flags
  0x01,                                                           // Enabled
  0x00000001,                                                     // NumberofRecordsToPreAllocate
  0x00000010,                                                     // MaxSectionsPerRecord
  MAX_ERROR_BLOCK_SIZE,                                           // MaxRawDataLength
  {0x00, 0x40, 0x00, 0x04, 0x00},                                 // ErrorStatusAddress. Address will be filled in runtime
  {
    HARDWARE_ERROR_NOTIFICATION_POLLED,                           // UINT8  NotifiyType = Polled, update in runtime
    sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),  // UINT8  ErrNotifyLen
    {0,0,0,0,0,0,0},                                              // UINT16 ConfigWrite
    5000,                                                         // UINT32 PollInterval = 5000 ms, update in runtime
    0,                                                            // UINT32 Interrupt Vector
    0,                                                            // UINT32 SwitchPollingThreshVal
    0,                                                            // UINT32 SwitchPollingThreshWindow
    0,                                                            // UINT32 ErrThresholdVal;
    0                                                             // UINT32 ErrThresholdWindow
  },
  MAX_ERROR_BLOCK_SIZE                                            // ErrorStatusSize
};

//A PCI-E port device/function loopup table.
PCIE_PORT_LIST gPciePortList[] = {
    {1, 1},         //PCI-E 0 Port A
    {1, 2},         //PCI-E 0 Port B
    {1, 3},         //PCI-E 0 Port C
    {1, 4},         //PCI-E 0 Port D
    {1, 5},         //PCI-E 0 Port E
    {2, 1},         //PCI-E 1 Port A
    {2, 2},         //PCI-E 1 Port B
    {2, 3},         //PCI-E 1 Port C
    {2, 4},         //PCI-E 1 Port D
    {2, 5},         //PCI-E 1 Port E
    {2, 6},         //PCI-E 1 Port F
};

EFI_GUID  PLATFORM_RAS_RMB_ASL_GUID = {
  0x36382983, 0xE892, 0x4AFB, 0x8C, 0x80, 0x6B, 0x8B, 0x78, 0x10, 0x8A, 0x7E
};

/*---------------------------------------------------------------------------------------*/
/*
 * Calculate an ACPI style checksum
 *
 * Computes the checksum and stores the value to the checksum
 * field of the passed in ACPI table's header.
 *
 * @param[in]  Table             ACPI table to checksum
 *
 */
VOID
ChecksumAcpiTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER *Table
  )
{
  UINT8  *BuffTempPtr;
  UINT8  Checksum;
  UINT32 BufferOffset;

  Table->Checksum = 0;
  Checksum = 0;
  BuffTempPtr = (UINT8 *) Table;
  for (BufferOffset = 0; BufferOffset < Table->Length; BufferOffset++) {
    Checksum = Checksum - *(BuffTempPtr + BufferOffset);
  }

  Table->Checksum = Checksum;
}

VOID
GetMpMcaBankCount (
  MP_MCA_BANK_INFO  *MpMcaBankInfoPtr
  )
{
  EFI_STATUS                      Status;
  EFI_MP_SERVICES_PROTOCOL        *MpServices;
  UINT8                           *BankCountArrPtr;
  UINTN                           ProcNum;

  MpServices = MpMcaBankInfoPtr->MpServices;
  BankCountArrPtr = MpMcaBankInfoPtr->MpMcaBankCntArrPtr;

  Status = MpServices->WhoAmI (MpServices, &ProcNum);
  if (!EFI_ERROR (Status)) {
    GetMcaBankCount(BankCountArrPtr+ProcNum);
  }

  return;
}

EFI_STATUS
SearchMaxMcaBankCnt (
  OUT      UINT8* MaxBankCount
)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  UINT8                         BankCountArr[256];  //256: RAS_MAX_CORES
  UINTN                         ProcessorNumber;
  UINTN                         NumberOfLogicProcessors;
  UINTN                         NumberOfEnabledProcessors;
  MP_MCA_BANK_INFO              MpMcaBankInfo;

  DEBUG((EFI_D_ERROR, "[RAS] SearchMaxMcaBankCnt - Entry \n"));

  *MaxBankCount = 0;

  // Get EFI MP service
  if (MpServices == NULL) {
    Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;    // Error detected while trying to locate MP Service protocol
    }
  }

  MpMcaBankInfo.MpServices = MpServices;
  MpMcaBankInfo.MpMcaBankCntArrPtr = BankCountArr;

  MpServices->StartupAllAPs (
    MpServices,
    GetMpMcaBankCount,
    FALSE,
    NULL,
    0,
    (VOID *) &MpMcaBankInfo,
    NULL
  );

  //BSP
  GetMpMcaBankCount (&MpMcaBankInfo);

  MpServices->GetNumberOfProcessors (MpServices, &NumberOfLogicProcessors, &NumberOfEnabledProcessors);
  for (ProcessorNumber = 0; ProcessorNumber < NumberOfLogicProcessors; ProcessorNumber++) {
    if (*MaxBankCount < BankCountArr[ProcessorNumber]) {
      *MaxBankCount = BankCountArr[ProcessorNumber];
    }

    DEBUG((EFI_D_ERROR, "[%04d]: %02d ", ProcessorNumber, BankCountArr[ProcessorNumber]));
    if (((ProcessorNumber+1) % 8) == 0) {
      DEBUG((EFI_D_ERROR, "\n"));
    }
  }

  DEBUG((EFI_D_ERROR, "MaxBankCount: %d \n", *MaxBankCount));
  DEBUG((EFI_D_ERROR, "[RAS] SearchMaxMcaBankCnt - Exit \n"));

  return Status;
}

EFI_STATUS
GetOemMemoryMapTable(
     OUT OEM_MEMORY_MAP_TABLE **OemMemoryMapTable
)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  DXE_RAS_OEM_DIMMMAP_PROTOCOL *mRasOemDimmMapProtocol;
  OEM_MEMORY_MAP_TABLE         *OemDimmMapTbl = NULL;
  UINT32                       OemMemMapTotalSize;

  if (OemMemoryMapTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gRasOemDimmMapProtocolGuid, NULL, &mRasOemDimmMapProtocol);
  if (EFI_ERROR (Status)) {
    *OemMemoryMapTable = NULL;
    return EFI_UNSUPPORTED;
  }

  //Local reserved memory space and copy the map.

  OemMemMapTotalSize = sizeof (OEM_MEMORY_MAP_TABLE) + mRasOemDimmMapProtocol->RasOemDimmMap->TableSize;
  //
  //  Allocate memory for the map
  //
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  OemMemMapTotalSize,
                  (VOID **)&OemDimmMapTbl
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (OemDimmMapTbl, OemMemMapTotalSize);

  gBS->CopyMem (OemDimmMapTbl, mRasOemDimmMapProtocol->RasOemDimmMap , sizeof (OEM_MEMORY_MAP_TABLE));

  OemDimmMapTbl->MemoryMapTableEntry = (OEM_MEMORY_MAP_TABLE_ENTRY*)(OemDimmMapTbl + 1);

  gBS->CopyMem (OemDimmMapTbl->MemoryMapTableEntry, mRasOemDimmMapProtocol->RasOemDimmMap->MemoryMapTableEntry , mRasOemDimmMapProtocol->RasOemDimmMap->TableSize);

  *OemMemoryMapTable = OemDimmMapTbl;

  return EFI_SUCCESS;
}

VOID
RasPciEnumerationCompleteCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
  EFI_STATUS           Status;
  VOID                 *Interface;
  UINT16               PciePortIndex;
  PCIE_PORT_PROFILE    *PciePortProfileInstance;

  DEBUG ((DEBUG_INFO, "[RAS] RasPciEnumerationCompleteCallBack Entry\n"));
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[RAS]   Locate gEfiPciEnumerationCompleteProtocolGuid failed\n"));
    return ;
  }

  PciePortDetect ();
  McaErrorDetection ();
  PcieLegacyRasErrorDection();
  AmdErrorLogDetection ();

  //PCIe Status Clear
  PciePortProfileInstance = mPlatformApeiPrivate->AmdPciePortMap->PciPortNumber;
  for (PciePortIndex = 0; PciePortIndex < mPlatformApeiPrivate->AmdPciePortMap->PortCount; PciePortIndex++, PciePortProfileInstance++) {
    PcieStsClr (PciePortProfileInstance, mPlatformApeiPrivate->PlatRasPolicy.RasRetryCnt);
  }

  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "[RAS] RasPciEnumerationCompleteCallBack Exit\n"));

  return;
}

/*********************************************************************************
 * Name: AmdPlatformRasRmbDxeInit
 *
 * Description
 *   Entry point of the AMD Platform RMB RAS DXE driver
 *   Perform the configuration init, resource reservation, Boot Error detection
 *   and install APEI data protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
AmdPlatformRasRmbDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_HANDLE    Handle = NULL;
  EFI_EVENT     ReadyToBootEvent;
  EFI_EVENT     RasPciEnumerationCompleteEvent;
  EFI_EVENT     PciIoEvent;
  VOID          *Registration;

  DEBUG((EFI_D_ERROR, "[RAS] RMB CPM RAS DXE driver entry\n"));

  // Locate ACPI Support table.
  Status = gBS->LocateProtocol (
                       &gEfiAcpiTableProtocolGuid,
                       NULL,
                       &AcpiTableProtocol
                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  // Locate Ras APEI protocol
  Status = gBS->LocateProtocol (
                       &gAmdRasApeiRmbProtocolGuid,
                       NULL,
                       &AmdRasApeiRmbProtocol
                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  PlatformConfigInit ();

  RasMiscAslInstall ();

  mPlatformApeiPrivate->Valid = TRUE;

  // Initialize BERT
  ApeiBertInstall ();
  // Initialize HEST
  ApeiHestInstall ();
  // Initialize ERST
  ApeiErstInstall ();

  NbioHestUpdate ();

  PcieAerHestUpdate ();

  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdPlatformApeiDataProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  mPlatformApeiPrivate
                  );
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  //
  // Register the event handling function for AmdPcieMiscInit
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             RasMidBootInit,
             NULL,
             NULL,
             &PciIoEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciIoProtocolGuid,
              PciIoEvent,
              &Registration
              );

  //
  // Register the event handling function
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  RasLateBootInit,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  RasPciEnumerationCompleteEvent = NULL;
  RasPciEnumerationCompleteEvent = EfiCreateProtocolNotifyEvent (
                                     &gEfiPciEnumerationCompleteProtocolGuid,
                                     TPL_NOTIFY,
                                     RasPciEnumerationCompleteCallBack,
                                     NULL,
                                     &Registration
                                     );
  if (RasPciEnumerationCompleteEvent == NULL) {
    DEBUG ((DEBUG_INFO, "[RAS] Register RasPciEnumerationCompleteCallBack failed\n"));
  }

  return (Status);
}

// EFI_STATUS
// RasLateBootInit2(
//   VOID
// )
// {
//   DEBUG ((DEBUG_INFO, "[RAS] Ras Late Init 2 Entry\n"));
//   // if (mPlatformApeiPrivate->PlatRasPolicy.CpmGhesAssistEnable) {
//   //   CreateMcaLookupTable();
//   //   SetupGhesAssist();
//   // }
//   DEBUG ((DEBUG_INFO, "[RAS] Ras Late Init 2 Exit\n"));

//   return EFI_SUCCESS;
// }

VOID
RasMidBootInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
  DEBUG ((DEBUG_INFO, "[RAS]Ras Mid Init - After PCI Scan\n"));

  ParsingRasHobInfo ();
  CheckMemTestErr ();
//  SmnErrorDetection ();
  NbioErrorDetection ();

  // Initialize EINJ
  //ApeiEinjInstall ();

  gBS->CloseEvent (Event);
}

VOID
RasLateBootInit(
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
  DEBUG ((DEBUG_INFO, "[RAS]Ras Late Init - After Boot Exit\n"));
  //PCI-E AER platform-first enablement
  PcieAerErrorConfig ();
  // UpdateDsdt();

  gBS->CloseEvent (Event);
}

EFI_STATUS
PlatformConfigInit ( VOID )
{
  EFI_STATUS                    Status = EFI_SUCCESS;

  //
  //  Allocate memory and Initialize for Data block
  //
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  sizeof (PLATFORM_APEI_PRIVATE_BUFFER_RMB),
                  (VOID **)&mPlatformApeiPrivate
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (mPlatformApeiPrivate, sizeof (PLATFORM_APEI_PRIVATE_BUFFER_RMB));

  mPlatformApeiPrivate->Valid = FALSE;
  mPlatformApeiPrivate->PlatformHestValid = FALSE;
  mPlatformApeiPrivate->PlatformBertValid = FALSE;
  mPlatformApeiPrivate->PlatformEinjValid = FALSE;

  mPlatformApeiPrivate->PlatRasPolicy.PFEHEnable = PcdGetBool (PcdAmdCcxCfgPFEHEnable);
  mPlatformApeiPrivate->PlatRasPolicy.FchApuRasSmiSupport = PcdGetBool (PcdAmdFchApuRasSmiSupport);
  mPlatformApeiPrivate->PlatRasPolicy.AmdMemPostPackageRepair = PcdGetBool (PcdAmdMemPostPackageRepair);

  mPlatformApeiPrivate->PlatRasPolicy.MceSwSmiData = PcdGet8 (PcdMceSwSmiData);
  mPlatformApeiPrivate->PlatRasPolicy.EinjSwSmiData = PcdGet8 (PcdCpmEinjSwSmiData);
  mPlatformApeiPrivate->PlatRasPolicy.McaErrThreshEn = PcdGetBool (PcdMcaErrThreshEn);
  mPlatformApeiPrivate->PlatRasPolicy.McaErrThreshCount = PcdGet16 (PcdMcaErrThreshCount);
  mPlatformApeiPrivate->PlatRasPolicy.McaPollInterval = PcdGet32 (PcdCpmMcaPollInterval);
  mPlatformApeiPrivate->PlatRasPolicy.NbioPollInterval = PcdGet32 (PcdCpmNbioPollInterval);
  mPlatformApeiPrivate->PlatRasPolicy.PciePollInterval = PcdGet32 (PcdCpmPciePollInterval);
  mPlatformApeiPrivate->PlatRasPolicy.NbioCorrectedErrThreshEn = PcdGetBool (PcdNbioCorrectedErrThreshEn);
  mPlatformApeiPrivate->PlatRasPolicy.NbioCorrectedErrThreshCount = PcdGet16 (PcdNbioCorrectedErrThreshCount);
  mPlatformApeiPrivate->PlatRasPolicy.NbioDeferredErrThreshEn = PcdGetBool (PcdNbioDeferredErrThreshEn);
  mPlatformApeiPrivate->PlatRasPolicy.NbioDeferredErrThreshCount = PcdGet16 (PcdNbioDeferredErrThreshCount);
  mPlatformApeiPrivate->PlatRasPolicy.RasSmiThreshold = PcdGet32 (PcdCpmSmiThreshold);
  mPlatformApeiPrivate->PlatRasPolicy.RasSmiScale = PcdGet32 (PcdCpmSmiScale);
  mPlatformApeiPrivate->PlatRasPolicy.RasThresholdPeriodicSmiEn = PcdGetBool (PcdCpmThresholdPeriodicSmiEn);
  mPlatformApeiPrivate->PlatRasPolicy.RasRetryCnt = PcdGet32 (PcdCpmRasRetryCount);
  mPlatformApeiPrivate->PlatRasPolicy.EinjTrigErrSwSmiId = PcdGet8 (PcdCpmEinjTrigErrSwSmiId);

  mPlatformApeiPrivate->PlatRasPolicy.GhesNotifyType = PcdGetBool (PcdCpmGhesNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.PcieGhesNotifyType = PcdGetBool (PcdCpmPcieGhesNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.PcieRpCorrectedErrorMask = PcdGet32 (PcdCpmPcieRpCorrectedErrorMask);
  mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorMask = PcdGet32 (PcdCpmPcieRpUnCorrectedErrorMask);
  mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorSeverity = PcdGet32 (PcdCpmPcieRpUnCorrectedErrorSeverity);
  mPlatformApeiPrivate->PlatRasPolicy.PcieDevCorrectedErrorMask = PcdGet32 (PcdCpmPcieDevCorrectedErrorMask);
  mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorMask = PcdGet32 (PcdCpmPcieDevUnCorrectedErrorMask);
  mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorSeverity = PcdGet32 (PcdCpmPcieDevUnCorrectedErrorSeverity);
  mPlatformApeiPrivate->PlatRasPolicy.CcixGhesCorrectedNotifyType = PcdGetBool (PcdCpmCcixGhesCorrectedNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.CcixGhesDeferredNotifyType = PcdGetBool (PcdCpmCcixGhesDeferredNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.HardPprEnable = PcdGetBool (PcdCpmHardPprEnable);
  mPlatformApeiPrivate->PlatRasPolicy.RasSmiScaleUnit = PcdGet8 (PcdCpmSmiScaleUnit);
  mPlatformApeiPrivate->PlatRasPolicy.RasSmiPeriod = (PcdGet32 (PcdCpmSmiPeriod) & 0x00007FFF);
  mPlatformApeiPrivate->PlatRasPolicy.UsbErrPollInterval = PcdGet32 (PcdCpmUsbErrPollInterval);
  mPlatformApeiPrivate->PlatRasPolicy.GhesUnCorrNotifyType = PcdGetBool (PcdCpmGhesUnCorrNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.PcieUnCorrGhesNotifyType = PcdGetBool (PcdCpmPcieUnCorrGhesNotifyType);
  mPlatformApeiPrivate->PlatRasPolicy.EinjBeginInjOptSwSmiData = PcdGet8 (PcdCpmEinjBeginInjOptSwSmiData);
  mPlatformApeiPrivate->PlatRasPolicy.EinjEndOptSwSmiData = PcdGet8 (PcdCpmEinjEndOptSwSmiData);
  mPlatformApeiPrivate->PlatRasPolicy.PcieAerReportMechanism = PcdGet8 (PcdAmdPcieAerReportMechanism);
  mPlatformApeiPrivate->PlatRasPolicy.HestDMCStrucEn = (UINT8)PcdGetBool (PcdCpmHestDmcStrucEn);

  //FCH software SMI command port
  mPlatformApeiPrivate->PlatRasPolicy.SwSmiCmdPortAddr = MmioRead16 ((ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG6A));

  //Get PCI configuration MMIO base address.
  GetPciMmioBase(&mPlatformApeiPrivate->PcieBaseAddress);
  DEBUG ((EFI_D_ERROR, "[RAS]PCI Express MMIO Base Address: %08x\n", mPlatformApeiPrivate->PcieBaseAddress));

  SearchMaxMcaBankCnt(&mPlatformApeiPrivate->MaxMcaBankCount);
  DEBUG ((EFI_D_ERROR, "[RAS]Max MCA Bank Count through cores: %d\n", mPlatformApeiPrivate->MaxMcaBankCount));

  Status = GetOemMemoryMapTable(&mPlatformApeiPrivate->OemMemoryMapTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[RAS]No Valid Oem Memory Map Table Found\n"));
  }

  //Default: Single socket
  mPlatformApeiPrivate->FchAcpiMmioBase[0] = ACPI_MMIO_BASE;  //Master FCH
  mPlatformApeiPrivate->FchAcpiMmioBase[1] = 0;               //Slave FCH
  DEBUG ((EFI_D_ERROR, "[RAS] Default ACPI MMIO Base Address - Socket0: 0x%0lx", mPlatformApeiPrivate->FchAcpiMmioBase[0]));

  return EFI_SUCCESS;
}


VOID
STATIC
PciAppendList (
  IN       PCI_ADDR          Device,
  IN       PCIE_DEVICE_TYPE  DeviceType,
  IN       PCIE_PORT_PROFILE *PciePortProfile
  )
{
  PCIE_DEV_ENTRY* PciDevEntry;
  EFI_STATUS      Status = EFI_SUCCESS;

  //
  // Allocate a new PCI entry
  //
  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  sizeof (PCIE_DEV_ENTRY),
                  (VOID **)&PciDevEntry
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, " Add Link failed, Out of Resource!!!\n"));
    return;
  }

  PciDevEntry->DevAddr = Device.AddressValue;
  PciDevEntry->DevType = DeviceType;
  DEBUG ((EFI_D_ERROR, " Add Device @0x%08x, Type: 0x%x to the Link List\n", PciDevEntry->DevAddr, PciDevEntry->DevType));
  //
  // Add the newly allocated PCI entry to the Root Port pci link list
  //
//  DEBUG ((EFI_D_ERROR, " PciLinkList Address: 0x%08x\n", (UINTN)&PciePortProfile->PciLinkList));
//  DEBUG ((EFI_D_ERROR, " Forward Link Address: 0x%08x, Backward Link Address: 0x%08x\n", PciePortProfile->PciLinkList.ForwardLink, PciePortProfile->PciLinkList.BackLink));

  InsertTailList (&PciePortProfile->PciLinkList, &PciDevEntry->ListEntry);
  PciePortProfile->EndPointDevCnt++;
}

SCAN_STATUS
STATIC
PcieDevScanCallback (
  IN       PCI_ADDR             Device,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  )
{
  SCAN_STATUS             ScanStatus;
  PCIE_DEVICE_TYPE        DeviceType;
  ScanStatus = SCAN_SUCCESS;
  DEBUG ((EFI_D_ERROR, "  PcieDevScanCallback for Device = %d:%d:%d\n",
    Device.Address.Bus,
    Device.Address.Device,
    Device.Address.Function
    ));
  ScanStatus = SCAN_SUCCESS;
  DeviceType = RasGetPcieDeviceType (Device);
  DEBUG ((EFI_D_ERROR, "  PCI-E device type = 0x%x\n", DeviceType));
  switch (DeviceType) {
  case  PcieDeviceRootComplex:
  case  PcieDeviceDownstreamPort:
  case  PcieDeviceUpstreamPort:
  case  PcieDevicePcieToPcix:
  case  PcieDeviceEndPoint:
  case  PcieDeviceLegacyEndPoint:
    PciAppendList (Device, DeviceType, (PCIE_PORT_PROFILE*)ScanData->Buffer);
    break;
  default:
    break;
  }
  return ScanStatus;
}

VOID
STATIC
PcieDevScan (
  IN       PCI_ADDR  DownstreamPort,
  IN       PCIE_PORT_PROFILE *PciePortProfile
  )
{
  RAS_PCI_SCAN_DATA  ScanData;

  DEBUG ((EFI_D_ERROR, "  Scan PCI device\n"));
  ScanData.Buffer = PciePortProfile;
  ScanData.RasScanCallback = PcieDevScanCallback;
  RasPciScanSecondaryBus (DownstreamPort, &ScanData);
}

EFI_STATUS
PciePortDetect ( VOID )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  PCIE_ACTIVE_PORT_MAP  *TempPciePortMap;
  PCIE_ACTIVE_PORT_MAP  *mPciePortMap;
  UINT32                PciePortMapSize;
  UINT8                 Nbio;
  UINT8                 DfNode;
  UINT8                 Port;
  UINT8                 NbioBusNum;
  UINT16                VendorID;
  PCI_ADDR              PciPortAddr;

  //
  //  Allocate memory and Initialize a temporary PCI-E Port Map data block and NBIO Error Data block pointer
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  MAX_PCIEMAP_BLOCK_SIZE,
                  (VOID **)&TempPciePortMap
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (TempPciePortMap, MAX_PCIEMAP_BLOCK_SIZE);

  for (DfNode = 0; DfNode < MAX_SOCKET_SUPPORT; DfNode++) {
    for (Nbio = 0; Nbio < MAX_NBIO_PER_DIE; Nbio++) {
      Status = AcquireNbioSecBusNumHelper(DfNode, Nbio, &NbioBusNum);
      if (EFI_ERROR (Status)) {
        //Find next
        continue;
      }

      DEBUG ((EFI_D_ERROR, "[RAS]DfNode %d, Nbio %d, Bus 0x%x\n", DfNode, Nbio, NbioBusNum));

      PciPortAddr.AddressValue = 0;

      for (Port = 0; Port < MAX_PCIE_PORT_SUPPORT; Port++) {
        PciPortAddr.Address.Bus = NbioBusNum;
        PciPortAddr.Address.Device = gPciePortList[Port].Device;
        PciPortAddr.Address.Function = gPciePortList[Port].Function;

        VendorID = PciRead16 (PciPortAddr.AddressValue);
        if (VendorID != AMD_VENDOR_ID) {
          continue;
        }
        DEBUG ((EFI_D_ERROR, "[RAS]Active PCI-E Root Port Bus:0x%x Dev:%d Fun:%d, Address: 0x%08x\n",
                PciPortAddr.Address.Bus,
                PciPortAddr.Address.Device,
                PciPortAddr.Address.Function,
                PciPortAddr.AddressValue));

        //Found active PCI-E port
        TempPciePortMap->PciPortNumber[TempPciePortMap->PortCount].NbioDieNum = Nbio;
        TempPciePortMap->PciPortNumber[TempPciePortMap->PortCount].NbioBusNum = NbioBusNum;
        TempPciePortMap->PciPortNumber[TempPciePortMap->PortCount].PciPortNumber = Port;
        TempPciePortMap->PciPortNumber[TempPciePortMap->PortCount].EndPointDevCnt = 0;
        TempPciePortMap->PciPortNumber[TempPciePortMap->PortCount].RpPciAddr = PciPortAddr.AddressValue;
        TempPciePortMap->PortCount++;
      }
    }
  }
  DEBUG ((EFI_D_ERROR, "[RAS]Total Active PCI-E port count %d\n", TempPciePortMap->PortCount));

  PciePortMapSize = (sizeof (PCIE_ACTIVE_PORT_MAP)) + ((sizeof (PCIE_PORT_PROFILE)) * (TempPciePortMap->PortCount - 1));

  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  PciePortMapSize,
                  (VOID **)&mPciePortMap
                  );
  if (EFI_ERROR (Status)) {
    //Release temporary PCI-E port map space.
    gBS->FreePool (TempPciePortMap);
    return Status;
  }

  //Copy to reserve memory space
  gBS->CopyMem (mPciePortMap, TempPciePortMap, PciePortMapSize);

  //Scan Device
  DEBUG ((EFI_D_ERROR, "[RAS]Scan PCI Device for Active Ports\n"));
  for (Port = 0; Port < mPciePortMap->PortCount; Port++) {
    InitializeListHead(&mPciePortMap->PciPortNumber[Port].PciLinkList);
    //The link list get ready here but the PCI scan needs to be down after PCI enumeration.
    PciPortAddr.AddressValue = mPciePortMap->PciPortNumber[Port].RpPciAddr;
    PcieDevScan(PciPortAddr, &mPciePortMap->PciPortNumber[Port]);
    DEBUG ((EFI_D_ERROR, "  RootPort Address: 0x%08x\n", PciPortAddr.AddressValue));
    DEBUG ((EFI_D_ERROR, "  Pci Device count: %d\n", mPciePortMap->PciPortNumber[Port].EndPointDevCnt));
    DEBUG ((EFI_D_ERROR, "  Head List Forward Link Addr: 0x%08x, Back Link Addr: 0x%08x\n",
            mPciePortMap->PciPortNumber[Port].PciLinkList.ForwardLink,
            mPciePortMap->PciPortNumber[Port].PciLinkList.BackLink));
  }

  //Release temporary PCI-E port map space.
  gBS->FreePool (TempPciePortMap);

  mPlatformApeiPrivate->AmdPciePortMap = mPciePortMap;
  mPlatformApeiPrivate->PciMapValid = TRUE;

  return Status;
}

EFI_STATUS
PcieStsClr (
  IN       PCIE_PORT_PROFILE  *PciePortProfileInstance,
  IN       UINT32             RetryCnt
)
{
  PCIE_DEV_ENTRY  *PcieDevEntry;
  LIST_ENTRY      *Node;
  PCI_ADDR        PciCfgAddr;
  UINT8           Index;

  Node = NULL;

  //Clear Root Port Error
  PciCfgAddr.AddressValue = PciePortProfileInstance->RpPciAddr;
  DEBUG ((EFI_D_ERROR, "  RootPort Address: 0x%08x\n", PciCfgAddr.AddressValue));

  //PCI-E Aer firmware first setup
  RasPcieStsClr (PciCfgAddr, RetryCnt);

  DEBUG ((EFI_D_ERROR, "[RAS]EndPointDevCnt : %d\n", PciePortProfileInstance->EndPointDevCnt));
  if (PciePortProfileInstance->EndPointDevCnt != 0) {
    //Search End Point device error
    for (Index = 0; Index <PciePortProfileInstance->EndPointDevCnt; Index++) {
      if (Index == 0) {
        DEBUG ((EFI_D_ERROR, "  Get First Node, Head Forward Link Addr: 0x%08x\n",PciePortProfileInstance->PciLinkList.ForwardLink));
        Node = GetFirstNode(&PciePortProfileInstance->PciLinkList);
      } else {
        DEBUG ((EFI_D_ERROR, "  Get Next Node, Current Node Address: 0x%08x\n",(UINTN)Node));
        Node = GetNextNode(&PciePortProfileInstance->PciLinkList, Node);
      }
      PcieDevEntry = (PCIE_DEV_ENTRY*)Node;
      PciCfgAddr.AddressValue = PcieDevEntry->DevAddr;
      DEBUG ((EFI_D_ERROR, "  Device Address: 0x%08x\n", PciCfgAddr.AddressValue));

      RasPcieStsClr (PciCfgAddr, RetryCnt);
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
PcieAerErrorConfig ( VOID )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  AMD_NBIO_PCIE_AER_PROTOCOL    *AmdNbioPcieAerProtocol;
  UINT16                        PciePortIndex;
  PCIE_PORT_PROFILE             *PciePortProfileInstance;
  PCI_ADDR                      PciPortAddr;
  PCIe_PORT_AER_CONFIG_EXTENDED PcieAerSetting;
  PCIe_AER_CONFIG_TEMP          PcieDevAerSetting;

  DEBUG ((EFI_D_ERROR, "[RAS]Platform PCI-E Aer Enablement entry\n"));

  // Get NBIO PCI-E AER protocol
  AmdNbioPcieAerProtocol = NULL;
  Status = gBS->LocateProtocol (&gAmdNbioPcieAerProtocolGuid, NULL, &AmdNbioPcieAerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error detected while trying to locate AMD_NBIO_PCIE_AER_PROTOCOL: %r\n", Status));
  }

  DEBUG ((EFI_D_ERROR, "[RAS] PCIE port Count: %d\n", mPlatformApeiPrivate->AmdPciePortMap->PortCount));
  PciePortProfileInstance = mPlatformApeiPrivate->AmdPciePortMap->PciPortNumber;
  for (PciePortIndex = 0; PciePortIndex < mPlatformApeiPrivate->AmdPciePortMap->PortCount; PciePortIndex++, PciePortProfileInstance++) {
    PciPortAddr.AddressValue = 0;
    PciPortAddr.Address.Bus = PciePortProfileInstance->NbioBusNum;
    PciPortAddr.Address.Device = gPciePortList[PciePortProfileInstance->PciPortNumber].Device;
    PciPortAddr.Address.Function = gPciePortList[PciePortProfileInstance->PciPortNumber].Function;

    PcieAerSetting.AerEnable = 1;
    PcieAerSetting.PciBus = PciePortProfileInstance->NbioBusNum;
    PcieAerSetting.PciDev = gPciePortList[PciePortProfileInstance->PciPortNumber].Device;
    PcieAerSetting.PciFunc = gPciePortList[PciePortProfileInstance->PciPortNumber].Function;

    PcieAerSetting.CorrectableMask.Value = mPlatformApeiPrivate->PlatRasPolicy.PcieRpCorrectedErrorMask;
    PcieAerSetting.UncorrectableMask.Value = mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorMask;
    if (IsHotPlugRp (PciPortAddr)) {
      PcieAerSetting.UncorrectableMask.Value |= (BIT5 + BIT14); //BIT5: Surprise Down Error Mask, BIT14: Completion Timeout Mask
      if (PcdGet8 (PcdAmdHotPlugHandlingMode) == 3) {  //Hotplug Firmware First mode
        PcieAerSetting.UncorrectableMask.Value &= ~(BIT14); //Do not mask BIT14: Completion Timeout Mask in Hotplug Firmware First mode
      }
    }
    PcieAerSetting.UncorrectableSeverity.Value = mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorSeverity;

    if (AmdNbioPcieAerProtocol != NULL) {
      AmdNbioPcieAerProtocol->SetPcieAerFeatureExtended (&PcieAerSetting);
    } else {
      RasSetPcieAerFeature (&PcieAerSetting);
    }

    //PCIe Status Clear
    PcieStsClr (PciePortProfileInstance, mPlatformApeiPrivate->PlatRasPolicy.RasRetryCnt);

    PcieDevAerSetting.AerEnable = 1;
    PcieDevAerSetting.PciBus = PciePortProfileInstance->NbioBusNum;
    PcieDevAerSetting.PciDev = gPciePortList[PciePortProfileInstance->PciPortNumber].Device;
    PcieDevAerSetting.PciFunc = gPciePortList[PciePortProfileInstance->PciPortNumber].Function;
    PcieDevAerSetting.CorrectableMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevCorrectedErrorMask;
    PcieDevAerSetting.UncorrectableMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorMask;
    PcieDevAerSetting.UncorrectableSeverity = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorSeverity;

    DEBUG ((EFI_D_ERROR, "  Device Correctable Error Mask: 0x%08x\n", PcieDevAerSetting.CorrectableMask));
    DEBUG ((EFI_D_ERROR, "  Device UnCorrectable Error Mask: 0x%08x\n", PcieDevAerSetting.UncorrectableMask));
    DEBUG ((EFI_D_ERROR, "  Device UnCorrectable Error Severity: 0x%08x\n", PcieDevAerSetting.UncorrectableSeverity));

    //Enable PCIe Error Report
    EnDevErrReport(PciPortAddr, &PcieDevAerSetting);
  }

  return Status;
}

UINT32
RasGetApicId(
    EFI_MP_SERVICES_PROTOCOL  *MpServices,
    UINTN ProcessorNumber
)
{
  UINT32 LocalApicId;
  if (ProcessorNumber == 0) {
  //This is BSP
    GetLocalApicId(&LocalApicId);
  }
  else {
    MpServices->StartupThisAP(
                MpServices,
                GetLocalApicId,
                ProcessorNumber,
                NULL,
                0,
                (VOID *) &LocalApicId,
                NULL
                );
  }
  return LocalApicId;
}

VOID
RasGetCpuIdInfo (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  UINTN ProcessorNumber,
  RAS_CPUID_FN *RasCpuidFn
)
{
  if (ProcessorNumber == 0) {
  //This is BSP
    GetCpuId(RasCpuidFn);
  }
  else {
    MpServices->StartupThisAP(
                MpServices,
                GetCpuId,
                ProcessorNumber,
                NULL,
                0,
                (VOID *) RasCpuidFn,
                NULL
                );
  }
}

VOID
RasGetSysEnterEip (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  UINTN ProcessorNumber,
  UINT64 *SysEnterEip
)
{
  if (ProcessorNumber == 0) {
  //This is BSP
    GetSysEnterEip(SysEnterEip);
  }
  else {
    MpServices->StartupThisAP(
                MpServices,
                GetSysEnterEip,
                ProcessorNumber,
                NULL,
                0,
                (VOID *) SysEnterEip,
                NULL
                );
  }
}

BOOLEAN
FillSecContextInfo (
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex,
  PROC_CONEXT_INFO_STRUC    *ProcContextInfoStruc,
  BOOLEAN                   LogUniDefrdErr
)
{
  UINT32   nodeId;
  UINT32   ccdId;
  UINT32   instanceId;
  UINT32   IndexId;
  UINT32   HwAssertStsLo;
  UINT32   HwAssertStsHi;
  UINT64   RspqWdtIoTransLog;
  UINT64   HwAssertStsArray[RMB_MAX_CCM_PER_DIE][2]; //HwAssertStsArray[RMB_MAX_CCM_PER_DIE][0]: DF CCM instance id, [1]: DF::HardwareAssertStatus*
  UINT64   RspqWdtArray[RMB_MAX_CCM_PER_DIE][2];     //RspqWdtArray[RMB_MAX_CCM_PER_DIE][0]: DF CCM instance id,     [1]: DF::RSPQWDTIoTransLog*
  BOOLEAN  SaveHwAssertToBert;
  BOOLEAN  SaveRspqWdtToBert;

  if (LogUniDefrdErr) {
    return  FALSE;
  }

  nodeId = RasMcaErrorInfo->CpuInfo.SocketId;
  ccdId = RasMcaErrorInfo->CpuInfo.DieId;
  if ((RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr.Field.HardwareID == MCA_DATA_FABRIC_ID) && \
      (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr.Field.McaType == PIE_MCA_TYPE)) {
    //*********************************************************************************
    // The format of DF HardwareAssert/DF WDT Error logs in SEC_PROC_CONTEXT_INFO - Register Array:
    //
    // DF HardwareAssert Error: (save all DF instances's HardwareAssertStatusHi/Lo)
    //      Element 1 (UINT64), Element 2 (UINT64)
    //    Note: 
    //      Element 1 = DF CCM instance ID, Element 2 = DF::HardwareAssertStatusHi/Lo
    //
    // DF WDT Error: (save all DF instances's SPQWDTIoTransLogHi/Lo)
    //      Element 1 (UINT64), Element 2 (UINT64)
    //    Note: 
    //      Element 1 = DF CCM instance ID, Element 2 = DF::RSPQWDTIoTransLogHi/Lo
    //
    // e.g.
    //  <-- Element 1 --> <-- Element 2 --> 
    //  00000010 00000000 00000000 00100000  <-+
    //  00000011 00000000 00000000 00000000    |
    //  00000012 00000000 00000020 00100000    |
    //  00000013 00000000 00000000 00000000     DF HardwareAssert Error
    //  00000014 00000000 00000000 00100000    |
    //  00000015 00000000 00000000 00000000    |
    //  00000016 00000000 00000000 00100000    |
    //  00000017 00000000 00000000 00000000  <-+
    //  00000010 00000000 00000000 00000000  <-+
    //  00000011 00000000 00000000 00000000    |
    //  00000012 00000000 fc000809 0000fffd    |
    //  00000013 00000000 00000000 00000000     DF WDT Error
    //  00000014 00000000 00000000 00000000    |
    //  00000015 00000000 00000000 00000000    |
    //  00000016 00000000 00000000 00000000    |
    //  00000017 00000000 00000000 00000000  <-+
    //*********************************************************************************
    //This is a PIE bank error.
    DEBUG ((EFI_D_ERROR, "[RAS] FillSecContextInfo: (MCA_DATA_FABRIC_ID, PIE_MCA_TYPE) Error found\n"));
    SaveHwAssertToBert = FALSE;
    SaveRspqWdtToBert = FALSE;
    for (IndexId = 0 ; IndexId < RMB_MAX_CCM_PER_DIE; IndexId++ ) {
      instanceId = IndexId + RMB_CCM0_INSTANCE_ID;
      //check Data Fabric Hardware Assert
      HwAssertStsLo = RasGetDfReg(instanceId, nodeId, DF_HARDWAREASSERTSTATUSLOW_ADDR);
      HwAssertStsHi = RasGetDfReg(instanceId, nodeId, DF_HARDWAREASSERTSTATUSHIGH_ADDR);
      DEBUG ((EFI_D_ERROR, "[RAS] PIE_MCA_TYPE- [%d]HwAssertStsHi: 0x%08x, HwAssertStsLo: 0x%08x\n",
              instanceId, HwAssertStsHi, HwAssertStsLo));
      HwAssertStsArray[IndexId][0] = instanceId;
      HwAssertStsArray[IndexId][1] = 0;
      RspqWdtArray[IndexId][0] = instanceId;
      RspqWdtArray[IndexId][1] = 0;
      if ((HwAssertStsHi != 0) || (HwAssertStsLo != 0)) {
        HwAssertStsArray[IndexId][1] = HwAssertStsHi;
        HwAssertStsArray[IndexId][1] = ((HwAssertStsArray[IndexId][1] << 32) | HwAssertStsLo);
        SaveHwAssertToBert = TRUE;

        //check CCM WDT status
      if (((HwAssertStsLo & DF_CCM_HARDWAREASSERTSTATUSLOW_MASK) | (HwAssertStsHi & DF_CCM_HARDWAREASSERTSTATUSHIGH_MASK)) != 0) {
          RspqWdtIoTransLog = RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGHIGH);
          RspqWdtIoTransLog = (RspqWdtIoTransLog << 32) | RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGLOW);
          RspqWdtArray[IndexId][1] = RspqWdtIoTransLog;
          DEBUG ((EFI_D_ERROR, "[RAS] PIE_MCA_TYPE [%d]RSPQWDTIOTRANSLOGHIGH: 0x%08x, RSPQWDTIOTRANSLOGLOW: 0x%08x\n", 
		         instanceId, RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGHIGH), RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGLOW)));
          SaveRspqWdtToBert = TRUE;

          //Reset DF::RSPQWDTIoTransLogLow and DF::RSPQWDTIoTransLogHi registers
          RasSetDfReg (instanceId, nodeId, RSPQWDTIOTRANSLOGLOW, BIT0);
        }
        //Clear DF::HardwareAssertStatusLow and DF::HardwareAssertStatusHigh registers
        RasSetDfReg (instanceId, nodeId, DF_HARDWAREASSERTSTATUSLOW_ADDR, 0x0);
        RasSetDfReg (instanceId, nodeId, DF_HARDWAREASSERTSTATUSHIGH_ADDR, 0x0);
      }
    }
    if (SaveHwAssertToBert) {
      ProcContextInfoStruc->RegisterContextType = UNCLASSIFIED_DATA;
      ProcContextInfoStruc->RegisterArraySize = (UINT16)RasAlignValue ((sizeof (UINT64) * (RMB_MAX_CCM_PER_DIE * 2)), PROC_CONEXT_INFO_STRUC_ALIGNMENT, TRUE);
      gBS->CopyMem (&ProcContextInfoStruc->RegisterArray[0], HwAssertStsArray, sizeof (HwAssertStsArray));
      if (SaveRspqWdtToBert) {
        ProcContextInfoStruc->RegisterArraySize = (UINT16)RasAlignValue ((sizeof (UINT64) * MAX_SEC_PROC_CONTEXT_INFO_ARRY_NUM), PROC_CONEXT_INFO_STRUC_ALIGNMENT, TRUE);
        gBS->CopyMem (&ProcContextInfoStruc->RegisterArray[RMB_MAX_CCM_PER_DIE * 2], RspqWdtArray, sizeof (RspqWdtArray));
      }
      return  TRUE;
    }
  }

  if ((RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr.Field.HardwareID == MCA_CPU_CORE_ID) && \
      (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr.Field.McaType == EX_MCA_TYPE)) {
    //*********************************************************************************
    // The format of CPU WDT log in SEC_PROC_CONTEXT_INFO - Register Array:
    // DF HardwareAssert Error (Ssae the RSPQWDTIoTransLogHi/Lo of DF CCM that has the error)
    //      Element 1 (UINT64), Element 2 (UINT64)
    //    Note: 
    //      Element 1 = DF CCM instance ID, Element 2 = DF::RSPQWDTIoTransLogHi/Lo
    // e.g.
    //  <-- Element 1 --> <-- Element 2 --> 
    //  00000010 00000000 fc000809 0000fffd  <- CCM0 DF::RSPQWDTIoTransLogHi/Lo
    //*********************************************************************************
    //This is a EX bank error. check CPU WDT status
    DEBUG ((EFI_D_ERROR, "[RAS] FillSecContextInfo: (MCA_CPU_CORE_ID, EX_MCA_TYPE) Error found\n"));
    if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCodeExt == 0) {
      if (ccdId < RMB_MAX_CCM_PER_DIE) {
        instanceId = ccdId + RMB_CCM0_INSTANCE_ID;
      }
      else {
        //Should not be here
        return FALSE;
      }

      //statically save the DF::RSPQWDTIoTransLogLow and DF::RSPQWDTIoTransLogHi registers when CPU WDT timeout
      ProcContextInfoStruc->RegisterContextType = UNCLASSIFIED_DATA;
      ProcContextInfoStruc->RegisterArraySize =  (UINT16)RasAlignValue ((sizeof (UINT64) * 2), PROC_CONEXT_INFO_STRUC_ALIGNMENT, TRUE);

      RspqWdtIoTransLog = RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGHIGH);
      RspqWdtIoTransLog = (RspqWdtIoTransLog << 32) | RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGLOW);
      ProcContextInfoStruc->RegisterArray[0] = instanceId;
      ProcContextInfoStruc->RegisterArray[1] = RspqWdtIoTransLog;

      DEBUG ((EFI_D_ERROR, "[RAS] EX_MCA_TYPE [%d]RSPQWDTIOTRANSLOGHIGH: 0x%08x, RSPQWDTIOTRANSLOGLOW: 0x%08x\n", 
              instanceId, RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGHIGH), RasGetDfReg(instanceId, nodeId, RSPQWDTIOTRANSLOGLOW)));

      //Reset DF::RSPQWDTIoTransLogLow and DF::RSPQWDTIoTransLogHi registers
      RasSetDfReg (instanceId, nodeId, RSPQWDTIOTRANSLOGLOW, BIT0);
      return  TRUE;
    }
  }

  return FALSE;
}

VOID
GetX64RegState (
  AMD_X64_REGISTER_STATE  *Amdx64RegState
)
{
  X64_REGISTER_STATE    *X64RegState;

  X64RegState = Amdx64RegState->Uefix64RegState;

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_THRD);
  Amdx64RegState->Amdx64RegGpr0State.Value = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RAX);
  X64RegState->RAX = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RBX);
  X64RegState->RBX = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RCX);
  X64RegState->RCX = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RDX);
  X64RegState->RDX = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RSI);
  X64RegState->RSI = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RDI);
  X64RegState->RDI = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RBP);
  X64RegState->RBP = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RSP);
  X64RegState->RSP = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R8);
  X64RegState->R8  = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R9);
  X64RegState->R9  = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R10);
  X64RegState->R10 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R11);
  X64RegState->R11 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R12);
  X64RegState->R12 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R13);
  X64RegState->R13 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R14);
  X64RegState->R14 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_R15);
  X64RegState->R15 = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  AsmWriteMsr64 (MSR_MCA_GPR_PR_CNTRL, AMD_GPR_RFLAGS);
  X64RegState->RFLAGS = AsmReadMsr64 (MSR_MCA_GPR_PR_DATA);

  return;
}

BOOLEAN
FillX64RegStateContextInfo (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  UINTN                     ProcessorNumber,
  UINT16                    ProcContextInfoSize,
  PROC_CONEXT_INFO_STRUC    *ProcContextInfoStruc
)
{
  AMD_X64_REGISTER_STATE  Amdx64RegState;

  ProcContextInfoStruc->RegisterContextType = MODE_EXECUTION_CONTEXT_64BIT;
  ProcContextInfoStruc->RegisterArraySize = ProcContextInfoSize;
  Amdx64RegState.Uefix64RegState = (X64_REGISTER_STATE *)ProcContextInfoStruc->RegisterArray;
  Amdx64RegState.Amdx64RegGpr0State.Value = 0;

  if (ProcessorNumber == 0) {
  //This is BSP
    GetX64RegState(&Amdx64RegState);
  }
  else {
    MpServices->StartupThisAP(
                MpServices,
                GetX64RegState,
                ProcessorNumber,
                NULL,
                0,
                (VOID *)&Amdx64RegState,
                NULL
                );
  }

  DEBUG ((EFI_D_ERROR, "%a: GPR0: 0x%llx\n", __FUNCTION__, Amdx64RegState.Amdx64RegGpr0State));
  DEBUG ((EFI_D_ERROR, "%a: RAX: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RAX));
  DEBUG ((EFI_D_ERROR, "%a: RBX: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RBX));
  DEBUG ((EFI_D_ERROR, "%a: RCX: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RCX));
  DEBUG ((EFI_D_ERROR, "%a: RDX: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RDX));
  DEBUG ((EFI_D_ERROR, "%a: RSI: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RSI));
  DEBUG ((EFI_D_ERROR, "%a: RDI: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RDI));
  DEBUG ((EFI_D_ERROR, "%a: RBP: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RBP));
  DEBUG ((EFI_D_ERROR, "%a: RSP: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->RSP));
  DEBUG ((EFI_D_ERROR, "%a: R8: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R8));
  DEBUG ((EFI_D_ERROR, "%a: R9: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R9));
  DEBUG ((EFI_D_ERROR, "%a: R10: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R10));
  DEBUG ((EFI_D_ERROR, "%a: R11: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R11));
  DEBUG ((EFI_D_ERROR, "%a: R12: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R12));
  DEBUG ((EFI_D_ERROR, "%a: R13: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R13));
  DEBUG ((EFI_D_ERROR, "%a: R14: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R14));
  DEBUG ((EFI_D_ERROR, "%a: R15: 0x%llx\n", __FUNCTION__, Amdx64RegState.Uefix64RegState->R15));
  DEBUG ((EFI_D_ERROR, "%a: Vaild: 0x%x\n", __FUNCTION__, (BOOLEAN)Amdx64RegState.Amdx64RegGpr0State.Field.Valid));

  return (BOOLEAN)Amdx64RegState.Amdx64RegGpr0State.Field.Valid;
}

EFI_STATUS
LogProcessorError (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex,
  UINTN                     ProcessorNumber,
  BOOLEAN                   LogUniDefrdErr
)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  GENERIC_PROC_ERR_ENTRY_GN     *GenericProcErrEntry;
  UINT32                        GenericProcErrEntrySize;
  EFI_GUID                      ProcErrorSectGuid = PROCESSOR_SPECIFIC_X86_SECT_GUID;
  EFI_GUID                      MsCheckGuid = IA32_X64_MS_CHECK_FIELD_DESC_GUID;
  EFI_GUID                      TlbCheckGuid = IA32_X64_TLB_CHECK_STRUC_GUID;
  EFI_GUID                      CacheCheckGuid = IA32_X64_CACHE_CHECK_STRUC_GUID;
  PROC_ERR_INFO_STRUC           *ProcErrInfoStruc;
  PROC_CONEXT_INFO_STRUC        *ProcContextInfoStruc;
  PROC_CONEXT_INFO_STRUC        *SecProcContextInfoStruc;
  UINT32                        LocalApicId;
  RAS_CPUID_FN                  RasCpuidFn;
  MS_CHECK_FIELD                MsCheckField;
  TLB_CHECK_STRUC               TlbCheckStruc;
  CACHE_CHECK_STRUC             CacheCheckStruc;
  UINT16                        McaErrorCode;
  UINT8                         McaExtErrorCode;
  MCA_TLB_ERROR                 TlbError;
  MCA_MEM_ERROR                 MemError;
  MCA_PP_ERROR                  PpError;
  MCA_INT_ERROR                 InternalError;
  UINT64                        SysEnterEip;
  UINT8                         BertSeverityType;
  UINT32                        ProcContextInfoSize;
  UINT32                        SecProcContextInfoSize;
  PROC_CONEXT_INFO_STRUC        *ThirdProcContextInfoStruc;
  UINT32                        ThirdProcContextInfoSize;
  UINT16                        X64RegStateProcContextInfoSize;

  ProcContextInfoSize = sizeof (PROC_CONEXT_INFO_STRUC) + ((sizeof (UINT64)) * (MCA_BANK_ERROR_INFO_REG_NUM + (MCA_BANK_ERROR_INFO_REG_NUM % 2)));
  //assume there will be a second processor context information structure for addition information
  SecProcContextInfoSize = sizeof (PROC_CONEXT_INFO_STRUC) + (UINT16)RasAlignValue ((sizeof (UINT64) * MAX_SEC_PROC_CONTEXT_INFO_ARRY_NUM), PROC_CONEXT_INFO_STRUC_ALIGNMENT, TRUE);
  X64RegStateProcContextInfoSize = (UINT16)RasAlignValue (sizeof (X64_REGISTER_STATE), PROC_CONEXT_INFO_STRUC_ALIGNMENT, TRUE);
  ThirdProcContextInfoSize = sizeof (PROC_CONEXT_INFO_STRUC) + X64RegStateProcContextInfoSize;

  //prepare structure, Processor Context field size need to be multiple of 16 bytes. pad 1 if MCA_BANK_ERROR_INFO_REG_NUM not even.
  GenericProcErrEntrySize = sizeof (GENERIC_PROC_ERR_ENTRY_GN) + sizeof (PROC_ERR_INFO_STRUC) + ProcContextInfoSize + SecProcContextInfoSize + ThirdProcContextInfoSize;
  DEBUG ((EFI_D_ERROR, "[RAS]Processor Error Section Size: 0x%x\n", GenericProcErrEntrySize));
  DEBUG ((EFI_D_ERROR, "[RAS]Processor Context Information Structure Size: 0x%x\n", sizeof (PROC_CONEXT_INFO_STRUC)));
  DEBUG ((EFI_D_ERROR, "[RAS]Processor Context Information Total Size: 0x%x\n", ProcContextInfoSize));
  DEBUG ((EFI_D_ERROR, "[RAS]Secondary Processor Context Information Total Size: 0x%x\n", SecProcContextInfoSize));
  DEBUG ((EFI_D_ERROR, "[RAS]The Third Processor Context Information Total Size: 0x%x\n", ThirdProcContextInfoSize));

  Status = gBS->AllocatePool (EfiBootServicesData, GenericProcErrEntrySize, &GenericProcErrEntry);
  ZeroMem (GenericProcErrEntry, GenericProcErrEntrySize);

  ProcErrInfoStruc = (PROC_ERR_INFO_STRUC*)((UINTN )GenericProcErrEntry + sizeof(GENERIC_PROC_ERR_ENTRY_GN));
  ProcContextInfoStruc = (PROC_CONEXT_INFO_STRUC*)((UINTN )ProcErrInfoStruc + sizeof(PROC_ERR_INFO_STRUC));
  SecProcContextInfoStruc = (PROC_CONEXT_INFO_STRUC*)((UINTN )ProcContextInfoStruc + ProcContextInfoSize);
  ThirdProcContextInfoStruc = (PROC_CONEXT_INFO_STRUC*)((UINTN )SecProcContextInfoStruc + SecProcContextInfoSize);

  DEBUG ((EFI_D_ERROR, "[RAS]GenericProcErrEntry Address: 0x%08x\n", GenericProcErrEntry));
  DEBUG ((EFI_D_ERROR, "[RAS]ProcErrInfoStruc Address: 0x%08x\n", ProcErrInfoStruc));
  DEBUG ((EFI_D_ERROR, "[RAS]ProcContextInfoStruc Address: 0x%08x\n", ProcContextInfoStruc));
  DEBUG ((EFI_D_ERROR, "[RAS]SecProcContextInfoStruc Address: 0x%08x\n", SecProcContextInfoStruc));
  DEBUG ((EFI_D_ERROR, "[RAS]ThirdProcContextInfoStruc Address: 0x%08x\n", ThirdProcContextInfoStruc));

  //Update generic data info
  gBS->CopyMem (GenericProcErrEntry, &gGenProcErrEntry, sizeof (GENERIC_PROC_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericProcErrEntry->GenErrorDataEntry.SectionType[0], &ProcErrorSectGuid, sizeof (EFI_GUID));

  //Checked Valid bits
  GenericProcErrEntry->ProcErrorSection.ValidBits.Field.CpuIdInfoValid = 1;
  GenericProcErrEntry->ProcErrorSection.ValidBits.Field.LocalApicIDValid = 1;
  GenericProcErrEntry->ProcErrorSection.ValidBits.Field.ProcErrInfoStrucNum = 1;
  GenericProcErrEntry->ProcErrorSection.ValidBits.Field.ProcContextInfoStrucNum = 1;

  //Get LocalApicId
  LocalApicId = RasGetApicId(MpServices, ProcessorNumber);
  DEBUG ((EFI_D_ERROR, "[RAS]LocalApicId: 0x%x\n", LocalApicId));
  GenericProcErrEntry->ProcErrorSection.LocalApicID = (UINT64)LocalApicId;

  //Get CPUID Information
  RasCpuidFn.FunctionId = 0x00000001;  //Register In EAX
  RasGetCpuIdInfo(MpServices, ProcessorNumber, &RasCpuidFn);
  DEBUG ((EFI_D_ERROR, "[RAS]Logical Processor : 0x%x, EAX: 0x%08x, EBX: 0x%08x, ECX: 0x%08x, EDX: 0x%08x\n", ProcessorNumber, RasCpuidFn.EAX_Reg, RasCpuidFn.EBX_Reg, RasCpuidFn.ECX_Reg, RasCpuidFn.EDX_Reg));

  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_EAX = RasCpuidFn.EAX_Reg;
  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_EBX = RasCpuidFn.EBX_Reg;
  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_ECX = RasCpuidFn.ECX_Reg;
  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_EDX = RasCpuidFn.EDX_Reg;
  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_PD1 = 0;
  GenericProcErrEntry->ProcErrorSection.CpuIdInfo_PD2 = 0;

  //Check Error code types
  McaErrorCode = (UINT16)RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCode;
  McaExtErrorCode = (UINT8)RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCodeExt;
  if (!LogUniDefrdErr) {
    if (((McaErrorCode & TLB_ERROR_MASK) >> TLB_ERROR_CHK_SHIFT) == 1) {
      //TLB error
      TlbError.Value = McaErrorCode;
      TlbCheckStruc.Value = 0;

      TlbCheckStruc.Field.Level = TlbError.Field.CacheLevel;
      TlbCheckStruc.Field.TranscationType = TlbError.Field.TransactionType;

      TlbCheckStruc.Field.ValidationBits = TLB_CHECK_LEVEL | TLB_CHECK_TRANSTYPE;

      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.UC) {
        TlbCheckStruc.Field.Uncorrected = 1;
        TlbCheckStruc.Field.ValidationBits |= TLB_CHECK_UNCORRECTED;
        if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 0 && RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.TCC == 0) {
          TlbCheckStruc.Field.ValidationBits |= TLB_CHECK_PRECISE_IP | TLB_CHECK_RESTARTABLE;
          TlbCheckStruc.Field.PreciseIp = 1;
          TlbCheckStruc.Field.RestartableIp = 1;
          ProcErrInfoStruc->ValidBits.Field.InstructionPointerValid = 1;
        } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 1) {
          TlbCheckStruc.Field.ValidationBits |= TLB_CHECK_PROC_CONEXT_CORRUPT;
          TlbCheckStruc.Field.ProcessorContextCorrupt = 1;
        }
      }
      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Overflow) {
          TlbCheckStruc.Field.Overflow = 1;
          TlbCheckStruc.Field.ValidationBits |= TLB_CHECK_OVERFLOW;
      }

      //Report TLB check field description
      gBS->CopyMem (&ProcErrInfoStruc->ErrorStructureType[0], &TlbCheckGuid, sizeof (EFI_GUID));

      //Update Processor check information
      ProcErrInfoStruc->ValidBits.Field.CheckInfoValid = 1;
      ProcErrInfoStruc->CheckInformation = TlbCheckStruc.Value;

    } else if (((McaErrorCode & MEM_ERROR_MASK) >> MEM_ERROR_CHK_SHIFT) == 1) {
      //Memory error
      MemError.Value = McaErrorCode;
      CacheCheckStruc.Value = 0;

      CacheCheckStruc.Field.Level = MemError.Field.CacheLevel;
      CacheCheckStruc.Field.TranscationType = MemError.Field.TransactionType;
      CacheCheckStruc.Field.Operation = MemError.Field.MemTransactionType;

      CacheCheckStruc.Field.ValidationBits = CACHE_CHECK_LEVEL | CACHE_CHECK_TRANSTYPE | CACHE_CHECK_OPERATION;

      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.UC) {
        CacheCheckStruc.Field.Uncorrected = 1;
        CacheCheckStruc.Field.ValidationBits |= CACHE_CHECK_UNCORRECTED;
        if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 0 && RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.TCC == 0) {
          CacheCheckStruc.Field.ValidationBits |= CACHE_CHECK_PRECISE_IP | CACHE_CHECK_RESTARTABLE;
          CacheCheckStruc.Field.PreciseIp = 1;
          CacheCheckStruc.Field.RestartableIp = 1;
          ProcErrInfoStruc->ValidBits.Field.InstructionPointerValid = 1;
        } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 1) {
          CacheCheckStruc.Field.ValidationBits |= CACHE_CHECK_PROC_CONEXT_CORRUPT;
          CacheCheckStruc.Field.ProcessorContextCorrupt = 1;
        }
      }
      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Overflow) {
          CacheCheckStruc.Field.Overflow = 1;
          CacheCheckStruc.Field.ValidationBits |= CACHE_CHECK_OVERFLOW;
      }

      //Report Cache check field description
      gBS->CopyMem (&ProcErrInfoStruc->ErrorStructureType[0], &CacheCheckGuid, sizeof (EFI_GUID));

      //Update Processor check information
      ProcErrInfoStruc->ValidBits.Field.CheckInfoValid = 1;
      ProcErrInfoStruc->CheckInformation = CacheCheckStruc.Value;

    } else if (((McaErrorCode & INT_ERROR_MASK) >> INT_ERROR_CHK_SHIFT) == 1) {
      //Bus Error
      PpError.Value = McaErrorCode;
      MsCheckField.Value = 0;

      MsCheckField.Field.ErrorType = MSCHK_ERRTYPE_INTERNAL_UNCLASSIFIED;
      MsCheckField.Field.ValidationBits = MS_CHECK_ERRORTYPE;

      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.UC) {
        MsCheckField.Field.Uncorrected = 1;
        MsCheckField.Field.ValidationBits |= MS_CHECK_UNCORRECTED;
        if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 0 && RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.TCC == 0) {
          MsCheckField.Field.ValidationBits |= MS_CHECK_PRECISE_IP | MS_CHECK_RESTARTABLE;
          MsCheckField.Field.PreciseIp = 1;
          MsCheckField.Field.RestartableIp = 1;
          ProcErrInfoStruc->ValidBits.Field.InstructionPointerValid = 1;
        } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 1) {
          MsCheckField.Field.ValidationBits |= MS_CHECK_PROC_CONEXT_CORRUPT;
          MsCheckField.Field.ProcessorContextCorrupt = 1;
        }
      }
      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Overflow) {
          MsCheckField.Field.Overflow = 1;
          MsCheckField.Field.ValidationBits |= MS_CHECK_OVERFLOW;
      }

      //Report MS check field description
      gBS->CopyMem (&ProcErrInfoStruc->ErrorStructureType[0], &MsCheckGuid, sizeof (EFI_GUID));

      //Update Processor check information
      ProcErrInfoStruc->ValidBits.Field.CheckInfoValid = 1;
      ProcErrInfoStruc->CheckInformation = MsCheckField.Value;
    } else if (((McaErrorCode & PP_ERROR_MASK) >> PP_ERROR_CHK_SHIFT) == 1) {
      //Internal unclassified error
      InternalError.Value = McaErrorCode;

      MsCheckField.Value = 0;
      MsCheckField.Field.ErrorType = MSCHK_ERRTYPE_INTERNAL_UNCLASSIFIED;
      MsCheckField.Field.ValidationBits = MS_CHECK_ERRORTYPE;

      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.UC) {
        MsCheckField.Field.Uncorrected = 1;
        MsCheckField.Field.ValidationBits |= MS_CHECK_UNCORRECTED;
        if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 0 && RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.TCC == 0) {
          MsCheckField.Field.ValidationBits |= MS_CHECK_PRECISE_IP | MS_CHECK_RESTARTABLE;
          MsCheckField.Field.PreciseIp = 1;
          MsCheckField.Field.RestartableIp = 1;
          ProcErrInfoStruc->ValidBits.Field.InstructionPointerValid = 1;
        } else if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.PCC == 1) {
          MsCheckField.Field.ValidationBits |= MS_CHECK_PROC_CONEXT_CORRUPT;
          MsCheckField.Field.ProcessorContextCorrupt = 1;
        }
      }
      if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Overflow) {
          MsCheckField.Field.Overflow = 1;
          MsCheckField.Field.ValidationBits |= MS_CHECK_OVERFLOW;
      }
      //Report MS check field description
      gBS->CopyMem (&ProcErrInfoStruc->ErrorStructureType[0], &MsCheckGuid, sizeof (EFI_GUID));

      //Update Processor check information
      ProcErrInfoStruc->ValidBits.Field.CheckInfoValid = 1;
      ProcErrInfoStruc->CheckInformation = MsCheckField.Value;
    } else {
      //Unknown error
      gBS->FreePool (GenericProcErrEntry);
      return RETURN_ABORTED;
    }
  } else {
    //Unique Deferred Error
    MsCheckField.Value = 0;
    MsCheckField.Field.ErrorType = MSCHK_ERRTYPE_INTERNAL_UNCLASSIFIED;
    MsCheckField.Field.ValidationBits = MS_CHECK_ERRORTYPE;
    if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaDeStatMsr.Field.Overflow) {
      MsCheckField.Field.Overflow = 1;
      MsCheckField.Field.ValidationBits |= MS_CHECK_OVERFLOW;
    }
    //Report MS check field description
    gBS->CopyMem (&ProcErrInfoStruc->ErrorStructureType[0], &MsCheckGuid, sizeof (EFI_GUID));

    //Update Processor check information
    ProcErrInfoStruc->ValidBits.Field.CheckInfoValid = 1;
    ProcErrInfoStruc->CheckInformation = MsCheckField.Value;
  }

  if (ProcErrInfoStruc->ValidBits.Field.InstructionPointerValid) {
    //Get SYSENTER_EIP
    RasGetSysEnterEip(MpServices,ProcessorNumber, &SysEnterEip);
    ProcErrInfoStruc->InstructionPointer = SysEnterEip;
  }

  //Update Processor Conext Information
  ProcContextInfoStruc->MSRAddress = MCA_EXTENSION_BASE + (BankIndex << 4) + MCA_STATUS_OFFSET;  //Ude Legacy MCA address
  ProcContextInfoStruc->RegisterContextType = MSR_REGISTERS;
  ProcContextInfoStruc->RegisterArraySize = (sizeof (UINT64)) * (MCA_BANK_ERROR_INFO_REG_NUM + (MCA_BANK_ERROR_INFO_REG_NUM % 2));
  gBS->CopyMem (&ProcContextInfoStruc->RegisterArray[0], &RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Value, (sizeof (UINT64)) * MCA_BANK_ERROR_INFO_REG_NUM);


  //Check if need second processor context inforamtion structure
  if (FillSecContextInfo(RasMcaErrorInfo, BankIndex, SecProcContextInfoStruc, LogUniDefrdErr)) {
    GenericProcErrEntry->ProcErrorSection.ValidBits.Field.ProcContextInfoStrucNum += 1;
    //Subtract old SecProcContextInfoSize from GenericProcErrEntrySize
    GenericProcErrEntrySize -= SecProcContextInfoSize;
    //Calculate new SecProcContextInfoSize
    SecProcContextInfoSize = sizeof (PROC_CONEXT_INFO_STRUC) + SecProcContextInfoStruc->RegisterArraySize;
    //Add new SecProcContextInfoSize to GenericProcErrEntrySize
    GenericProcErrEntrySize += SecProcContextInfoSize;
    //Calculate the new ThirdProcContextInfoStruc
    ThirdProcContextInfoStruc = (PROC_CONEXT_INFO_STRUC*)((UINTN)SecProcContextInfoStruc + SecProcContextInfoSize);
  } else {
    //Install only two processor contxt information structures
    GenericProcErrEntrySize = GenericProcErrEntrySize - SecProcContextInfoSize;
    ThirdProcContextInfoStruc = SecProcContextInfoStruc;
  }

  if (FillX64RegStateContextInfo (MpServices, ProcessorNumber, X64RegStateProcContextInfoSize, ThirdProcContextInfoStruc)) {
    GenericProcErrEntry->ProcErrorSection.ValidBits.Field.ProcContextInfoStrucNum += 1;
  } else {
    GenericProcErrEntrySize -= ThirdProcContextInfoSize;
  }

  if (!LogUniDefrdErr) {
    RasGetMcaSeverityType(RasMcaErrorInfo, BankIndex, &BertSeverityType);
  } else {
    BertSeverityType = ERROR_RECOVERABLE;
  }
  GenericProcErrEntry->GenErrorDataEntry.ErrorDataLength = GenericProcErrEntrySize - sizeof (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE);
  GenericProcErrEntry->GenErrorDataEntry.ErrorSeverity = BertSeverityType;

  // Add a new record to the BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)GenericProcErrEntry,      // IN UINT8* pErrRecord
                                                       GenericProcErrEntrySize,             // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                  // IN UINT8 ErrorType - GENERIC error type
                                                       BertSeverityType                     // IN UINT8 SeverityType - NON-CORRECTABLE
                                                       );

  gBS->FreePool (GenericProcErrEntry);

  return Status;
}

EFI_STATUS
LogMemError (
  RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  UINT8 BankIndex
)
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT64                        SystemMemoryAddress;
  NORMALIZED_ADDRESS            NormalizedAddress;
  DIMM_INFO                     DimmInfo;
  GENERIC_MEM_ERR_ENTRY_GN      *GenericMemErrEntry;
  UINT8                         SeverityType;
  EFI_GUID                      MemErrorSectGuid = PLATFORM_MEMORY_SECT_GUID;
  UMC_SYND_ECC_ERR_INFO         UmcSyndEccErrInfo;

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_MEM_ERR_ENTRY_GN), &GenericMemErrEntry);
  gBS->CopyMem (GenericMemErrEntry, &gGenMemErrEntry, sizeof (GENERIC_MEM_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericMemErrEntry->GenErrorDataEntry.SectionType[0], &MemErrorSectGuid, sizeof (EFI_GUID));

  NormalizedAddress.normalizedAddr = RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaAddrMsr.Field.ErrorAddr;
  NormalizedAddress.normalizedSocketId = RasMcaErrorInfo->CpuInfo.SocketId;
  NormalizedAddress.normalizedDieId = 0;  //SSP only has one IOD per socket so it is always 0.
  //Use IPID Instance ID check the UMC channel number.

  NormalizedAddress.normalizedChannelId = McaInstanceIdSearch(RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr);

  //Get Chip Select number from MCA_SYND_UMC.
  UmcSyndEccErrInfo.Value = (UINT32)RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaSyndMsr.Field.ErrorInformation;

  DEBUG ((EFI_D_ERROR, "[RAS]Memory ECC Error Found. Socket: %d, Die: %d, UMC Channel: %d, ChipSelect: %d\n", NormalizedAddress.normalizedSocketId, NormalizedAddress.normalizedDieId, NormalizedAddress.normalizedChannelId, UmcSyndEccErrInfo.Fields.ChipSelect));

  Status = AmdRasApeiRmbProtocol->McaErrorAddrTranslate (&NormalizedAddress, &SystemMemoryAddress, &DimmInfo);
  if (Status == EFI_INVALID_PARAMETER) {
    GenericMemErrEntry->MemErrorSection.ValidBits.Value = MEM_PARITY_VALID_BIT_MAP;
    GenericMemErrEntry->MemErrorSection.ValidBits.Field.ModuleValid = 1;
    //Node = Socket Id.
    GenericMemErrEntry->MemErrorSection.Node = (UINT16) (NormalizedAddress.normalizedSocketId);
    //Card = Channel Id
    GenericMemErrEntry->MemErrorSection.Card = (UINT16) (NormalizedAddress.normalizedChannelId);
    //Module = (ChipSelect) >> 1, ChipSelect 0,1 = Module 0, ChipSelect 2,3 = Module 1
    GenericMemErrEntry->MemErrorSection.Module = (UINT16) (UmcSyndEccErrInfo.Fields.ChipSelect >> 1);
    UpdateMemErrFruText (GenericMemErrEntry->GenErrorDataEntry.FruText,
                         GenericMemErrEntry->MemErrorSection.Node,
                         GenericMemErrEntry->MemErrorSection.Card,
                         GenericMemErrEntry->MemErrorSection.Module,
                         mPlatformApeiPrivate->OemMemoryMapTable
                         );
    GenericMemErrEntry->MemErrorSection.MemErrType = INVALID_ADDRESS;
  } else {
    GenericMemErrEntry->MemErrorSection.ValidBits.Value = MEM_VALID_BIT_MAP;
    GenericMemErrEntry->MemErrorSection.PhyAddr = SystemMemoryAddress;
    //Node = Socket Id.
    GenericMemErrEntry->MemErrorSection.Node = (UINT16) (NormalizedAddress.normalizedSocketId);
    //Card = Channel Id
    GenericMemErrEntry->MemErrorSection.Card = (UINT16) (NormalizedAddress.normalizedChannelId);
    //Module = (ChipSelect & 0x03) >> 1, ChipSelect 0,1 = Module 0, ChipSelect 2,3 = Module 1
    GenericMemErrEntry->MemErrorSection.Module = (UINT16) ((DimmInfo.ChipSelect & 0x03) >> 1);

    GenericMemErrEntry->MemErrorSection.Bank = (UINT16) DimmInfo.Bank;
    GenericMemErrEntry->MemErrorSection.Row = (UINT16) DimmInfo.Row;

    GenericMemErrEntry->MemErrorSection.Column = (UINT16) DimmInfo.Column;
    GenericMemErrEntry->MemErrorSection.RankNumber = (UINT16) DimmInfo.ChipSelect;
    UpdateMemErrFruText (GenericMemErrEntry->GenErrorDataEntry.FruText,
                         GenericMemErrEntry->MemErrorSection.Node,
                         GenericMemErrEntry->MemErrorSection.Card,
                         GenericMemErrEntry->MemErrorSection.Module,
                         mPlatformApeiPrivate->OemMemoryMapTable
                         );
    GenericMemErrEntry->MemErrorSection.MemErrType = UNKNOWN;  // not a valid field.
  }

  GenericMemErrEntry->MemErrorSection.ErrStatus = ((1 << 18) | (4 << 8));   // Error Detected on Data Transaction | Internal DRAM Error (0x40400)

  SeverityType = ERROR_SEVERITY_FATAL;
  if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.CECC) {
    SeverityType = ERROR_SEVERITY_CORRECTED;
  }
  GenericMemErrEntry->GenErrorDataEntry.ErrorSeverity = SeverityType;

  // Add a new record to the BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)GenericMemErrEntry,       // IN UINT8* pErrRecord
                                                       sizeof (GENERIC_MEM_ERR_ENTRY_GN),    // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                // IN UINT8 ErrorType - GENERIC error type
                                                       SeverityType                       // IN UINT8 SeverityType
                                                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    gBS->FreePool (GenericMemErrEntry);
    return EFI_SUCCESS;
  }
  gBS->FreePool (GenericMemErrEntry);

  return Status;
}

EFI_STATUS
LogMemParityError (
  RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  UINT8 BankIndex
)
{
  EFI_STATUS                Status = EFI_SUCCESS;
  GENERIC_MEM_ERR_ENTRY_GN  *MemParityErrEntry;
  EFI_GUID                  MemErrorSectGuid = PLATFORM_MEMORY_SECT_GUID;
  UINT8                     UmcExtErrorCode;
  UINT16                    ChannelId;
  UINT16                    ChipSelect;
  UINT32                    ChipSelectBit;
  CPER_ERROR_STATUS         CperErrorStatus;

  UmcExtErrorCode = (UINT8)RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCodeExt;
  //Use IPID Instance ID check the UMC channel number. Need update for 8 channel support for SSP
  ChannelId = McaInstanceIdSearch(RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr);
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_MEM_ERR_ENTRY_GN), &MemParityErrEntry);
  gBS->CopyMem (MemParityErrEntry, &gGenMemParityErrEntry, sizeof (GENERIC_MEM_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&MemParityErrEntry->GenErrorDataEntry.SectionType[0], &MemErrorSectGuid, sizeof (EFI_GUID));

  MemParityErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;  // 0x01;
  MemParityErrEntry->MemErrorSection.ValidBits.Value = MEM_PARITY_VALID_BIT_MAP;
  MemParityErrEntry->MemErrorSection.Node = (UINT16) (RasMcaErrorInfo->CpuInfo.SocketId);
  if ((UMC_ADDRCMDPARITYERR == UmcExtErrorCode) || (UMC_WRITEDATACRCERR == UmcExtErrorCode)) {
    ChipSelectBit = (UINT32)(RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaAddrMsr.Field.ErrorAddr >> 32) & 0x0F;
    if (ChipSelectBit == 0) {
      AsciiStrCpyS (MemParityErrEntry->GenErrorDataEntry.FruText, FRU_TEXT_MAX_LENGTH, "DIMM# Not Sourced");  // Error Fru Text String
    } else {
      ChipSelect = (UINT16)RasBitPositionToInt(ChipSelectBit);

      MemParityErrEntry->MemErrorSection.ValidBits.Field.RankNumberValid = 1;
      MemParityErrEntry->MemErrorSection.ValidBits.Field.ModuleValid = 1;

      //Card = Channel Id
      MemParityErrEntry->MemErrorSection.Card = ChannelId;
      //Module = (ChipSelect & 0x03) >> 1, ChipSelect 0,1 = Module 0, ChipSelect 2,3 = Module 1
      MemParityErrEntry->MemErrorSection.Module = (UINT16)((ChipSelect & 0x03) >> 1);

      MemParityErrEntry->MemErrorSection.RankNumber = ChipSelect;
      DEBUG ((EFI_D_ERROR, "[RAS]Memory CRC/Parity Error Found. Node: %d, UMC Channel: %d, Chip Select: %d\n", MemParityErrEntry->MemErrorSection.Node, ChannelId, ChipSelect));

      UpdateMemErrFruText (MemParityErrEntry->GenErrorDataEntry.FruText,
                           MemParityErrEntry->MemErrorSection.Node,
                           MemParityErrEntry->MemErrorSection.Card,
                           MemParityErrEntry->MemErrorSection.Module,
                           mPlatformApeiPrivate->OemMemoryMapTable
                           );
    }
  } else {
    AsciiStrCpyS (MemParityErrEntry->GenErrorDataEntry.FruText, FRU_TEXT_MAX_LENGTH, "DIMM# Not Sourced");  // Error Fru Text String
  }

  CperErrorStatus.Value = 0;
  switch (UmcExtErrorCode) {
  case UMC_ADDRCMDPARITYERR:
    CperErrorStatus.Field.Address = 1;
    CperErrorStatus.Field.ErrorType = CPER_ERROR_TYPE_ERR_PARITY;
    break;
  case UMC_WRITEDATACRCERR:
    CperErrorStatus.Field.Data = 1;
    CperErrorStatus.Field.ErrorType = CPER_ERROR_TYPE_ERR_BUS;
    break;
  default:
    break;
  }
  MemParityErrEntry->MemErrorSection.ErrStatus = CperErrorStatus.Value;

  //
  MemParityErrEntry->MemErrorSection.MemErrType = PARITY_ERROR;

  // Add a new record to the BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)MemParityErrEntry,          // IN UINT8* pErrRecord
                                                       sizeof (GENERIC_MEM_ERR_ENTRY_GN),      // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                  // IN UINT8 ErrorType - GENERIC error type
                                                       ERROR_SEVERITY_FATAL                 // IN UINT8 SeverityType - NON-CORRECTABLE
                                                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    gBS->FreePool (MemParityErrEntry);
    return EFI_SUCCESS;
  }
  gBS->FreePool (MemParityErrEntry);

  return Status;
}

BOOLEAN
IsEccEn (
  RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  UINT8                 BankIndex
)
{
  EFI_STATUS            Status;
  UINT8                 BusNum;
  UINT16                ChannelId;
  UINT32                UmcRegBase;
  UMC_ECCCTRL_REG       UmcEccCtrl;

  //Get IOHC bus number
  Status = AcquireNbioSecBusNumHelper(RasMcaErrorInfo->CpuInfo.SocketId, 0, &BusNum);
  //Get Channel number
  ChannelId = McaInstanceIdSearch(RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr);
  //Get UMC0CHx00000000...UMC7CHx0000000C [DRAM CS Base Address] (UMC::CH::BaseAddr)
  UmcRegBase = (UMC0_CH_REG_BASE + (UMC_SMN_ADDR_OFFSET * ChannelId));
  //Get UMC[0...7]CHx0000014C [DRAM ECC Control] (UMC::CH::EccCtrl)
  RasSmnRead (BusNum, UmcRegBase + UMC_ECC_CTRL, &UmcEccCtrl.Value);

  return (BOOLEAN)(UmcEccCtrl.Field.RdEccEn);
}

EFI_STATUS
MemErrorDecode (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex,
  UINTN                     ProcessorNumber
)
{
  EFI_STATUS                Status = EFI_SUCCESS;
  UINT8                     UmcExtErrorCode;

  UmcExtErrorCode = (UINT8)RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCodeExt;

  if ((UmcExtErrorCode == UMC_WRITEDATAPOISONERR) && !IsEccEn(RasMcaErrorInfo, BankIndex)) {
    UmcExtErrorCode = 0xFF;
  }

  //Add error record to BERT table.
  switch (UmcExtErrorCode) {
  case UMC_DRAMECCERR:
  case UMC_WRITEDATAPOISONERR:
    if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.AddrV) {
      LogMemError (RasMcaErrorInfo, BankIndex);
    } else {
      LogProcessorError (MpServices, RasMcaErrorInfo, BankIndex, ProcessorNumber, FALSE);
    }
    break;
  case UMC_ADDRCMDPARITYERR:
  case UMC_WRITEDATACRCERR:
    //Decode
    LogMemParityError (RasMcaErrorInfo, BankIndex);
    break;
  default:
    //Log all other errors from UMC as processor error.
    LogProcessorError (MpServices, RasMcaErrorInfo, BankIndex, ProcessorNumber, FALSE);
    break;
  }

  return Status;
}

EFI_STATUS
NbioErrorDecode (
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 Nbio;
  UINT8                 NbioBusNum;

  if (RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaStatusMsr.Field.ErrorCodeExt != NBIO_PCIE_SIDEBAND) {
    return EFI_NOT_FOUND;
  }

  Nbio = McaInstanceIdSearch(RasMcaErrorInfo->McaBankErrorInfo[BankIndex].McaIpidMsr);
  if (Nbio == INSTANCEID_NOT_FOUND) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_ERROR, "[RAS]MCA PCIE Error Found in Socket %d, Instance : %d\n", RasMcaErrorInfo->CpuInfo.SocketId, Nbio));

  Status = AcquireNbioSecBusNumHelper(RasMcaErrorInfo->CpuInfo.SocketId, Nbio, &NbioBusNum);
  if (EFI_ERROR (Status)) {
    return EFI_INVALID_PARAMETER;
  }

  PcieErrorScanDxe (NbioBusNum);

  return Status;
}

EFI_STATUS
McaErrorDetection ( VOID )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  RAS_MCA_ERROR_INFO_V2         RasMcaErrorInfo;
  UINTN                         ProcessorNumber;
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  UINTN                         NumberOfLogicProcessors;
  UINTN                         NumberOfEnabledProcessors;
  UINT8                         BankNum;
  UINT8                         BankIndex;
  BOOLEAN                       DefrdErrInMcaStat;

  // Get EFI MP service
  if (MpServices == NULL) {
    Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;    // Error detected while trying to locate MP Service protocol
    }
  }

  // Get StartThisApFrom, StartThisApTo, StartThisApSteps
  MpServices->GetNumberOfProcessors (MpServices, &NumberOfLogicProcessors, &NumberOfEnabledProcessors);

  for (ProcessorNumber = 0; ProcessorNumber < NumberOfLogicProcessors; ProcessorNumber++) {
    ZeroMem (&RasMcaErrorInfo, sizeof (RasMcaErrorInfo));
    RasMcaErrorInfo.CpuInfo.ProcessorNumber = ProcessorNumber;
    AmdRasApeiRmbProtocol->SearchMcaError (&RasMcaErrorInfo);

    BankNum = (UINT8)RasMcaErrorInfo.McaBankCount;
    for (BankIndex = 0; BankIndex < BankNum; BankIndex++) {
      if (RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaIpidMsr.Field.HardwareID == 0) {
        continue;
      }
      //a Valid error return?
      DefrdErrInMcaStat = FALSE;
      if (RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Val) {
        DEBUG ((EFI_D_ERROR, "[RAS]MCA Error Found in bank : 0x%x\n", RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaBankNumber));
        DefrdErrInMcaStat = (BOOLEAN)RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaStatusMsr.Field.Deferred;

        if (RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaIpidMsr.Field.HardwareID == MCA_NBIO_ID) {
          Status = NbioErrorDecode(&RasMcaErrorInfo, BankIndex);
          if (EFI_SUCCESS == Status) {
            //PCIE error logged, clear MCA Error status registers.
            McaStatusClear (MpServices, &RasMcaErrorInfo, BankIndex);
            continue;
          }
          //log as Processor Error if return EFI_ERROR
        }
        if (RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
          MemErrorDecode(MpServices, &RasMcaErrorInfo, BankIndex, ProcessorNumber);
        } else {
          LogProcessorError(MpServices, &RasMcaErrorInfo, BankIndex, ProcessorNumber, FALSE);
        }
        //Clear MCA Error status registers.
        McaStatusClear (MpServices, &RasMcaErrorInfo, BankIndex);
      }

      if ((RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaDeStatMsr.Field.Val) && !DefrdErrInMcaStat) {
        DEBUG ((EFI_D_ERROR, "[RAS]Unique MCA Deferred Error Found in bank : 0x%x\n", RasMcaErrorInfo.McaBankErrorInfo[BankIndex].McaBankNumber));
        LogProcessorError(MpServices, &RasMcaErrorInfo, BankIndex, ProcessorNumber, TRUE);
        //Clear MCA Error status registers.
        McaStatusClear (MpServices, &RasMcaErrorInfo, BankIndex);
      }
    }
  }
  return Status;
}

VOID
McaStsClear (
  UINT32  *McaBank
  )
{
  UINT64 SaveHwcr;
  UINT64 MsrData;
  UINT32 McaBankAddrBase;
  MCA_MISC0_MSR McaMisc0Msr;
  MCA_MISC1_MSR McaMisc1Msr;
  MCA_IPID_MSR McaIpidMsr;
  BOOLEAN DestatExists;

  McaBankAddrBase = MCA_EXTENSION_BASE + (*McaBank * SMCA_REG_PER_BANK);

  // MSR_C001_0015[18][McStatusWrEn] = 1
  SaveHwcr = AsmReadMsr64 (MSR_HWCR);
  MsrData = SaveHwcr | BIT18;
  AsmWriteMsr64 (MSR_HWCR, MsrData);

  AsmWriteMsr64 (McaBankAddrBase | MCA_ADDR_OFFSET, 0);
  AsmWriteMsr64 (McaBankAddrBase | MCA_SYND_OFFSET, 0);
  AsmWriteMsr64 (McaBankAddrBase | MCA_STATUS_OFFSET, 0);

  McaMisc0Msr.Value = AsmReadMsr64 (McaBankAddrBase | MCA_MISC0_OFFSET);
  McaMisc0Msr.Field.Ovrflw = 0;
  AsmWriteMsr64 (McaBankAddrBase | MCA_MISC0_OFFSET, McaMisc0Msr.Value);

  McaIpidMsr.Value = AsmReadMsr64 (McaBankAddrBase | MCA_IPID_OFFSET);
  if (McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
    McaMisc1Msr.Value = AsmReadMsr64 (McaBankAddrBase | MCA_MISC1_OFFSET);
    McaMisc1Msr.Field.Ovrflw = 0;
    AsmWriteMsr64 (McaBankAddrBase | MCA_MISC1_OFFSET, McaMisc1Msr.Value);
  }

  DestatExists = TRUE;
  switch (McaIpidMsr.Field.HardwareID)
  {
  case MCA_CPU_CORE_ID:
    switch (McaIpidMsr.Field.McaType) {
      case IF_MCA_TYPE:
      case DE_MCA_TYPE:
      case EX_MCA_TYPE:
      case FP_MCA_TYPE:
        DestatExists = FALSE;
        break;
    }
    break;
  case MCA_PARAMETER_BLOCK_ID:
  case MCA_PSP_ID:
  case MCA_SMU_ID:
    DestatExists = FALSE;
    break;
  }

  if (DestatExists) {
    AsmWriteMsr64 (McaBankAddrBase | MCA_DEADDR_OFFSET, 0);
    AsmWriteMsr64 (McaBankAddrBase | MCA_DESTAT_OFFSET, 0);
  }

  // Restore MSR_C001_0015[18][McStatusWrEn]
  AsmWriteMsr64 (MSR_HWCR, SaveHwcr);

}

EFI_STATUS
McaStatusClear (
  EFI_MP_SERVICES_PROTOCOL  *MpServices,
  RAS_MCA_ERROR_INFO_V2     *RasMcaErrorInfo,
  UINT8                     BankIndex
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINT32        McaBank;
  UINTN         NumberOfEnabledProcessors;
  UINTN         NumberOfLogicProcessors;

  NumberOfLogicProcessors = 0;

  MpServices->GetNumberOfProcessors (MpServices, &NumberOfLogicProcessors, &NumberOfEnabledProcessors);

  if (RasMcaErrorInfo->CpuInfo.ProcessorNumber > NumberOfLogicProcessors) {
    return EFI_INVALID_PARAMETER;
  }

  McaBank = BankIndex;

  //Program BSP first
  if (RasMcaErrorInfo->CpuInfo.ProcessorNumber == 0) {
    McaStsClear (&McaBank);
  } else {
    MpServices->StartupThisAP (
                               MpServices,
                               McaStsClear,
                               RasMcaErrorInfo->CpuInfo.ProcessorNumber,
                               NULL,
                               0,
                               (VOID *) &McaBank ,
                               NULL
    );
  }
  return Status;
}

EFI_STATUS
NbioLogError (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  GENERIC_NBIO_ERR_ENTRY_GN *GenericNbioErrEntry;
  EFI_GUID                 NbioErrorSectGuid = NBIO_ERROR_SECT_GUID;

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_NBIO_ERR_ENTRY_GN), &GenericNbioErrEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->CopyMem (GenericNbioErrEntry, &gGenNbioErrEntry, sizeof (GENERIC_NBIO_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericNbioErrEntry->GenErrorDataEntry.SectionType[0], &NbioErrorSectGuid, sizeof (EFI_GUID));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Value = NBIO_VALID_BIT_MAP;
  GenericNbioErrEntry->NbioErrorSection.BusId = RasNbioErrorInfo->DieBusNumber;
  GenericNbioErrEntry->NbioErrorSection.ErrorSource.Field.Nbio = 1;
  GenericNbioErrEntry->NbioErrorSection.ErrorType.Value = (UINT8) (RasNbioErrorInfo->NbioGlobalStatusLo & 0x0000000F);
  GenericNbioErrEntry->NbioErrorSection.GroupId = RasNbioErrorInfo->GroupId;
  GenericNbioErrEntry->NbioErrorSection.GroupType = RasNbioErrorInfo->TypeId;
  GenericNbioErrEntry->NbioErrorSection.ParityErrSts = RasNbioErrorInfo->NbioParityErrorSts;

  // Add a new record to the BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)GenericNbioErrEntry,          // IN UINT8* pErrRecord
                                                       sizeof (GENERIC_NBIO_ERR_ENTRY_GN),       // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                   // IN UINT8 ErrorType - GENERIC error type
                                                       ERROR_SEVERITY_FATAL                  // IN UINT8 SeverityType - NON-CORRECTABLE
                                                       );

  gBS->FreePool (GenericNbioErrEntry);
  return Status;
}

EFI_STATUS
NbifLogError (
  UINT8         DieBusNum,
  UINT8         NbifMm,
  UINT8         Leaf,
  NBIO_ERR_SRC  ErrorSource,
  NBIO_ERR_TYPE ErrorType

  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  GENERIC_NBIO_ERR_ENTRY_GN *GenericNbioErrEntry;
  EFI_GUID                 NbioErrorSectGuid = NBIO_ERROR_SECT_GUID;

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_NBIO_ERR_ENTRY_GN), &GenericNbioErrEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->CopyMem (GenericNbioErrEntry, &gGenNbioErrEntry, sizeof (GENERIC_NBIO_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericNbioErrEntry->GenErrorDataEntry.SectionType[0], &NbioErrorSectGuid, sizeof (EFI_GUID));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Value = NBIF_VALID_BIT_MAP;
  GenericNbioErrEntry->NbioErrorSection.BusId = DieBusNum;
  GenericNbioErrEntry->NbioErrorSection.ErrorSource = ErrorSource;
  GenericNbioErrEntry->NbioErrorSection.ErrorType = ErrorType;
  GenericNbioErrEntry->NbioErrorSection.NbifId = NbifMm;
  GenericNbioErrEntry->NbioErrorSection.LeafId = Leaf;

  // Add a new record to the NBIO BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)GenericNbioErrEntry,          // IN UINT8* pErrRecord
                                                       sizeof (GENERIC_NBIO_ERR_ENTRY_GN),       // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                   // IN UINT8 ErrorType - GENERIC error type
                                                       ERROR_SEVERITY_FATAL                  // IN UINT8 SeverityType - NON-CORRECTABLE
                                                       );

  gBS->FreePool (GenericNbioErrEntry);

  return Status;
}

EFI_STATUS
NbioErrorDetection ( VOID )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 DfNode;
  UINT8                 Nbio;
  UINT8                 NbioBusNum;

  //Register Error handler per Nbio.
  for (DfNode = 0; DfNode < MAX_SOCKET_SUPPORT; DfNode++) {
    for (Nbio = 0; Nbio < MAX_NBIO_PER_DIE; Nbio++) {
      Status = AcquireNbioSecBusNumHelper(DfNode, Nbio, &NbioBusNum);
      if (EFI_ERROR (Status)) {
        //Find next
        continue;
      }
      NbifErrorScan (Nbio, NbioBusNum);

      // // PcdAmdNbioPoisonConsumption = TRUE
      // // causes the PCIe Root Port to issue a system fatal error event
      // // on poison sent or received from PCIe links.
      // if (PcdGetBool(PcdAmdNbioPoisonConsumption)) {
      //   NbioEgressPoisonErrorScan (Nbio, NbioBusNum);
      // }
    }
  }
  return Status;
}


EFI_STATUS
NbioErrorScan (
  RAS_NBIO_ERROR_INFO   *RasNbioErrorInfo
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  //Scan Error Reporting Status by Type (UNCORR, CORR, UCP)
    RasNbioErrorInfo->TypeId = NBIO_GRP_UNCORR;
    while (NbioSearchErr(RasNbioErrorInfo)) {
      NbioLogError (RasNbioErrorInfo);
      NbioClrStatus (RasNbioErrorInfo);
    }

    RasNbioErrorInfo->TypeId = NBIO_GRP_CORR;
    while (NbioSearchErr(RasNbioErrorInfo)) {
      NbioLogError (RasNbioErrorInfo);
      NbioClrStatus (RasNbioErrorInfo);
    }

    RasNbioErrorInfo->TypeId = NBIO_GRP_UCP;
    while (NbioSearchErr(RasNbioErrorInfo)) {
      NbioLogError (RasNbioErrorInfo);
      NbioClrStatus (RasNbioErrorInfo);
  }

  return Status;
}

EFI_STATUS
NbifErrorScan (
  UINT8     Nbio,
  UINT8     NbioBusNum
  )
{
  NBIF_SHUB_TODET_CLIENT_STATUS_REG ShubTodetStatus;

  DEBUG ((EFI_D_ERROR, "[RAS]NBIF error scanning at Bus:0x%x, NBIO%d\n",NbioBusNum, Nbio));

  //SHUB_TODET_STATUS
  RasSmnRead ((UINT32)NbioBusNum, NBIF_SHUB_TODET_CLIENT_STATUS, &ShubTodetStatus.Value);
  if (ShubTodetStatus.Value) {
    DEBUG((EFI_D_ERROR, "[RAS] Nbio: %0d, NbioBusNum: %0x, BaseAddr: 0x%0x, SHUB_TODET_STATUS: 0x%0x\n",
           Nbio, NbioBusNum, NBIF_SHUB_TODET_CLIENT_STATUS, ShubTodetStatus.Value));
    
    if (ShubTodetStatus.Field.AthubTimeoutDetected || 
        ShubTodetStatus.Field.Mp1TimeoutDetected ||
        ShubTodetStatus.Field.Mp0TimeoutDetected) {
          NbifSysHubWDLogErr (NbioBusNum);
    }

    //Clear the status of SHUB_TODET_STATUS. (Write-1-to-clear)
    RasSmnWrite ((UINT32)NbioBusNum, NBIF_SHUB_TODET_CLIENT_STATUS, &ShubTodetStatus.Value);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
PcieErrorLog (
  IN       UINT16               ErrorEntryCnt,
  IN OUT   PCIE_ERR_ENTRY       *PcieErrorEntry
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  PCI_ADDR                    PciPortAddr;
  UINT16                      AerCapPtr;
  UINT8                       PcieCapPtr;
  GENERIC_PCIE_AER_ERR_ENTRY_GN *GenPcieAerErrEntry;
  PCIE_ERROR_SECTION          *PcieErrorSection;
  UINT32                      Index;
  EFI_GUID                    PcieErrorSectGuid = PCIE_SECT_GUID;
  UINT8                       SeverityType = 0;
  UINT16                      ErrEntryIndx;
  PCIE_ERR_ENTRY              *LocalErrEntry;
  PCIE_DEVICE_TYPE            PortType;
  ROOT_ERR_STS_REG            RootErrSts;
  UINT32                      MaxIndex = 0;

  LocalErrEntry = PcieErrorEntry;

  DEBUG ((EFI_D_ERROR, "  Local Error Entry Address: 0x%08x\n", (UINTN)LocalErrEntry));

  for (ErrEntryIndx = 0; ErrEntryIndx < ErrorEntryCnt; ErrEntryIndx++) {
    if (LocalErrEntry->EntryValid) {
      PciPortAddr.AddressValue = LocalErrEntry->DevAddr;
      DEBUG((EFI_D_ERROR, "  PCI Error @ Address : 0x%08x Type: %x\n", PciPortAddr.AddressValue, LocalErrEntry->DevType));

      AerCapPtr = RasFindPcieExtendedCapability (PciPortAddr.AddressValue, PCIE_EXT_AER_CAP_ID, 0xFFFF);
      PcieCapPtr = RasFindPciCapability (PciPortAddr.AddressValue, PCIE_CAP_ID);

      Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN), &GenPcieAerErrEntry);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      gBS->CopyMem (GenPcieAerErrEntry, &gGenPcieErrEntry, sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN));

      //Update Error section GUID
      gBS->CopyMem (&GenPcieAerErrEntry->GenErrorDataEntry.SectionType[0], &PcieErrorSectGuid, sizeof (EFI_GUID));

      RootErrSts.Value = LocalErrEntry->RootErrSts;

      //end-point device severity will aligned to the root port in BERT.
      SeverityType = ERROR_NONE;
      if (RootErrSts.Field.ErrCorReceived) {
        SeverityType = ERROR_SEVERITY_CORRECTED;
      }
      if (RootErrSts.Field.NonFatalErrMesgReceived) {
        SeverityType = ERROR_RECOVERABLE;
      }
      if (RootErrSts.Field.FatalErrMesgReceived) {
        SeverityType = ERROR_SEVERITY_FATAL;
      }

      GenPcieAerErrEntry->GenErrorDataEntry.ErrorSeverity = SeverityType;

      PortType = RasGetPcieDeviceType (PciPortAddr);
      PcieErrorSection = &GenPcieAerErrEntry->PcieAerErrorSection;
      PcieErrorSection->Validation.Value = 0xEF;
      PcieErrorSection->PortType = (UINT32)PortType;
      PcieErrorSection->Revision = 0x02;
      PcieErrorSection->CommandStatus = PciRead32 (PciPortAddr.AddressValue + PCI_COMMAND_REG);

      PcieErrorSection->DeviceId.VendorId = PciRead16 (PciPortAddr.AddressValue + PCI_VENDORID_REG);
      PcieErrorSection->DeviceId.DeviceId = PciRead16 (PciPortAddr.AddressValue + PCI_DEVICEID_REG);
      PcieErrorSection->DeviceId.ClassCode[0] = PciRead8 (PciPortAddr.AddressValue + PCI_CLASS_CODE_0_REG);
      PcieErrorSection->DeviceId.ClassCode[1] = PciRead8 (PciPortAddr.AddressValue + PCI_CLASS_CODE_1_REG);
      PcieErrorSection->DeviceId.ClassCode[2] = PciRead8 (PciPortAddr.AddressValue + PCI_CLASS_CODE_2_REG);
      PcieErrorSection->DeviceId.Function = (UINT8)PciPortAddr.Address.Function;
      PcieErrorSection->DeviceId.Device = (UINT8)PciPortAddr.Address.Device;
      PcieErrorSection->DeviceId.Segment = (UINT8)PciPortAddr.Address.Segment;

      // bus number information to uniquely identify the root port or bridge. Default values for both the bus numbers is zero.
      PcieErrorSection->DeviceId.PrimaryBus =
        ((PortType <= PcieDeviceLegacyEndPoint))? (UINT8)PciPortAddr.Address.Bus : PciRead8 (PciPortAddr.AddressValue + PCI_PRIMARY_BUS_REG);
      PcieErrorSection->DeviceId.SecondaryBus =
        ((PortType <= PcieDeviceLegacyEndPoint))? 0x00: PciRead8 (PciPortAddr.AddressValue + PCI_SECONDARY_BUS_REG);

      PcieErrorSection->BridgeCtrlStatus = (UINT32) (PciRead16 (PciPortAddr.AddressValue + PCI_BRIDGE_CONTROL_REG)) << 16 | (UINT32) (PciRead16 (PciPortAddr.AddressValue + PCI_SEC_STATUS_REG));

      PcieErrorSection->DeviceId.Slot = 0;

      if (PcieCapPtr != 0) {
        PcieErrorSection->DeviceId.Slot = (UINT16)((PciRead32 (PciPortAddr.AddressValue + PcieCapPtr + PCIE_SLOT_CAP_REGISTER) >> PCIE_SLOT_NUMBER_SHIFT) << 3);
        // Check PCIE CAP version
        MaxIndex = ((PciRead16 (PciPortAddr.AddressValue + PcieCapPtr + 2) & 0xF) < 2)? 9 : 15;
        for (Index = 0; Index < MaxIndex; Index++) {
          PcieErrorSection->CapabilityStructure.CapabilityData[Index] = \
                                                PciRead32 (PciPortAddr.AddressValue + (PcieCapPtr + (4 * Index)));
        }
      }

      if (AerCapPtr != 0) {
        for (Index = 0; Index < 24; Index++) {
          PcieErrorSection->AerInfo.AerInfoData[Index] = PciRead32 (PciPortAddr.AddressValue + (AerCapPtr + (4 * Index)));
        }
      }

      // Add a new record to the BERT table
      Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)GenPcieAerErrEntry,          // IN UINT8* pErrRecord
                                                           sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),       // IN UINT nSize
                                                           ERROR_TYPE_GENERIC,                   // IN UINT8 ErrorType - GENERIC error type
                                                           SeverityType                             // IN UINT8 SeverityType
                                                           );

      gBS->FreePool (GenPcieAerErrEntry);
    }
    LocalErrEntry++;
  }

  return Status;
}

EFI_STATUS
PcieErrorScanDxe (
  UINT8 NbioBusNum
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT16                PciePortIndex;
  PCIE_PORT_PROFILE     *PciePortProfileInstance;
  PCIE_ERR_ENTRY        *PcieErrorEntry;
  UINT16                ScanDeviceCnt;
  UINT16                ErrorEntryCnt;

  PciePortProfileInstance = mPlatformApeiPrivate->AmdPciePortMap->PciPortNumber;

  //Search active PCI-E port for error only
  for (PciePortIndex = 0; PciePortIndex < mPlatformApeiPrivate->AmdPciePortMap->PortCount; PciePortIndex++, PciePortProfileInstance++) {
    if (PciePortProfileInstance->NbioBusNum != NbioBusNum) {
      //find next
      continue;
    }
    DEBUG ((EFI_D_ERROR, "  PCI-E Error search at Bus: 0x%x, Port: %0d, Device count: %d\n", NbioBusNum, PciePortProfileInstance->PciPortNumber, PciePortProfileInstance->EndPointDevCnt));
    //PCI error detection
    ScanDeviceCnt = PciePortProfileInstance->EndPointDevCnt + 1;
    Status = gBS->AllocatePool (EfiBootServicesData, sizeof (PCIE_ERR_ENTRY) * ScanDeviceCnt, &PcieErrorEntry);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    ZeroMem (PcieErrorEntry, sizeof (PCIE_ERR_ENTRY) * ScanDeviceCnt);

    DEBUG ((EFI_D_ERROR, "  PCIE Error Entry Address: 0x%08x\n", (UINTN)PcieErrorEntry));
    RasPcieErrStsCheckRmb(PciePortProfileInstance, PcieErrorEntry, &ErrorEntryCnt, FALSE);
    DEBUG ((EFI_D_ERROR, "  Error Entry Counter: %d\n", ErrorEntryCnt));

    if (ErrorEntryCnt != 0) {
      //Have at least one error entry found, log the error
      PcieErrorLog(ErrorEntryCnt, PcieErrorEntry);
    }

    gBS->FreePool (PcieErrorEntry);
  }

  return Status;
}

EFI_STATUS
SmnErrorDetection ( VOID )
{
  EFI_STATUS            Status;
  GENERIC_SMN_ERR_ENTRY_GN  *GenericSmnErrEntry;
  UINT32                PmBreakEvent;
  UINT32                PmBreakEvent_Raw;
  UINT8                 DfNode;
  UINT8                 NbioBusNum;
  UINTN                 Index;
  EFI_GUID              SmnErrorSectGuid = SMN_ERROR_SECT_GUID;

  Status = gBS->AllocatePool (
      EfiBootServicesData,
      sizeof (GENERIC_SMN_ERR_ENTRY_GN),
      &GenericSmnErrEntry
      );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //Check for SMN error per IO Die.
  for (DfNode = 0; DfNode < MAX_SOCKET_SUPPORT; DfNode++) {
    Status = AcquireNbioSecBusNumHelper(DfNode, 0, &NbioBusNum);
    if (EFI_ERROR (Status)) {
      //Find next
      continue;
    }
    if (DfNode >= (sizeof(mPlatformApeiPrivate->FchAcpiMmioBase)/sizeof(mPlatformApeiPrivate->FchAcpiMmioBase[0]))) {
      continue;
    }
    if (mPlatformApeiPrivate->FchAcpiMmioBase[DfNode] == 0) {
      continue;
    }

    //Check PM BreakEvent Status
    PmBreakEvent = ACPIMMIO32 (mPlatformApeiPrivate->FchAcpiMmioBase[DfNode] + PMIO_BASE + FCH_PMIOA_REG80);
    PmBreakEvent_Raw = PmBreakEvent;

    // Keep only SMN related bits
    PmBreakEvent &= SMN_CATEGORY_MASK;
    if (PmBreakEvent == 0) {
      continue;
    }

    //Error found!!
    for (Index = 0; Index <= (UINTN)HighBitSet32 (SMN_CATEGORY_MASK); Index ++) {
      if ((PmBreakEvent & (1 << Index)) != 0) {
        gBS->CopyMem (GenericSmnErrEntry, &gGenSmnErrEntry, sizeof (GENERIC_SMN_ERR_ENTRY_GN));

        //Update Error section GUID
        gBS->CopyMem (&GenericSmnErrEntry->GenErrorDataEntry.SectionType[0], &SmnErrorSectGuid, sizeof (EFI_GUID));

        GenericSmnErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;
        GenericSmnErrEntry->SmnErrorSection.ValidationBits.Value = SMN_VALID_BIT_MAP;
        GenericSmnErrEntry->SmnErrorSection.BusId = NbioBusNum;
        GenericSmnErrEntry->SmnErrorSection.Category = SMN_CATEGORY_FATAL;
        GenericSmnErrEntry->SmnErrorSection.ErrorSource.Value = (UINT8) (PmBreakEvent & (1 << Index));

        // Add a new record to the SMN BERT table
        Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry (
            (UINT8*)GenericSmnErrEntry,           // IN UINT8* pErrRecord
            sizeof (GENERIC_SMN_ERR_ENTRY_GN),    // IN UINT nSize
            ERROR_TYPE_GENERIC,                   // IN UINT8 ErrorType - GENERIC error type
            ERROR_SEVERITY_FATAL                  // IN UINT8 SeverityType - NON-CORRECTABLE
            );
      }
    }
    // Clear any SMN sources in PM BreakEvent register
    PmBreakEvent_Raw &= ~SMN_CATEGORY_MASK;
    ACPIMMIO32 (mPlatformApeiPrivate->FchAcpiMmioBase[DfNode] + PMIO_BASE + FCH_PMIOA_REG80) = PmBreakEvent_Raw;
  }
  gBS->FreePool (GenericSmnErrEntry);

  return Status;
}

EFI_STATUS
ParsingRasHobInfo (VOID)
{
  EFI_STATUS                Status = EFI_SUCCESS;
  AMD_CPM_RAS_INFO_HOB      *AmdCpmRasInfoHob;
  CPM_PCIe_DPC_STATUS_DATA  *CpmDpcStatus;
  UINT16                    ErrorEntryCnt;
  UINT32                    RootPortNum;
  UINT32                    Index;
  UINT8                     NbioSubBus;

  AmdCpmRasInfoHob = GetFirstGuidHob (&gAmdCpmRasHobGuid);
  if (AmdCpmRasInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }
  DEBUG ((EFI_D_ERROR, "[RAS]Parsing RAS Hob Infomation\n"));

  CpmDpcStatus = &AmdCpmRasInfoHob->CpmPcieDpcStatusData;
  ErrorEntryCnt = CpmDpcStatus->size;

  if (ErrorEntryCnt !=0 && ErrorEntryCnt < CPM_MAX_NUMBER_DPCSTATUS) {
    //Print out DPC error status.
    for (Index = 0; Index < ErrorEntryCnt;Index++) {
       Status = AcquireNbioSecBusNum(CpmDpcStatus->DpcStatusArray[Index].SocketId, CpmDpcStatus->DpcStatusArray[Index].RBIndex, &NbioSubBus);
       RootPortNum = CpmDpcStatus->DpcStatusArray[Index].PCIeCoreID * MCA_PCIE_CORE0_PORT_SUPPORT + CpmDpcStatus->DpcStatusArray[Index].PCIePortID;
       //Error should already logged in AER registers so we just print out DPC status regsiter value here.
       if (RootPortNum < MAX_PCIE_PORT_SUPPORT) {
         DEBUG ((EFI_D_ERROR, " Bus: 0x%x, Device: 0x%x, Function: 0x%x, DPC Status: 0x%04x\n",
                 NbioSubBus,
                 gPciePortList[RootPortNum].Device,
                 gPciePortList[RootPortNum].Function,
                 CpmDpcStatus->DpcStatusArray[Index].DpcStatus));
       }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
NbifSysHubWDLogErr (
  IN UINT8   DieBusNum
  )
{
  EFI_STATUS                 Status;
  GENERIC_NBIO_ERR_ENTRY_GN  *GenericNbioErrEntry;
  EFI_GUID                   NbioErrorSectGuid = NBIO_ERROR_SECT_GUID;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (GENERIC_NBIO_ERR_ENTRY_GN),
                  &GenericNbioErrEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CopyMem (GenericNbioErrEntry, &gGenNbioErrEntry, sizeof (GENERIC_NBIO_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericNbioErrEntry->GenErrorDataEntry.SectionType[0], &NbioErrorSectGuid, sizeof (EFI_GUID));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorDataLength =
    (sizeof (GENERIC_NBIO_ERR_ENTRY_GN) - sizeof (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;

  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.BusId = 1;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.ErrorSource = 1;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.ErrorType = 1;

  GenericNbioErrEntry->NbioErrorSection.BusId = DieBusNum;

  GenericNbioErrEntry->NbioErrorSection.ErrorSource.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ErrorSource.Field.SyshubMm = 1;

  GenericNbioErrEntry->NbioErrorSection.ErrorType.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ErrorType.Field.WatchdogTimeOut = 1;

  // Add a new record to the NBIO BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry (
             (UINT8*)GenericNbioErrEntry,                                                              // IN UINT8* pErrRecord
             sizeof (GENERIC_NBIO_ERR_ENTRY_GN),                                                       // IN UINT nSize
             ERROR_TYPE_GENERIC,                                                                       // IN UINT8 ErrorType - GENERIC error type
             ERROR_SEVERITY_FATAL                                                                      // IN UINT8 SeverityType - NON-CORRECTABLE
             );

  gBS->FreePool (GenericNbioErrEntry);

  return Status;
}

EFI_STATUS
NbioEgressPoisonErrLog (
  IN UINT8    Nbio,
  IN UINT8    NbioBusNum,
  IN UINT32   NbioEgressPoisonStatusLo,
  IN UINT32   NbioEgressPoisonStatusHi
  )
{
  EFI_STATUS                 Status;
  GENERIC_NBIO_ERR_ENTRY_GN  *GenericNbioErrEntry;
  EFI_GUID                   NbioErrorSectGuid = NBIO_ERROR_SECT_GUID;
  UINT32                     ErrorRecord[4];
  UINT32                     *ErrRecordPtr;

  DEBUG ((EFI_D_ERROR, "In NbioEgressPoisonErrLog\n"));

  //ErrorRecord Format:
  //ErrorRecord[0]: NBIO0_EGRESS_POISON_STATUS_LO Address
  //ErrorRecord[1]: NBIO0_EGRESS_POISON_STATUS_LO Data
  //ErrorRecord[2]: NBIO0_EGRESS_POISON_STATUS_HI Address
  //ErrorRecord[3]: NBIO0_EGRESS_POISON_STATUS_HI Date
  ErrorRecord[0] = NBIO0_EGRESS_POISON_STATUS_LO + (NBIO_SMN_ADDR_OFFSET * Nbio);
  ErrorRecord[1] = NbioEgressPoisonStatusLo;
  ErrorRecord[2] = NBIO0_EGRESS_POISON_STATUS_HI + (NBIO_SMN_ADDR_OFFSET * Nbio);
  ErrorRecord[3] = NbioEgressPoisonStatusHi;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (GENERIC_NBIO_ERR_ENTRY_GN) + (sizeof(ErrorRecord)),
                  &GenericNbioErrEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CopyMem (GenericNbioErrEntry, &gGenNbioErrEntry, sizeof (GENERIC_NBIO_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&GenericNbioErrEntry->GenErrorDataEntry.SectionType[0], &NbioErrorSectGuid, sizeof (EFI_GUID));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorDataLength =
    (sizeof (AMD_NBIO_ERROR_RECORD) + (sizeof(ErrorRecord)));

  GenericNbioErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;

  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.BusId = 1;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.ErrorSource = 1;
  GenericNbioErrEntry->NbioErrorSection.ValidationBits.Field.ErrorType = 1;

  GenericNbioErrEntry->NbioErrorSection.BusId = NbioBusNum;

  GenericNbioErrEntry->NbioErrorSection.ErrorSource.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ErrorSource.Field.Nbio = 1;

  GenericNbioErrEntry->NbioErrorSection.ErrorType.Value = 0;
  GenericNbioErrEntry->NbioErrorSection.ErrorType.Field.PoisonError = 1;

  ErrRecordPtr = (UINT32 *)(GenericNbioErrEntry + 1);
  gBS->CopyMem (ErrRecordPtr, ErrorRecord, (sizeof(ErrorRecord)));
  // Add a new record to the NBIO BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry (
             (UINT8*)GenericNbioErrEntry,                                // IN UINT8* pErrRecord
             (sizeof (GENERIC_NBIO_ERR_ENTRY_GN) + sizeof(ErrorRecord)), // IN UINT nSize
             ERROR_TYPE_GENERIC,                                         // IN UINT8 ErrorType - GENERIC error type
             ERROR_SEVERITY_FATAL                                        // IN UINT8 SeverityType - NON-CORRECTABLE
             );

  gBS->FreePool (GenericNbioErrEntry);

  return Status;
}

EFI_STATUS
NbioEgressPoisonErrorScan (
  IN UINT8  Nbio,
  IN UINT8  NbioBusNum
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                BaseAddr;
  UINT32                NbioOffset;
  UINT32                NbioEgressPoisonStatusLo;
  UINT32                NbioEgressPoisonStatusHi;

  NbioOffset = NBIO_SMN_ADDR_OFFSET * Nbio;

  //Check NBIO Global Error Status
  BaseAddr = NBIO0_EGRESS_POISON_STATUS_LO + NbioOffset;
  RasSmnRead ((UINT32)NbioBusNum, BaseAddr, &NbioEgressPoisonStatusLo);
  DEBUG ((EFI_D_ERROR, "EgressPoisonStsLo Addr: 0x%08x, Data: 0x%08x\n", BaseAddr, NbioEgressPoisonStatusLo));
  BaseAddr = NBIO0_EGRESS_POISON_STATUS_HI + NbioOffset;
  RasSmnRead ((UINT32)NbioBusNum, BaseAddr, &NbioEgressPoisonStatusHi);
  DEBUG ((EFI_D_ERROR, "EgressPoisonStsHi Addr: 0x%08x, Data: 0x%08x\n", BaseAddr, NbioEgressPoisonStatusHi));
  if ((NbioEgressPoisonStatusLo != 0) || (NbioEgressPoisonStatusHi != 0)) {
    //Error Found
    Status = NbioEgressPoisonErrLog (Nbio, NbioBusNum, NbioEgressPoisonStatusLo, NbioEgressPoisonStatusHi);
    //Clear Status
    BaseAddr = NBIO0_EGRESS_POISON_STATUS_LO + NbioOffset;
    RasSmnWrite ((UINT32)NbioBusNum, BaseAddr, &NbioEgressPoisonStatusLo);
    BaseAddr = NBIO0_EGRESS_POISON_STATUS_HI + NbioOffset;
    RasSmnWrite ((UINT32)NbioBusNum, BaseAddr, &NbioEgressPoisonStatusHi);
  }

  return Status;
}

EFI_STATUS
UpdateCpmRasSsdt (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER *Table
  )
{
  EFI_STATUS                                Status;
  EFI_PHYSICAL_ADDRESS                      RasAcpiSmmDataAddress;
  UINT8                                     *CurrentPtr;
  UINT32                                    *CurrentSig;
  UINTN                                     TableSize;
  // EFI_PHYSICAL_ADDRESS                      EdrDsmAcpiSmmData;
  // EFI_PHYSICAL_ADDRESS                      EdrOstAcpiSmmData;
  BOOLEAN                                   RecalculateChecksum;

  RecalculateChecksum = FALSE;

  RasAcpiSmmDataAddress = 0xFFFFFFFF;
  // EdrDsmAcpiSmmData = 0xFFFFFFFF;
  // EdrOstAcpiSmmData = 0xFFFFFFFF;

  CurrentPtr = (UINT8 *)Table;
  TableSize = Table->Length;
  while ((UINTN) CurrentPtr < ((UINTN) Table + (UINTN) TableSize)) {
    CurrentSig = (UINT32 *) CurrentPtr;

    switch (*CurrentSig) {
    case (RAS_OPERATION_REGION_SIGNATURE):  //('P', 'R', 'A', 'S')
      //
      // Allocate ACPI to SMM data buffer
      //
      Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof(RAS_ACPI_SMM_DATA)),
                  &RasAcpiSmmDataAddress);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Unable to allocate ACPI NVS memory! %r\n", Status));
        break;
      }
      mPlatformApeiPrivate->RasAcpiSmmData = (RAS_ACPI_SMM_DATA *)RasAcpiSmmDataAddress;
      gBS->SetMem (mPlatformApeiPrivate->RasAcpiSmmData, sizeof(RAS_ACPI_SMM_DATA), 0);

      //Setup RAS_ACPI_SMM_DATA non-0 initial value
      mPlatformApeiPrivate->RasAcpiSmmData->Sig = RAS_ACPI_SMM_DATA_BUFFER_SIGNATURE;
      mPlatformApeiPrivate->RasAcpiSmmData->PcieOscSupp.Value = 0x1F;
      mPlatformApeiPrivate->RasAcpiSmmData->PcieOscCtrl.Value = 0x1F;
      if (PcdGet8 (PcdAmdHotPlugHandlingMode) == 3) {
        mPlatformApeiPrivate->RasAcpiSmmData->PcieOscCtrl.Field.PcieDpcCtrl = 0;
      }

      *(UINT32 *)CurrentPtr = (UINT32)(UINTN)mPlatformApeiPrivate->RasAcpiSmmData;
      DEBUG ((EFI_D_ERROR, "Platform RAS ACPI to SMM Data OperationRegion found!!! Update to 0x%08x\n", *(UINT32 *)CurrentPtr));
      RecalculateChecksum = TRUE;
      break;

    // case (EDR_DSM_OPERATION_REGION_SIGNATURE):  //('E', 'D', 'R', 'D')
    //   //
    //   // Allocate EDR_DSM_ACPI_SMM_DATA for Asynchronous NVME Hot Plug - Error Disconnect Recovery (EDR)
    //   //
    //   Status = gBS->AllocatePages (
    //               AllocateMaxAddress,
    //               EfiACPIMemoryNVS,
    //               EFI_SIZE_TO_PAGES (sizeof(EDR_DSM_ACPI_SMM_DATA)),
    //               &EdrDsmAcpiSmmData);
    //   if (EFI_ERROR (Status)) {
    //     DEBUG ((EFI_D_ERROR, "Unable to allocate EDR_DSM_ACPI_SMM_DATA ACPI NVS memory!\n"));
    //     break;
    //   }
    //   mPlatformApeiPrivate->EdrDsmAcpiSmmData = (EDR_DSM_ACPI_SMM_DATA *)EdrDsmAcpiSmmData;
    //   gBS->SetMem (mPlatformApeiPrivate->EdrDsmAcpiSmmData, sizeof(EDR_DSM_ACPI_SMM_DATA), 0);

    //   //Setup EDR_DSM_ACPI_SMM_DATA non-0 initial value
    //   //mPlatformApeiPrivate->EdrDsmAcpiSmmData->EdrDsmSwSmiCmd = PcdGet8 (PcdCpmEdrDsmSwSmiId);

    //   *(UINT32 *)CurrentPtr = (UINT32)(UINTN)mPlatformApeiPrivate->EdrDsmAcpiSmmData;
    //   DEBUG ((EFI_D_ERROR, "EDR_DSM_ACPI_SMM_DATA OperationRegion found!!! Update to 0x%08x, EdrDsmSwSmiCmd: 0x%02x\n",
    //     *(UINT32 *)CurrentPtr, mPlatformApeiPrivate->EdrDsmAcpiSmmData->EdrDsmSwSmiCmd));
    //   RecalculateChecksum = TRUE;
    //   break;

    // case (EDR_OST_OPERATION_REGION_SIGNATURE):  //('E', 'D', 'R', 'O')
    //   //
    //   // Allocate EDR_OST_ACPI_SMM_DATA for Asynchronous NVME Hot Plug - Error Disconnect Recovery (EDR)
    //   //
    //   Status = gBS->AllocatePages (
    //               AllocateMaxAddress,
    //               EfiACPIMemoryNVS,
    //               EFI_SIZE_TO_PAGES (sizeof(EDR_OST_ACPI_SMM_DATA)),
    //               &EdrOstAcpiSmmData);
    //   if (EFI_ERROR (Status)) {
    //     DEBUG ((EFI_D_ERROR, "Unable to allocate EDR_OST_ACPI_SMM_DATA ACPI NVS memory!\n"));
    //     break;
    //   }
    //   mPlatformApeiPrivate->EdrOstAcpiSmmData = (EDR_OST_ACPI_SMM_DATA *)EdrOstAcpiSmmData;
    //   gBS->SetMem (mPlatformApeiPrivate->EdrOstAcpiSmmData, sizeof(EDR_OST_ACPI_SMM_DATA), 0);

    //   //Setup EDR_DSM_ACPI_SMM_DATA non-0 initial value
    //   //mPlatformApeiPrivate->EdrOstAcpiSmmData->EdrOstSwSmiCmd = PcdGet8 (PcdCpmEdrOstSwSmiId);

    //   *(UINT32 *)CurrentPtr = (UINT32)(UINTN)mPlatformApeiPrivate->EdrOstAcpiSmmData;
    //   DEBUG ((EFI_D_ERROR, "EDR_OST_ACPI_SMM_DATA OperationRegion found!!! Update to 0x%08x, EdrOstSwSmiCmd: 0x%02x\n",
    //     *(UINT32 *)CurrentPtr, mPlatformApeiPrivate->EdrOstAcpiSmmData->EdrOstSwSmiCmd));
    //   RecalculateChecksum = TRUE;
    //   break;

    default:
      break;
    }

    CurrentPtr++;
  }

  if (RecalculateChecksum) {
    ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) Table);
    DEBUG ((EFI_D_ERROR, "UpdateCpmRasSsdt - ChecksumAcpiTable: Done\n"));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RasMiscAslInstall ( VOID )
{
  EFI_STATUS                                Status;
  UINTN                                     Index;
  INTN                                      Instance;
  UINTN                                     Size;
  UINTN                                     NumberOfHandles;
  UINTN                                     TableHandle;
  UINTN                                     TableSize;
  UINT32                                    FvStatus;
  EFI_HANDLE                                *HandleBuffer;
  EFI_FV_FILETYPE                           FileType;
  EFI_FV_FILE_ATTRIBUTES                    Attributes;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  EFI_ACPI_DESCRIPTION_HEADER               *PlatformRasGnAslBlockPtr  = NULL;
  EFI_FIRMWARE_VOLUME2_PROTOCOL              *FwVol = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID**) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &PLATFORM_RAS_RMB_ASL_GUID,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  Instance = 0;
  CurrentTable = NULL;
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &PLATFORM_RAS_RMB_ASL_GUID,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId == SIGNATURE_64 ('C', 'P', 'M', 'R', 'A', 'S', 0, 0)) {
        PlatformRasGnAslBlockPtr = (EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable;
        UpdateCpmRasSsdt ((EFI_ACPI_DESCRIPTION_HEADER *) PlatformRasGnAslBlockPtr);
        TableSize = PlatformRasGnAslBlockPtr->Length;

        //
        // Install ACPI table
        //
        TableHandle = 0;
        Status = AcpiTableProtocol->InstallAcpiTable (
                                      AcpiTableProtocol,
                                      PlatformRasGnAslBlockPtr,
                                      TableSize,
                                      &TableHandle
                                      );
      }

      Status = gBS->FreePool (CurrentTable);
      CurrentTable = NULL;
      Instance++;
    }
  }

  return Status;
}

EFI_STATUS
PcieLegacyRasErrorDection ( VOID )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 DfNode;
  UINT8                 Nbio;
  UINT8                 NbioBusNum;

  if ((mPlatformApeiPrivate->PlatRasPolicy.PcieAerReportMechanism == 2) ||
      (mPlatformApeiPrivate->PlatRasPolicy.PcieAerReportMechanism == 1)) { //OS or FW First
    DEBUG((EFI_D_ERROR, "[RAS] PCIe boot error scanning in FW First mode.\n"));

    for (DfNode = 0; DfNode < MAX_SOCKET_SUPPORT; DfNode++) {
      for (Nbio = 0; Nbio < MAX_NBIO_PER_DIE; Nbio++) {
        Status = AcquireNbioSecBusNumHelper(DfNode, Nbio, &NbioBusNum);
        if (EFI_ERROR (Status)) {
          //Find next
          continue;
        }

        Status = PcieErrorScanDxe (NbioBusNum);
        DEBUG((EFI_D_ERROR, "  NbioBusNum: 0x%02x, PcieErrorScan Status:%r\n", NbioBusNum, Status));
      }
    }
  }

  return Status;
}

EFI_STATUS
LogMemTestErr (
  IN       UINT16   Node,
  IN       UINT16   Card,
  IN       UINT16   Module
)
{
  EFI_STATUS                Status = EFI_SUCCESS;
  GENERIC_MEM_ERR_ENTRY_GN  *MemTestErrEntry;
  EFI_GUID                  MemErrorSectGuid = PLATFORM_MEMORY_SECT_GUID;

  DEBUG((EFI_D_ERROR, "  Bad memory at node: %x, channel: %x, module: %x\n", Node, Card, Module));

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (GENERIC_MEM_ERR_ENTRY_GN), &MemTestErrEntry);
  gBS->CopyMem (MemTestErrEntry, &gGenMemTestErrEntry, sizeof (GENERIC_MEM_ERR_ENTRY_GN));

  //Update Error section GUID
  gBS->CopyMem (&MemTestErrEntry->GenErrorDataEntry.SectionType[0], &MemErrorSectGuid, sizeof (EFI_GUID));

  MemTestErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;  // 0x01;
  MemTestErrEntry->MemErrorSection.ValidBits.Value = MEM_TEST_VALID_BIT_MAP;
  //Card = Socket Id
  MemTestErrEntry->MemErrorSection.Node = Node;
  //Card = Channel Id
  MemTestErrEntry->MemErrorSection.Card = Card;
  //Module = (ChipSelect & 0x03) >> 1, ChipSelect 0,1 = Module 0, ChipSelect 2,3 = Module 1
  MemTestErrEntry->MemErrorSection.Module = Module;

  UpdateMemErrFruText (MemTestErrEntry->GenErrorDataEntry.FruText,
                       MemTestErrEntry->MemErrorSection.Node,
                       MemTestErrEntry->MemErrorSection.Card,
                       MemTestErrEntry->MemErrorSection.Module,
                       mPlatformApeiPrivate->OemMemoryMapTable
                       );

  MemTestErrEntry->MemErrorSection.ErrStatus = ((1 << 18) | (4 << 8));   // Error Detected on Data Transaction | ERR_MEM (0x40400)
  MemTestErrEntry->MemErrorSection.MemErrType = PHYSICAL_MEM_MAPOUT;

  // Add a new record to the BERT table
  Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)MemTestErrEntry,           // IN UINT8* pErrRecord
                                                       sizeof (GENERIC_MEM_ERR_ENTRY_GN),   // IN UINT nSize
                                                       ERROR_TYPE_GENERIC,                  // IN UINT8 ErrorType - GENERIC error type
                                                       ERROR_SEVERITY_FATAL                 // IN UINT8 SeverityType - NON-CORRECTABLE
                                                       );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    gBS->FreePool (MemTestErrEntry);
    return EFI_SUCCESS;
  }
  gBS->FreePool (MemTestErrEntry);
  return Status;
}

EFI_STATUS
CheckMemTestErr ( VOID )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *ErrorLogServices;
  ERROR_LOG_DATA_STRUCT   *ErrorLogDataPtr;
  UINT32                  i;
  UINT16                Node;
  UINT16                Channel;
  UINT16                Module;

  DEBUG((EFI_D_ERROR, "[RAS]Check ABL memory test error\n"));
  Status = gBS->LocateProtocol(&gAmdErrorLogServiceProtocolGuid, NULL, &ErrorLogServices);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "  Locate gAmdErrorLogServiceProtocolGuid failed\n"));
    return Status;
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (ERROR_LOG_DATA_STRUCT), (VOID **)&ErrorLogDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "  Allocate buffer failed\n"));
    return Status;
  }
  ZeroMem (ErrorLogDataPtr, sizeof (ERROR_LOG_DATA_STRUCT));

  ErrorLogServices->AmdAquireErrorLogWithFlagDxe (ErrorLogServices, ErrorLogDataPtr, FALSE);
  for (i = 0; i < ErrorLogDataPtr->Count; i++) {
    if ((ABL_MEM_PMU_TRAIN_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo) ||
      (ABL_MEM_AGESA_MEMORY_TEST_ERROR == ErrorLogDataPtr->ErrorLog_Param[i].ErrorInfo)) {
      //Log error to BERT
      DEBUG((EFI_D_ERROR, "  Memory test error found at EventLog[%x]\n", i));

      Node = (UINT16)(ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 & 0xFF);            // Socket #
      Channel = (UINT16)((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 8) & 0xFF);  // Channel #

      //It is possible both DIMM 0, 1 are bad. log separately.
      Module = (UINT16)((ErrorLogDataPtr->ErrorLog_Param[i].DataParam1 >> 16) & 0x3);
      if (Module & BIT0) {
        LogMemTestErr (Node, Channel, 0);
      }
      if (Module & BIT1) {
        LogMemTestErr (Node, Channel, 1);
      }
    }
  }
  return Status;
}

EFI_STATUS
CreateMcaLookupTable ( VOID )
{
  EFI_STATUS                                           Status;
  UINTN                                                Index;
  EFI_ACPI_SDT_PROTOCOL                                *AcpiSdtProtocol;
  EFI_ACPI_TABLE_VERSION                               Version;
  UINT32                                               TableSignature;
  EFI_ACPI_SDT_HEADER                                  *TempTable;
  UINTN                                                TempTableKey;
  EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER  *Table = NULL;
  UINTN                                                TableKey;
  UINT8                                                *CurrPtr = NULL;
  UINT8                                                *EndPtr = NULL;
  MADT_APIC_ENTRY_PTR_RMB                              *ApicPtr = NULL;
  MCA_LOCALAPICID_MAPPING_STRUCTURE                    TmpMcaLocalapicidMappingStructure[256];  //256: RAS_MAX_CORES
  MCA_LOCALAPICID_MAPPING_TABLE                        *McaLocalapicidMappingTable;
  UINTN                                                TableSize;

  DEBUG ((EFI_D_ERROR, "[RAS] CreateMcaLookupTable\n"));

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Search MADT table
  //
  Index  = 0;
  TableSignature = EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE; //SIGNATURE_32('A', 'P', 'I', 'C')

  do {
    TempTable = NULL;
    Status = AcpiSdtProtocol->GetAcpiTable (
                                Index,
                                &TempTable,
                                &Version,
                                &TempTableKey
                                );
    if (EFI_ERROR (Status)) {
      //no more table
      break;
    }

    //
    // Check MADT Signture
    //
    if (TempTable->Signature == TableSignature) {
        Table = (EFI_ACPI_6_3_MULTIPLE_APIC_DESCRIPTION_TABLE_HEADER *)TempTable;
        TableKey = TempTableKey;
        break;
    }

    //GetAcpiTable will allocate a buffer to store a copy of the table and return the pointer to the caller.
    //Free the buffer in every search.
    if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
      gBS->FreePool(TempTable);
    }
    Index++;
  } while (TRUE);

  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[RAS] MADT table not found.\n"));
    return Status;
  }

  EndPtr  = (UINT8*)(Table) + (Table->Header.Length);
  CurrPtr = (UINT8 *)(Table + 1);

  Index = 0;
  while (CurrPtr < EndPtr) {
    ApicPtr = (MADT_APIC_ENTRY_PTR_RMB *)CurrPtr;

    switch (ApicPtr->CommonApicEntry.Type) {
    case EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC:
      if ((ApicPtr->LocalApicEntry.Flags & EFI_ACPI_6_3_LOCAL_APIC_ENABLED) == EFI_ACPI_6_3_LOCAL_APIC_ENABLED) {
        TmpMcaLocalapicidMappingStructure[Index].ApicId = ApicPtr->LocalApicEntry.ApicId;
        TmpMcaLocalapicidMappingStructure[Index].ProcessorUid = ApicPtr->LocalApicEntry.AcpiProcessorUid;
        Index++;
      }
      break;
    case EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC:
      if ((ApicPtr->Localx2ApicEntry.Flags & EFI_ACPI_6_3_LOCAL_APIC_ENABLED) == EFI_ACPI_6_3_LOCAL_APIC_ENABLED) {
        TmpMcaLocalapicidMappingStructure[Index].ApicId = ApicPtr->Localx2ApicEntry.X2ApicId;
        TmpMcaLocalapicidMappingStructure[Index].ProcessorUid = ApicPtr->Localx2ApicEntry.AcpiProcessorUid;
        Index++;
      }
      break;
    default:
      break;
    }
    CurrPtr = CurrPtr + ApicPtr->CommonApicEntry.Length;
  }

  TableSize = sizeof (UINT32) + (Index * sizeof(MCA_LOCALAPICID_MAPPING_STRUCTURE));
  Status = gBS->AllocatePool (
                EfiReservedMemoryType,
                TableSize,
                (VOID **)&McaLocalapicidMappingTable
                );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[RAS] CreateMcaLookupTable - Allocate buffer failed\n"));
    if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
      gBS->FreePool (TempTable);
    }
    return Status;
  }

  ZeroMem (McaLocalapicidMappingTable, TableSize);
  McaLocalapicidMappingTable->TableEntryNum = (UINT32)Index;

  //Copy to reserve memory space
  gBS->CopyMem (McaLocalapicidMappingTable->McaLocalApicIdMappingStructure, TmpMcaLocalapicidMappingStructure,
                (Index * sizeof(MCA_LOCALAPICID_MAPPING_STRUCTURE)));

  mPlatformApeiPrivate->McaLocalApicIdMappingTable = McaLocalapicidMappingTable;

  DEBUG((EFI_D_ERROR, "[RAS] CreateMcaLookupTable - Total element: %d \n",
    mPlatformApeiPrivate->McaLocalApicIdMappingTable->TableEntryNum));
  for (Index = 0; Index < mPlatformApeiPrivate->McaLocalApicIdMappingTable->TableEntryNum; Index++) {
    DEBUG((EFI_D_ERROR, "  McaLocalapicidMappingTable[%d] - ApicId = 0x%04x, ProcessorUid = 0x%04x \n",
      Index,
      mPlatformApeiPrivate->McaLocalApicIdMappingTable->McaLocalApicIdMappingStructure[Index].ApicId,
      mPlatformApeiPrivate->McaLocalApicIdMappingTable->McaLocalApicIdMappingStructure[Index].ProcessorUid));
  }

  if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
    gBS->FreePool(TempTable);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
UpdateDsdt ( VOID )
{
  EFI_STATUS              Status;
  EFI_ACPI_SDT_PROTOCOL   *AcpiSdtProtocol;
  UINTN                   AcpiTblIndex;
  EFI_ACPI_SDT_HEADER     *AcpiTbl = NULL;
  EFI_ACPI_TABLE_VERSION  Version;
  UINTN                   AcpiHandle;
  EFI_PHYSICAL_ADDRESS    Dsdt;
  EFI_PHYSICAL_ADDRESS    XDsdt;
  EFI_PHYSICAL_ADDRESS    DsdtAddr=0;
  UINT32                  DsdtSize;
  UINT8                   *CurrentPtr;
  UINT32                  *CurrentSig;
  UINT8                   *TmpPtr;
  UINT8                   *AslDeviceOpPtr;
  UINT8                   *PkgLengthPtr;
  UINT8                   PkgLengthBytecount;
  UINT32                  *DeviceNamePtr;

  DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt\n"));

  Dsdt = 0;
  XDsdt = 0;

  Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, &AcpiSdtProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: LocateProtocol EFI_ACPI_SDT_PROTOCOL_GUID failure, return status %x\n", Status));
    return EFI_NOT_AVAILABLE_YET;
  }

  for (AcpiTblIndex = 0; ; AcpiTblIndex++) {
    Status = AcpiSdtProtocol->GetAcpiTable (AcpiTblIndex, &AcpiTbl, &Version, &AcpiHandle);

    if (AcpiTbl->Signature == EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {    // ('F', 'A', 'C', 'P')
      if (Version == EFI_ACPI_TABLE_VERSION_1_0B) {
        Dsdt = (EFI_PHYSICAL_ADDRESS) (((EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE *) AcpiTbl)->Dsdt);
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: DSDT pointer = %08lX\n", Dsdt));
      }

      if ((Version & EFI_ACPI_TABLE_VERSION_X) != 0) {
        Dsdt = (EFI_PHYSICAL_ADDRESS) (((EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE *) AcpiTbl)->Dsdt);
        XDsdt = (EFI_PHYSICAL_ADDRESS) (((EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE *) AcpiTbl)->XDsdt);
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: DSDT pointer = %08lX, X_DSDT pointer = %08lX\n", Dsdt, XDsdt));
      }
    }

    //GetAcpiTable will allocate a buffer to store a copy of the table and return the pointer to the caller.
    //Free the buffer in every search.
    if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
      gBS->FreePool((VOID *) AcpiTbl);    // GetAcpiTable allocates pool, need to free it here
    }

    if (Dsdt || XDsdt) {
      break;
    }
  }

  // Choose XSDT first, then DSDT, Error if neither is set
  if (XDsdt != 0) {
    DsdtAddr = XDsdt;
    Status = EFI_SUCCESS;
  } else if (Dsdt != 0) {
    DsdtAddr = Dsdt;
    Status = EFI_SUCCESS;
  } else {
    DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: Neither DSDT nor X_DSDT pointer set, invalid\n"));
    ASSERT (FALSE);
  }

  DsdtSize = ((EFI_ACPI_SDT_HEADER *) DsdtAddr)->Length;
  CurrentPtr = (UINT8 *) DsdtAddr;

  Status = EFI_NOT_FOUND;
  while ((UINTN) CurrentPtr < ((UINTN) DsdtAddr + (UINTN) DsdtSize)) {
    CurrentSig = (UINT32 *) CurrentPtr;
    if (*CurrentSig == SIGNATURE_32 ('D', 'E', 'V', 'N')) {
      DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: 01. Signature DEVN was found\n"));
      //
      // Look for ASL DeviceOp code: 5B 82
      //
      TmpPtr = CurrentPtr;
      AslDeviceOpPtr = NULL;
      while ((UINTN) --TmpPtr > (UINTN) DsdtAddr) {
        if (*TmpPtr == 0x82) {
          if (*(TmpPtr-1) == 0x5B) {
            AslDeviceOpPtr = (TmpPtr-1);
            break;
          }
        }
      }

      if (AslDeviceOpPtr != NULL) {
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: 02. AslDeviceOpPtr was found = %08lX\n", AslDeviceOpPtr));
        //
        // Search PkgLength
        //
        PkgLengthPtr = AslDeviceOpPtr+2;
        PkgLengthBytecount = (*PkgLengthPtr) >> 6;
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: 03. PkgLength has %d bytes, PkgLengthPtr = %08lx\n", (PkgLengthBytecount + 1), PkgLengthPtr));

        //
        // Get Device Name
        //
        DeviceNamePtr = (UINT32 *)(PkgLengthPtr + (1 + PkgLengthBytecount));
        *(UINT32 *)CurrentPtr = *DeviceNamePtr;
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: 04. DeviceName = 0x%08x\n", *DeviceNamePtr));
        Status = EFI_SUCCESS;
      } else {
        DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: 02. Search for AslDeviceOpPtr failed\n"));
      }
    }
    CurrentPtr++;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[RAS] UpdateDsdt: Cannot find any DeviceNamePtr\n"));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AmdErrorLogDetection ( VOID )
{
  EFI_STATUS                            Status;
  DXE_AMD_ERROR_LOG_SERVICES_PROTOCOL   *ErrorLogServices;
  ERROR_LOG_DATA_STRUCT                 ErrorLogDataPtr;
  UINT32                                Index;
  UINT8                                 LoopA;
  UINT8                                 LoopB;
  UINT8                                 Step;
  PROC_GENERIC_ERR_ENTRY_GN             *ProcGenErrEntry;
  EFI_GUID                              ProcGenErrorSectGuid = PROCESSOR_GENERIC_SECT_GUID;
  UINT32                                ProcGenErrEntrySize;
  RAS_CPUID_FN                          RasCpuidFn;
  CHAR8                                 *ProcBrandStr;

  Status = gBS->LocateProtocol(&gAmdErrorLogServiceProtocolGuid, NULL, &ErrorLogServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[RAS] %a - Locate gAmdErrorLogServiceProtocolGuid: %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = ErrorLogServices->AmdAquireErrorLogWithFlagDxe (ErrorLogServices, &ErrorLogDataPtr, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[RAS] %a - AquireErrorLogWithFlagDxe: %r\n", __FUNCTION__, Status));
    return Status;
  }

  Index = 0;
  while (Index < ErrorLogDataPtr.Count) {
    switch (ErrorLogDataPtr.ErrorLog_Param[Index].ErrorInfo) {
    case (ABL_CCD_BIST_FAILURE):  //0x4065
      ProcGenErrEntrySize = sizeof (PROC_GENERIC_ERR_ENTRY_GN);
      Status = gBS->AllocatePool (EfiBootServicesData, ProcGenErrEntrySize, &ProcGenErrEntry);
      ZeroMem (ProcGenErrEntry, ProcGenErrEntrySize);

      //Update generic data info
      gBS->CopyMem (ProcGenErrEntry, &gProcGenErrEntry, ProcGenErrEntrySize);

      //Update Error section GUID
      gBS->CopyMem (&ProcGenErrEntry->GenErrorDataEntry.SectionType[0], &ProcGenErrorSectGuid, sizeof (EFI_GUID));

      ProcGenErrEntry->ProcGenErrorSection.ValidFields =
        (GENERIC_ERROR_PROC_TYPE_VALID | GENERIC_ERROR_PROC_ISA_VALID | GENERIC_ERROR_PROC_ERROR_TYPE_VALID | \
         GENERIC_ERROR_PROC_OPERATION_VALID | GENERIC_ERROR_PROC_VERSION_VALID  | GENERIC_ERROR_PROC_BRAND_VALID | \
         GENERIC_ERROR_PROC_TARGET_ADDR_VALID);
      DEBUG ((EFI_D_ERROR, "[RAS] %a - ValidFields: 0x%0lx\n", __FUNCTION__, ProcGenErrEntry->ProcGenErrorSection.ValidFields));

      ProcGenErrEntry->ProcGenErrorSection.Type = GENERIC_ERROR_PROC_TYPE_IA32_X64;
      ProcGenErrEntry->ProcGenErrorSection.Isa = GENERIC_ERROR_PROC_ISA_X64;
      ProcGenErrEntry->ProcGenErrorSection.ErrorType = GENERIC_ERROR_PROC_ERROR_TYPE_UNKNOWN;
      ProcGenErrEntry->ProcGenErrorSection.Operation = GENERIC_ERROR_PROC_OPERATION_GENERIC;

      //Get CPUID Information from BSP
      RasCpuidFn.FunctionId = 0x00000001;  //Register In EAX
      GetCpuId (&RasCpuidFn);
      ProcGenErrEntry->ProcGenErrorSection.VersionInfo = RasCpuidFn.EAX_Reg;

      //Get CPUID Brand String from BSP
      ProcBrandStr = ProcGenErrEntry->ProcGenErrorSection.BrandString;
      for (LoopA = 0; LoopA < 3; LoopA++) {
        RasCpuidFn.FunctionId = 0x80000002 + LoopA;  //Register In EAX: 0x80000002, 0x80000003, 0x80000004
        GetCpuId (&RasCpuidFn);
        for (LoopB = 0; LoopB < 4; LoopB++) {
          Step = (LoopB * sizeof (UINT32));
          gBS->CopyMem (ProcBrandStr + (LoopA * sizeof (UINT32) * 4) + Step, ((CHAR8 *)&(RasCpuidFn.EAX_Reg) + Step), sizeof (UINT32));
        }
      }
      ProcBrandStr[127] = '\0';
      DEBUG ((EFI_D_ERROR, "[RAS] %a - ProcBrandStr: [%a]\n", __FUNCTION__, ProcGenErrEntry->ProcGenErrorSection.BrandString));

      //CCD BIST Error Record(32 bits) => Bit[31:16] Socket, Bit[15:8] Die, Bit[7:0] CcdBistMap
      ProcGenErrEntry->ProcGenErrorSection.TargetAddr = ErrorLogDataPtr.ErrorLog_Param[Index].DataParam1;
      DEBUG ((EFI_D_ERROR, "[RAS] %a - CCD BIST Error Record(32 bits) => Bit[31:16] Socket, Bit[15:8] Die, Bit[7:0] CcdBistMap = 0x%08x \n",
              __FUNCTION__, ProcGenErrEntry->ProcGenErrorSection.TargetAddr));

      ProcGenErrEntry->GenErrorDataEntry.ErrorDataLength = sizeof (PROC_GENERIC_ERR_SEC);
      ProcGenErrEntry->GenErrorDataEntry.ErrorSeverity = ERROR_SEVERITY_FATAL;

      // Add a new record to the BERT table
      Status = AmdRasApeiRmbProtocol->AddBootErrorRecordEntry ((UINT8*)ProcGenErrEntry,      // IN UINT8* pErrRecord
                                                           ProcGenErrEntrySize,             // IN UINT nSize
                                                           ERROR_TYPE_GENERIC,              // IN UINT8 ErrorType - GENERIC error type
                                                           ERROR_SEVERITY_FATAL             // IN UINT8 SeverityType - NON-CORRECTABLE
                                                           );

      gBS->FreePool (ProcGenErrEntry);

      break;
    default:
      break;
    }
    Index++;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
AcquireNbioSecBusNumHelper (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;

  if ((mPlatformApeiPrivate->PlatRasPolicy.OpnFuseConfig != 0) && (NbioNumber == 1)) {
    return EFI_INVALID_PARAMETER;
  }
  Status = AcquireNbioSecBusNum(DfNode, NbioNumber, NbioSecBusNum);
  return Status;
}
