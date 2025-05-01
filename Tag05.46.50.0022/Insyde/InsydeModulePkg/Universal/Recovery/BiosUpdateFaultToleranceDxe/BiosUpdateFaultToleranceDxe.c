/** @file
  BIOS update fault tolerance DXE driver.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

**/

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/VariableLib.h>
#include <Library/H2OCpLib.h>
#include <Library/CapsuleUpdateCriteriaLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>
#include <Library/SeamlessRecoveryLib.h>
#include <Library/FdSupportLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/PostCodeLib.h>
#include <Library/GenericBdsLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SwapAddressRange.h>
#include <Protocol/VariableLock.h>
#include <Guid/FileSystemInfo.h>
#include <Guid/FileInfo.h>

#include <PostCode.h>

#define MAX_BOOT_OPTIONS        128

/**
  Get the offset of the boot loader file path from system partition for the boot
  device path of the current boot option

  @param[in] BootDevicePath      The device path of the boot option
  @param[out] BootFilePathOffset The offset from the boot device path of the boot
                                 loader file path

  @retval EFI_SUCCESS            The BootFilePathOffset is correctly set
  @return Others                 Unable to get boot file path offset

**/
STATIC
EFI_STATUS
GetBootFilePathDevicePathOffset (
  IN     EFI_DEVICE_PATH_PROTOCOL       *BootDevicePath,
  OUT    UINTN                          *BootFilePathDevicePathOffset
  )
{
  EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  EFI_STATUS                 Status;

  if (BootDevicePath == NULL || BootFilePathDevicePathOffset == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = EFI_NOT_FOUND;
  DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)BootDevicePath;
  while (!IsDevicePathEnd(DevicePath)) {
    if ((DevicePathType (DevicePath) == MEDIA_DEVICE_PATH) &&
      (DevicePathSubType (DevicePath) == MEDIA_FILEPATH_DP)) {
      *BootFilePathDevicePathOffset = (UINTN)((UINT8 *)DevicePath - (UINT8 *)BootDevicePath);
      Status = EFI_SUCCESS;
      break;
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }
  return Status;
}

/**
 Check if root file system has capsule image file or not.

 @param[in] SysRootHandle     A pointer to a file handle.

 @retval TRUE                  It has the capsule image file.
 @retval FALSE                 It does not has the capsule image file.
**/
STATIC
BOOLEAN
HaveCapsuleImageFile (
  IN EFI_FILE_HANDLE                     SysRootHandle
  )
{
  EFI_STATUS                             Status;
  EFI_FILE_HANDLE                        CapsuleHandle;
  UINTN                                  FileInfoSize;
  EFI_FILE_INFO                          *FileInfo;
  UINTN                                  FileSize;
  BOOLEAN                                Result;

  if (SysRootHandle == NULL) {
    return FALSE;
  }

  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            EFI_CAPSULE_FILE_PATH,
                            EFI_FILE_MODE_READ,
                            EFI_FILE_DIRECTORY
                            );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Result   = FALSE;
  FileInfo = NULL;

  FileInfoSize = 0;
  Status = CapsuleHandle->GetInfo (CapsuleHandle, &gEfiFileInfoGuid, &FileInfoSize, NULL);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    goto Exit;
  }
  FileInfo = AllocatePool (FileInfoSize);
  if (FileInfo == NULL) {
    goto Exit;
  }
  Status = CapsuleHandle->GetInfo (CapsuleHandle,&gEfiFileInfoGuid, &FileInfoSize, (VOID *) FileInfo);
  if (EFI_ERROR (Status) || FileInfo->FileSize == 0) {
    goto Exit;
  }

  FileInfoSize = (UINTN) FileInfo->FileSize;
  FreePool (FileInfo);
  FileInfo = AllocatePool (FileInfoSize);
  if (FileInfo == NULL) {
    goto Exit;
  }

  while (TRUE) {
    FileSize = FileInfoSize;
    Status = CapsuleHandle->Read (
                              CapsuleHandle,
                              &FileSize,
                              (VOID *) FileInfo
                              );
    if (EFI_ERROR (Status) || FileSize == 0) {
      break;
    }

    if (FileInfo->FileSize > 0 &&
        ((FileInfo->Attribute & EFI_FILE_DIRECTORY) != EFI_FILE_DIRECTORY) &&
        StrnCmp (FileInfo->FileName, EFI_CAPSULE_FILE_NAME, sizeof (EFI_CAPSULE_FILE_NAME) / sizeof (CHAR16) - 1) == 0) {
      Result = TRUE;
      break;
    }
  }

Exit:
  CapsuleHandle->Close (CapsuleHandle);
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }
  return Result;
}

/**
 Get the information of system drive which contain capsule image file from all simple file system instances.

 @param[out] SysRootDevicePath    The pointer of system root device path pointer
 @param[out] SysRoorHandle        The pointer of EFI_FILE_HANDLE of system drive

 @retval EFI_SUCCESS              The system drive information is correctly get
 @return Others                   Unable to get system drive information
**/
STATIC
EFI_STATUS
GetSystemRootInfoFromFileSystems (
  OUT EFI_DEVICE_PATH                    **SysRootDevicePath,
  OUT EFI_FILE_HANDLE                    *SysRootHandle
  )
{
  EFI_STATUS                             Status;
  EFI_HANDLE                             *HandleBuffer;
  UINTN                                  NumberOfHandles;
  UINTN                                  Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL        *SimpleFileSystem;
  EFI_FILE_HANDLE                        RootHandle;
  EFI_DEVICE_PATH                        *DevicePath;

  if (SysRootDevicePath == NULL || SysRootHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **) &SimpleFileSystem);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = SimpleFileSystem->OpenVolume (SimpleFileSystem, &RootHandle);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = gBS->HandleProtocol(HandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID **) &DevicePath);
    if (EFI_ERROR (Status)) {
      RootHandle->Close (RootHandle);
      continue;
    }

    *SysRootDevicePath = DevicePath;
    *SysRootHandle     = RootHandle;
    break;
  }

  FreePool (HandleBuffer);
  return (Index < NumberOfHandles) ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
  Get the system drive information through the boot loader file path under system partition

  @param[out] SysRootDevicePath    The pointer of system root device path pointer
  @param[out] SysRoorHandle        The pointer of EFI_FILE_HANDLE of system drive

  @retval EFI_SUCCESS              The system drive information is correctly get
  @return Others                   Unable to get system drive information

**/
STATIC
EFI_STATUS
GetSystemRootInfoFromCurrentBootOption (
  OUT    EFI_DEVICE_PATH        **SysRootDevicePath,
  OUT    EFI_FILE_HANDLE        *SysRootHandle
  )
{
  EFI_STATUS                      Status;
  UINTN                           TotalBootOptions;
  UINT16                          BootOptions[MAX_BOOT_OPTIONS];
  UINTN                           Size;
  CHAR16                          BootOption[] = L"Boot0000";
  UINT8                           *BootOptionData;
  CHAR16                          *BootOptionDesc;
  EFI_DEVICE_PATH_PROTOCOL        *BootDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *ExpandedBootDevicePath;
  UINTN                           BootFilePathDevicePathOffset;
  UINTN                           CompareSize;
  BOOLEAN                         Found;
  EFI_HANDLE                      *HandleBuffer;
  UINTN                           NumberOfHandles;
  UINTN                           CurrentOption;
  UINTN                           Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;

  if (SysRootDevicePath == NULL || SysRootHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ExpandedBootDevicePath = NULL;
  TotalBootOptions = 0;
  Status = EFI_NOT_FOUND;
  Size = sizeof(UINT16);
  Status = CommonGetVariable (
             L"BootNext",
             &gEfiGlobalVariableGuid,
             &Size,
             &BootOptions[0]
             );
  if (!EFI_ERROR (Status)) {
    TotalBootOptions ++;
  }
  Size = sizeof(UINT16) * (MAX_BOOT_OPTIONS - TotalBootOptions);
  Status = CommonGetVariable (
             L"BootOrder",
             &gEfiGlobalVariableGuid,
             &Size,
             &BootOptions[TotalBootOptions]
             );
  if (EFI_ERROR (Status) && TotalBootOptions == 0) {
    return Status;
  }
  TotalBootOptions += (Size / sizeof(UINT16));
  for (CurrentOption = 0, Found = FALSE, Status = EFI_NOT_FOUND; CurrentOption < TotalBootOptions && !Found; CurrentOption ++) {
    UnicodeSPrint (BootOption, sizeof(BootOption), L"Boot%04x", BootOptions[CurrentOption]);
    BootOptionData = CommonGetVariableData (BootOption, &gEfiGlobalVariableGuid);
    if (BootOptionData == NULL) {
      continue;
    }
    //
    // Get the boot loader file path from the current Boot Option data
    //
    BootOptionDesc = (CHAR16 *)(BootOptionData + sizeof(UINT32) + sizeof(UINT16));
    BootDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)(BootOptionData + sizeof(UINT32) + sizeof(UINT16) + StrSize(BootOptionDesc));
    if (IS_USB_SHORT_FORM_DEVICE_PATH (BootDevicePath)){
      ExpandedBootDevicePath = BdsLibExpandUsbShortFormDevPath (BootDevicePath);
      BootDevicePath = ExpandedBootDevicePath;
    }
    Status = GetBootFilePathDevicePathOffset (BootDevicePath, &BootFilePathDevicePathOffset);
    if (Status == EFI_SUCCESS) {
      CompareSize = BootFilePathDevicePathOffset;
    } else {
      CompareSize = GetDevicePathSize (BootDevicePath) - sizeof (EFI_DEVICE_PATH_PROTOCOL);
    }
    Status = gBS->LocateHandleBuffer(
                    ByProtocol,
                    &gEfiSimpleFileSystemProtocolGuid,
                    NULL,
                    &NumberOfHandles,
                    &HandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      FreePool (BootOptionData);
      continue;
    }

    for (Index = 0; Index < NumberOfHandles; Index++) {
      Status = gBS->HandleProtocol(
                      HandleBuffer[Index],
                      &gEfiDevicePathProtocolGuid,
                      (VOID **)SysRootDevicePath
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (CompareMem (
            ((UINT8 *)*SysRootDevicePath) + GetDevicePathSize (*SysRootDevicePath) - CompareSize - sizeof(EFI_DEVICE_PATH),
            BootDevicePath,
            CompareSize) == 0) {
        Found = TRUE;
        break;
      }
    }
    if (Found) {
      Status = gBS->HandleProtocol(
                      HandleBuffer[Index],
                      &gEfiSimpleFileSystemProtocolGuid,
                      (VOID **)&SimpleFileSystem
                      );
      if (!EFI_ERROR (Status)) {
        Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, SysRootHandle);
      }
    } else {
      Status = EFI_NOT_FOUND;
    }
    FreePool (HandleBuffer);
    FreePool (BootOptionData);
    if (ExpandedBootDevicePath != NULL){
      FreePool (ExpandedBootDevicePath);
    }
  }
  return Status;
}

/**
 Get the information of system drive which contain capsule image file.

 @param[out] SysRootDevicePath    The pointer of system root device path pointer
 @param[out] SysRoorHandle        The pointer of EFI_FILE_HANDLE of system drive

 @retval EFI_SUCCESS              The system drive information is correctly get
 @return Others                   Unable to get system drive information
**/
STATIC
EFI_STATUS
GetSystemRootInfo (
  OUT EFI_DEVICE_PATH                    **SysRootDevicePath,
  OUT EFI_FILE_HANDLE                    *SysRootHandle
  )
{
  EFI_STATUS                             Status;

  Status = GetSystemRootInfoFromCurrentBootOption (SysRootDevicePath, SysRootHandle);
  if (!EFI_ERROR (Status)) {
    if (HaveCapsuleImageFile (*SysRootHandle)) {
      return EFI_SUCCESS;
    }

    (*SysRootHandle)->Close (*SysRootHandle);
  }

  return GetSystemRootInfoFromFileSystems (SysRootDevicePath, SysRootHandle);
}

/**
  Dump raw data.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8   *Data8,
  IN UINTN   DataSize
  )
{
  DEBUG_CODE_BEGIN();

  UINTN      Index;

  for (Index = 0; Index < DataSize; Index++) {
    if (Index % 0x10 == 0) {
      DEBUG ((DEBUG_INFO, "\n%08X:", Index));
    }
    DEBUG ((DEBUG_INFO, " %02X", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG_CODE_END();
}

/**
  Calculate SHA256 Hash

  @param[in]  Data   data
  @param[in]  Size   data size
  @param[out] Digest SHA256 digest

**/
VOID
CreateSha256Hash (
  IN  UINT8     *Data,
  IN  UINTN     Size,
  OUT UINT8     *Digest
  )
{
  UINTN       CtxSize;
  VOID        *HashCtx;

  CtxSize = Sha256GetContextSize ();
  HashCtx = AllocatePool (CtxSize);
  ASSERT (HashCtx != NULL);
  Sha256Init (HashCtx);
  Sha256Update (HashCtx, Data, Size);
  Sha256Final (HashCtx, Digest);
  InternalDumpData (Digest, 32);

  FreePool (HashCtx);
}

/**
  Delete a file from root file handle.

  @param[in] Root                The root file handle.
  @param[in] FileName            Pointer to file name.

  @retval EFI_SUCCESS            File does not exist or deleted the file successfully.
  @retval Others                 Failed to delete the file.

**/
EFI_STATUS
DeleteFileFromRoot (
  IN   EFI_FILE_HANDLE  Root,
  IN   CHAR16           *FileName
  )
{
  EFI_STATUS                            Status;
  EFI_FILE                              *FileHandle;

  DEBUG ((DEBUG_INFO, "DeleteFileFromRoot - start\n"));

  Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "File %s does not exist. No need to delete\n", FileName));
    return EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot open file: %s. Status = %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Failed to open root dir - %r\n", Status));
    return Status;
  }

  Status = FileHandle->Delete (FileHandle);
  DEBUG ((DEBUG_INFO, "DeleteFileFromRoot %s %r\n", FileName, Status));

  return Status;
}

/**
  Write a file to an root file handle.

  @param[in] SysRoot             The system root file handle.
  @param[in] FileName            Pointer to file name.
  @param[in] FileBuffer          The buffer to be written into file system.
  @param[in] FileSize            The size of FileBuffer.

  @retval EFI_SUCCESS            Wrote the file successfully.
  @retval Others                 Failed to write the file.

**/
EFI_STATUS
WriteFileToSysRoot (
  IN   EFI_FILE_HANDLE  SysRoot,
  IN   CHAR16           *FileName,
  IN   UINT8            *FileBuffer,
  IN   UINTN            FileSize
  )
{
  EFI_STATUS                            Status;
  EFI_FILE                              *FileHandle;
  UINTN                                 WriteSize;

  DEBUG ((DEBUG_INFO, "WriteRecoveryFile - start\n"));

  Status = SysRoot->Open (SysRoot, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Open file on root: %s. error - %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Open file on root: %s. error - %r\n", FileName, Status));
    return Status;
  }

  do {
    WriteSize = (FileSize > SIZE_4KB) ? SIZE_4KB : FileSize;
    Status = FileHandle->Write (FileHandle, &WriteSize, FileBuffer);
    if (EFI_ERROR (Status)) {
      break;
    }
    FileSize = FileSize - WriteSize;
    FileBuffer = FileBuffer + WriteSize;
  } while (FileSize > 0);

  DEBUG ((DEBUG_INFO, "WriteRecoveryFile %s %r\n", FileName, Status));
  FileHandle->Close (FileHandle);

  return Status;
}


/**
  Read a file from a root file handle.

  @param[in] SysRoot             The system root file handle.
  @param[in] FileName            Pointer to file name.
  @param[in] FileBuffer          The buffer read from file system.
  @param[in] FileSize            The size of FileBuffer.

  @retval EFI_SUCCESS            Read the file successfully.
  @retval Others                 Failed to read the file.

**/
EFI_STATUS
ReadFileFromSysRoot (
  IN  EFI_FILE_HANDLE   SysRoot,
  IN  CHAR16            *FileName,
  OUT VOID              **Buffer,
  OUT UINTN             *BufferSize
  )
{
  EFI_STATUS                            Status;
  EFI_FILE                              *FileHandle;
  UINTN                                 FileInfoSize;
  EFI_FILE_INFO                         *FileInfo;
  EFI_GUID                              FileInfoGuid = EFI_FILE_INFO_ID;

  DEBUG ((DEBUG_INFO, "ReadRecoveryFile - start\n"));

  Status = SysRoot->Open (SysRoot, &FileHandle, FileName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Open file on root: %s. error - %r\n", FileName, Status));
    return Status;
  }

  if (FileHandle == NULL) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "Open file on root: %s. error - %r\n", FileName, Status));
    return Status;
  }

  FileInfoSize = 0;
  FileInfo = NULL;

  Status = FileHandle->GetInfo (
                         FileHandle,
                         &FileInfoGuid,
                         &FileInfoSize,
                         NULL
                         );

  if (EFI_ERROR (Status)) {
    if (Status != EFI_BUFFER_TOO_SMALL) {
      DEBUG ((DEBUG_ERROR, "GetInfo error - %r\n", Status));
      goto Exit;
    }
  }

  DEBUG ((DEBUG_INFO, "FileRead\n"));
  FileInfo = AllocatePool (FileInfoSize);
  if (FileInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "GetInfo fail, AllocatePool(Size: %x) error\n", FileInfoSize));
    goto Exit;
  }

  Status = FileHandle->GetInfo (
                         FileHandle,
                         &FileInfoGuid,
                         &FileInfoSize,
                         FileInfo
                         );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FileRead fail, GetInfo error, Status: %r\n", Status));
    goto Exit;
  }

  *BufferSize = (UINT32) FileInfo->FileSize;
  if (*BufferSize != 0) {
    *Buffer = AllocateZeroPool (*BufferSize);
    if (*Buffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Read fail, AllocatePool(Size: %x) error\n", FileInfoSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    Status = FileHandle->Read (
                           FileHandle,
                           BufferSize,
                           *Buffer
                           );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read fail, error - %r\n", Status));
      FreePool (*Buffer);
      goto Exit;
    }

  } else {
    DEBUG ((DEBUG_INFO, "File size is 0, set return Buffer to NULL.\n"));
    *Buffer = NULL;
  }

  Status = EFI_SUCCESS;

Exit:
  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }

  FileHandle->Close (FileHandle);

  DEBUG ((DEBUG_INFO, "ReadRecoveryFile %s BufferSize: %x %r\n", FileName, *BufferSize, Status));
  return Status;
}

/**
  Update a block on flash.

  @param[in] Src                    Number of blocks to be erased
  @param[in] NumBytes               Contains the total size of the buffer.
  @param[in] WriteAddress           Target address to be updated

  @retval EFI_SUCCESS.             Operation is successful.
  @retval EFI_INVALID_PARAMETER    Src is NULL or WriteAddress is not well aligned.
  @retval EFI_OUT_OF_RESOURCES     Failed to allocate needed memory buffer.
  @retval EFI_VOLUME_CORRUPTED     The block is not updated as expected.
  @retval Others                   If there is any device errors.

**/
STATIC
EFI_STATUS
FlashUpdate (
  IN UINT8                             *Src,
  IN UINTN                             NumBytes,
  IN UINTN                             WriteAddress
  )
{
  EFI_STATUS                           Status;
  UINT8                                *CompareBuffer;
  UINTN                                Size;

  if ((Src == NULL) || ((NumBytes % GetFlashBlockSize ()) != 0) || ((WriteAddress % SIZE_4KB) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  CompareBuffer = AllocateZeroPool (NumBytes);
  if (CompareBuffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = FlashRead (
             CompareBuffer,
             (UINT8 *)WriteAddress,
             NumBytes
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read flash address 0x%x error %r\n", WriteAddress));
    goto Exit;
  }

  if (CompareMem (CompareBuffer, Src, NumBytes) == 0) {
    //
    // No need to update
    //
    Status = EFI_SUCCESS;
    goto Exit;
  }

  Status = FlashErase (WriteAddress, NumBytes);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Erase flash address 0x%x error %r\n", WriteAddress));
    goto Exit;
  }

  Size = NumBytes;
  Status = FlashProgram (
             (UINT8 *)WriteAddress,
             (UINT8 *)Src,
             &Size,
             WriteAddress
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Program flash address 0x%x error %r\n", WriteAddress));
    goto Exit;
  }

  //
  // Read buffer back to verify udpate status
  //
  ZeroMem (CompareBuffer, NumBytes);
  Status = FlashRead (
             CompareBuffer,
             (UINT8 *)WriteAddress,
             NumBytes
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read flash address 0x%x error %r\n", WriteAddress));
    goto Exit;
  }

  if (CompareMem (CompareBuffer, Src, NumBytes) != 0) {
    //
    // Block is not updated as expected.
    //
    Status = EFI_VOLUME_CORRUPTED;
    DEBUG ((DEBUG_ERROR, "FlashUpdate: Block is not updated as expected.\n"));
  }

Exit:
  if (CompareBuffer !=  NULL) {
    FreePool (CompareBuffer);
  }

  return Status;
}

/**
  Check whether need to backup the current SBB into storge.

  @param  None

  @retval TRUE      Backup is needed.
          FALSE     Backup is not needed.

**/
BOOLEAN
IsSbbBackupRequired (
  VOID
  )
{
  EFI_STATUS     Status;
  UINTN          DataSize;
  UINT8          Sha256[SHA256_DIGEST_SIZE];

  ZeroMem (Sha256, SHA256_DIGEST_SIZE);
  DataSize = SHA256_DIGEST_SIZE;
  Status = CommonGetVariable (
             BACKUP_SBB_FILE_DIGEST_SHA256_NAME,
             &gH2OSeamlessRecoveryDigestGuid,
             &DataSize,
             &Sha256
             );
  if (EFI_ERROR (Status)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check whether the input SBB is identical with the sha256 digest.

 @param [in]   FileBuffer  The Backup SBB file data buffer
 @param [in]   FileSize    The Backup SBB file size

  @retval TRUE      The digest of FileBuffer is identical with the digest in variable.
          FALSE     The digest of FileBuffer is not identical with the digest in variable.

**/
BOOLEAN
IsBackupSbbValid (
  IN VOID     *FileBuffer,
  IN UINTN    FileSize
  )
{
  EFI_STATUS     Status;
  UINT8          Sha256[SHA256_DIGEST_SIZE];
  UINT8          Sha256Digest[SHA256_DIGEST_SIZE];
  UINTN          VariableSize;

  ZeroMem (Sha256, SHA256_DIGEST_SIZE);
  CreateSha256Hash ((UINT8 *)(UINTN)FileBuffer, FileSize, Sha256);
  //
  // Check SBB digest
  //
  ZeroMem (Sha256Digest, SHA256_DIGEST_SIZE);
  VariableSize = SHA256_DIGEST_SIZE;
  Status = CommonGetVariable (
             BACKUP_SBB_FILE_DIGEST_SHA256_NAME,
             &gH2OSeamlessRecoveryDigestGuid,
             &VariableSize,
             Sha256Digest
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get message digest of the backup SBB.\n"));
    return FALSE;
  }

  if (CompareMem (Sha256Digest, Sha256, VariableSize) != 0) {
    DEBUG ((DEBUG_ERROR, "SBB image loaded from media is corrupted.\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Backup the current SBB to the storage.

 @retval EFI_SUCCESS             Operation was successful.
 @retval Others                  Operation was failed.

**/
STATIC
EFI_STATUS
BackupCurrentSbbToStorage (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_DEVICE_PATH_PROTOCOL          *SysRootDevicePath;
  EFI_FILE_HANDLE                   SysRootHandle;
  UINT8                             Sha256[SHA256_DIGEST_SIZE];
  UINT8                             *Buffer;
  UINTN                             BufferSize;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLock;

  Status = GetSystemRootInfo (&SysRootDevicePath, &SysRootHandle);
  if (EFI_ERROR (Status) || SysRootHandle == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Read the current SBB from flash part
  //
  BufferSize = (UINTN)PcdGet32 (PcdFlashSbbSize);
  Buffer = AllocateZeroPool (BufferSize);
  if (Buffer == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = FlashRead (
             Buffer,
             (UINT8 *)(UINTN)PcdGet32 (PcdFlashSbbBase),
             BufferSize
             );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Backup SBB to the system root.
  //
  Status = WriteFileToSysRoot (
             SysRootHandle,
             BACKUP_SBB_FILE_NAME,
             Buffer,
             BufferSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to write backup SBB to hdd. %r\n"));
    goto Exit;
  }

  //
  // Calculate the hash
  //
  ZeroMem (Sha256, SHA256_DIGEST_SIZE);
  DEBUG ((DEBUG_INFO, "Create recovery file digest:\n"));
  CreateSha256Hash ((UINT8 *) (UINTN)Buffer, BufferSize, Sha256);

  Status = CommonSetVariable (
             BACKUP_SBB_FILE_DIGEST_SHA256_NAME,
             &gH2OSeamlessRecoveryDigestGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             SHA256_DIGEST_SIZE,
             Sha256
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to save sha256 digest for recovery file. %r\n"));
    goto Exit;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLock);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = VariableLock->RequestToLock (
                           VariableLock,
                           BACKUP_SBB_FILE_DIGEST_SHA256_NAME,
                           &gH2OSeamlessRecoveryDigestGuid
                           );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to lock variable %s, Status = %r\n", BACKUP_SBB_FILE_DIGEST_SHA256_NAME, Status));
    goto Exit;
  }

Exit:
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
  if (SysRootHandle != NULL) {
    SysRootHandle->Close (SysRootHandle);
  }
  return Status;
}

/**
  Determine if two buffers overlap in memory.

  @param[in] Buff1   pointer to first buffer
  @param[in] Size1   size of Buff1
  @param[in] Buff2   pointer to second buffer
  @param[in] Size2   size of Buff2

  @retval TRUE    Buffers overlap in memory.
  @retval FALSE   Buffer doesn't overlap.

**/
STATIC
BOOLEAN
IsOverlapped (
  IN  UINT8     *Buff1,
  IN  UINTN     Size1,
  IN  UINT8     *Buff2,
  IN  UINTN     Size2
  )
{
  //
  // If buff1's end is less than the start of buff2, then it's ok.
  // Also, if buff1's start is beyond buff2's end, then it's ok.
  //
  if (((Buff1 + Size1) <= Buff2) || (Buff1 >= (Buff2 + Size2))) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if fault tolerance is correctly supported.

  @retval EFI_SUCCESS                 Sanity check pass.
  @retval EFI_SECURITY_VIOLATION      Sanity check fail.

**/
STATIC
EFI_STATUS
FaultToleranceSanityCheck (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_PHYSICAL_ADDRESS                      BootBlockBase;
  UINTN                                     BootBlockSize;
  EFI_PHYSICAL_ADDRESS                      BackupBlockBase;
  UINTN                                     BackupBlockSize;
  UINT64                                    RecoveryFvSize;
  EFI_SWAP_ADDRESS_RANGE_PROTOCOL           *SwapAddressRange;

  if (!FeaturePcdGet (PcdH2OBiosUpdateFaultToleranceEnabled)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - PcdH2OBiosUpdateFaultToleranceEnabled is FALSE\n"));
    return EFI_SECURITY_VIOLATION;
  }

  Status = gBS->LocateProtocol (
                &gEfiSwapAddressRangeProtocolGuid,
                NULL,
                (VOID **)&SwapAddressRange
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - SwapAddressRange not found\n"));
    return EFI_SECURITY_VIOLATION;
  }

  Status = SwapAddressRange->GetRangeLocation (
                               SwapAddressRange,
                               &BootBlockBase,
                               &BootBlockSize,
                               &BackupBlockBase,
                               &BackupBlockSize
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - SwapAddressRange->GetRangeLocation %r\n", Status));
    return EFI_SECURITY_VIOLATION;
  }

  if ((BootBlockSize == 0) ||
      (BackupBlockSize == 0)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - Invalid BootBlockSize/BackupBlockSize\n"));
    return EFI_SECURITY_VIOLATION;
  }

  if (((UINTN)PcdGet32 (PcdFlashPbbSize) != (UINTN)BootBlockSize) ||
      ((UINTN)PcdGet32 (PcdFlashPbbSize) != (UINTN)BackupBlockSize) ||
      ((UINTN)PcdGet32 (PcdFlashPbbBase) != (UINTN)BootBlockBase) ||
      ((UINTN)PcdGet32 (PcdFlashPbbRBase) != (UINTN)BackupBlockBase)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - PBB/SBB PCDs are incorrect\n"));
    return EFI_SECURITY_VIOLATION;
  }

  RecoveryFvSize = FdmGetNAtSize (
                     &gH2OFlashMapRegionBootFvGuid,
                     1
                     );
  if ((UINTN)RecoveryFvSize > (UINTN)BackupBlockSize) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - PBB size is too small for fault tolerance\n"));
    return EFI_SECURITY_VIOLATION;
  }

  if (IsFirmwareUpdateResiliencySupported ()) {
    if (IsOverlapped (
          (UINT8 *)(UINTN)BackupBlockBase,
          BackupBlockSize,
          (UINT8 *)(UINTN)PcdGet32 (PcdFlashSbbBase),
          PcdGet32 (PcdFlashSbbSize)
          )) {
      DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck - PBBR and SBB are overlapped. Invalid flash layout\n"));
      return EFI_SECURITY_VIOLATION;
    }
  }

  return EFI_SUCCESS;
}

/**
  BIOS update fault tolerance handler on ready to boot before.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
BiosUpdateFaultToleranceOnReadyToBootBefore (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  EFI_STATUS                           Status;
  EFI_DEVICE_PATH_PROTOCOL             *SysRootDevicePath;
  EFI_FILE_HANDLE                      SysRootHandle;

  H2OCpUnregisterHandler (Handle);

  //
  // The update has been processed before this funtion called.
  // Clear the update progress at this point.
  //
  ClearFirmwareUpdateProgress ();

  //
  // Delete backup file if system doesn't support resiliency.
  //
  if (!IsFirmwareUpdateResiliencySupported () && !IsSbbBackupRequired ()) {
    Status = GetSystemRootInfo (&SysRootDevicePath, &SysRootHandle);
    if (EFI_ERROR (Status) || SysRootHandle == NULL) {
      return;
    }
    DeleteFileFromRoot (SysRootHandle, BACKUP_SBB_FILE_NAME);
    SysRootHandle->Close (SysRootHandle);
  }
}

/**
  BIOS update fault tolerance handler on end of dxe before.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
BiosUpdateFaultToleranceOnEndOfDxeBefore (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  )
{
  EFI_STATUS                           Status;
  UINT8                                *Buffer;
  UINTN                                BufferSize;
  EFI_SWAP_ADDRESS_RANGE_PROTOCOL      *SwapAddressRange;
  BOOLEAN                              SwapState;
  EFI_DEVICE_PATH_PROTOCOL             *SysRootDevicePath;
  EFI_FILE_HANDLE                      SysRootHandle;

  H2OCpUnregisterHandler (Handle);

  SysRootHandle = NULL;

  Status = gBS->LocateProtocol (
                &gEfiSwapAddressRangeProtocolGuid,
                NULL,
                (VOID **)&SwapAddressRange
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Locate SwapAddressRangeProtocol %r\n", Status));
    return;
  }

  Status = FaultToleranceSanityCheck ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FaultToleranceSanityCheck %r\n", Status));
    POST_CODE (DXE_FAULT_TOLERANCE_FCHECK_FAILED);
    CpuDeadLoop ();
  }

  if (IsSbbBackupRequired ()) {
    DEBUG ((DEBUG_INFO, "BackupCurrentSbbToStorage for fault tolerence update\n"));
    Status = BackupCurrentSbbToStorage ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "BackupCurrentSbbToStorage %r\n"));
    }
  }

  if (!IsFirmwareUpdateResiliencyEnabled ()) {
    return;
  }

  DEBUG ((DEBUG_INFO, "There was a firmware update completion. Start the resiliency procedure\n"));

  Status = SwapAddressRange->GetSwapState (SwapAddressRange, &SwapState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetSwapState %r\n", Status));
    return;
  }

  if (!SwapState) {
    //
    // Update was successfully.
    // Flash the current PBB content into PBBR.
    //
    DEBUG ((DEBUG_INFO, "PBB SyncUp. Source Base = %X\n", (UINTN) PcdGet32 (PcdFlashPbbBase)));
    //
    // Read the current PBB from flash part
    //
    BufferSize = (UINTN)PcdGet32 (PcdFlashPbbSize);
    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    Status = FlashRead (
               Buffer,
               (UINT8 *)(UINTN)PcdGet32 (PcdFlashPbbBase),
               BufferSize
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read the current PBB error %r\n"));
      goto Exit;
    }

    //
    // Flash current PBB into PBBR
    //
    Status = FlashUpdate (
               (UINT8 *)Buffer,
               BufferSize,
               (UINTN)PcdGet32 (PcdFlashPbbRBase)
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Flash current PBB into PBBR error %r\n"));
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "SBB SyncUp in Disk\n"));
    Status = BackupCurrentSbbToStorage ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "BackupCurrentSbbToStorage %r\n"));
    }
  } else {
    //
    // Update was failed. TopSwap is enabled.
    // Rollback is processing.
    //
    DEBUG ((DEBUG_INFO, "Start RollBack\n"));
    //
    // Read the current PBBR from flash part
    //
    BufferSize = (UINTN)PcdGet32 (PcdFlashPbbSize);
    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }

    Status = FlashRead (
               Buffer,
               (UINT8 *)(UINTN)PcdGet32 (PcdFlashPbbRBase),
               BufferSize
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read the current PBBR error %r\n"));
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "PBB RollBack. Source Base = %X\n", (UINTN)PcdGet32(PcdFlashPbbRBase)));
    //
    // Flash current PBBR into PBB
    //
    Status = FlashUpdate (
               (UINT8 *)Buffer,
               BufferSize,
               (UINTN)PcdGet32 (PcdFlashPbbBase)
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Flash PBBR %r\n"));
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "SBB RollBack\n"));
    Status = GetSystemRootInfo (&SysRootDevicePath, &SysRootHandle);
    if (EFI_ERROR (Status) || SysRootHandle == NULL) {
      goto Exit;
    }
    //
    // Read the backup SBB from sys root.
    //
    Status = ReadFileFromSysRoot (
               SysRootHandle,
               BACKUP_SBB_FILE_NAME,
               (VOID **)&Buffer,
               &BufferSize
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Can't fetch bakcup SBB on disk %r\n"));
      goto Exit;
    }

    if (!IsBackupSbbValid ((VOID *)Buffer, BufferSize)) {
      DEBUG ((DEBUG_ERROR, "BackupSbb in disk is invalid\n"));
      goto Exit;
    }

    Status = FlashUpdate (
               (UINT8 *)Buffer,
               BufferSize,
               (UINTN)PcdGet32 (PcdFlashSbbBase)
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Flash PBBR %r\n"));
      goto Exit;
    }
  }

  //
  // Delete progress variable
  //
  Status = ClearFirmwareUpdateProgress ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Delete progress variable %r\n"));
  }

  //
  // Disable topswap bit
  //
  Status = SwapAddressRange->SetSwapState (SwapAddressRange, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SetSwapState %r\n", Status));
  }

  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

Exit:
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
  if (SysRootHandle != NULL) {
    SysRootHandle->Close (SysRootHandle);
  }
}

/**
  Lock Progress varaible.

  @param[in]  Event        Event whose notification function is being invoked
  @param[in]  Context      Pointer to the notification function's context

**/
VOID
EFIAPI
LockProgressVariable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                        Status;
  EDKII_VARIABLE_LOCK_PROTOCOL      *VariableLockProtocol;

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLockProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // This variable controls the update progress.
  // It should be protected from malicious software. We set it as read-only variable here.
  //
  Status = VariableLockProtocol->RequestToLock (
                                   VariableLockProtocol,
                                   UPDATE_PROGRESS_NAME,
                                   &gH2OSeamlessRecoveryGuid
                                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Error when lock variable %s, Status = %r\n", UPDATE_PROGRESS_NAME, Status));
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}

/**
  Initialize BIOS update fault tolerance support for system firmware update.

  @retval EFI_SUCCESS    Fault Tolerence Support is initialized successfully
  @retval EFI_NOT_FOUND  Fault Tolerence Support is not initialized successfully
**/

EFI_STATUS
EFIAPI
BiosUpdateFaultToleranceDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  H2O_CP_HANDLE             CpHandle;
  VOID                      *Registration;

  if (FeaturePcdGet (PcdH2OBdsCpEndOfDxeBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpEndOfDxeBeforeGuid,
               BiosUpdateFaultToleranceOnEndOfDxeBefore,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpEndOfDxeBeforeGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpEndOfDxeBeforeGuid, Status));
  }

  if (FeaturePcdGet (PcdH2OBdsCpReadyToBootBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpReadyToBootBeforeGuid,
               BiosUpdateFaultToleranceOnReadyToBootBefore,
               H2O_CP_LOW,
               &CpHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
  }

  //
  // Lock the Progress variable.
  //
  EfiCreateProtocolNotifyEvent (
    &gEdkiiVariableLockProtocolGuid,
    TPL_CALLBACK,
    LockProgressVariable,
    NULL,
    (VOID **)&Registration
    );

  return EFI_SUCCESS;
}
