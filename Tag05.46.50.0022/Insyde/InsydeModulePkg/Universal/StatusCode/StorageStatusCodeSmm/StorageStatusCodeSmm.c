/** @file
  SMM driver for Storage Status Code Handler.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "StorageStatusCodeSmm.h"

EFI_SMM_RSC_HANDLER_PROTOCOL            *mRscHandlerProtocol = NULL;

/**
  Write data in the buffer to ram.

  @param  Buffer                        Pointer to the data buffer to be written.
  @param  NumberOfBytes                 Number of bytes to written to the RAM device.

  @retval EFI_SUCCESS                   The operation completed successfully.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
RamDebugWrite (
  IN  CHAR8                             *Buffer,
  IN  UINTN                             BufferSize
  )
{
  H2O_STATUS_CODE_STORAGE_LOG           *Log;
  UINTN                                 LogMaxSize;
  CHAR8                                 *LogAsciiStr;

  if (Buffer == NULL || BufferSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Log = (H2O_STATUS_CODE_STORAGE_LOG *)(UINTN) PcdGet64 (PcdStatusCodeStorageMemoryBase);
  if (Log == NULL) {
    return EFI_NOT_READY;
  }

  LogMaxSize = PcdGet32 (PcdStatusCodeStorageMemorySize);
  LogAsciiStr = (CHAR8 *) (Log + 1);
  Log->TotalSize += (UINT32) BufferSize;

  if (BufferSize > LogMaxSize) {
    BufferSize = LogMaxSize;
  }

  if (BufferSize + Log->LogSize > LogMaxSize) {
    Log->LogSize = 0;
    ZeroMem (LogAsciiStr, LogMaxSize);
  }

  CopyMem (
   LogAsciiStr + Log->LogSize,
   Buffer,
   BufferSize
   );
  Log->LogSize += (UINT32) BufferSize;

  return EFI_SUCCESS;
}

/**
  Convert status code value and extended data to readable ASCII string, send string to ram device.

  @param  CodeType                      Indicates the type of status code being reported.
  @param  CodeValue                     Describes the current status of a hardware or
                                        software entity. This includes information about the class and
                                        subclass that is used to classify the entity as well as an operation.
                                        For progress codes, the operation is the current activity.
                                        For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance                      The enumeration of a hardware or software entity within
                                        the system. A system may contain multiple entities that match a class/subclass
                                        pairing. The instance differentiates between them. An instance of 0 indicates
                                        that instance information is unavailable, not meaningful, or not relevant.
                                        Valid instance numbers start with 1.
  @param  CallerId                      This optional parameter may be used to identify the caller.
                                        This parameter allows the status code driver to apply different rules to
                                        different callers.
  @param  Data                          This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
StorageStatusCodeReportWorker (
  IN  EFI_STATUS_CODE_TYPE              CodeType,
  IN  EFI_STATUS_CODE_VALUE             Value,
  IN  UINT32                            Instance   OPTIONAL,
  IN  EFI_GUID                          *CallerId  OPTIONAL,
  IN  EFI_STATUS_CODE_DATA              *Data      OPTIONAL
  )
{
  CHAR8                                 *Filename;
  CHAR8                                 *Description;
  CHAR8                                 Buffer[STORAGE_STATUS_CODE_MAX_DEBUG_MESSAGE_LENGTH];
  CHAR8                                 *Format;
  UINT32                                ErrorLevel;
  UINT32                                LineNumber;
  BASE_LIST                             Marker;
  UINTN                                 CharCount;

  CharCount = 0;

  if (Data != NULL &&
      CompareGuid (&Data->Type, &gEfiStatusCodeDataTypeStringGuid) &&
      ((EFI_STATUS_CODE_STRING_DATA *) Data)->StringType == EfiStringAscii) {
    //
    // EFI_STATUS_CODE_STRING_DATA
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "%a",
                  ((EFI_STATUS_CODE_STRING_DATA *) Data)->String.Ascii
                  );
  } else if (Data != NULL &&
             ReportStatusCodeExtractAssertInfo (CodeType, Value, Data, &Filename, &Description, &LineNumber)) {
    //
    // ASSERT info
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "SMM_ASSERT!: %a (%d): %a\n",
                  Filename,
                  LineNumber,
                  Description
                  );
  } else if (Data != NULL &&
             ReportStatusCodeExtractDebugInfo (Data, &ErrorLevel, &Marker, &Format)) {
    //
    // DEBUG info
    //
    CharCount = AsciiBSPrint (
                  Buffer,
                  sizeof (Buffer),
                  Format,
                  Marker
                  );
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    //
    // ERROR info
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "ERROR: C%08x:V%08x I%x",
                  CodeType,
                  Value,
                  Instance
                  );

    if (CallerId != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %g",
                     CallerId
                     );
    }

    if (Data != NULL) {
      CharCount += AsciiSPrint (
                     &Buffer[CharCount],
                     (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                     " %x",
                     Data
                     );
    }

    CharCount += AsciiSPrint (
                   &Buffer[CharCount],
                   (sizeof (Buffer) - (sizeof (Buffer[0]) * CharCount)),
                   "\n"
                   );
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
    //
    // PROGRESS info
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "PROGRESS CODE: V%08x I%x\n",
                  Value,
                  Instance
                  );
  } else {
    //
    // Code type is not defined
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "Undefined: C%08x:V%08x I%x\n",
                  CodeType,
                  Value,
                  Instance
                  );
  }

  if (CharCount == 0) {
    return EFI_UNSUPPORTED;
  }

  RamDebugWrite (Buffer, CharCount);

  return EFI_SUCCESS;
}

/**
  Notification function for ReportStatusCode Handler Protocol.

  This routine is the notification function for EFI_RSC_HANDLER_PROTOCOL.

  @param  Event                         Event whose notification function is being invoked.
  @param  Context                       Pointer to the notification function's context, which is
                                        always zero in current implementation.
**/
EFI_STATUS
EFIAPI
RscHandlerProtocolCallback (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{
  EFI_STATUS                        Status;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mRscHandlerProtocol->Register (StorageStatusCodeReportWorker);

  return Status;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle                   The firmware allocated handle for the UEFI image.
  @param  SystemTable                   A pointer to the EFI System Table.

  @retval EFI_SUCCESS                   The operation completed successfully.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
StatusCodeHandlerSmmEntryPoint (
  IN  EFI_HANDLE                        ImageHandle,
  IN  EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_EVENT                             RscHandlerProtocolEvent;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  if (EFI_ERROR (Status)) {
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEfiSmmRscHandlerProtocolGuid,
                      RscHandlerProtocolCallback,
                      &RscHandlerProtocolEvent
                      );
  } else {
    RscHandlerProtocolCallback (NULL, NULL, NULL);
  }

  return EFI_SUCCESS;
}

