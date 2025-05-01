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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/RasRmbBaseLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/MpService.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/AmdRasApeiRmbProtocol.h>
#include "AmdRasRmbApeiDxe.h"
#include <Library/LocalApicLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE RAS_AMDRASRMBAPEIDXE_AMDRASRMBAPEIDXE_FILECODE


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_MP_SERVICES_PROTOCOL *mMpServices = NULL;
AMD_RAS_POLICY           *mAmdRasPolicy;
ADDR_DATA                *gAddrData;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
AddBertErrorRecord (
  IN UINT8       *ErrorRecord,
  IN UINT32      RecordLen,
  IN UINT8       ErrorType,
  IN UINT8       SeverityType
  );

EFI_STATUS
AddHestErrorRecord (
  IN UINT8       *ErrorRecord,
  IN UINT32      RecordLen
  );

EFI_STATUS
AmdMcaErrorAddrTranslate (
  IN       NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      UINT64             *SystemMemoryAddress,
  OUT      DIMM_INFO          *DimmInfo
  );

EFI_STATUS
AmdTranslateSysAddrToCS (
  IN       UINT64             *SystemMemoryAddress,
  OUT      NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      DIMM_INFO          *DimmInfo
  );

EFI_STATUS
AmdSearchMcaError (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
  );

VOID
CollectMcaErrorInfo (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
  );

EFI_STATUS
SearchAcpiTable (
  IN       EFI_ACPI_TABLE_PROTOCOL      *AcpiTableProtocol,
  IN       EFI_ACPI_SDT_PROTOCOL        *AcpiSdtProtocol,
  IN       UINT32                       TableSignature,
  OUT      EFI_ACPI_SDT_HEADER          **Table,
  OUT      UINTN                        *TableKey
  );

EFI_STATUS
AddMultipleHestErrorRecord (
  IN UINT8       *ErrorRecord,
  IN UINT32      RecordLen,
  IN UINT32      RecordCount
  );

VOID
ProgramCoreMcaIpIdInstanceId (
  IN       VOID  *Void
  );

VOID
CoreMcaIpIdInstanceIdInit (
  VOID
  );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern  EFI_BOOT_SERVICES       *gBS;

/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

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

/*---------------------------------------------------------------------------------------*/
/**
 * MCA_ADDR Address Translate
 *
 * Translate UMC local address into specific memory DIMM information and system address
 *
 *
 * @param[in]   NormalizedAddress      UMC memory address Information
 * @param[out]  SystemMemoryAddress    System Address
 * @param[out]  DimmInfo               DIMM information
 *
 */

EFI_STATUS
AmdMcaErrorAddrTranslate (
  IN       NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      UINT64             *SystemMemoryAddress,
  OUT      DIMM_INFO          *DimmInfo
)
{

  translate_norm_to_dram_addr (NormalizedAddress->normalizedAddr,
                               NormalizedAddress->normalizedSocketId,
                               NormalizedAddress->normalizedDieId,
                               NormalizedAddress->normalizedChannelId,
                               0,
                               &DimmInfo->ChipSelect,
                               &DimmInfo->Bank,
                               &DimmInfo->Row,
                               &DimmInfo->Column,
                               &DimmInfo->rankmul,
                               &DimmInfo->subchan
                               );

  *SystemMemoryAddress = calcSysAddr(NormalizedAddress->normalizedAddr,
                                     NormalizedAddress->normalizedSocketId,
                                     NormalizedAddress->normalizedDieId,
                                     NormalizedAddress->normalizedChannelId
                                     );
  if (0xffffffffffffffff == *SystemMemoryAddress) {   // Return with EFI_INVALID_PARAMETER on invalid system address
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * System Address Translate
 *
 * Translate system address into specific memory DIMM information and normalized address
 * information
 *
 * @param[in]   SystemMemoryAddress    System Address
 * @param[out]  NormalizedAddress      UMC memory address Information
 * @param[out]  DimmInfo               DIMM information
 *
 */

EFI_STATUS
AmdTranslateSysAddrToCS (
  IN       UINT64             *SystemMemoryAddress,
  OUT      NORMALIZED_ADDRESS *NormalizedAddress,
  OUT      DIMM_INFO          *DimmInfo
)
{

  *NormalizedAddress = calcNormAddr(*SystemMemoryAddress);

  if (0xffffffffffffffff == NormalizedAddress->normalizedAddr) {   // Return with EFI_INVALID_PARAMETER on invalid system address
    return EFI_INVALID_PARAMETER;
  }

  translate_norm_to_dram_addr (NormalizedAddress->normalizedAddr,
                               NormalizedAddress->normalizedSocketId,
                               NormalizedAddress->normalizedDieId,
                               NormalizedAddress->normalizedChannelId,
                               0,
                               &DimmInfo->ChipSelect,
                               &DimmInfo->Bank,
                               &DimmInfo->Row,
                               &DimmInfo->Column,
                               &DimmInfo->rankmul,
                               &DimmInfo->subchan
                               );

  return EFI_SUCCESS;
}

EFI_STATUS
AmdSearchMcaError (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
  )
{

  UINTN Index;
  BOOLEAN CpuMapFound = FALSE;

  if (RasMcaErrorInfo->CpuInfo.ProcessorNumber > mAmdRasPolicy->TotalNumberOfProcessors) {
    return EFI_INVALID_PARAMETER;
  }

  Index = 0;
  for (Index = 0; Index < mAmdRasPolicy->TotalNumberOfProcessors; Index++) {
      if (mAmdRasPolicy->RasCpuMap[Index].ProcessorNumber == RasMcaErrorInfo->CpuInfo.ProcessorNumber) {
        RasMcaErrorInfo->CpuInfo.SocketId = mAmdRasPolicy->RasCpuMap[Index].SocketId;
        RasMcaErrorInfo->CpuInfo.CcxId = mAmdRasPolicy->RasCpuMap[Index].CcxId;
        RasMcaErrorInfo->CpuInfo.DieId = mAmdRasPolicy->RasCpuMap[Index].DieId;
        RasMcaErrorInfo->CpuInfo.CoreId = mAmdRasPolicy->RasCpuMap[Index].CoreId;
        RasMcaErrorInfo->CpuInfo.ThreadID = mAmdRasPolicy->RasCpuMap[Index].ThreadID;
        CpuMapFound = TRUE;
        break;
      }
  }
  if (!CpuMapFound) {
      return EFI_NOT_FOUND;
  }

  //Program BSP first
  if (RasMcaErrorInfo->CpuInfo.ProcessorNumber == 0) {
    CollectMcaErrorInfo(RasMcaErrorInfo);
  } else {
    mMpServices->StartupThisAP (
                               mMpServices,
                               CollectMcaErrorInfo,
                               RasMcaErrorInfo->CpuInfo.ProcessorNumber,
                               NULL,
                               0,
                               (VOID *) RasMcaErrorInfo ,
                               NULL
    );
  }

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: AmdRasRmbApeiDxeInit
 *
 * Description
 *   Entry point of the AMD RAS RMB APEI DXE driver
 *   Install all the supported protocol
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
AmdRasRmbApeiDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  EFI_HANDLE                 Handle = NULL;
  AMD_RAS_APEIRMB_PROTOCOL   *AmdRasApeiRmbProtocol;
  UINTN                      NumberOfEnabledProcessors;
  UINTN                      NumberOfLogicProcessors = 0;

  DEBUG((EFI_D_ERROR, "[RAS] RMB AGESA RAS APEI driver entry\n"));

  // Allocate Memory for the the AMD_RAS_APEI_PROTOCOL protocol.
  Status = gBS->AllocatePool (
                              EfiBootServicesData,              // IN EFI_MEMORY_TYPE PoolType
                              sizeof (AMD_RAS_APEIRMB_PROTOCOL),  // IN UINTN Size
                              &AmdRasApeiRmbProtocol               // OUT VOID **Buffer
                              );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;    // Error detected while trying to locate pool
  }

  // Get EFI MP service
  if (mMpServices == NULL) {
    Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &mMpServices);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;    // Error detected while trying to locate pool
    }
  }

  // Get AMD Ras Policy
  Status = gBS->LocateProtocol (&gAmdRasInitDataProtocolGuid, NULL, &mAmdRasPolicy);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;    // Error detected while trying to locate pool
  }

  //Init memory address data pointer
  gAddrData = mAmdRasPolicy->AddrData;

  // Get StartThisApFrom, StartThisApTo, StartThisApSteps
  mMpServices->GetNumberOfProcessors (mMpServices, &NumberOfLogicProcessors, &NumberOfEnabledProcessors);

  DEBUG((EFI_D_ERROR, "[RAS] TotalNumberOfProcessor = %d\n", mAmdRasPolicy->TotalNumberOfProcessors));
  DEBUG((EFI_D_ERROR, "[RAS] MP Service NumberOfProcessor = %d\n", NumberOfEnabledProcessors));

  //The logical processors number should match to the avaliable processors reported from AGESA.
  ASSERT (NumberOfLogicProcessors == mAmdRasPolicy->TotalNumberOfProcessors);

  // Initialize function pointers to protocol interfaces
  AmdRasApeiRmbProtocol->McaErrorAddrTranslate = &AmdMcaErrorAddrTranslate;
  AmdRasApeiRmbProtocol->TranslateSysAddrToCS = &AmdTranslateSysAddrToCS;
  AmdRasApeiRmbProtocol->AddBootErrorRecordEntry = &AddBertErrorRecord;
  AmdRasApeiRmbProtocol->AddHestErrorSourceEntry = &AddHestErrorRecord;
  AmdRasApeiRmbProtocol->SearchMcaError = &AmdSearchMcaError;
  AmdRasApeiRmbProtocol->AddMultipleHestErrorSourceEntry = &AddMultipleHestErrorRecord;

  Status = gBS->InstallProtocolInterface (
                                          &Handle,                   // IN OUT EFI_HANDLE
                                          &gAmdRasApeiRmbProtocolGuid,// IN EFI_GUID
                                          EFI_NATIVE_INTERFACE,      // IN EFI_INITERFACE_TYPE
                                          AmdRasApeiRmbProtocol        // IN VOID* Interface
                                          );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_PROTOCOL_ERROR;
  }

  CoreMcaIpIdInstanceIdInit ();

  return (Status);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AddBertErrorRecord
 *
 *  Description:
 *     Adds a new Boot Error Record Entry to the APEI Boot Error Record Table (BERT).
 *
 *  Parameters:
 *    @param[in]     *ErrorRecord
 *    @param[in]     RecordLen
 *    @param[in]     ErrorType
 *    @param[in]     SeverityType
 *
 *    @retval         EFI_SUCCESS   Error record has been added to BERT table
 *                    EFI_UNSUPPORTED ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL   cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AddBertErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen,
  IN UINT8  ErrorType,
  IN UINT8  SeverityType
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_ACPI_TABLE_PROTOCOL   *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL     *AcpiSdtProtocol;
  EFI_ACPI_SDT_HEADER       *Table;
  UINTN                     TableKey;
  EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER   *BertTable;             // Local helper pointer
  EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE      *NewBertErrRgnTable;
  EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE      *OrgBertErrRgnTable;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Table = NULL;
  Status = SearchAcpiTable(AcpiTableProtocol, AcpiSdtProtocol, BERT_SIG, &Table, &TableKey);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  BertTable = (EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER*)Table;

  // Get pointer to original Boot Error Region Ptr
  OrgBertErrRgnTable = (EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE*) BertTable->BootErrorRegion;
  // First check to see if Error Data Entry Count has reached the maximum allowed (error count is stored in Bit[13:4])
  if (OrgBertErrRgnTable->BlockStatus.ErrorDataEntryCount == 0x3FF) {
      return EFI_VOLUME_FULL;
  }

  // Reserve ACPI system memory for new BERT Error Region Table + add-on error entries (Raw or Generic)
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, BertTable->BootErrorRegionLength + RecordLen, &NewBertErrRgnTable);
  if (EFI_ERROR (Status))  {
    return Status;
  }

  switch (ErrorType)
  {
  case ERROR_TYPE_RAW:    // Raw data Error
    // Copy Error Region Data from Original BERT table Error Region into New BERT table
    gBS->CopyMem (NewBertErrRgnTable, OrgBertErrRgnTable, BertTable->BootErrorRegionLength);
    // Copy new Error Record data at the end of the current BERT table
    gBS->CopyMem ((UINT8*) NewBertErrRgnTable + BertTable->BootErrorRegionLength, ErrorRecord, RecordLen);
    // Update the RAW data length parameter with the new data length
    NewBertErrRgnTable->RawDataLength = OrgBertErrRgnTable->RawDataLength + RecordLen;
    break;

  case ERROR_TYPE_GENERIC:     // Generic Error (Note: **According to WHEA, RAW data must follow any Generic error data entries**)
    // Copy Original Error Region raw data into New BERT Error Region data
    gBS->CopyMem (NewBertErrRgnTable, OrgBertErrRgnTable, OrgBertErrRgnTable->RawDataOffset);
    // Copy new Generic Error Record data at the end of the current BERT table Error Region Offset but before the Raw data
    gBS->CopyMem ((UINT8*) NewBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset, ErrorRecord, RecordLen);
    // Copy Original Raw Data after the new Generic Error Record Data
    if (OrgBertErrRgnTable->RawDataLength != 0) {
      gBS->CopyMem((UINT8*) (NewBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset + RecordLen),\
                   (UINT8*) (OrgBertErrRgnTable + OrgBertErrRgnTable->RawDataOffset), OrgBertErrRgnTable->RawDataLength);
    }
    // Update the Raw data offset value
    NewBertErrRgnTable->RawDataOffset = OrgBertErrRgnTable->RawDataOffset + RecordLen;
    // Update the Generic data offset value
    NewBertErrRgnTable->DataLength = OrgBertErrRgnTable->DataLength + RecordLen;
    break;
  default:
    Status = EFI_UNSUPPORTED;
      return Status;
  }

  //The Severity Type priority from high to low:
  //FATAL > RECOVERABLE > CORRECTED
  //Always keep highest severity level in Generic Error Status Block.
  switch (SeverityType)
  {
  case ERROR_RECOVERABLE:
      // If error type Correctable AND multiple corrected errors
      if (NewBertErrRgnTable->BlockStatus.MultipleCorrectableErrors == 0) {
        if (NewBertErrRgnTable->BlockStatus.CorrectableErrorValid) {
          NewBertErrRgnTable->BlockStatus.MultipleCorrectableErrors = 1;
        } else {
          NewBertErrRgnTable->BlockStatus.CorrectableErrorValid = 1;
        }
      }
      if (NewBertErrRgnTable->ErrorSeverity != ERROR_SEVERITY_FATAL) {
        NewBertErrRgnTable->ErrorSeverity = ERROR_RECOVERABLE;
      }
    break;
  case ERROR_SEVERITY_FATAL:
    // If error type Uncorrectable AND multiple uncorrectable errors
    if (SeverityType == ERROR_SEVERITY_FATAL && (NewBertErrRgnTable->BlockStatus.MultipleUncorrectableErrors == 0)) {
      // If block status uncorrectable error valid
      if (NewBertErrRgnTable->BlockStatus.UncorrectableErrorValid) {
        NewBertErrRgnTable->BlockStatus.MultipleUncorrectableErrors = 1;
      } else {
        NewBertErrRgnTable->BlockStatus.UncorrectableErrorValid = 1;
      }
    }
    NewBertErrRgnTable->ErrorSeverity = ERROR_SEVERITY_FATAL;
    break;
  case ERROR_SEVERITY_CORRECTED:
    // If error type Correctable AND multiple corrected errors
    if (NewBertErrRgnTable->BlockStatus.MultipleCorrectableErrors == 0) {
      if (NewBertErrRgnTable->BlockStatus.CorrectableErrorValid) {
        NewBertErrRgnTable->BlockStatus.MultipleCorrectableErrors = 1;
      } else {
        NewBertErrRgnTable->BlockStatus.CorrectableErrorValid = 1;
      }
    }
    if (NewBertErrRgnTable->ErrorSeverity > ERROR_SEVERITY_CORRECTED) {
      NewBertErrRgnTable->ErrorSeverity = ERROR_SEVERITY_CORRECTED;
    }
    break;
  }

  // Update Error Data Entry Count to 1 in upper nibble
  NewBertErrRgnTable->BlockStatus.ErrorDataEntryCount = NewBertErrRgnTable->BlockStatus.ErrorDataEntryCount + 1;

  // Set Boot Error Region Pointer to new Bert Boot Error Region memory location
  BertTable->BootErrorRegion = (UINT64)((UINTN) NewBertErrRgnTable);
  // Set Boot Error Region length to new Bert Boot Error Region length
  BertTable->BootErrorRegionLength = BertTable->BootErrorRegionLength + RecordLen;

  ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) BertTable);

  if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
    //
    // Remove previous table
    //
    Status = AcpiTableProtocol->UninstallAcpiTable (
                                  AcpiTableProtocol,
                                  TableKey
                                  );
    //
    // Add new table
    //
    Status = AcpiTableProtocol->InstallAcpiTable (
                                  AcpiTableProtocol,
                                  BertTable,
                                  (UINTN)BertTable->Header.Length,
                                  &TableKey
                                  );

    gBS->FreePool (BertTable);
  }

  // Free memory allocated for original Error Region record
  gBS->FreePool (OrgBertErrRgnTable);

  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AddMultipleHestErrorRecord
 *
 *  Description:
 *     Adds multiple new Hardware Error Source Record Entries to the APEI Hardware Error Source Table (HEST).
 *
 *  Parameters:
 *    @param[in]     *ErrorRecord
 *    @param[in]     RecordLen
 *    @param[in]     RecordCount
 *
 *    @retval         EFI_SUCCESS  Error record has been added to HEST table
 *                    EFI_UNSUPPORTED  ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL  Cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AddMultipleHestErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen,
  IN UINT32 RecordCount
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_ACPI_TABLE_PROTOCOL   *AcpiTableProtocol;
  EFI_ACPI_SDT_PROTOCOL     *AcpiSdtProtocol;
  EFI_ACPI_SDT_HEADER       *Table;
  UINTN                     TableKey;
  UINT8                     *HestTableEnd;
  EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER *OldHestTable;             // Local helper pointer
  EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER *NewHestTable;

  if (RecordCount == 0) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  &AcpiTableProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  &AcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Table = NULL;
  Status = SearchAcpiTable(AcpiTableProtocol, AcpiSdtProtocol, EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE, &Table, &TableKey);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "[RAS]HEST table not found !!!\n"));
    return Status;
  }

  OldHestTable = (EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER *)Table;

  Status = gBS->AllocatePool (EfiACPIMemoryNVS, OldHestTable->Header.Length + RecordLen, &NewHestTable);

  gBS->CopyMem (NewHestTable, OldHestTable, OldHestTable->Header.Length);

  HestTableEnd = (UINT8*)((UINTN)NewHestTable + NewHestTable->Header.Length);
  gBS->CopyMem (HestTableEnd, ErrorRecord, RecordLen);

  NewHestTable->Header.Length += RecordLen;
  NewHestTable->ErrorSourceCount += RecordCount;

  ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) NewHestTable);

  //
  // Remove previous table
  //
  Status = AcpiTableProtocol->UninstallAcpiTable (
                                AcpiTableProtocol,
                                TableKey
                                );
  //
  // Add new table
  //
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                NewHestTable,
                                (UINTN)NewHestTable->Header.Length,
                                &TableKey
                                );

  if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
    gBS->FreePool (OldHestTable);
  }
  gBS->FreePool (NewHestTable);

 return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AddHestErrorRecord
 *
 *  Description:
 *     Adds a new Hardware Error Source Record Entry to the APEI Hardware Error Source Table (HEST).
 *
 *  Parameters:
 *    @param[in]     *ErrorRecord
 *    @param[in]     RecordLen
 *
 *    @retval         EFI_SUCCESS  Error record has been added to HEST table
 *                    EFI_UNSUPPORTED  ErrorType passed in is unsupported
 *                    EFI_OUT_OF_RESOURCES  Could not allocate memory
 *                    EFI_VOLUME_FULL  Cannot add one more error record
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
AddHestErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen
  )
{
  return AddMultipleHestErrorRecord (ErrorRecord, RecordLen, 1);
}

VOID
CollectMcaErrorInfo (
  IN OUT   RAS_MCA_ERROR_INFO_V2* RasMcaErrorInfo
  )
{
  UINT32 i;
  UINT8 BankNum;
  UINT64 MsrData;
  MCA_STATUS_MSR McaStatusMsr;
  MCA_IPID_MSR McaIpidMsr;
  MCA_DESTAT_MSR McaDeStatusMsr;
  BOOLEAN DestatExists;

  MsrData = AsmReadMsr64 (MSR_MCG_CAP);            // MCG_CAP
  BankNum = (UINT8)(MsrData & 0xFF);
  RasMcaErrorInfo->McaBankCount = BankNum;

  for (i = 0; i < BankNum; i++) {
    McaIpidMsr.Value = AsmReadMsr64 ((MCA_EXTENSION_BASE + (i * 0x10) | MCA_IPID_OFFSET));            // MCA_IPID
    if (McaIpidMsr.Field.HardwareID == 0) {
      RasMcaErrorInfo->McaBankErrorInfo[i].McaBankNumber = i;
      RasMcaErrorInfo->McaBankErrorInfo[i].McaStatusMsr.Value = 0;
      continue;
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

    //Find error log
    McaStatusMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_STATUS_OFFSET));
    McaDeStatusMsr.Value = 0;
    if (DestatExists) {
      McaDeStatusMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_DESTAT_OFFSET));
    }

    if (McaStatusMsr.Field.Val || McaDeStatusMsr.Field.Val) {
      //Collect MSR value
      RasMcaErrorInfo->McaBankErrorInfo[i].McaBankNumber = i;
      RasMcaErrorInfo->McaBankErrorInfo[i].McaStatusMsr.Value = McaStatusMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[i].McaAddrMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_ADDR_OFFSET));
      RasMcaErrorInfo->McaBankErrorInfo[i].McaConfigMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_CONFIG_OFFSET));
      RasMcaErrorInfo->McaBankErrorInfo[i].McaIpidMsr.Value = McaIpidMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[i].McaSyndMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_SYND_OFFSET));
      RasMcaErrorInfo->McaBankErrorInfo[i].McaMisc0Msr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_MISC0_OFFSET));
      if (McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
        RasMcaErrorInfo->McaBankErrorInfo[i].McaMisc1Msr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_MISC1_OFFSET));
      }
      if (DestatExists) {
        RasMcaErrorInfo->McaBankErrorInfo[i].McaDeStatMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_DESTAT_OFFSET));
        RasMcaErrorInfo->McaBankErrorInfo[i].McaDeAddrMsr.Value = AsmReadMsr64((MCA_EXTENSION_BASE + (i * 0x10) | MCA_DEADDR_OFFSET));
      }
    }
  }
}



EFI_STATUS
SearchAcpiTable (
  IN       EFI_ACPI_TABLE_PROTOCOL      *AcpiTableProtocol,
  IN       EFI_ACPI_SDT_PROTOCOL        *AcpiSdtProtocol,
  IN       UINT32                       TableSignature,
  OUT      EFI_ACPI_SDT_HEADER          **Table,
  OUT      UINTN                        *TableKey
)
{
  EFI_STATUS                                Status;
  INTN                                      Index;
  EFI_ACPI_TABLE_VERSION                    Version;
  UINTN                                     TempTableKey;
  EFI_ACPI_SDT_HEADER                       *TempTable;

  //
  // Search table
  //
  Index  = 0;
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
    // Check Signture
    //
    if (TempTable->Signature == TableSignature) {
        *Table = TempTable;
        *TableKey = TempTableKey;
        return Status;
    }
    //GetAcpiTable will allocate a buffer to store a copy of the table and return the pointer to the caller.
    //Free the buffer in every search.
    if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
      gBS->FreePool(TempTable);
    }
    Index++;
  } while (TRUE);

  return Status;
}

VOID
ProgramCoreMcaIpIdInstanceId (
  IN       VOID  *Void
  )
{
  EFI_STATUS           Status;
  UINT32               i;
  MCA_IPID_MSR         McaIpidMsr;
  UINTN                ProcNum;
  CPU_INFO             *RasCpuMap;
  CORE_MCA_INSTANCEID  CoreMcainstanceid;

  Status = mMpServices->WhoAmI (mMpServices, &ProcNum);
  if (EFI_ERROR (Status)) {
    return;
  }

  RasCpuMap = mAmdRasPolicy->RasCpuMap;
  CoreMcainstanceid.Value = 0;
  CoreMcainstanceid.Field.ThreadID = RasCpuMap[ProcNum].ThreadID;
  CoreMcainstanceid.Field.CoreId   = RasCpuMap[ProcNum].CoreId;
  CoreMcainstanceid.Field.DieId    = RasCpuMap[ProcNum].DieId;
  CoreMcainstanceid.Field.CcxId    = RasCpuMap[ProcNum].CcxId;
  CoreMcainstanceid.Field.SocketId = RasCpuMap[ProcNum].SocketId;

  for (i = 0; i < MAX_CORE_MCA_BANK_COUNT; i++) {
    McaIpidMsr.Value = AsmReadMsr64 ((MCA_EXTENSION_BASE + (i * 0x10) | MCA_IPID_OFFSET));            // MCA_IPID
    McaIpidMsr.Field.InstanceId = CoreMcainstanceid.Value;
    McaIpidMsr.Field.InstanceIdHi = 0;
    AsmWriteMsr64 ((MCA_EXTENSION_BASE + (i * 0x10) | MCA_IPID_OFFSET), McaIpidMsr.Value);
  }

  return;
}

VOID
CoreMcaIpIdInstanceIdInit (
  VOID
  )
{
  DEBUG ((EFI_D_ERROR, "[RAS] CoreMcaIpIdInstanceIdInit - Entry\n"));

  // Execute on running APs
  mMpServices->StartupAllAPs (
      mMpServices,
      ProgramCoreMcaIpIdInstanceId,
      FALSE,
      NULL,
      0,
      NULL,
      NULL);

  // For BSP
  ProgramCoreMcaIpIdInstanceId(NULL);

  DEBUG ((EFI_D_ERROR, "[RAS] CoreMcaIpIdInstanceIdInit - Exit\n"));

  return;
}
