/** @file
  PEIM for Storage Status Code Handler.

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

#include "StorageStatusCodePei.h"

EFI_PEI_NOTIFY_DESCRIPTOR               mRscHandlerPpiNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiRscHandlerPpiGuid,
  RscHandlerPpiNotifyCallback
};

EFI_PEI_NOTIFY_DESCRIPTOR               mPeiMemoryDiscoveredNotifyDescriptor = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  RamInitialize
};

/**
  Initialize RAM log instance.

  @param  PeiServices                   General purpose services available to every PEIM.
  @param  NotifyDescriptor              Pointer of the notificaiton data structure.
  @param  Ppi                           Pointer of PPI.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
RamInitialize (
  IN  EFI_PEI_SERVICES                  **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN  VOID                              *Ppi
  )
{
  H2O_STATUS_CODE_STORAGE_LOG           *LogInRam;
  EFI_STATUS                            Status;
  EFI_BOOT_MODE                         BootMode;
  RETURN_STATUS                         PcdStatus;

  //
  // No need to enable functionality during S3.
  //
  Status = (**PeiServices).GetBootMode ((CONST EFI_PEI_SERVICES **)PeiServices, &BootMode);
  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_UNSUPPORTED;
  }

  LogInRam = AllocateRuntimePages (EFI_SIZE_TO_PAGES(sizeof(H2O_STATUS_CODE_STORAGE_LOG) + PcdGet32 (PcdStatusCodeStorageMemorySize)));
  if (LogInRam == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  LogInRam->LogSize   = 0;
  LogInRam->TotalSize = 0;

  PcdStatus = PcdSet64S (PcdStatusCodeStorageMemoryBase, (UINT64) (UINTN) LogInRam);
  ASSERT_EFI_ERROR (PcdStatus);

  return EFI_SUCCESS;
}

/**
  Initialize HOB log instance.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval EFI_OUT_OF_RESOURCES          Create HOB failed.
**/
STATIC
EFI_STATUS
HobInitialize (
  VOID
  )
{
  H2O_STATUS_CODE_STORAGE_LOG           *LogInHob;

  LogInHob = BuildGuidHob (
               &gH2OStatusCodeStorageHobGuid,
               sizeof(H2O_STATUS_CODE_STORAGE_LOG) + PcdGet32 (PcdStatusCodeStorageHobSize)
               );
  if (LogInHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  LogInHob->LogSize   = 0;
  LogInHob->TotalSize = 0;

  return EFI_SUCCESS;
}

/**
  Write data to HOB or RAM.

  @param  Buffer                        Pointer to the data buffer to be written.
  @param  BufferSize                    Buffer size in bytes to written to the RAM device.

  @retval EFI_SUCCESS                   The operation completed successfully.
  @retval Others                        An unexpected error occurred.
**/
STATIC
EFI_STATUS
StorageStatusCodeWrite (
  IN CHAR8                              *Buffer,
  IN UINTN                              BufferSize
  )
{
  H2O_STATUS_CODE_STORAGE_LOG           *Log;
  UINTN                                 LogMaxSize;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  CHAR8                                 *LogAsciiStr;

  if (Buffer == NULL || BufferSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Log = (H2O_STATUS_CODE_STORAGE_LOG *)(UINTN) PcdGet64 (PcdStatusCodeStorageMemoryBase);
  if (Log == NULL) {
    GuidHob = GetFirstGuidHob (&gH2OStatusCodeStorageHobGuid);
    if (GuidHob == NULL) {
      return EFI_NOT_FOUND;
    }
    Log = (H2O_STATUS_CODE_STORAGE_LOG *) GET_GUID_HOB_DATA (GuidHob);

    LogMaxSize = PcdGet32 (PcdStatusCodeStorageHobSize);
  } else {
    LogMaxSize = PcdGet32 (PcdStatusCodeStorageMemorySize);
  }
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
  Convert status code value and extended data to readable ASCII string, send string to HOB or RAM.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to RAM device successfully.
**/
EFI_STATUS
EFIAPI
StorageStatusCodeReportWorker (
  IN CONST  EFI_PEI_SERVICES     **PeiServices,
  IN EFI_STATUS_CODE_TYPE        CodeType,
  IN EFI_STATUS_CODE_VALUE       Value,
  IN UINT32                      Instance,
  IN CONST EFI_GUID              *CallerId,
  IN CONST EFI_STATUS_CODE_DATA  *Data OPTIONAL
  )
{
  CHAR8                                 *Filename;
  CHAR8                                 *Description;
  CHAR8                                 *Format;
  CHAR8                                 Buffer[STORAGE_STATUS_CODE_MAX_DEBUG_MESSAGE_LENGTH];
  UINT32                                ErrorLevel;
  UINT32                                LineNumber;
  UINTN                                 CharCount;
  BASE_LIST                             Marker;

  CharCount = 0;
  Buffer[0] = '\0';

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
    // Print ASSERT() information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "\nPEI_ASSERT!: %a (%d): %a\n",
                  Filename,
                  LineNumber,
                  Description
                  );
  } else if (Data != NULL &&
             ReportStatusCodeExtractDebugInfo (Data, &ErrorLevel, &Marker, &Format)) {
    //
    // Print DEBUG() information into output buffer.
    //
    CharCount = AsciiBSPrint (
                  Buffer,
                  sizeof (Buffer),
                  Format,
                  Marker
                  );
  } else if ((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    //
    // Print ERROR information into output buffer.
    //
    CharCount = AsciiSPrint (
                  Buffer,
                  sizeof (Buffer),
                  "ERROR: C%08x:V%08x I%x",
                  CodeType,
                  Value,
                  Instance
                  );

    ASSERT (CharCount > 0);

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
    // Print PROGRESS information into output buffer.
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
    // Code type is not defined.
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

  StorageStatusCodeWrite (Buffer, CharCount);

  return EFI_SUCCESS;
}

/**
  Notification function for ReportStatusCode Handler Ppi.

  This routine is the notification function for EFI_RSC_HANDLER_PPI.

  @param  PeiServices                   Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor              Address of the notification descriptor data structure. Type
                                        EFI_PEI_NOTIFY_DESCRIPTOR is defined above.
  @param  Ppi                           Address of the PPI that was installed.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
RscHandlerPpiNotifyCallback (
  IN  EFI_PEI_SERVICES                  **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN  VOID                              *Ppi
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_RSC_HANDLER_PPI               *RscHandlerPpi;

  Status = (**PeiServices).LocatePpi (
                             (CONST EFI_PEI_SERVICES **)PeiServices,
                             &gEfiPeiRscHandlerPpiGuid,
                             0,
                             NULL,
                             (VOID **) &RscHandlerPpi
                             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = HobInitialize ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Register the worker function to ReportStatusCodeRouter driver
  //
  Status = RscHandlerPpi->Register (StorageStatusCodeReportWorker);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiServicesNotifyPpi (&mPeiMemoryDiscoveredNotifyDescriptor);

  return Status;
}

/**
  Entry point function for the PEIM.

  @param  FileHandle                    Handle of the file being invoked.
  @param  PeiServices                   General purpose services available to every PEIM.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
StorageStatusCodePeiEntryPoint (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN  CONST EFI_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_RSC_HANDLER_PPI               *RscHandlerPpi;

  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gEfiPeiRscHandlerPpiGuid,
                             0,
                             NULL,
                             (VOID **) &RscHandlerPpi
                             );
  if (EFI_ERROR (Status)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mRscHandlerPpiNotify);
  } else {
    Status = RscHandlerPpiNotifyCallback ((EFI_PEI_SERVICES **) PeiServices, NULL, NULL);
  }

  return Status;
}

