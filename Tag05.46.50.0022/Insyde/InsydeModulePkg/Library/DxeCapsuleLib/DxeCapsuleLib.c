/** @file
  Capsule Library instance to update capsule image to flash.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/CapsuleUpdateCriteriaLib.h>
#include <Library/VariableLib.h>
#include <Library/H2OCpLib.h>
#include <Library/SeamlessRecoveryLib.h>
#include <Library/GenericBdsLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareManagement.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePath.h>
#include <Protocol/H2OCapsuleUpdateProgress.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>
#include <Guid/H2OCp.h>
#include <Guid/EfiSystemResourceTable.h>
#include <Guid/FileInfo.h>
#include "CapsuleProgressBar.h"

#define MAX_STRING_LENGTH       128
#define MAX_BOOT_OPTIONS        128
//
// FMP descriptor size with each version
//
#define FMP_DESCRIPTOR_SIZE_V1  0x58
#define FMP_DESCRIPTOR_SIZE_V2  0x60
#define FMP_DESCRIPTOR_SIZE_V3  0x70
#define FMP_DESCRIPTOR_SIZE_V4  0x78

typedef struct _FMP_IMAGE_INFO {
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *Fmp;
  UINTN                             ImageInfoSize;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR     *ImageInfo;
  UINTN                             ImageInfoCount;
  UINTN                             DescriptorSize;
} FMP_IMAGE_INFO;


typedef struct _FMP_INSTANCE_INFO {
  UINTN              ImageCount;
  FMP_IMAGE_INFO     *ImageInfos;
} FMP_INSTANCE_INFO;

typedef enum {
  DESCRIPTOR_V1 = 0x01,
  DESCRIPTOR_V2,
  DESCRIPTOR_V3,
  DESCRIPTOR_V4
} FMP_DESCRIPTOR_VERSION;

typedef struct _FMP_DESCRIPTOR_INFO {
  UINT32    DescriptorVersion;
  UINTN     DescriptorSize;
} FMP_DESCRIPTOR_INFO;

//
// The times of calling ProcessCapsuleImage ()
//
STATIC UINTN                                 mTimes = 0;
STATIC EFI_EVENT                             mVirtualAddressChangeEvent = NULL;
STATIC EFI_EVENT                             mFirmwareManagementEvent = NULL;
STATIC EFI_EVENT                             mReadyToBootEvent = NULL;
STATIC FMP_INSTANCE_INFO                     *mFmpInstanceInfo = NULL;
STATIC EFI_GUID                              mFmpInstanceInfoProtocol =  { 0x6c7140f, 0xff00, 0x4a81, { 0x83, 0xab, 0x7f, 0xd, 0xbc, 0x39, 0x1d, 0x21 } };
STATIC EFI_GUID                              mWinUxFoundProtocol      =  { 0xe06613cb, 0xc520, 0x447d, { 0x93, 0x57, 0x3d, 0x59, 0x7a, 0x88, 0x6e, 0x10 } };
STATIC BOOLEAN                               mVirtualAddressEventSignaled = FALSE;
STATIC H2O_CAPSULE_UPDATE_PROGRESS_PROTOCOL  mCapsuleUpdateProgressProtocol;
STATIC UINT8                                 *mCurrentBootOptionData = NULL;

/**
  Check that whether the size of ImageInfo is large enough or not.

  @param[in] DescriptorVersion   The version of the EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[in] ImageInfoSize       The size of ImageInfo.

  @retval    TRUE                ImageInfoSize is large enough.
             FALSE               ImageInfoSize is less than DescriptorSize in the corresponding version.

**/
STATIC
BOOLEAN
CheckImageInfoSize (
  IN  UINT32    DescriptorVersion,  
  IN  UINTN     ImageInfoSize
  )
{
  UINT8                Index;
  FMP_DESCRIPTOR_INFO  FmpDescriptorInfo[] = {
                         {DESCRIPTOR_V1, FMP_DESCRIPTOR_SIZE_V1},
                         {DESCRIPTOR_V2, FMP_DESCRIPTOR_SIZE_V2},
                         {DESCRIPTOR_V3, FMP_DESCRIPTOR_SIZE_V3},
                         {DESCRIPTOR_V4, FMP_DESCRIPTOR_SIZE_V4}
                         };

  //
  // Make sure ImageInfoSize is at least greater than or equal to FMP_DESCRIPTOR_SIZE_V1.
  //
  if (ImageInfoSize < FMP_DESCRIPTOR_SIZE_V1) {
    return FALSE;
  }

  for (Index = 0; Index < sizeof (FmpDescriptorInfo) / sizeof (FMP_DESCRIPTOR_INFO); Index++) {
    if (DescriptorVersion == FmpDescriptorInfo[Index].DescriptorVersion && ImageInfoSize < FmpDescriptorInfo[Index].DescriptorSize) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Remove the read-only bit if the file has it.

  @param[in] SysRootHandle       EFI_FILE_HANDLE of system drive.
  @param[in] CapsuleImagePath    Pointer to the file name of capsule.

  @retval EFI_SUCCESS            Successfully remove EFI_FILE_READ_ONLY
                                 bit or the bit has not set.
  @return others                 Fail to remove the bit.

**/
STATIC
EFI_STATUS
EnableFileWriteAttribute (
  IN  EFI_FILE_HANDLE  SysRootHandle,
  IN  CHAR16           *CapsuleImagePath
  )
{
  EFI_STATUS       Status;
  EFI_FILE_INFO    *FileInfo;
  EFI_FILE_HANDLE  CapsuleHandle;
  UINTN            BufferSize;

  CapsuleHandle  = NULL;
  FileInfo       = NULL;
  BufferSize     = 0;

  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            CapsuleImagePath,
                            EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE,
                            0
                            );
  if (!EFI_ERROR(Status)) {
    CapsuleHandle->Close (CapsuleHandle);
    return Status;
  }
  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            CapsuleImagePath,
                            EFI_FILE_MODE_READ,
                            0
                            );
  if (!EFI_ERROR (Status)) {
    //
    // Get file info to check file attribute
    //
    Status = CapsuleHandle->GetInfo (
               CapsuleHandle,
               &gEfiFileInfoGuid,
               &BufferSize,
               NULL
               );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      FileInfo = AllocatePool (BufferSize);
      if (FileInfo == NULL) {
        CapsuleHandle->Close (CapsuleHandle);
        return EFI_BUFFER_TOO_SMALL;
      }
      Status = CapsuleHandle->GetInfo (
                 CapsuleHandle,
                 &gEfiFileInfoGuid,
                 &BufferSize,
                 FileInfo
                 );
      if (EFI_ERROR (Status)) {
        CapsuleHandle->Close (CapsuleHandle);
        FreePool (FileInfo);
        return Status;
      }
      if ((FileInfo->Attribute & EFI_FILE_READ_ONLY) == EFI_FILE_READ_ONLY) {
        FileInfo->Attribute &= ~EFI_FILE_READ_ONLY;
        Status = CapsuleHandle->SetInfo (
                   CapsuleHandle,
                   &gEfiFileInfoGuid,
                   BufferSize,
                   FileInfo
                   );
      }
    }
    CapsuleHandle->Close (CapsuleHandle);
    FreePool (FileInfo);
  }
  return Status;
}

/**
  Record capsule file name by a variable

  @param[in] CapsuleFileName     A pointer to capsule file name.

**/
STATIC
VOID
RecordCapsuleFileName (
  IN CHAR16 *CapsuleFileName
  )
{
  CommonSetVariable (
    L"CapsuleFileName",
    &gH2OSeamlessRecoveryGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS,
    (UINTN)StrSize(CapsuleFileName),
    (VOID*)CapsuleFileName
    );
}

/**
  Notification function of EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It convers pointer to new virtual address.

  @param[in] Event     Event whose notification function is being invoked.
  @param[in] Context   Pointer to the notification function's context.
**/
STATIC
VOID
EFIAPI
VariableAddressChangeEvent (
  IN EFI_EVENT                            Event,
  IN VOID                                 *Context
  )
{
  FMP_IMAGE_INFO       *FmpImageInfo;

  mVirtualAddressEventSignaled = TRUE;

  if (mFmpInstanceInfo != NULL) {
    if (mFmpInstanceInfo->ImageInfos != NULL) {
      for (FmpImageInfo = mFmpInstanceInfo->ImageInfos; FmpImageInfo->Fmp != NULL; FmpImageInfo++) {
        gRT->ConvertPointer (0x0, (VOID **) &FmpImageInfo->Fmp);
        if (FmpImageInfo->ImageInfo != NULL) {
          gRT->ConvertPointer (0x0, (VOID **) &FmpImageInfo->ImageInfo);
        }
      }
      gRT->ConvertPointer (0x0, (VOID **) &mFmpInstanceInfo->ImageInfos);
    }
    gRT->ConvertPointer (0x0, (VOID **) &mFmpInstanceInfo);
  }
}

/**
  According to Input EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance to set related
  information to FMP_IMAGE_INFO structure.

  @param[in]  Fmp                Pointer to EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out] FmpImageInfo       Pointer to FMP_IMAGE_INFO instance.

  @retval EFI_SUCCESS            Set Fmp image information to FMP_IMAGE_INFO strcture successfully.
  @retval EFI_UNSUPPORTED        The ImageInfoSize get from GetImageInfo() is unsupported.
  @retval EFI_INVALID_PARAMETER  Fmp or FmpImageInfo is NULL.
  @retval EFI_OUT_OF_RESOURCES   Unable to allocate memory to save FMP image descriptor.
**/
STATIC
EFI_STATUS
SetFmpImageInfo (
  IN   EFI_FIRMWARE_MANAGEMENT_PROTOCOL     *Fmp,
  OUT  FMP_IMAGE_INFO                       *FmpImageInfo
  )
{
  EFI_STATUS                            Status;
  UINTN                                 ImageInfoSize;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR         *ImageInfo;
  UINT32                                ImageInfoDescriptorVer;
  UINT8                                 ImageInfoCount;
  UINTN                                 DescriptorSize;
  UINT32                                PackageVersion;
  CHAR16                                *PackageVersionName;

  if (Fmp == NULL || FmpImageInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FmpImageInfo->Fmp = Fmp;
  ImageInfoSize = 0;
  Status = Fmp->GetImageInfo (
                  Fmp,
                  &ImageInfoSize,
                  NULL,
                  NULL,
                  &ImageInfoCount,
                  &DescriptorSize,
                  NULL,
                  NULL
                  );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return EFI_SUCCESS;
  }
  ImageInfo = AllocateRuntimeZeroPool (ImageInfoSize);
  if (ImageInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  PackageVersionName = NULL;
  Status = Fmp->GetImageInfo (
                  Fmp,
                  &ImageInfoSize,
                  ImageInfo,
                  &ImageInfoDescriptorVer,
                  &ImageInfoCount,
                  &DescriptorSize,
                  &PackageVersion,
                  &PackageVersionName
                  );
  if (Status != EFI_SUCCESS) {
    FreePool (ImageInfo);
    return EFI_SUCCESS;
  }
  //
  // Some third party FMP may return incorrect ImageInfoSize (ex: 1) even if it
  // returns EFI_SUCCESS. Add code to check this situation here.
  //
  if (!CheckImageInfoSize (ImageInfoDescriptorVer, ImageInfoSize)) {
    DEBUG ((DEBUG_ERROR, "The ImageInfoSize is wrong (0x%x) !!!\n", ImageInfoSize));
    FreePool (ImageInfo);
    return EFI_UNSUPPORTED;
  }
  if (PackageVersionName != NULL) {
    FreePool (PackageVersionName);
  }
  FmpImageInfo->ImageInfoSize   = ImageInfoSize;
  FmpImageInfo->ImageInfo       = ImageInfo;
  FmpImageInfo->ImageInfoCount  = ImageInfoCount;
  FmpImageInfo->DescriptorSize  = DescriptorSize;

  return EFI_SUCCESS;
}

/**
  Get Fmp image information from FMP_INSTANCE_INFO.

  @param[in] Handle     Pointer a handle which contains FMP protocol.

  return  Pointer FMP_INSTACE_INFO or null if not found.
 **/
STATIC
FMP_IMAGE_INFO *
RetrieveFmpImageInfo (
  IN   EFI_HANDLE      Handle
  )
{
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL     *Fmp;
  UINTN                                Index;
  EFI_STATUS                           Status;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiFirmwareManagementProtocolGuid,
                  (VOID **)&Fmp
                  );
   ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < mFmpInstanceInfo->ImageCount; Index++) {
    if (mFmpInstanceInfo->ImageInfos[Index].Fmp == Fmp) {
      return &mFmpInstanceInfo->ImageInfos[Index];
    }
  }
  return NULL;
}

/**
  Notification function of gEfiFirmwareManagementProtocolGuid.

  @param[in] Event     Event whose notification function is being invoked.
  @param[in] Context   Pointer to the notification function's context.
**/
STATIC
VOID
EFIAPI
FirmwareManagementCallback (
  IN EFI_EVENT                            Event,
  IN VOID                                 *Context
  )
{
  EFI_STATUS                           Status;
  EFI_HANDLE                           *HandleBuffer;
  UINTN                                NumberOfHandles;
  UINTN                                Index;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL     *Fmp;
  FMP_IMAGE_INFO                       *FmpImageInfos;
  FMP_IMAGE_INFO                       *FmpImageInfo;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareManagementProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // Needn't do anything if all of the Fmp image information already parsed.
  //
  if (NumberOfHandles == mFmpInstanceInfo->ImageCount) {
    for (Index = 0; Index < NumberOfHandles; Index++) {
      if (RetrieveFmpImageInfo (HandleBuffer[Index]) == NULL) {
        break;
      }
    }
    if (Index == NumberOfHandles) {
      FreePool (HandleBuffer);
      return;
    }
  }

  FmpImageInfos = AllocateRuntimeZeroPool ((NumberOfHandles + 1) * sizeof (FMP_IMAGE_INFO));
  if (FmpImageInfos == NULL) {
    FreePool (HandleBuffer);
    return;
  }

  for(Index = 0; Index < NumberOfHandles; Index++) {
    FmpImageInfo = RetrieveFmpImageInfo (HandleBuffer[Index]);
    if (FmpImageInfo != NULL) {
      CopyMem (&FmpImageInfos[Index], FmpImageInfo, sizeof (FMP_IMAGE_INFO));
      continue;
    }
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareManagementProtocolGuid,
                    (VOID **)&Fmp
                    );
    ASSERT_EFI_ERROR (Status);
    if (Status == EFI_SUCCESS) {
      Status = SetFmpImageInfo (Fmp, &FmpImageInfos[Index]);
    }
  }
  if (mFmpInstanceInfo->ImageInfos != NULL) {
    FreePool (mFmpInstanceInfo->ImageInfos);
  }
  mFmpInstanceInfo->ImageCount = NumberOfHandles;
  mFmpInstanceInfo->ImageInfos = FmpImageInfos;
  FreePool (HandleBuffer);
}

/**
  Get boot option data from BootCurrent variable.

  @param[in] Event     Event whose notification function is being invoked.
  @param[in] Context   Checkpoint handle.
**/
STATIC
VOID
EFIAPI
ReadyToBootBeforeCpHandler (
  IN EFI_EVENT                            Event,
  IN H2O_CP_HANDLE                        Handle
  )
{
  EFI_STATUS         Status;
  UINTN              Size;
  UINT16             BootOptionValue;
  CHAR16             BootOption[] = L"Boot0000";

  if (mCurrentBootOptionData != NULL) {
    FreePool (mCurrentBootOptionData);
    mCurrentBootOptionData = NULL;
  }

  Size = sizeof(UINT16);
  Status = CommonGetVariable (
             L"BootCurrent",
             &gEfiGlobalVariableGuid,
             &Size,
             &BootOptionValue
             );
  if (EFI_ERROR (Status)) {
    return;
  }
  UnicodeSPrint (BootOption, sizeof(BootOption), L"Boot%04x", BootOptionValue);
  mCurrentBootOptionData = CommonGetVariableData (BootOption, &gEfiGlobalVariableGuid);
}

/**
  Get the offset of the boot loader file path from system partition for the boot
  device path of the current boot option

  @param[in] BootDevicePath      The device path of the boot option
  @param[out] BootFilePathOffset The offset from the boot device path of the boot
                                 loader file path

  @retval EFI_SUCCESS            The BootFilePathOffset is correctly set
  @return others                 Unable to get boot file path offset

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
  Those capsules supported by the firmwares.

  @param[in]   CapsuleHeader        A pointer to a capsule header.
  @param[out]  ImageFmp             Double Pointer to a EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out]  ImageIndex           A pointer to index of a EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out]  ImageAttributes      A pointer to attributes of a EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out]  ImageDescriptor      Double pointer to FMP information by EFI_FIRMWARE_IMAGE_DESCRIPTOR.

  @retval EFI_SUCESS                Input capsule is supported by firmware.
  @retval EFI_UNSUPPORTED           Input capsule is not supported by the firmware.
**/
STATIC
EFI_STATUS
GetSupportedFmp (
  IN     EFI_CAPSULE_HEADER                     *CapsuleHeader,
  OUT    EFI_FIRMWARE_MANAGEMENT_PROTOCOL       **ImageFmp,
  OUT    UINT8                                  *ImageIndex,
  OUT    UINT64                                 *ImageAttributes,
  OUT    EFI_FIRMWARE_IMAGE_DESCRIPTOR          **ImageDescriptor
  )
{
  EFI_FIRMWARE_IMAGE_DESCRIPTOR         *ImageInfo;
  FMP_IMAGE_INFO                        *FmpImageInfo;
  UINTN                                 Index;

  if (CapsuleHeader == NULL || mFmpInstanceInfo->ImageInfos == NULL) {
    return EFI_UNSUPPORTED;
  }
  if (!mVirtualAddressEventSignaled) {
    FirmwareManagementCallback (NULL, NULL);
  }
  for (FmpImageInfo = mFmpInstanceInfo->ImageInfos; FmpImageInfo->Fmp != NULL; FmpImageInfo++) {
    ImageInfo = FmpImageInfo->ImageInfo;
    if (ImageInfo == NULL) {
      continue;
    }
    for (Index = 0; Index < FmpImageInfo->ImageInfoCount; Index++) {
      //
      // Check the capsule guid
      //
      if (CompareGuid (&CapsuleHeader->CapsuleGuid, &ImageInfo->ImageTypeId)) {
        if (ImageFmp) *ImageFmp = mVirtualAddressEventSignaled ? NULL : FmpImageInfo->Fmp;
        if (ImageIndex) *ImageIndex = ImageInfo->ImageIndex;
        if (ImageAttributes) *ImageAttributes = ImageInfo->AttributesSetting;
        if (ImageDescriptor) *ImageDescriptor = ImageInfo;
        return EFI_SUCCESS;
      }
      //
      // Use DescriptorSize to move ImageInfo Pointer to stay compatible with different ImageInfo version
      //
      ImageInfo = (EFI_FIRMWARE_IMAGE_DESCRIPTOR *)((UINT8 *)ImageInfo + FmpImageInfo->DescriptorSize);
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Those capsules supported by the firmwares.

  @param[in]  CapsuleHeader         Points to a capsule header.

  @retval EFI_SUCESS                Input capsule is supported by firmware.
  @retval EFI_INCOMPATIBLE_VERSION  Incompatible firmware version.
  @retval EFI_UNSUPPORTED           Input capsule is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
SupportCapsuleImage (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader
  )
{
  if (GetSupportedFmp (CapsuleHeader, NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    return EFI_SUCCESS;
  }
  return GetSupportedFmp ((EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize), NULL, NULL, NULL, NULL);
}

/**
 Check if the capsule is a system firmware capsule.

  @param[in] CapsuleHeader       Pointer to the Capsule header.

 @retval TRUE                The capsule is a system firmware capsule.
 @return others              The capsule is not a system firmware capsule.
**/
STATIC
BOOLEAN
IsSystemFirmwareCapsule (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader
  )
{
  return CompareGuid (&CapsuleHeader->CapsuleGuid, (EFI_GUID *)PcdGetPtr (PcdH2OEsrtSystemFirmwareGuid));
}

/**
 Check if root file system has capsule image file or not.

 @param[in] SysRootHandle     A pointer to a file handle.

 @retval TRUE                  It has the capsule image file.
 @retval FALSE                 It does not has the capsule image file or the founded capsule is invalid.
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
  CHAR16                                 *CapsuleImagePath;
  EFI_CAPSULE_HEADER                     *CapsuleHeader;
  EFI_FILE_INFO                          **CapsuleFileInfo;
  UINTN                                  TotalCapsules;
  UINTN                                  Index;
  UINT8                                  *FileInfoPtr;
  UINTN                                  LongestCapsuleFileNameSize;
  UINTN                                  ReallocatedSize;

  CapsuleFileInfo = NULL;
  CapsuleImagePath = NULL;

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

  FileInfoSize               = 0;
  TotalCapsules              = 0;
  LongestCapsuleFileNameSize = 0;
  ReallocatedSize            = 0;
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
  FileInfoPtr = (UINT8*)FileInfo;
  while (TRUE) {
    FileSize = FileInfoSize;
    Status = CapsuleHandle->Read (
                              CapsuleHandle,
                              &FileSize,
                              (VOID *) FileInfoPtr
                              );
    if (EFI_ERROR (Status) || FileSize == 0) {
      break;
    }
    if (((EFI_FILE_INFO*)FileInfoPtr)->FileSize > 0 &&
        ((((EFI_FILE_INFO*)FileInfoPtr)->Attribute & EFI_FILE_DIRECTORY) != EFI_FILE_DIRECTORY)) {
      if (CapsuleFileInfo == NULL) {
        CapsuleFileInfo = AllocatePool (sizeof (EFI_FILE_INFO*) * (UINTN)DivU64x32((UINT64)FileInfoSize, SIZE_OF_EFI_FILE_INFO));
        if (CapsuleFileInfo == NULL) {
          goto Exit;
        }
      }
      LongestCapsuleFileNameSize = MAX (LongestCapsuleFileNameSize, StrSize (((EFI_FILE_INFO*)FileInfoPtr)->FileName));
      CapsuleFileInfo[TotalCapsules] = (EFI_FILE_INFO*)FileInfoPtr;
      TotalCapsules ++;
      FileInfoPtr += FileSize;

      if ((FileInfoPtr < (UINT8*)FileInfo) || (FileInfoPtr > ((UINT8*)FileInfo + FileInfoSize))) {
        ReallocatedSize = ReallocatedSize + FileInfoSize + (0x1000 * ((((UINT64 *)FileInfoPtr - (UINT64 *)FileInfo) / FileInfoSize) + 1));
        FileInfo = ReallocatePool(FileInfoSize, ReallocatedSize, (VOID*)FileInfo);
        if (FileInfo == NULL) {
          goto Exit;
        }
        FileInfoSize = ReallocatedSize;
        FileInfoPtr = (UINT8*)FileInfo;
        for (Index = 0; Index < TotalCapsules; Index ++) {
          CapsuleFileInfo[Index] = (EFI_FILE_INFO*)FileInfoPtr;
          FileInfoPtr += ((EFI_FILE_INFO*)FileInfoPtr)->Size;
        }
      }
    }
  }

  if (CapsuleFileInfo != NULL) {
    CapsuleHandle->Close (CapsuleHandle);
    CapsuleImagePath = AllocatePool (StrSize (EFI_CAPSULE_FILE_PATH) + LongestCapsuleFileNameSize);
    if (CapsuleImagePath == NULL) {
      goto Exit;
    }
    for (Index = 0, CapsuleHandle = NULL; Index < TotalCapsules; Index ++) {
      UnicodeSPrint (CapsuleImagePath, StrSize (EFI_CAPSULE_FILE_PATH) + StrSize (CapsuleFileInfo[Index]->FileName), \
        L"%s\\%s", EFI_CAPSULE_FILE_PATH,CapsuleFileInfo[Index]->FileName);
      Status = EnableFileWriteAttribute (SysRootHandle,CapsuleImagePath);
      if (EFI_ERROR (Status)) {
        goto Exit;
      }
      Status = SysRootHandle->Open (
                                SysRootHandle,
                                &CapsuleHandle,
                                CapsuleImagePath,
                                EFI_FILE_MODE_READ,
                                0
                                );
      if (EFI_ERROR (Status)) {
        goto Exit;
      }
      FileSize = (UINTN)CapsuleFileInfo[Index]->FileSize;
      CapsuleHeader = AllocatePool (FileSize);
      if (CapsuleHeader == NULL) {
        goto Exit;
      }
      Status = CapsuleHandle->Read (
                                CapsuleHandle,
                                &FileSize,
                                (VOID*) CapsuleHeader
                                );
      if (!EFI_ERROR (Status)) {
        if (SupportCapsuleImage (CapsuleHeader) == EFI_SUCCESS) {
          Result = TRUE;
          if (IsSystemFirmwareCapsule (CapsuleHeader)) {
            RecordCapsuleFileName(CapsuleFileInfo[Index]->FileName);
          }
        }
      }
      CapsuleHandle->Close (CapsuleHandle);
      FreePool (CapsuleHeader);
      CapsuleHandle = NULL;
    }
  }

Exit:

  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }
  if (CapsuleFileInfo != NULL) {
    FreePool (CapsuleFileInfo);
  }
  if (CapsuleImagePath != NULL) {
    FreePool (CapsuleImagePath);
  }
  if (CapsuleHandle != NULL) {
    CapsuleHandle->Close (CapsuleHandle);
  }
  return Result;
}

/**
 Get the information of system drive which contain capsule image file from all simple file system instances.

 @param[out] SysRootDevicePath    The pointer of system root device path pointer
 @param[out] SysRoorHandle        The pointer of EFI_FILE_HANDLE of system drive

 @retval EFI_SUCCESS              The system drive information is correctly get
 @return others                   Unable to get system drive information
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
    if (!HaveCapsuleImageFile (RootHandle)) {
      RootHandle->Close (RootHandle);
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
  @return others                   Unable to get system drive information

**/
EFI_STATUS
EFIAPI
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
  if (mCurrentBootOptionData != NULL) {
    TotalBootOptions ++;
  } else {
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
  }
  for (CurrentOption = 0, Found = FALSE; CurrentOption < TotalBootOptions && !Found; CurrentOption ++) {
    if (mCurrentBootOptionData == NULL) {
      UnicodeSPrint (BootOption, sizeof(BootOption), L"Boot%04x", BootOptions[CurrentOption]);
      BootOptionData = CommonGetVariableData (BootOption, &gEfiGlobalVariableGuid);
      if (BootOptionData == NULL) {
        continue;
      }
    } else {
      BootOptionData = mCurrentBootOptionData;
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
      if (mCurrentBootOptionData == NULL) {
        FreePool (BootOptionData);
      }
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
    if (mCurrentBootOptionData == NULL) {
      FreePool (BootOptionData);
    }
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
 @return others                   Unable to get system drive information
**/
EFI_STATUS
EFIAPI
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
 Check if the capsule is a H2OFFT capsule.

  @param[in] CapsuleHeader       Pointer to the Capsule header.

 @retval TRUE                The capsule is a H2OFFT capsule.
 @return others              The capsule is not a H2OFFT capsule.
**/
STATIC
BOOLEAN
IsH2OFFTCapsule (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader
  )
{
  UINT8         *CapsuleImage;

  CapsuleImage = (UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize;

  if ((CapsuleImage[0] == (UINT8)'M') && (CapsuleImage[1] == (UINT8)'Z')) {
    return TRUE;
  }
  return FALSE;
}

/**
  Capsule persistence across reset

  @param[in] CapsuleHeader       Pointer to the Capsule header of the Capsule to
                                 be persisted, the Capsule header must be followed
                                 by Capsule image data
  @param[in] IsUpdatableImage    boolean value indicates the image updatable or not

  @retval EFI_SUCCESS            The Capsule is successfully saved
  @return others                 Failed to persist the Capsule image

**/
STATIC
EFI_STATUS
PersistCapsuleImage (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader,
  IN     BOOLEAN                IsUpdatableImage
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *SysRootDevicePath;
  EFI_FILE_HANDLE           SysRootHandle;
  EFI_FILE_HANDLE           CapsuleHandle;
  CHAR16                    CapsuleImagePath[MAX_STRING_LENGTH];
  UINTN                     Size;
  UINT64                    OsIndications;

  SysRootHandle     = NULL;
  CapsuleHandle     = NULL;
  Status = GetSystemRootInfoFromCurrentBootOption (&SysRootDevicePath, &SysRootHandle);
  if (EFI_ERROR (Status) || SysRootHandle == NULL) {
    return Status;
  }
  //
  // Make sure the Capsule Image directory is created
  //
  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            EFI_CAPSULE_FILE_PATH,
                            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                            EFI_FILE_ARCHIVE | EFI_FILE_DIRECTORY
                            );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  CapsuleHandle->Close (CapsuleHandle);
  //
  // Save Capsule Image, the Capsule image file are named EFI_CAPSULE_FILE_NAME with number
  // if user calls UpdateCapsule multiple times.
  //
  UnicodeSPrint (CapsuleImagePath, MAX_STRING_LENGTH, L"%s\\%s%04X.bin", EFI_CAPSULE_FILE_PATH, EFI_CAPSULE_FILE_NAME, mTimes + ((IsUpdatableImage) ? 0x1000 : 0));
  Status = EnableFileWriteAttribute (SysRootHandle,CapsuleImagePath);
  //
  // If Capsule image file exists, delete it first
  //
  if (!EFI_ERROR (Status)) {
    CapsuleHandle = NULL;
    Status = SysRootHandle->Open (
                              SysRootHandle,
                              &CapsuleHandle,
                              CapsuleImagePath,
                              EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE,
                              0
                              );
    if (Status == EFI_SUCCESS) {
      Status = CapsuleHandle->Delete (CapsuleHandle);
      ASSERT_EFI_ERROR(Status);
    }
  }
  CapsuleHandle = NULL;
  Status = SysRootHandle->Open (
                            SysRootHandle,
                            &CapsuleHandle,
                            CapsuleImagePath,
                            EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
                            EFI_FILE_ARCHIVE
                            );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Size = CapsuleHeader->CapsuleImageSize;
  Status = CapsuleHandle->Write (
                            CapsuleHandle,
                            &Size,
                            (VOID *)CapsuleHeader
                            );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  mTimes ++;
  //
  // Set the EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED into OsIndications variable
  // to indicates the capsule file has ready
  //
  Size          = sizeof(UINT64);
  OsIndications = 0;
  Status = CommonGetVariable (
             L"OsIndications",
             &gEfiGlobalVariableGuid,
             &Size,
             &OsIndications
             );
  if (EFI_ERROR (Status) || (OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED) == 0) {
    OsIndications |= EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED;
    Status = CommonSetVariable (
               L"OsIndications",
               &gEfiGlobalVariableGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
               sizeof(UINT64),
               &OsIndications
               );
  }
  //
  // Make fast boot inactive at next boot because capsule file may be saved in external storage device.
  //
  SetVariableToSensitiveVariable (
    L"TargetHddDevPath",
    &gEfiGenericVariableGuid,
    0,
    0,
    NULL
    );
Exit:
  if (SysRootHandle != NULL) {
    SysRootHandle->Close (SysRootHandle);
  }
  if (CapsuleHandle != NULL) {
    CapsuleHandle->Close (CapsuleHandle);
  }
  return Status;
}

/**
  Function indicate the current completion progress of the firmware
  update

  @param  Completion    A value between 1 and 100 indicating the current completion progress of the firmware update

  @retval EFI_SUCESS    Input capsule is a correct FMP capsule.
**/
STATIC
EFI_STATUS
EFIAPI
CapsuleUpdateProgress (
  IN     UINTN          Completion
  )
{
  H2O_BDS_CP_CAPSULE_UPDATE_PROGRESS_DATA       BdsCapsuleUpdateProgressData;
  EFI_STATUS                                    Status;
  H2O_CAPSULE_UPDATE_PROGRESS_PROTOCOL          *UpdateProgress;

  Status = gBS->LocateProtocol (&gH2OCapsuleUpdateProgressProtocolGuid, NULL, (VOID **) &UpdateProgress);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Use checkpoint to provide project customization interface and use PCD to control this interface is
  // enabled or disabled.
  //
  if (FeaturePcdGet (PcdH2OBdsCpCapsuleUpdateProgressSupported)) {
    BdsCapsuleUpdateProgressData.Size       = sizeof (H2O_BDS_CP_CAPSULE_UPDATE_PROGRESS_DATA);
    BdsCapsuleUpdateProgressData.Status     = H2O_CP_TASK_NORMAL;
    BdsCapsuleUpdateProgressData.Completion = Completion;
    CopyGuid (&BdsCapsuleUpdateProgressData.FwClass, &UpdateProgress->FwClass);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpCapsuleUpdateProgressGuid));
    Status = H2OCpTrigger (&gH2OBdsCpCapsuleUpdateProgressGuid, &BdsCapsuleUpdateProgressData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsCapsuleUpdateProgressData.Status));
    if (BdsCapsuleUpdateProgressData.Status == H2O_CP_TASK_SKIP) {
      return EFI_SUCCESS;
    }
  }
  return CapsuleProgressBar (Completion);
}

/**
  Internal function to check windows UX capsule is whether available.

  @retval TRUE       Windows UX capsule is available.
  @retval FALSE      Windows UX capsule is not available.
**/
STATIC
BOOLEAN
IsWinUxCapsuleAvaiable (
  VOID
  )
{
  EFI_STATUS        Status;
  VOID              *Interface;

  Status = gBS->LocateProtocol (&mWinUxFoundProtocol, NULL, (VOID **) &Interface);
  return (BOOLEAN) (Status == EFI_SUCCESS);
}

/**
  Internal function to set windows UX capsule is available.

  @retval EFI_SUCCESS  Set windows UX capsule to available successfully.
  @retval Other        Unable to set windows UX capsule to available.
**/
STATIC
EFI_STATUS
SetWinUxCapsuleAvailable (
  VOID
  )
{
  EFI_HANDLE         Handle;

  if (IsWinUxCapsuleAvaiable ()) {
    return EFI_SUCCESS;
  }

  Handle = NULL;
  return gBS->InstallProtocolInterface (
                &Handle,
                &mWinUxFoundProtocol,
                EFI_NATIVE_INTERFACE,
                NULL
                );
}

/**
  Internal firmware implements to process the capsule image.

  @param[in]  CapsuleHeader     Points to a capsule header.

  @retval EFI_SUCESS            Process Capsule Image successfully.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
**/
STATIC
EFI_STATUS
InternalProcessCapsuleImage (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader
  )
{
  EFI_STATUS                            Status;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL      *Fmp;
  UINT8                                 FmpImageIndex;
  UINT64                                FmpAttributes;
  UINT32                                Result;
  CHAR16                                *AbortReason;
  EFI_HANDLE                            Handle;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR         *ImageDescriptor;

  Status = EFI_UNSUPPORTED;
  Fmp    = NULL;
  if ((GetSupportedFmp (CapsuleHeader, &Fmp, &FmpImageIndex, &FmpAttributes, &ImageDescriptor)) != EFI_SUCCESS || Fmp == NULL) {
    return Status;
  }
  if ((CapsuleHeader->Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) == CAPSULE_FLAGS_PERSIST_ACROSS_RESET) {
    //
    // Persist capsule image, check image before persist capsule to make the status able to
    // record into permanent storage in case the actual operation failed
    //
    if ((PcdGet64 (PcdOsIndicationsSupported) & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED) == 0) return EFI_UNSUPPORTED;
    Result = IMAGE_UPDATABLE_VALID;
    Fmp->CheckImage (
           Fmp,
           FmpImageIndex,
           CapsuleHeader,
           CapsuleHeader->CapsuleImageSize,
           &Result
           );
    if (Result == IMAGE_UPDATABLE_VALID) {
      //
      // Persist capsule image to make the actual update to CapsuleLoaderTriggerDxe driver after reset
      //
      Status = PersistCapsuleImage (CapsuleHeader, ((FmpAttributes & IMAGE_ATTRIBUTE_IMAGE_UPDATABLE) ? TRUE : FALSE));
      if (EFI_ERROR (Status)) {
        //
        // Check image again but use large number of size to make it has chance to
        // record EFI_OUT_OF_RESOURCES status into permanent storage
        //
        Fmp->CheckImage (
               Fmp,
               FmpImageIndex,
               (UINT8*)(UINTN)0xffffffff,
               0xffffffff,
               &Result
               );
      } else {
        if (FeaturePcdGet (PcdH2OBiosUpdateFaultToleranceEnabled)) {
          if (IsSystemFirmwareCapsule (CapsuleHeader)) {
            SetFirmwareUpdateProgress (FlashStart);
          }
        }
      }
    }
  } else {
    AbortReason = NULL;
    //
    // Install gH2OCapsuleUpdateProgressProtocolGuid protocol for tool side to send capsule update
    // percentage to BIOS.
    //
    if (CompareGuid (&ImageDescriptor->ImageTypeId, &gWindowsUxCapsuleGuid)) {
      SetWinUxCapsuleAvailable ();
    }
    Handle = NULL;
    if (IsWinUxCapsuleAvaiable () || !IsH2OFFTCapsule (CapsuleHeader)) {
      mCapsuleUpdateProgressProtocol.CapsuleUpdateProgress = CapsuleUpdateProgress;
      CopyGuid (&mCapsuleUpdateProgressProtocol.FwClass, &ImageDescriptor->ImageTypeId);
      Status = gBS->InstallProtocolInterface (
                      &Handle,
                      &gH2OCapsuleUpdateProgressProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mCapsuleUpdateProgressProtocol
                      );
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
    //
    // Update capsule now
    //
    Status = Fmp->SetImage (
                    Fmp,
                    FmpImageIndex,
                    CapsuleHeader,
                    CapsuleHeader->CapsuleImageSize,
                    NULL,
                    CapsuleUpdateProgress,
                    &AbortReason
                    );
    if (Handle != NULL) {
      //
      // Uninstall gH2OCapsuleUpdateProgressProtocolGuid protocol to prevent from system has multiple
      // gH2OCapsuleUpdateProgressProtocolGuid protocol instances.
      //
      Status = gBS->UninstallProtocolInterface (
                      Handle,
                      &gH2OCapsuleUpdateProgressProtocolGuid,
                      &mCapsuleUpdateProgressProtocol
                      );
    }
    if (AbortReason != NULL) {
      DEBUG ((EFI_D_ERROR, "%s\n", AbortReason));
      FreePool(AbortReason);
    }
  }
  if (EFI_ERROR (Status)) {
    //
    // Only EFI_UNSUPPORTED can be returned when error occurred according to UEFI spec 7.5.3
    //
    Status = EFI_UNSUPPORTED;
  }
  return Status;
}

/**
  The firmware implements to process the capsule image.
  The function processes the capsule as a nested FMP capsule first.

  @param[in]  CapsuleHeader     Points to a capsule header.

  @retval EFI_SUCESS            Process Capsule Image successfully.
  @retval EFI_UNSUPPORTED       Capsule image is not supported by the firmware.
**/
EFI_STATUS
EFIAPI
ProcessCapsuleImage (
  IN     EFI_CAPSULE_HEADER     *CapsuleHeader
  )
{
  if (GetSupportedFmp ((EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize), NULL, NULL, NULL, NULL) == EFI_SUCCESS) {
    return InternalProcessCapsuleImage ((EFI_CAPSULE_HEADER *)((UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize));
  }
  return InternalProcessCapsuleImage (CapsuleHeader);
}

/**
  Internal function uses image handle to check this driver is runtime driver or not

  @param[in] ImageHandle   Input Image handle.

  @retval TRUE             This is a runtime driver.
  @retval FALSE            This isn't a runtime driver.
**/
STATIC
BOOLEAN
IsRuntimeDriver (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS                       Status;
  EFI_LOADED_IMAGE_PROTOCOL        *LoadedImage;


  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **)&LoadedImage
                  );
  if (!EFI_ERROR (Status) && LoadedImage->ImageCodeType == EfiRuntimeServicesCode) {
    return TRUE;
  }

  return FALSE;
}


/**
  The constructor function to check allocate runtime data memory to store
  gEfiFirmwareManagementProtocolGuid instances and create callback event for
  gEfiFirmwareManagementProtocolGuid to save new insatlled instance.
  Also create


  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
DxeCapsuleLibConstructor (
  IN      EFI_HANDLE                ImageHandle,
  IN      EFI_SYSTEM_TABLE          *SystemTable
  )
{

  EFI_STATUS          Status;
  VOID                *Registration;
  EFI_HANDLE          Handle;

  Status = gBS->LocateProtocol (&mFmpInstanceInfoProtocol, NULL, (VOID **) &mFmpInstanceInfo);
  if (EFI_ERROR (Status)) {
    mFmpInstanceInfo = AllocateRuntimeZeroPool (sizeof (FMP_INSTANCE_INFO));
    if (mFmpInstanceInfo == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &mFmpInstanceInfoProtocol,
                    EFI_NATIVE_INTERFACE,
                    (VOID *) mFmpInstanceInfo
                    );
    ASSERT_EFI_ERROR (Status);
  }

  if (FeaturePcdGet (PcdH2OBdsCpReadyToBootBeforeSupported)) {
    H2O_CP_HANDLE        CpHandle;

    //
    // Ues of H2O_CP_HIGH to make sure current boot option data is updated before CapsuleLoaderTrigger call GetSystemRootInfo().
    //
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpReadyToBootBeforeGuid,
               ReadyToBootBeforeCpHandler,
               H2O_CP_HIGH,
               &CpHandle
               );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpReadyToBootBeforeGuid, Status));
  }

  if (IsRuntimeDriver (ImageHandle)) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    VariableAddressChangeEvent,
                    NULL,
                    &gEfiEventVirtualAddressChangeGuid,
                    &mVirtualAddressChangeEvent
                    );
    ASSERT_EFI_ERROR (Status);
  }

  mFirmwareManagementEvent = EfiCreateProtocolNotifyEvent(
                               &gEfiFirmwareManagementProtocolGuid,
                               TPL_NOTIFY,
                               FirmwareManagementCallback,
                               NULL,
                               &Registration
                               );
  ASSERT (mFirmwareManagementEvent != NULL);

  return EFI_SUCCESS;
}

/**
  If a runtime driver exits with an error, it must call this routine
  to free the allocated resource before the exiting.
  It will ASSERT() if gBS is NULL.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval  EFI_SUCCESS      The Runtime Driver Lib shutdown successfully.
**/
EFI_STATUS
EFIAPI
DxeCapsuleLibDestruct (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  ASSERT (gBS != NULL);
  if (mFirmwareManagementEvent != NULL) {
    Status = gBS->CloseEvent (mFirmwareManagementEvent);
    ASSERT_EFI_ERROR (Status);
  }
  if (mVirtualAddressChangeEvent != NULL) {
    Status = gBS->CloseEvent (mVirtualAddressChangeEvent);
    ASSERT_EFI_ERROR (Status);
  }
  if (mReadyToBootEvent != NULL) {
    Status = gBS->CloseEvent (mReadyToBootEvent);
    ASSERT_EFI_ERROR (Status);
  }
  return EFI_SUCCESS;
}
