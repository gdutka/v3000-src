/** @file
  DXE driver for Storage Status Code Handler.

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

#include "StorageStatusCodeDxe.h"

EFI_RSC_HANDLER_PROTOCOL                *mRscHandlerProtocol = NULL;
EFI_HANDLE                              mSimpleFileSysHandle = NULL;
EFI_EVENT                               mTimerEvent          = NULL;
H2O_STATUS_CODE_STORAGE_LOG             *mLog                = NULL;

/**
  Determine whether a given device path is target storage device.

  @param[in] BlockIoDevPath  A pointer to a block IO device path

  @retval TRUE        Given device path is target storage device
  @retval FALSE       Given device path is not target storage device
**/
STATIC
BOOLEAN
IsTargetDevice (
  IN EFI_DEVICE_PATH_PROTOCOL  *BlockIoDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL     *DevPath;
  UINTN                        DevPathSize;
  UINTN                        MaxSize;

  if (BlockIoDevPath == NULL) {
    return FALSE;
  }

  DevPath = (EFI_DEVICE_PATH_PROTOCOL *) PcdGetPtr (PcdStatusCodeStorageDevicePath);
  MaxSize = PcdGetSize (PcdStatusCodeStorageDevicePath);

  while (IsDevicePathValid (DevPath, MaxSize)) {
    DevPathSize = GetDevicePathSize (DevPath);

    if (CompareMem (BlockIoDevPath, DevPath, DevPathSize - END_DEVICE_PATH_LENGTH) == 0) {
      return TRUE;
    }

    DevPath = (EFI_DEVICE_PATH_PROTOCOL *)(((UINT8 *)DevPath) + DevPathSize);
    MaxSize -= DevPathSize;
  }

  return FALSE;
}

/**
  Record log from RAM to storage device.

  @retval EFI_SUCCESS                   The operation completed successfully
  @retval EFI_NOT_READY                 Storage device is not found
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
StorageWrite (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSys;
  CHAR8                                 *Buffer;
  UINTN                                 BufferSize;
  UINT32                                DataSize;
  CHAR8                                 *LogAsciiStr;
  CHAR16                                *FilePath;
  CHAR16                                *FilePathBuffer;
  CHAR16                                *FilePathPtr;
  EFI_FILE_HANDLE                       RootFs;
  EFI_FILE_HANDLE                       FileHandle;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  H2O_STATUS_CODE_STORAGE_LOG           *LogInHob;
  STATIC BOOLEAN                        FirstTimeWrite = TRUE;
  STATIC UINT32                         LoggedTotalSize = 0;

  if (mSimpleFileSysHandle == NULL || mLog == NULL) {
    return EFI_NOT_READY;
  }

  if (!FirstTimeWrite && LoggedTotalSize >= mLog->TotalSize) {
    //
    // No new log to be written
    //
    return EFI_SUCCESS;
  }

  Status = gBS->HandleProtocol (mSimpleFileSysHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **) &SimpleFileSys);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SimpleFileSys->OpenVolume (SimpleFileSys, &RootFs);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  FilePath = (CHAR16 *) PcdGetPtr (PcdStatusCodeStorageFilePath);

  if (FirstTimeWrite) {
    //
    // If the stale log file exist, delete it.
    // Otherwise, create the folder(s) if need.
    //
    Status = RootFs->Open (RootFs, &FileHandle, FilePath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    if (!EFI_ERROR (Status)) {
      FileHandle->Delete (FileHandle);
    } else {
      FilePathBuffer = AllocateCopyPool (StrSize (FilePath), (VOID *) FilePath);
      if (FilePathBuffer == NULL) {
        RootFs->Close (RootFs);
        return EFI_OUT_OF_RESOURCES;
      }
      for (FilePathPtr = FilePathBuffer; *FilePathPtr != CHAR_NULL;FilePathPtr++) {
        FilePathPtr = StrStr (FilePathPtr, L"\\");
        if (FilePathPtr == NULL) {
          break;
        }

        *FilePathPtr = CHAR_NULL;
        Status = RootFs->Open (
                           RootFs,
                           &FileHandle,
                           FilePathBuffer,
                           EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                           EFI_FILE_DIRECTORY
                           );
        if (EFI_ERROR (Status)) {
          Status = RootFs->Open (
                             RootFs,
                             &FileHandle,
                             FilePathBuffer,
                             EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                             EFI_FILE_ARCHIVE | EFI_FILE_DIRECTORY
                             );
          if (EFI_ERROR (Status)) {
            break;
          }
        }
        *FilePathPtr = L'\\';
        FileHandle->Close (FileHandle);
      }
      H2OFreePool ((VOID **) &FilePathBuffer);
    }
  }

  Status = RootFs->Open (
                     RootFs,
                     &FileHandle,
                     FilePath,
                     EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                     0
                     );
  if (EFI_ERROR (Status)) {
    RootFs->Close (RootFs);
    return Status;
  }

  if (FirstTimeWrite) {
    FirstTimeWrite = FALSE;

    //
    // Write log from HOB to storage device
    //
    GuidHob = GetFirstGuidHob (&gH2OStatusCodeStorageHobGuid);
    if (GuidHob != NULL) {
      LogInHob   = (H2O_STATUS_CODE_STORAGE_LOG *) GET_GUID_HOB_DATA (GuidHob);
      Buffer     = (CHAR8 *) (LogInHob + 1);
      BufferSize = (UINTN) LogInHob->LogSize;

      Status = FileHandle->Write (
                             FileHandle,
                             &BufferSize,
                             Buffer
                             );
    }
  }

  //
  // Write log from memory to storage device
  //
  DataSize        = mLog->TotalSize - LoggedTotalSize;
  LoggedTotalSize = mLog->TotalSize;
  LogAsciiStr     = (CHAR8 *) (mLog + 1);
  if (DataSize < mLog->LogSize) {
    Buffer     = LogAsciiStr + mLog->LogSize - DataSize;
    BufferSize = DataSize;
  } else {
    Buffer     = LogAsciiStr;
    BufferSize = mLog->LogSize;
  }

  FileHandle->SetPosition (FileHandle, (UINT64) -1);
  Status = FileHandle->Write (
                         FileHandle,
                         &BufferSize,
                         Buffer
                         );

  FileHandle->Close (FileHandle);
  RootFs->Close (RootFs);
  return Status;
}

/**
  Process to write log to storage device.

  @param[in]  Event                  The Event this notify function registered to.
  @param[in]  Context                Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
StorageTimerHandler (
  IN  EFI_EVENT                         Event,
  IN  VOID                              *Context
  )
{
  StorageWrite ();
}

/**
  Notification event to initialize storage device.

  @param[in] Event              The Event whose notification function is being invoked.
  @param[in] Context            Pointer to the notification function's context.
**/
VOID
EFIAPI
StorageInitialize (
  IN  EFI_EVENT                             Event,
  IN  VOID                                  *Context
  )
{
  EFI_STATUS                                Status;
  EFI_HANDLE                                *FileSystemHandles;
  UINTN                                     NumberOfHandles;
  UINTN                                     Index;
  EFI_DEVICE_PATH_PROTOCOL                  *BlockIoDevPath;
  EFI_BLOCK_IO_PROTOCOL                     *BlockIo;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL           *SimpleFileSystem;
  UINT64                                    TimerPeriodMilliSec;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &FileSystemHandles
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    if (PcdGetSize (PcdStatusCodeStorageDevicePath) < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      //
      // If no specified device path, choose first found removable and writable storage device.
      //
      Status = gBS->HandleProtocol (FileSystemHandles[Index], &gEfiBlockIoProtocolGuid, (VOID **) &BlockIo);
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (!BlockIo->Media->RemovableMedia || BlockIo->Media->ReadOnly) {
        continue;
      }
    } else {
      Status = gBS->HandleProtocol (FileSystemHandles[Index], &gEfiDevicePathProtocolGuid, (VOID **) &BlockIoDevPath);
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (!IsTargetDevice (BlockIoDevPath)) {
        continue;
      }
    }

    Status = gBS->HandleProtocol (FileSystemHandles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **) &SimpleFileSystem);
    if (!EFI_ERROR (Status)) {
      mSimpleFileSysHandle = FileSystemHandles[Index];
      break;
    }
  }
  H2OFreePool ((VOID**)&FileSystemHandles);
  if (Index == NumberOfHandles) {
    return;
  }

  gBS->CloseEvent (Event);

  //
  // Periodically write log data to storage device.
  //
  TimerPeriodMilliSec = PcdGet32 (PcdStatusCodeStorageTimerPeriodMs);
  if (TimerPeriodMilliSec != STORAGE_STATUS_CODE_TIMER_DIRECT_WRITE &&
      TimerPeriodMilliSec != STORAGE_STATUS_CODE_TIMER_DISABLED) {
    Status = gBS->CreateEvent (
                    EVT_TIMER | EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    StorageTimerHandler,
                    NULL,
                    &mTimerEvent
                    );
    if (!EFI_ERROR (Status)) {
      gBS->SetTimer (mTimerEvent, TimerPeriodic, EFI_TIMER_PERIOD_MILLISECONDS(TimerPeriodMilliSec));
    }
  }
}

/**
  Write log to ram.

  @param[out] Buffer                    Pointer to the data buffer to be written.
  @param[in]  BufferSize                Number of bytes to written to the RAM device.

  @retval EFI_SUCCESS                   The operation completed successfully.
  @retval Others                        An unexpected error occurred.
**/
EFI_STATUS
RamDebugWrite (
  OUT CHAR8                             *Buffer,
  IN  UINTN                             BufferSize
  )
{
  UINTN                                 LogMaxSize;
  CHAR8                                 *LogAsciiStr;

  if (Buffer == NULL || BufferSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (mLog == NULL) {
    return EFI_NOT_READY;
  }

  LogMaxSize = PcdGet32 (PcdStatusCodeStorageMemorySize);
  LogAsciiStr = (CHAR8 *) (mLog + 1);
  mLog->TotalSize += (UINT32) BufferSize;

  if (BufferSize > LogMaxSize) {
    BufferSize = LogMaxSize;
  }

  if (BufferSize + mLog->LogSize > LogMaxSize) {
    mLog->LogSize = 0;
    ZeroMem (LogAsciiStr, LogMaxSize);
  }

  CopyMem (
   LogAsciiStr + mLog->LogSize,
   Buffer,
   BufferSize
   );
  mLog->LogSize += (UINT32) BufferSize;

  return EFI_SUCCESS;
}

/**
  Initialize RAM log instance.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
STATIC
EFI_STATUS
RamInitialize (
  VOID
  )
{
  RETURN_STATUS                         PcdStatus;

  mLog = (H2O_STATUS_CODE_STORAGE_LOG *)(UINTN) PcdGet64 (PcdStatusCodeStorageMemoryBase);
  if (mLog != NULL) {
    return EFI_SUCCESS;
  }

  mLog = AllocateRuntimePages (EFI_SIZE_TO_PAGES(sizeof(H2O_STATUS_CODE_STORAGE_LOG) + PcdGet32 (PcdStatusCodeStorageMemorySize)));
  if (mLog == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mLog->LogSize   = 0;
  mLog->TotalSize = 0;

  PcdStatus = PcdSet64S (PcdStatusCodeStorageMemoryBase, (UINT64) (UINTN) mLog);
  ASSERT_EFI_ERROR (PcdStatus);

  return EFI_SUCCESS;
}

/**
  Release RAM log instance.

  @retval EFI_SUCCESS                   Operation is successful.
  @retval Others                        An unexpected error occurred.
**/
STATIC
EFI_STATUS
RamShutdown (
  VOID
  )
{
  VOID                                  *Log;
  RETURN_STATUS                         PcdStatus;

  if (mLog == NULL) {
    return EFI_SUCCESS;
  }

  Log  = (VOID *) mLog;
  mLog = NULL;
  PcdStatus = PcdSet64S (PcdStatusCodeStorageMemoryBase, 0);
  ASSERT_EFI_ERROR (PcdStatus);

  FreePages (Log, EFI_SIZE_TO_PAGES(sizeof(H2O_STATUS_CODE_STORAGE_LOG) + PcdGet32 (PcdStatusCodeStorageMemorySize)));

  return EFI_SUCCESS;
}

/**
  Write log from memory to storage device and then turn off the functionality.

  @param[in] Event                      The Event this notify function registered to.
  @param[in] Handle                     The handle associated with a previously registered checkpoint handler.
**/
VOID
EFIAPI
StorageStatusCodeCpCallback (
  IN  EFI_EVENT                         Event,
  IN  H2O_CP_HANDLE                     Handle
  )
{
  EFI_HOB_GUID_TYPE                     *GuidHob;
  H2O_STATUS_CODE_STORAGE_LOG           *LogInHob;

  H2OCpUnregisterHandler (Handle);

  if (mTimerEvent != NULL) {
    gBS->SetTimer (mTimerEvent, TimerCancel, 0);
    gBS->CloseEvent (mTimerEvent);
    mTimerEvent = NULL;
  }

  if (mLog == NULL) {
    return;
  }

  GuidHob = GetFirstGuidHob (&gH2OStatusCodeStorageHobGuid);
  if (GuidHob != NULL) {
    LogInHob = (H2O_STATUS_CODE_STORAGE_LOG *) GET_GUID_HOB_DATA (GuidHob);

    if (LogInHob->TotalSize > PcdGet32 (PcdStatusCodeStorageHobSize)) {
      DEBUG ((DEBUG_INFO, "StorageStatusCodeDxe: Status code log size(0x%x) exceeds PcdStatusCodeStorageHobSize(0x%x).\n", LogInHob->TotalSize, PcdGet32 (PcdStatusCodeStorageHobSize)));
    }
  }
  if (mLog->TotalSize > PcdGet32 (PcdStatusCodeStorageMemorySize)) {
    DEBUG ((DEBUG_INFO, "StorageStatusCodeDxe: Status code log size(0x%x) exceeds PcdStatusCodeStorageMemorySize(0x%x).\n", mLog->TotalSize, PcdGet32 (PcdStatusCodeStorageMemorySize)));
  }

  StorageWrite ();
  RamShutdown ();
  mSimpleFileSysHandle = NULL;
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
                  "DXE_ASSERT!: %a (%d): %a\n",
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

  if ((PcdGet32 (PcdStatusCodeStorageTimerPeriodMs) == STORAGE_STATUS_CODE_TIMER_DIRECT_WRITE) &&
      (mSimpleFileSysHandle != NULL) &&
      EfiGetCurrentTpl () <= TPL_CALLBACK) {
    StorageWrite ();
  }

  return EFI_SUCCESS;
}

/**
  Unregister status code callback functions only available at boot time from
  report status code router when exiting boot services.

  @param  Event                         Event whose notification function is being invoked.
  @param  Context                       Pointer to the notification function's context, which is
                                        always zero in current implementation.
**/
VOID
EFIAPI
UnregisterBootTimeHandlers (
  IN  EFI_EVENT                         Event,
  IN  VOID                              *Context
  )
{
  if (mRscHandlerProtocol != NULL) {
    mRscHandlerProtocol->Unregister (StorageStatusCodeReportWorker);
  }
}

/**
  Notification function for ReportStatusCode Handler Protocol.

  This routine is the notification function for EFI_RSC_HANDLER_PROTOCOL.

  @param  Event                         Event whose notification function is being invoked.
  @param  Context                       Pointer to the notification function's context, which is
                                        always zero in current implementation.
**/
VOID
EFIAPI
RscHandlerCallBack (
  IN  EFI_EVENT                         Event,
  IN  VOID                              *Context
  )
{
  EFI_STATUS                            Status;
  EFI_EVENT                             ExitBootServicesEvent;
  H2O_CP_HANDLE                         CpHandle;
  VOID                                  *Registration;

  Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid, NULL, (VOID **) &mRscHandlerProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = RamInitialize ();
  if (EFI_ERROR (Status)) {
    return;
  }

  if (PcdGetSize (PcdStatusCodeStorageFilePath) > sizeof (CHAR16)) {
    EfiCreateProtocolNotifyEvent (
      &gEfiSimpleFileSystemProtocolGuid,
      TPL_CALLBACK,
      StorageInitialize,
      NULL,
      (VOID **) &Registration
      );
  }

  Status = mRscHandlerProtocol->Register (StorageStatusCodeReportWorker, TPL_HIGH_LEVEL);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Stop writing log data to storage device at NoBootDevice or BootBefore checkpoints.
  //
  if (FeaturePcdGet (PcdH2OBdsCpNoBootDeviceSupported)) {
    H2OCpRegisterHandler (&gH2OBdsCpNoBootDeviceGuid, StorageStatusCodeCpCallback, H2O_CP_LOW, &CpHandle);
  }
  if (FeaturePcdGet (PcdH2OBdsCpBootBeforeSupported)) {
    H2OCpRegisterHandler (&gH2OBdsCpBootBeforeGuid, StorageStatusCodeCpCallback, H2O_CP_LOW, &CpHandle);
  }
  if (FeaturePcdGet (PcdH2OBdsCpLegacyBootBeforeSupported)) {
    H2OCpRegisterHandler (&gH2OBdsCpLegacyBootBeforeGuid, StorageStatusCodeCpCallback, H2O_CP_LOW, &CpHandle);
  }

  //
  // Create an event for unregister status code callback function
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterBootTimeHandlers,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServicesEvent
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
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
StorageStatusCodeDxeEntryPoint (
  IN  EFI_HANDLE                        ImageHandle,
  IN  EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_EVENT                             RscHandlerProtocolEvent;

  EfiCreateProtocolNotifyEvent (
    &gEfiRscHandlerProtocolGuid,
    TPL_NOTIFY,
    RscHandlerCallBack,
    NULL,
    &RscHandlerProtocolEvent
    );

  return EFI_SUCCESS;
}

