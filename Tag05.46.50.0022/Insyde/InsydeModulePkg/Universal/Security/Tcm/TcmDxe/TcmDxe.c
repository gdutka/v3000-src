/** @file
  This module implements TCG EFI Protocol for TCM.

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>
#include <IndustryStandard/Tpm12.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/PeImage.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/TcpaAcpi.h>

#include <Guid/GlobalVariable.h>
#include <Guid/SmBios.h>
#include <Guid/HobList.h>
#include <Guid/TcgEventHob.h>
#include <Guid/EventGroup.h>
#include <Guid/EventExitBootServiceFailed.h>
#include <Guid/TpmInstance.h>
#include <Guid/TcmInstance.h>
#include <Guid/ImageAuthentication.h>

#include <Protocol/DevicePath.h>
#include <Protocol/TcgService.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/TrEEProtocol.h>
#include <Protocol/MpService.h>
#include <Protocol/Smbios.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/TcmDeviceLib.h>
#include <Library/TcmCommandLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/H2OCpLib.h>

#include <Tcm.h>
#include <PostCode.h>
#include <TpmPolicy.h>

#define  EFI_TCG_LOG_AREA_SIZE        0x10000

#define TCG_DXE_DATA_FROM_THIS(this)  \
  BASE_CR (this, TCG_DXE_DATA, TcgProtocol)

typedef struct _TCG_DXE_DATA {
  EFI_TCG_PROTOCOL                  TcgProtocol;
  TCG_EFI_BOOT_SERVICE_CAPABILITY   BsCap;
  EFI_PHYSICAL_ADDRESS              LogAreaStartAddress;
  UINTN                             MaxLogAreaSize;
  UINTN                             EventLogSize;
  UINT8                             *LastEvent;
} TCG_DXE_DATA;

typedef struct {
  CHAR16                                  *VariableName;
  EFI_GUID                                *VendorGuid;
} VARIABLE_TYPE;

UINTN  mBootAttempts  = 0;
CHAR16 mBootVarName[] = L"BootOrder";

VARIABLE_TYPE mVariableType[] = {
  {EFI_SECURE_BOOT_MODE_NAME,    &gEfiGlobalVariableGuid},        // 1. SecureBoot Variable
  {EFI_PLATFORM_KEY_NAME,        &gEfiGlobalVariableGuid},        // 2. PK Variable
  {EFI_KEY_EXCHANGE_KEY_NAME,    &gEfiGlobalVariableGuid},        // 3. KEK Variable
  {EFI_IMAGE_SECURITY_DATABASE,  &gEfiImageSecurityDatabaseGuid}, // 4. EFI_IMAGE_SECURITY_DATABASE_GUID / EFI_IMAGE_SECURITY_DATABASE(db)
  {EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid}, // 5. EFI_IMAGE_SECURITY_DATABASE_GUID / EFI_IMAGE_SECURITY_DATABASE1(dbx)
};

BOOLEAN mTcgEventLogging = TRUE;

EFI_PHYSICAL_ADDRESS mLasa = 0;

BOOLEAN mTcmPassthruDisabled = FALSE;

/**
  Get All processors EFI_CPU_LOCATION in system. LocationBuf is allocated inside the function
  Caller is responsible to free LocationBuf.

  @param[out] LocationBuf          Returns Processor Location Buffer.
  @param[out] Num                  Returns processor number.

  @retval EFI_SUCCESS              Operation completed successfully.
  @retval EFI_UNSUPPORTED          MpService protocol not found.

**/
STATIC
EFI_STATUS
GetProcessorsCpuLocation (
    OUT  EFI_CPU_PHYSICAL_LOCATION   **LocationBuf,
    OUT  UINTN                       *Num
  )
{
  EFI_STATUS                        Status;
  EFI_MP_SERVICES_PROTOCOL          *MpProtocol;
  UINTN                             ProcessorNum;
  UINTN                             EnabledProcessorNum;
  EFI_PROCESSOR_INFORMATION         ProcessorInfo;
  EFI_CPU_PHYSICAL_LOCATION         *ProcessorLocBuf;
  UINTN                             Index;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&MpProtocol);
  if (EFI_ERROR (Status)) {
    //
    // MP protocol is not installed
    //
    return EFI_UNSUPPORTED;
  }

  Status = MpProtocol->GetNumberOfProcessors (
                         MpProtocol,
                         &ProcessorNum,
                         &EnabledProcessorNum
                         );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ProcessorLocBuf = AllocatePool(sizeof (EFI_CPU_PHYSICAL_LOCATION) * ProcessorNum);
  if (ProcessorLocBuf == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get each processor Location info
  //
  for (Index = 0; Index < ProcessorNum; Index++) {
    Status = MpProtocol->GetProcessorInfo (
                           MpProtocol,
                           Index,
                           &ProcessorInfo
                           );
    if (EFI_ERROR (Status)) {
      FreePool (ProcessorLocBuf);
      return Status;
    }

    //
    // Get all Processor Location info & measure
    //
    CopyMem (
      &ProcessorLocBuf[Index],
      &ProcessorInfo.Location,
      sizeof (EFI_CPU_PHYSICAL_LOCATION)
      );
  }

  *LocationBuf = ProcessorLocBuf;
  *Num = ProcessorNum;

  return Status;
}

/**
  This service provides EFI protocol capability information, state information
  about the TCM, and Event Log state information.

  @param[in]  This               Indicates the calling context
  @param[out] ProtocolCapability The callee allocates memory for a TCG_BOOT_SERVICE_CAPABILITY
                                 structure and fills in the fields with the EFI protocol
                                 capability information and the current TCM state information.
  @param[out] TCGFeatureFlags    This is a pointer to the feature flags. No feature
                                 flags are currently defined so this parameter
                                 MUST be set to 0. However, in the future,
                                 feature flags may be defined that, for example,
                                 enable hash algorithm agility.
  @param[out] EventLogLocation   This is a pointer to the address of the event log in memory.
  @param[out] EventLogLastEntry  If the Event Log contains more than one entry,
                                 this is a pointer to the address of the start of
                                 the last entry in the event log in memory.

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_INVALID_PARAMETER  ProtocolCapability does not match TCG capability.

**/
EFI_STATUS
EFIAPI
TcgDxeStatusCheck (
  IN      EFI_TCG_PROTOCOL                 *This,
  OUT     TCG_EFI_BOOT_SERVICE_CAPABILITY  *ProtocolCapability,
  OUT     UINT32                           *TCGFeatureFlags,
  OUT     EFI_PHYSICAL_ADDRESS             *EventLogLocation,
  OUT     EFI_PHYSICAL_ADDRESS             *EventLogLastEntry
  )
{
  TCG_DXE_DATA                      *TcgData;

  TcgData = TCG_DXE_DATA_FROM_THIS (This);

  if (ProtocolCapability != NULL) {
    *ProtocolCapability = TcgData->BsCap;
  }

  if (TCGFeatureFlags != NULL) {
    *TCGFeatureFlags = 0;
  }

  if (EventLogLocation != NULL) {
    *EventLogLocation = TcgData->LogAreaStartAddress;
  }

  if (EventLogLastEntry != NULL) {
    if (TcgData->BsCap.TPMDeactivatedFlag) {
      *EventLogLastEntry = (EFI_PHYSICAL_ADDRESS)(UINTN)0;
    } else {
      *EventLogLastEntry = (EFI_PHYSICAL_ADDRESS)(UINTN)TcgData->LastEvent;
    }
  }

  return EFI_SUCCESS;
}

/**
  Single function calculates SM3 256 digest value for all raw data. It
  combines Sm3Init(), Sm3Update() and Sm3Final().

  @param[in]  Data          Raw data to be digested.
  @param[in]  DataLen       Size of the raw data.
  @param[out] Digest        Pointer to a buffer that stores the final digest.

  @retval     EFI_SUCCESS   Always successfully calculate the final digest.
**/
EFI_STATUS
EFIAPI
TcmCommHashAll (
  IN  CONST UINT8       *Data,
  IN        UINTN       DataLen,
  OUT       TCM_DIGEST  *Digest
  )
{
  VOID     *Sm3Ctx;
  UINTN    CtxSize;

  CtxSize = Sm3GetContextSize ();
  Sm3Ctx = AllocatePool (CtxSize);
  ASSERT (Sm3Ctx != NULL);

  Sm3Init (Sm3Ctx);
  Sm3Update (Sm3Ctx, Data, DataLen);
  Sm3Final (Sm3Ctx, (UINT8 *)Digest);

  FreePool (Sm3Ctx);

  return EFI_SUCCESS;
}

/**
  This service abstracts the capability to do a hash operation on a data buffer.

  @param[in]      This             Indicates the calling context
  @param[in]      HashData         Pointer to the data buffer to be hashed
  @param[in]      HashDataLen      Length of the data buffer to be hashed
  @param[in]      AlgorithmId      Identification of the Algorithm to use for the hashing operation
  @param[in, out] HashedDataLen    Resultant length of the hashed data
  @param[in, out] HashedDataResult Resultant buffer of the hashed data

  @retval EFI_SUCCESS              Operation completed successfully.
  @retval EFI_INVALID_PARAMETER    HashDataLen is NULL.
  @retval EFI_INVALID_PARAMETER    HashDataLenResult is NULL.
  @retval EFI_OUT_OF_RESOURCES     Cannot allocate buffer of size *HashedDataLen.
  @retval EFI_UNSUPPORTED          AlgorithmId not supported.
  @retval EFI_BUFFER_TOO_SMALL     *HashedDataLen < sizeof (TCM_DIGEST).

**/
EFI_STATUS
EFIAPI
TcgDxeHashAll (
  IN      EFI_TCG_PROTOCOL          *This,
  IN      UINT8                     *HashData,
  IN      UINT64                    HashDataLen,
  IN      TCG_ALGORITHM_ID          AlgorithmId,
  IN OUT  UINT64                    *HashedDataLen,
  IN OUT  UINT8                     **HashedDataResult
  )
{
  if (HashedDataLen == NULL || HashedDataResult == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  switch (AlgorithmId) {
    case TCM_ALG_SM3_256:
      if (*HashedDataLen == 0) {
        *HashedDataLen    = sizeof (TCM_DIGEST);
        *HashedDataResult = AllocatePool ((UINTN) *HashedDataLen);
        if (*HashedDataResult == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
      }

      if (*HashedDataLen < sizeof (TCM_DIGEST)) {
        *HashedDataLen = sizeof (TCM_DIGEST);
        return EFI_BUFFER_TOO_SMALL;
      }
      *HashedDataLen = sizeof (TCM_DIGEST);

      if (*HashedDataResult == NULL) {
        *HashedDataResult = AllocatePool ((UINTN) *HashedDataLen);
        if (*HashedDataResult == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
      }

      return TcmCommHashAll (
               HashData,
               (UINTN) HashDataLen,
               (TCM_DIGEST*)*HashedDataResult
               );
    default:
      return EFI_UNSUPPORTED;
  }
}

/**
  Add a new entry to the Event Log.

  @param[in, out] EventLogPtr   Pointer to the Event Log data.
  @param[in, out] LogSize       Size of the Event Log.
  @param[in]      MaxSize       Maximum size of the Event Log.
  @param[in]      NewEventHdr   Pointer to a TCM_PCR_EVENT_HDR data structure.
  @param[in]      NewEventData  Pointer to the new event data.

  @retval EFI_SUCCESS           The new event log entry was added.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.

**/
EFI_STATUS
TcmCommLogEvent (
  IN OUT  UINT8                     **EventLogPtr,
  IN OUT  UINTN                     *LogSize,
  IN      UINTN                     MaxSize,
  IN      TCM_PCR_EVENT_HDR         *NewEventHdr,
  IN      UINT8                     *NewEventData
  )
{
  UINTN                            NewLogSize;

  //
  // Prevent Event Overflow
  //
  if ((UINTN) NewEventHdr->EventSize > MAX_UINTN - sizeof (*NewEventHdr)) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewLogSize = sizeof (*NewEventHdr) + NewEventHdr->EventSize;
  if (NewLogSize > MaxSize - *LogSize) {
    return EFI_OUT_OF_RESOURCES;
  }

  *EventLogPtr += *LogSize;
  *LogSize += NewLogSize;
  CopyMem (*EventLogPtr, NewEventHdr, sizeof (*NewEventHdr));
  CopyMem (
    *EventLogPtr + sizeof (*NewEventHdr),
    NewEventData,
    NewEventHdr->EventSize
    );
  return EFI_SUCCESS;
}

/**
  Add a new entry to the Event Log.

  @param[in] TcgData       TCG_DXE_DATA structure.
  @param[in] NewEventHdr   Pointer to a TCM_PCR_EVENT_HDR data structure.
  @param[in] NewEventData  Pointer to the new event data.

  @retval EFI_SUCCESS           The new event log entry was added.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.

**/
EFI_STATUS
EFIAPI
TcgDxeLogEventI (
  IN      TCG_DXE_DATA              *TcgData,
  IN      TCM_PCR_EVENT_HDR         *NewEventHdr,
  IN      UINT8                     *NewEventData
  )
{
  if (!mTcgEventLogging) {
   return EFI_SUCCESS;
  }

  TcgData->LastEvent = (UINT8*)(UINTN)TcgData->LogAreaStartAddress;

  return TcmCommLogEvent (
           &TcgData->LastEvent,
           &TcgData->EventLogSize,
           (UINTN)TcgData->MaxLogAreaSize,
           NewEventHdr,
           NewEventData
           );
}

/**
  This service abstracts the capability to add an entry to the Event Log.

  @param[in]      This           Indicates the calling context
  @param[in]      TCGLogData     Pointer to the start of the data buffer containing
                                 the TCG_PCR_EVENT data structure. All fields in
                                 this structure are properly filled by the caller.
  @param[in, out] EventNumber    The event number of the event just logged
  @param[in]      Flags          Indicate additional flags. Only one flag has been
                                 defined at this time, which is 0x01 and means the
                                 extend operation should not be performed. All
                                 other bits are reserved.

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES   Insufficient memory in the event log to complete this action.

**/
EFI_STATUS
EFIAPI
TcgDxeLogEvent (
  IN      EFI_TCG_PROTOCOL          *This,
  IN      TCG_PCR_EVENT             *TCGLogData,
  IN OUT  UINT32                    *EventNumber,
  IN      UINT32                    Flags
  )
{
  TCG_DXE_DATA  *TcgData;

  if (TCGLogData == NULL){
    return EFI_INVALID_PARAMETER;
  }

  TcgData = TCG_DXE_DATA_FROM_THIS (This);

  if (TcgData->BsCap.TPMDeactivatedFlag) {
    return EFI_DEVICE_ERROR;
  }

  return TcgDxeLogEventI (
           TcgData,
           (TCM_PCR_EVENT_HDR*)TCGLogData,
           TCGLogData->Event
           );
}

/**
  Convert TPM 1.2 command to TCM command.

  @param[in,out]  InputParameterBlock      Pointer to the input parameter block

**/
VOID
TcmCommandConvert (
  IN OUT UINT8                     *InputParameterBlock
  )
{
  TCM_RQU_COMMAND_HDR    *Hdr;

  Hdr = (TCM_RQU_COMMAND_HDR *)InputParameterBlock;
  Hdr->ordinal = SwapBytes32 (SwapBytes32 (Hdr->ordinal) | TCM_ORD_BASE);
}

/**
  This service is a proxy for commands to the TCM.

  @param[in]  This                     Indicates the calling context
  @param[in]  InputParameterBlockSize  Size of the input parameter block
  @param[in]  InputParameterBlock      Pointer to the input parameter block
  @param[in]  OutputParameterBlockSize Size of the output parameter block
  @param[in]  OutputParameterBlock     Pointer to the output parameter block

  @retval     EFI_SUCCESS                 Operation completed successfully.
  @retval     EFI_INVALID_PARAMETER       Invalid ordinal.
  @retval     EFI_UNSUPPORTED             Current Task Priority Level  >= EFI_TPL_CALLBACK.
  @retval     EFI_TIMEOUT                 The TIS timed-out.

**/
EFI_STATUS
EFIAPI
TcgDxePassThroughToTcm (
  IN      EFI_TCG_PROTOCOL          *This,
  IN      UINT32                    InputParameterBlockSize,
  IN      UINT8                     *InputParameterBlock,
  IN      UINT32                    OutputParameterBlockSize,
  IN      UINT8                     *OutputParameterBlock
  )
{
  if (InputParameterBlock == NULL ||
      OutputParameterBlock == NULL ||
      InputParameterBlockSize == 0 ||
      OutputParameterBlockSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (mTcmPassthruDisabled) {
    return EFI_UNSUPPORTED;
  }

  TcmCommandConvert (InputParameterBlock);

  return TcmSubmitCommand (
           InputParameterBlockSize,
           InputParameterBlock,
           &OutputParameterBlockSize,
           OutputParameterBlock
           );
}

/**
  Do a hash operation on a data buffer, extend a specific TCM PCR with the hash result,
  and add an entry to the Event Log.

  @param[in]      TcgData       TCG_DXE_DATA structure.
  @param[in]      HashData      Physical address of the start of the data buffer
                                to be hashed, extended, and logged.
  @param[in]      HashDataLen   The length, in bytes, of the buffer referenced by HashData
  @param[in, out] NewEventHdr   Pointer to a TCM_PCR_EVENT_HDR data structure.
  @param[in]      NewEventData  Pointer to the new event data.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
  @retval EFI_DEVICE_ERROR      The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
TcgDxeHashLogExtendEventI (
  IN      TCG_DXE_DATA              *TcgData,
  IN      UINT8                     *HashData,
  IN      UINT64                    HashDataLen,
  IN OUT  TCM_PCR_EVENT_HDR         *NewEventHdr,
  IN      UINT8                     *NewEventData
  )
{
  EFI_STATUS                        Status;

  if (HashData == NULL && HashDataLen > 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (HashDataLen > 0 || HashData != NULL) {
    Status = TcmCommHashAll (
               HashData,
               (UINTN) HashDataLen,
               &NewEventHdr->Digest
               );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "TcmCommHashAll Failed. %x\n", Status));
      return Status;
    }
  }

  Status = TcmExtend (
             &NewEventHdr->Digest,
             NewEventHdr->PCRIndex,
             NULL
             );
  if (!EFI_ERROR (Status)) {
    Status = TcgDxeLogEventI (TcgData, NewEventHdr, NewEventData);
  }

  return Status;
}

/**
  This service abstracts the capability to do a hash operation on a data buffer,
  extend a specific TCM PCR with the hash result, and add an entry to the Event Log

  @param[in]      This               Indicates the calling context
  @param[in]      HashData           Physical address of the start of the data buffer
                                     to be hashed, extended, and logged.
  @param[in]      HashDataLen        The length, in bytes, of the buffer referenced by HashData
  @param[in]      AlgorithmId        Identification of the Algorithm to use for the hashing operation
  @param[in, out] TCGLogData         The physical address of the start of the data
                                     buffer containing the TCG_PCR_EVENT data structure.
  @param[in, out] EventNumber        The event number of the event just logged.
  @param[out]     EventLogLastEntry  Physical address of the first byte of the entry
                                     just placed in the Event Log. If the Event Log was
                                     empty when this function was called then this physical
                                     address will be the same as the physical address of
                                     the start of the Event Log.

  @retval EFI_SUCCESS                Operation completed successfully.
  @retval EFI_UNSUPPORTED            AlgorithmId != TCM_ALG_SM3_256.
  @retval EFI_UNSUPPORTED            Current TPL >= EFI_TPL_CALLBACK.
  @retval EFI_DEVICE_ERROR           The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
TcgDxeHashLogExtendEvent (
  IN      EFI_TCG_PROTOCOL          *This,
  IN      EFI_PHYSICAL_ADDRESS      HashData,
  IN      UINT64                    HashDataLen,
  IN      TPM_ALGORITHM_ID          AlgorithmId,
  IN OUT  TCG_PCR_EVENT             *TCGLogData,
  IN OUT  UINT32                    *EventNumber,
     OUT  EFI_PHYSICAL_ADDRESS      *EventLogLastEntry
  )
{
  TCG_DXE_DATA  *TcgData;
  EFI_STATUS    Status;

  if (TCGLogData == NULL || EventLogLastEntry == NULL){
    return EFI_INVALID_PARAMETER;
  }

  TcgData = TCG_DXE_DATA_FROM_THIS (This);

  if (TcgData->BsCap.TPMDeactivatedFlag) {
    return EFI_DEVICE_ERROR;
  }

  if (AlgorithmId != TCM_ALG_SM3_256) {
    return EFI_UNSUPPORTED;
  }

  Status = TcgDxeHashLogExtendEventI (
             TcgData,
             (UINT8 *) (UINTN) HashData,
             HashDataLen,
             (TCM_PCR_EVENT_HDR *)TCGLogData,
             TCGLogData->Event
             );

  if (!EFI_ERROR(Status)){
    *EventLogLastEntry = (EFI_PHYSICAL_ADDRESS)(UINTN) TcgData->LastEvent;
  }

  return Status;
}

TCG_DXE_DATA                 mTcgDxeData = {
  {
    TcgDxeStatusCheck,
    TcgDxeHashAll,
    TcgDxeLogEvent,
    TcgDxePassThroughToTcm,
    TcgDxeHashLogExtendEvent
  },
  {
    sizeof (mTcgDxeData.BsCap),
    { 1, 2, 0, 0 },
    { 1, 2, 0, 0 },
    HASH_ALG_SM3_256, // SM3-256
    TRUE,
    FALSE
  },
  0,
  0,
  0,
  NULL
};

/**
  Initialize the Event Log and log events passed from the PEI phase.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.

**/
EFI_STATUS
EFIAPI
SetupEventLog (
  VOID
  )
{
  EFI_STATUS                              Status;
  TCG_PCR_EVENT                           *TcmEvent;
  EFI_PEI_HOB_POINTERS                    GuidHob;
  
  mTcgDxeData.LogAreaStartAddress = mLasa;
  mTcgDxeData.EventLogSize   = 0;
  mTcgDxeData.MaxLogAreaSize = EFI_TCG_LOG_AREA_SIZE;
  //
  // Determines if TCG Event Logging is needed.
  // All changes of the policy of TCG Event Logging should be done before here.
  //
  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_TCG_DXE_LOG_EVENT_I) != 0) {
    mTcgEventLogging = FALSE;
  }
  
  Status = EFI_SUCCESS;
  GuidHob.Raw = GetHobList ();
  while (!EFI_ERROR (Status) &&
         (GuidHob.Raw = GetNextGuidHob (&gTcgEventEntryHobGuid, GuidHob.Raw)) != NULL) {
    TcmEvent    = GET_GUID_HOB_DATA (GuidHob.Guid);
    GuidHob.Raw = GET_NEXT_HOB (GuidHob);
    Status = TcgDxeLogEventI (
               &mTcgDxeData,
               (TCM_PCR_EVENT_HDR *)TcmEvent,
               TcmEvent->Event
               );
  }

  return Status;
}

/**
  Measure and log an action string, and extend the measurement result into PCR[5].

  @param[in] String           A specific string that indicates an Action event.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval EFI_DEVICE_ERROR    The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
TcgMeasureAction (
  IN      CHAR8                     *String
  )
{
  TCM_PCR_EVENT_HDR                 TcmEvent;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_ACTION) != 0) {
    return EFI_SUCCESS;
  }

  TcmEvent.PCRIndex  = 5;
  TcmEvent.EventType = EV_EFI_ACTION;
  TcmEvent.EventSize = (UINT32)AsciiStrLen (String);
  return TcgDxeHashLogExtendEventI (
           &mTcgDxeData,
           (UINT8*)String,
           TcmEvent.EventSize,
           &TcmEvent,
           (UINT8 *) String
           );
}

/**
  Measure and log EFI handoff tables, and extend the measurement result into PCR[1].

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval EFI_DEVICE_ERROR    The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureHandoffTables (
  VOID
  )
{
  EFI_STATUS                        Status;
  TCM_PCR_EVENT_HDR                 TcmEvent;
  EFI_HANDOFF_TABLE_POINTERS        HandoffTables;
  UINTN                             ProcessorNum;
  EFI_CPU_PHYSICAL_LOCATION         *ProcessorLocBuf;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_HANDOFF_TABLES) != 0) {
    return EFI_SUCCESS;
  }

  ProcessorNum = 0;
  ProcessorLocBuf = NULL;
  Status = EFI_SUCCESS;

  if (PcdGet8 (PcdTpmPlatformClass) == TCG_PLATFORM_TYPE_SERVER) {
    //
    // Tcg Server spec. 
    // Measure each processor EFI_CPU_PHYSICAL_LOCATION with EV_TABLE_OF_DEVICES to PCR[1]
    //
    Status = GetProcessorsCpuLocation(&ProcessorLocBuf, &ProcessorNum);

    if (!EFI_ERROR(Status)){
      TcmEvent.PCRIndex  = 1;
      TcmEvent.EventType = EV_TABLE_OF_DEVICES;
      TcmEvent.EventSize = sizeof (HandoffTables);

      HandoffTables.NumberOfTables = 1;
      HandoffTables.TableEntry[0].VendorGuid  = gEfiMpServiceProtocolGuid;
      HandoffTables.TableEntry[0].VendorTable = ProcessorLocBuf;

      Status = TcgDxeHashLogExtendEventI (
                 &mTcgDxeData,
                 (UINT8*)(UINTN)ProcessorLocBuf,
                 sizeof(EFI_CPU_PHYSICAL_LOCATION) * ProcessorNum,
                 &TcmEvent,
                 (UINT8*)&HandoffTables
                 );

      FreePool(ProcessorLocBuf);
    }
  }

  return Status;
}

/**
  Measure and log Separator event, and extend the measurement result into a specific PCR.

  @param[in] PCRIndex         PCR index.

  @retval EFI_SUCCESS         Operation completed successfully.
  @retval EFI_DEVICE_ERROR    The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureSeparatorEvent (
  IN      TCM_PCRINDEX              PCRIndex
  )
{
  TCM_PCR_EVENT_HDR                 TcmEvent;
  UINT32                            EventData;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_SEPARATOR_EVENT) != 0) {
    return EFI_SUCCESS;
  }

  EventData = 0;
  TcmEvent.PCRIndex  = PCRIndex;
  TcmEvent.EventType = EV_SEPARATOR;
  TcmEvent.EventSize = (UINT32)sizeof (EventData);
  return TcgDxeHashLogExtendEventI (
           &mTcgDxeData,
           (UINT8 *)&EventData,
           sizeof (EventData),
           &TcmEvent,
           (UINT8 *)&EventData
           );
}

/**
  Measure and log an EFI variable, and extend the measurement result into a specific PCR.

  @param[in]  PCRIndex          PCR Index.
  @param[in]  EventType         Event type.
  @param[in]  VarName           A Null-terminated string that is the name of the vendor's variable.
  @param[in]  VendorGuid        A unique identifier for the vendor.
  @param[in]  VarData           The content of the variable data.
  @param[in]  VarSize           The size of the variable data.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureVariable (
  IN      TCM_PCRINDEX              PCRIndex,
  IN      TCG_EVENTTYPE             EventType,
  IN      CHAR16                    *VarName,
  IN      EFI_GUID                  *VendorGuid,
  IN      VOID                      *VarData,
  IN      UINTN                     VarSize
  )
{
  EFI_STATUS                        Status;
  TCM_PCR_EVENT_HDR                 TcmEvent;
  UINTN                             VarNameLength;
  EFI_VARIABLE_DATA                 *VarLog;

  VarNameLength      = StrLen (VarName);
  TcmEvent.PCRIndex  = PCRIndex;
  TcmEvent.EventType = EventType;
  TcmEvent.EventSize = (UINT32)(sizeof (*VarLog) + VarNameLength * sizeof (*VarName) + VarSize
                        - sizeof (VarLog->UnicodeName) - sizeof (VarLog->VariableData));

  VarLog = (EFI_VARIABLE_DATA *)AllocatePool (TcmEvent.EventSize);
  if (VarLog == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VarLog->VariableName       = *VendorGuid;
  VarLog->UnicodeNameLength  = VarNameLength;
  VarLog->VariableDataLength = VarSize;
  CopyMem (
     VarLog->UnicodeName,
     VarName,
     VarNameLength * sizeof (*VarName)
     );
  if (VarSize != 0) {
    CopyMem (
       (CHAR16 *)VarLog->UnicodeName + VarNameLength,
       VarData,
       VarSize
       );
  }

  if (EventType == EV_EFI_VARIABLE_DRIVER_CONFIG || EventType == EV_EFI_VARIABLE_AUTHORITY) {
    //
    // Digest is the event data (EFI_VARIABLE_DATA_TREE)
    //
    Status = TcgDxeHashLogExtendEventI (
               &mTcgDxeData,
               (UINT8*)VarLog,
               TcmEvent.EventSize,
               &TcmEvent,
               (UINT8*)VarLog
               );
  } else {
    Status = TcgDxeHashLogExtendEventI (
               &mTcgDxeData,
               (UINT8*)VarData,
               VarSize,
               &TcmEvent,
               (UINT8*)VarLog
               );
  }

  FreePool (VarLog);
  return Status;
}

/**
  Read then Measure and log an EFI variable, and extend the measurement result into a specific PCR.

  @param[in]  PCRIndex          PCR Index.
  @param[in]  EventType         Event type.
  @param[in]   VarName          A Null-terminated string that is the name of the vendor's variable.
  @param[in]   VendorGuid       A unique identifier for the vendor.
  @param[out]  VarSize          The size of the variable data.
  @param[out]  VarData          Pointer to the content of the variable.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
ReadAndMeasureVariable (
  IN      TCM_PCRINDEX                    PCRIndex,
  IN      TCG_EVENTTYPE                   EventType,
  IN      CHAR16                          *VarName,
  IN      EFI_GUID                        *VendorGuid,
  OUT     UINTN                           *VarSize,
  OUT     VOID                            **VarData
  )
{
  EFI_STATUS                              Status;

  Status = GetVariable2 (VarName, VendorGuid, VarData, VarSize);
  if (EventType == EV_EFI_VARIABLE_DRIVER_CONFIG || EventType == EV_EFI_VARIABLE_AUTHORITY) {
    if (EFI_ERROR (Status)) {
      //
      // It is valid case, so we need handle it.
      //
      *VarData = NULL;
      *VarSize = 0;
    }
  } else {
    //
    // if status error, VarData is freed and set NULL by GetVariable2
    //
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  }

  Status = MeasureVariable (
             PCRIndex,
             EventType,
             VarName,
             VendorGuid,
             *VarData,
             *VarSize
             );
  return Status;
}

/**
  Read then Measure and log an EFI boot variable, and extend the measurement result into PCR[5].

  @param[in]   VarName          A Null-terminated string that is the name of the vendor's variable.
  @param[in]   VendorGuid       A unique identifier for the vendor.
  @param[out]  VarSize          The size of the variable data.
  @param[out]  VarData          Pointer to the content of the variable.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
ReadAndMeasureBootVariable (
  IN      CHAR16                    *VarName,
  IN      EFI_GUID                  *VendorGuid,
  OUT     UINTN                     *VarSize,
  OUT     VOID                      **VarData
  )
{
  return ReadAndMeasureVariable (
           5,
           EV_EFI_VARIABLE_BOOT,
           VarName,
           VendorGuid,
           VarSize,
           VarData
           );
}

/**
  Read then Measure and log an EFI Secure variable, and extend the measurement result into PCR[7].

  @param[in]   VarName          A Null-terminated string that is the name of the vendor's variable.
  @param[in]   VendorGuid       A unique identifier for the vendor.
  @param[out]  VarSize          The size of the variable data.
  @param[out]  VarData          Pointer to the content of the variable.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
ReadAndMeasureSecureVariable (
  IN      CHAR16                          *VarName,
  IN      EFI_GUID                        *VendorGuid,
  OUT     UINTN                           *VarSize,
  OUT     VOID                            **VarData
  )
{
  return ReadAndMeasureVariable (
           7,
           EV_EFI_VARIABLE_DRIVER_CONFIG,
           VarName,
           VendorGuid,
           VarSize,
           VarData
           );
}

/**
  Measure and log all EFI boot variables, and extend the measurement result into a specific PCR.

  The EFI boot variables are BootOrder and Boot#### variables.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureAllBootVariables (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINT16                            *BootOrder;
  UINTN                             BootCount;
  UINTN                             Index;
  VOID                              *BootVarData;
  UINTN                             Size;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_ALL_BOOT_VARIABLES) != 0) {
    return EFI_SUCCESS;
  }

  Status = ReadAndMeasureBootVariable (
             mBootVarName,
             &gEfiGlobalVariableGuid,
             &BootCount,
             (VOID **) &BootOrder
             );
  if (Status == EFI_NOT_FOUND) {
    return EFI_SUCCESS;
  }
  
  if (BootOrder == NULL) {
    return EFI_DEVICE_ERROR;
  }

  if (EFI_ERROR (Status)) {
    FreePool (BootOrder);
    return Status;
  }

  BootCount /= sizeof (*BootOrder);
  for (Index = 0; Index < BootCount; Index++) {
    UnicodeSPrint (mBootVarName, sizeof (mBootVarName), L"Boot%04x", BootOrder[Index]);
    Status = ReadAndMeasureBootVariable (
               mBootVarName,
               &gEfiGlobalVariableGuid,
               &Size,
               &BootVarData
               );
    if (!EFI_ERROR (Status)) {
      FreePool (BootVarData);
    }
  }

  FreePool (BootOrder);
  return EFI_SUCCESS;
}

/**
  Measure and log all EFI Secure variables, and extend the measurement result into a specific PCR.

  The EFI boot variables are BootOrder and Boot#### variables.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureAllSecureVariables (
  VOID
  )
{
  EFI_STATUS                              Status;
  VOID                                    *Data;
  UINTN                                   DataSize;
  UINTN                                   Index;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_SECURE_BOOT_VARIABLES) != 0) {
    return EFI_SUCCESS;
  }

  Status = EFI_NOT_FOUND;
  for (Index = 0; Index < sizeof (mVariableType) / sizeof (mVariableType[0]); Index++) {
    DataSize = 0;
    Data = NULL;
    Status = ReadAndMeasureSecureVariable (
               mVariableType[Index].VariableName,
               mVariableType[Index].VendorGuid,
               &DataSize,
               &Data
               );
    if (!EFI_ERROR (Status)) {
      if (Data != NULL) {
        FreePool (Data);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Measure and log launch of FirmwareDebugger, and extend the measurement result into a specific PCR.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of memory.
  @retval EFI_DEVICE_ERROR      The operation was unsuccessful.

**/
EFI_STATUS
EFIAPI
MeasureLaunchOfFirmwareDebugger (
  VOID
  )
{
  TCM_PCR_EVENT_HDR                       TcmEvent;

  if ((PcdGet32 (PcdDxeTpmPolicy) & SKIP_MEASURE_LAUNCH_OF_FIRMWARE_DEBUGGER) != 0) {
    return EFI_SUCCESS;
  }

  TcmEvent.PCRIndex  = 7;
  TcmEvent.EventType = EV_EFI_ACTION;
  TcmEvent.EventSize = sizeof(FIRMWARE_DEBUGGER_EVENT_STRING) - 1;

  return TcgDxeHashLogExtendEventI (
           &mTcgDxeData,
           (UINT8 *)FIRMWARE_DEBUGGER_EVENT_STRING,
           sizeof(FIRMWARE_DEBUGGER_EVENT_STRING) - 1,
           &TcmEvent,
           (UINT8*)FIRMWARE_DEBUGGER_EVENT_STRING
           );
}

/**
  Ready to Boot Event notification handler.

  Sequence of OS boot events is measured in this event notification handler.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
OnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                        Status;
  TCM_PCRINDEX                      PcrIndex;

  if (mBootAttempts == 0) {

    //
    // Measure handoff tables.
    //
    Status = MeasureHandoffTables ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "HOBs not Measured. Error!\n"));
    }

    //
    // Measure BootOrder & Boot#### variables.
    //
    Status = MeasureAllBootVariables ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Boot Variables not Measured. Error!\n"));
    }

    //
    // This is the first boot attempt.
    //
    Status = TcgMeasureAction (
               EFI_CALLING_EFI_APPLICATION
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%s not Measured. Error!\n", EFI_CALLING_EFI_APPLICATION));
    }

    //
    // Measure secure boot variable
    //
    Status = MeasureAllSecureVariables ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "MeasureAllSecureVariables - %r\n", Status));
    }

    //
    // Draw a line between pre-boot env and entering post-boot env.
    //
    for (PcrIndex = 0; PcrIndex < 8; PcrIndex++) {
      Status = MeasureSeparatorEvent (PcrIndex);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Seperator Event not Measured. Error!\n"));
      }
    }

    //
    // Measure GPT. It would be done in SAP driver.
    //

    //
    // Measure PE/COFF OS loader. It would be done in SAP driver.
    //

    //
    // Read & Measure variable. BootOrder already measured.
    //

  } else {
    //
    // 6. Not first attempt, meaning a return from last attempt
    //
    Status = TcgMeasureAction (
               EFI_RETURNING_FROM_EFI_APPLICATION
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%s not Measured. Error!\n", EFI_RETURNING_FROM_EFI_APPLICATION));
    }
  }

  DEBUG ((EFI_D_INFO, "TCM TcmDxe Measure Data when ReadyToBoot\n"));
  //
  // Increase boot attempt counter.
  //
  mBootAttempts++;
}

/**
  Exit Boot Services Event notification handler.

  Measure invocation and success of ExitBootServices.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
OnExitBootServices (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS    Status;

  //
  // Measure invocation of ExitBootServices,
  //
  Status = TcgMeasureAction (
             EFI_EXIT_BOOT_SERVICES_INVOCATION
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%s not Measured. Error!\n", EFI_EXIT_BOOT_SERVICES_INVOCATION));
  }

  //
  // Measure success of ExitBootServices
  //
  Status = TcgMeasureAction (
             EFI_EXIT_BOOT_SERVICES_SUCCEEDED
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%s not Measured. Error!\n", EFI_EXIT_BOOT_SERVICES_SUCCEEDED));
  }
}

/**
  Exit Boot Services Failed Event notification handler.

  Measure Failure of ExitBootServices.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
OnExitBootServicesFailed (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS    Status;

  //
  // Measure Failure of ExitBootServices,
  //
  Status = TcgMeasureAction (
             EFI_EXIT_BOOT_SERVICES_FAILED
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%s not Measured. Error!\n", EFI_EXIT_BOOT_SERVICES_FAILED));
  }
}

/**
  Get TCM Deactivated state.

  @param[out] DeactivatedFlag   Returns Deactivated state.

  @retval EFI_SUCCESS              Operation completed successfully.
  @retval EFI_DEVICE_ERROR         The operation was unsuccessful.

**/
EFI_STATUS
GetTcmStatus (
     OUT  BOOLEAN                   *DeactivatedFlag
  )
{
  EFI_STATUS                        Status;
  TCM_STCLEAR_FLAGS                 VolatileFlags;

  Status = TcmGetCapabilityFlagVolatile (&VolatileFlags);
  if (!EFI_ERROR (Status)) {
    *DeactivatedFlag = VolatileFlags.deactivated;
  }

  return Status;
}

EFI_STATUS
GetTcmDisabledFlag (
  OUT  BOOLEAN                   *DisabledFlag
  )
{
  EFI_STATUS                     Status;
  TCM_PERMANENT_FLAGS            PermanentFlags;

  Status = TcmGetCapabilityFlagPermanent (&PermanentFlags);
  if (!EFI_ERROR (Status)) {
    *DisabledFlag |= PermanentFlags.disable << 1;
  }

  return Status;
}

/**
 Turn on hot ket detection at Boot after check point.

 @param[in] Event                      The Event this notify function registered to.
 @param[in] Handle                     The handle associated with a previously registered checkpoint handler.
**/
STATIC
VOID
EFIAPI
TcmBootAfterCpHandler (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  mTcmPassthruDisabled = FALSE;
  H2OCpUnregisterHandler (Handle);
}

/**
 Turn off hot key detection at Boot before check point.

 @param[in] Event                      The Event this notify function registered to.
 @param[in] Handle                     The handle associated with a previously registered checkpoint handler.
**/
STATIC
VOID
EFIAPI
TcmBootBeforeCpHandler (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                        Status;
  H2O_CP_HANDLE                     CpHandle;

  mTcmPassthruDisabled = TRUE;
  if (FeaturePcdGet (PcdH2OBdsCpBootAfterSupported)) {
    //
    // Previous hot key status is enabled, it should restore hot key status to enabled after this boot.
    //
    Status = H2OCpRegisterHandler (&gH2OBdsCpBootAfterGuid , TcmBootAfterCpHandler , H2O_CP_MEDIUM, &CpHandle);
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpBootAfterGuid, Status));
      return;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootAfterGuid, Status));
  }
}

/**
  The driver's entry point.

  It publishes EFI TCG Protocol.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval other           Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
DriverEntry (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_EVENT                         Event;
  BOOLEAN                           DebuggerInitialized;
  H2O_CP_HANDLE                     CpHandle;

  if (!CompareGuid (PcdGetPtr (PcdTpmInstanceGuid), &gEfiTpmDeviceInstanceTcmGuid)) {
    DEBUG ((EFI_D_ERROR, "No TCM instance required!\n"));
    return EFI_UNSUPPORTED;
  }

  if (PcdGetBool (PcdTpmHide)) {
    DEBUG((EFI_D_INFO, "TCM is hidden\n"));
    return EFI_UNSUPPORTED;
  }

  Status = TisPcRequestUseTcm ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "TCM not detected!\n"));
    return Status;
  }

  POST_CODE (DXE_TCGDXE); //PostCode = 0x40, TCM initial in DXE

  Status = GetTcmDisabledFlag (&mTcgDxeData.BsCap.TPMPresentFlag);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetTcmDisabledFlag - %r\n", Status));
  }

  Status = GetTcmStatus (&mTcgDxeData.BsCap.TPMDeactivatedFlag);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "DriverEntry: TCM not working properly\n"));
    return Status;
  }

  //
  // Allocate memory for EventLog in advance to prevent S3 boot script from being modified.
  //
  mLasa = (EFI_PHYSICAL_ADDRESS)(SIZE_4GB - 1);
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  EFI_SIZE_TO_PAGES (EFI_TCG_LOG_AREA_SIZE),
                  &mLasa
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiTcgProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mTcgDxeData.TcgProtocol
                  );
  if (!EFI_ERROR (Status) && !mTcgDxeData.BsCap.TPMDeactivatedFlag) {
    //
    // Setup the log area and copy event log from hob list to it
    //
    Status = SetupEventLog ();
    ASSERT_EFI_ERROR (Status);

    //
    // Measure handoff tables, Boot#### variables etc.
    //
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               OnReadyToBoot,
               NULL,
               &Event
               );

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    OnExitBootServices,
                    NULL,
                    &gEfiEventExitBootServicesGuid,
                    &Event
                    );

    //
    // Measure Exit Boot Service failed
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    OnExitBootServicesFailed,
                    NULL,
                    &gEventExitBootServicesFailedGuid,
                    &Event
                    );

    if ((PcdGet32 (PcdDxeTpmPolicy) & CUSTOMIZE_FIREWARE_DEBUGGER_CHECK) != 0) {
      DebuggerInitialized = PcdGetBool (PcdFirmwareDebuggerInitialized);
    } else {
      DebuggerInitialized = (PcdGetBool (PcdH2ODdtSupported)
                             || PcdGetBool (PcdStatusCodeUseSerial)
                             || PcdGetBool (PcdStatusCodeUseDdt)    
                             || PcdGetBool (PcdStatusCodeUseUsb));
    }
    if (DebuggerInitialized) {
      Status = MeasureLaunchOfFirmwareDebugger ();
      DEBUG ((EFI_D_ERROR, "MeasureLaunchOfFirmwareDebugger - %r\n", Status));
    }
  }

  if (FeaturePcdGet (PcdH2OBdsCpBootBeforeSupported)) {
    //
    // Register boot before check point to disable TCG protocol.
    //
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpBootBeforeGuid,
               TcmBootBeforeCpHandler,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpBootBeforeGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootBeforeGuid, Status));
  }

  return Status;
}
