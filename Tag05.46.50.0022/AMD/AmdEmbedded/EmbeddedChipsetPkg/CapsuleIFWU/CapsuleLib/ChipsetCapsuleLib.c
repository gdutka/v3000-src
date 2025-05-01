/** @file
  Library instance to Capsule Update Criteria check

;******************************************************************************
;* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
//[-start-190613-IB16990069-add]//
#include <H2OIhisi.h>
//[-end-190613-IB16990069-add]//
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FdSupportLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/VariableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EfiSystemResourceTable.h>
#include <Library/ChipsetCapsuleLib.h>
#include <Library/SioLib.h>

#include <Library/FWUpdateLib.h>
#include <Protocol/FirmwareManagement.h>

STATIC UINT8 AmdEcSig[]  = {'P', 'H', 'C', 'M'};

//
//  MSFT compiler uses this variable to inform itself whether floating
//  point support is loaded.
//
static int                         _fltused;
//[-start-190613-IB16990069-add]//
UINT8                  mAcStatus        = AC_PlugIn;
UINT8                  mBatteryLife     = 0x64;
UINT8                  mBatteryLowBound = 0;

UINT8
EFIAPI
GetPlatformInfo (
  IN OUT UINT8            *FbtsBuffer,
  IN     UINT16           SmiPort
  );

UINT8
EFIAPI
GetCmdBuffer (
  OUT    UINT32           *CmdBuffer,
  IN     UINT16           SmiPort
  );

/**
  Decode character in ASCII to hexadecimal

  @param[in] Asc           Character in ASCII

  @retval Value in hexadecimal.
**/
UINT8
AsciiToHexadecimal (
  IN UINT8  Asc
  )
{
  if (Asc >= '0' && Asc <= '9') {
    return Asc - '0';
  } else if (Asc >= 'A' && Asc <= 'F') {
    return Asc - 'A' + 0xA;
  } else if (Asc >= 'a' && Asc <= 'f') {
    return Asc - 'a' + 0xA;
  }

  return 0;
}

VOID
EFIAPI
UpdatePlatformStatus (
  VOID
  )
{
  FBTS_TOOLS_VERSION_BUFFER   *FbtsSupportBuffer;
  UINT32                      CmdBuffer;
  UINT8                       IhisiStatus;

  FbtsSupportBuffer = NULL;
  IhisiStatus = GetCmdBuffer (&CmdBuffer, PcdGet16 (PcdSoftwareSmiPort));
  if (IhisiStatus != 0) {
    FbtsSupportBuffer = AllocateZeroPool (sizeof (FBTS_TOOLS_VERSION_BUFFER));
    if (FbtsSupportBuffer == NULL) {
      return ;
    }
  } else {
    FbtsSupportBuffer = (FBTS_TOOLS_VERSION_BUFFER *) ((UINTN) CmdBuffer);
    ZeroMem (FbtsSupportBuffer, sizeof (FBTS_TOOLS_VERSION_BUFFER));
  }

  //
  // Append signature for IHISI identification
  //
  FbtsSupportBuffer->Signature   = FBTS_VERSION_SIGNATURE;

  //
  // Call IHISI FBTS 10h
  //
  GetPlatformInfo ((UINT8 *) FbtsSupportBuffer, PcdGet16 (PcdSoftwareSmiPort));

  mAcStatus        = ((FBTS_PLATFORM_STATUS_BUFFER *) FbtsSupportBuffer)->AcStatus;
  mBatteryLife     = ((FBTS_PLATFORM_STATUS_BUFFER *) FbtsSupportBuffer)->Battery;
  mBatteryLowBound = ((FBTS_PLATFORM_STATUS_BUFFER *) FbtsSupportBuffer)->Bound;

  if (IhisiStatus != 0) {
    gBS->FreePool (FbtsSupportBuffer);
  }
}
//[-end-190613-IB16990069-add]//

/**
  Get system firmware revision for ESRT from capsule image

  @param  CapsuleHeader         Points to a capsule header.

  @return                       The system firmware revision from the capsule image
                                If the signature cannot be found, 0x00000000 will
                                be returned
**/
UINT32
EFIAPI
GetCapsuleFirmwareVersion (
  IN EFI_CAPSULE_HEADER          *CapsuleHeader
  )
{
  UINTN                          Index;
  UINT8                          *FwCapsuleImage;
  UINT32                         FwVersion;
  UINT8                          FrontNum;
  UINT8                          MediumNum;
  UINT8                          BackNum;
  UINT8                          HdrFlag;

  FwCapsuleImage    = NULL;
  FwVersion         = 0x00;
  FrontNum          = 0x00;
  MediumNum         = 0x00;
  BackNum           = 0x00;
  HdrFlag           = 0x00;

  if (CompareGuid (&CapsuleHeader->CapsuleGuid, PcdGetPtr (PcdWindowsEcFirmwareCapsuleGuid))) {
    for (Index = 0; Index < CapsuleHeader->CapsuleImageSize; Index++) {
      FwCapsuleImage = (UINT8 *)CapsuleHeader + CapsuleHeader->HeaderSize + Index;
      if (CompareMem (FwCapsuleImage, AmdEcSig, sizeof (AmdEcSig)) == 0) {
        if (HdrFlag != 0) {
          //
          // Get EC Version
          //
          CopyMem (&FrontNum, FwCapsuleImage + 0x1E, 0x01);
          CopyMem (&MediumNum, FwCapsuleImage + 0x20, 0x01);
          CopyMem (&BackNum, FwCapsuleImage + 0x22, 0x01);
          FrontNum = AsciiToHexadecimal (FrontNum);
          MediumNum = AsciiToHexadecimal (MediumNum);
          BackNum = AsciiToHexadecimal (BackNum);
          FwVersion = ((FwVersion | (UINT32) FrontNum) << 8) + ((UINT32) MediumNum << 4) + (UINT32) BackNum;

          return FwVersion;
        } else {
          HdrFlag++;
          continue;
        }
      }
    }
  }

  return 0;
}


/**
  AC power source existence check

  @param  CapsuleHeader                      Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
AcPowerCheck (
  EFI_CAPSULE_HEADER             *CapsuleHeader
  )
{
//[-start-190613-IB16990069-add]//
  if (mAcStatus == AC_PlugOut) {
    return FALSE;
  }
//[-end-190613-IB16990069-add]//
  return TRUE;
}

/**
  Battery power check

  @param  CapsuleHeader                      Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
BatteryPowerCheck (
  EFI_CAPSULE_HEADER             *CapsuleHeader
  )
{
//[-start-190613-IB16990069-add]//
  if (mBatteryLife < mBatteryLowBound) {
    return FALSE;
  }
//[-end-190613-IB16990069-add]//
  return TRUE;
}

/**
  Security check

  @param  CapsuleHeader                      Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
SecurityCheck (
  EFI_CAPSULE_HEADER             *CapsuleHeader
  )
{
  return TRUE;
}

/**
  Capsule image integrity check

  @param  CapsuleHeader                      Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
IntegrityCheck (
  EFI_CAPSULE_HEADER             *CapsuleHeader
  )
{
  return TRUE;
}

/**
  Get FMP LowestSupportedImageVersion by ImageTypeId.

  @param[in]     ImageTypeId                  Used to identify device firmware targeted by this update.
  @param[out]    LowestSupportedImageVersion  Describes the lowest ImageDescriptor version that the device will accept.

  @retval EFI_SUCCESS            LowestSupportedImageVersion is found in existing FMP instances.
  @retval EFI_NOT_FOUND          No FMP instances match the search.
  @retval EFI_INVALID_PARAMETER  Invalid parameters.
**/
EFI_STATUS
GetFmpLowestSupportedImageVersionByImageTypeId (
  IN        EFI_GUID                           *ImageTypeId,
  OUT       UINT32                             *LowestSupportedImageVersion
  )
{
  EFI_STATUS                                    Status;
  EFI_HANDLE                                    *HandleBuffer;
  UINTN                                         NumberOfHandles;
  EFI_FIRMWARE_MANAGEMENT_PROTOCOL              *Fmp;
  UINTN                                         Index;
  UINTN                                         Index2;
  UINTN                                         ImageInfoSize;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                 *FmpImageInfoBuf;
  UINT32                                        FmpImageInfoDescriptorVer;
  UINT8                                         FmpImageInfoCount;
  UINTN                                         DescriptorSize;
  UINT32                                        PackageVersion;
  CHAR16                                        *PackageVersionName;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR                 *TempFmpImageInfo;

  if ((ImageTypeId == NULL) || (LowestSupportedImageVersion == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareManagementProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol(
                    HandleBuffer[Index],
                    &gEfiFirmwareManagementProtocolGuid,
                    (VOID **)&Fmp
                    );
    if (EFI_ERROR(Status)) {
      continue;
    }

    ImageInfoSize = 0;
    Status = Fmp->GetImageInfo (
                    Fmp,
                    &ImageInfoSize,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                    );
    if (Status != EFI_BUFFER_TOO_SMALL) {
      continue;
    }

    FmpImageInfoBuf = AllocateZeroPool (ImageInfoSize);
    if (FmpImageInfoBuf == NULL) {
      continue;
    }

    PackageVersionName = NULL;
    Status = Fmp->GetImageInfo (
                    Fmp,
                    &ImageInfoSize,               // ImageInfoSize
                    FmpImageInfoBuf,              // ImageInfo
                    &FmpImageInfoDescriptorVer,   // DescriptorVersion
                    &FmpImageInfoCount,           // DescriptorCount
                    &DescriptorSize,              // DescriptorSize
                    &PackageVersion,              // PackageVersion
                    &PackageVersionName           // PackageVersionName
                    );
    if (EFI_ERROR(Status)) {
      FreePool(FmpImageInfoBuf);
      continue;
    }

    if (PackageVersionName != NULL) {
      FreePool(PackageVersionName);
    }

    TempFmpImageInfo = FmpImageInfoBuf;
    for (Index2 = 0; Index2 < FmpImageInfoCount; Index2++) {
      //
      // Check if this FMP instance matches.
      // LowestSupportedImageVersion is introduced with DescriptorVersion 2+
      //
      if ((CompareGuid(ImageTypeId, &TempFmpImageInfo->ImageTypeId)) && FmpImageInfoDescriptorVer >= 2) {
        *LowestSupportedImageVersion = TempFmpImageInfo->LowestSupportedImageVersion;
        FreePool (FmpImageInfoBuf);
        FreePool (HandleBuffer);
        return EFI_SUCCESS;
      }
      TempFmpImageInfo = (EFI_FIRMWARE_IMAGE_DESCRIPTOR *)((UINT8 *)TempFmpImageInfo + DescriptorSize);
    }
    FreePool (FmpImageInfoBuf);
  }

  FreePool (HandleBuffer);

  return EFI_NOT_FOUND;
}

/**
  Storage check

  @param  CapsuleHeader                      Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
StorageCheck (
  EFI_CAPSULE_HEADER             *CapsuleHeader
  )
{
  return TRUE;
}


/**
  Pre-installation check for Capsule Update

  @param  CapsuleHeader                      Points to a capsule header.

  @retval ESRT_SUCCESS                       The Capsule passed the pre-installation criteria
  @retval ESRT_ERROR_UNSUCCESSFUL            The pre-installation criteria check failed
  @retval ESRT_ERROR_INSUFFICIENT_RESOURCES  Out of memory or persistent storage
  @retval ESRT_ERROR_INCORRECT_VERSION       Incorrect/incompatible firmware version
  @retval ESRT_ERROR_INVALID_IMAGE_FORMAT    Invalid Capsule image format
  @retval ESRT_ERROR_AUTHENTICATION          Capsule image authentication failed
  @retval ESRT_ERROR_AC_NOT_CONNECTED        The system is not connected to the AC power
  @retval ESRT_ERROR_INSUFFICIENT_BATTERY    The battery capacity is low

**/
ESRT_STATUS
EFIAPI
PreInstallationCheck (
  EFI_CAPSULE_HEADER             *Capsule
  )
{
//[-start-190613-IB16990069-add]//
  UpdatePlatformStatus();

  if (!BatteryPowerCheck(Capsule)) {
    return ESRT_ERROR_INSUFFICIENT_BATTERY;
  }
  //
  // According to Pre-installation criteria from MSFT,
  // (1) System must have at least 25% battery charge.
  // (2) Tethered power (power via USB cable and/or AC power) is not required.
  // so needn't check AC power in PreInstallationCheck function.
  //
 //[-end-190613-IB16990069-add]//
  if (!StorageCheck(Capsule)) {
    return ESRT_ERROR_INSUFFICIENT_RESOURCES;
  }

  if (!IntegrityCheck(Capsule)) {
    return ESRT_ERROR_INVALID_IMAGE_FORMAT;
  }

  if (!SecurityCheck(Capsule)) {
    return ESRT_ERROR_AUTHENTICATION;
  }

  return ESRT_SUCCESS;
}

/**
  Post-installation check for Capsule Update

  @param  CapsuleHeader                      Points to a capsule header.

  @retval ESRT_SUCCESS                       The Capsule passed the pre-installation criteria
  @retval ESRT_ERROR_UNSUCCESSFUL            The pre-installation criteria check failed
  @retval ESRT_ERROR_INSUFFICIENT_RESOURCES  Out of memory or persistent storage
  @retval ESRT_ERROR_AUTHENTICATION          Capsule image authentication failed

**/
ESRT_STATUS
EFIAPI
PostInstallationCheck (
  EFI_CAPSULE_HEADER             *Capsule
  )
{
  return ESRT_SUCCESS;
}
