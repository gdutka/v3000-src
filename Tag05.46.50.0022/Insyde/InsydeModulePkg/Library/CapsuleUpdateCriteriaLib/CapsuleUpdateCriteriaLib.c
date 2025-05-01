/** @file
  Library instance to Capsule Update Criteria check

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

#include <Uefi.h>
#include <H2OIhisi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CapsuleUpdateCriteriaLib.h>
#include <Library/H2OIhisiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/H2OLib.h>
#include <Library/FlashRegionLib.h>
#include <Guid/EfiSystemResourceTable.h>
#include <Protocol/FirmwareManagement.h>

#define BVDT_BME_OFFSET     0x12F
#define BVDT_DYNAMIC_OFFSET 0x15B
#define BVDT_SIZE           FdmGetNAtSize(&gH2OFlashMapRegionBvdtGuid, 1)    //FixedPcdGet32(PcdFlashNvStorageBvdtSize)

STATIC UINT8 EsrtSig[] = {'$', 'E', 'S', 'R', 'T'};
STATIC UINT8 BvdtSig[] = {'$', 'B', 'V', 'D', 'T'};
STATIC UINT8 BmeSig[]  = {'$', 'B', 'M', 'E'};

UINT8                  mAcStatus        = AC_PlugIn;
UINT8                  mBatteryLife     = 0x64;
UINT8                  mBatteryLowBound = 0;

typedef
BOOLEAN
(*CAPABILITY_CHECK_FUNCTION) (
  IN EFI_CAPSULE_HEADER       *CapsuleHeader
  );

typedef struct _CAPSULE_UPDATE_CAPABILITY_FUNCTION {
  CAPABILITY_CHECK_FUNCTION          CheckFn;
  ESRT_STATUS                        ErrorStatus;
  UINT32                             ErrorBit;
} CAPSULE_UPDATE_CAPABILITY_CHECK_INFO;


VOID
EFIAPI
UpdatePlatformStatus (
  VOID
  )
{
  FBTS_PLATFORM_STATUS_BUFFER  FbtsSupportBuffer;
  IHISI_STATUS_CODE            IhisiStatus;
  UINT8                        Version[4];
  UINT8                        AtpVersion[6];
  UINT8                        Permission;
  UINT16                       FbtsVersion;

  ZeroMem (Version, sizeof (Version));
  ZeroMem (AtpVersion, sizeof (AtpVersion));
  H2OIhisiAuthLock ();
  IhisiStatus = H2OIhisiFbtsGetSupportVersion (Version, AtpVersion, &Permission, &FbtsVersion, &FbtsSupportBuffer);
  H2OIhisiAuthUnlock ();

  if (IhisiStatus == IhisiSuccess) {
    mAcStatus        = FbtsSupportBuffer.AcStatus;
    mBatteryLife     = FbtsSupportBuffer.Battery;
    mBatteryLowBound = FbtsSupportBuffer.Bound;
  }
}

/**
  Get system firmware revision for ESRT from capsule image

  @param  CapsuleHeader         Points to a capsule header.

  @return                       The system firmware revision from the capsule image
                                If the signature cannot be found, 0x00000000 will
                                be returned
**/
UINT32
EFIAPI
GetCapsuleSystemFirmwareVersion (
  IN EFI_CAPSULE_HEADER  *CapsuleHeader
  )
{
  UINT8   *Bvdt;
  UINTN   Index;
  BOOLEAN BvdtFound;

  BvdtFound = FALSE;
  Bvdt = NULL;
  for (Index = CapsuleHeader->HeaderSize; Index < CapsuleHeader->CapsuleImageSize; Index++ ) {
    Bvdt = (UINT8 *)CapsuleHeader + Index;
    if ( CompareMem (Bvdt, BvdtSig, sizeof (BvdtSig)) == 0) {
      //
      // $BVDT found, continue to search $BME
      //
      if ( CompareMem (Bvdt + BVDT_BME_OFFSET, BmeSig, sizeof (BmeSig)) == 0) {
        BvdtFound = TRUE;
        break;
      }
    }
  }

  //
  // Search for "$ESRT" from BVDT dynamic signature start
  //
  if (BvdtFound) {
    for (Index = BVDT_DYNAMIC_OFFSET; Index < BVDT_SIZE; Index++) {
      if (CompareMem(Bvdt + Index, EsrtSig, sizeof(EsrtSig)) == 0) {
        return  *(UINT32 *)(Bvdt + Index + sizeof(EsrtSig));
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
  EFI_CAPSULE_HEADER *CapsuleHeader
  )
{

  if (mAcStatus == AC_PlugOut) {
    return FALSE;
  }

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
  EFI_CAPSULE_HEADER *CapsuleHeader
  )
{

  if (mBatteryLife < mBatteryLowBound) {
    return FALSE;
  }

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
  EFI_CAPSULE_HEADER *CapsuleHeader
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
  EFI_CAPSULE_HEADER *CapsuleHeader
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
      // Check if this FMP instance matches
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
  Firmware version check

  @param[in]  CapsuleHeader                  Points to a capsule header.

  @retval TRUE                               Criteria check is successful
  @return others                             Failed to pass the criteria check
**/
BOOLEAN
EFIAPI
VersionCheck (
  IN     EFI_CAPSULE_HEADER *CapsuleHeader
  )
{
  EFI_STATUS                Status;
  EFI_SYSTEM_RESOURCE_TABLE *Esrt;
  UINTN                     Index;
  UINT32                    LowestSupportedImageVersion;

  Status = EfiGetSystemConfigurationTable (&gEfiSystemResourceTableGuid, (VOID **)&Esrt);
  if (Status == EFI_SUCCESS && Esrt != NULL) {
    for (Index = 0; Index < Esrt->FirmwareResourceCount; Index++) {
      if (CompareGuid (&Esrt->FirmwareResources[Index].FirmwareClass, &CapsuleHeader->CapsuleGuid)) {
        if (GetCapsuleSystemFirmwareVersion(CapsuleHeader) >= Esrt->FirmwareResources[Index].LowestSupportedFirmwareVersion) {
          return TRUE;
        }
      }
    }
  }
  //
  // ESRT may not installed. Check version with FMP instances.
  //
  LowestSupportedImageVersion = 0;
  Status = GetFmpLowestSupportedImageVersionByImageTypeId (&CapsuleHeader->CapsuleGuid, &LowestSupportedImageVersion);
  if (Status == EFI_SUCCESS) {
    if (GetCapsuleSystemFirmwareVersion (CapsuleHeader) >= LowestSupportedImageVersion) {
      return TRUE;
    }
  }
  return FALSE;
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
  EFI_CAPSULE_HEADER *CapsuleHeader
  )
{
  return TRUE;
}

//
// According to Pre-installation criteria from MSFT,
// (1) System must have at least 25% battery charge.
// (2) Tethered power (power via USB cable and/or AC power) is not required.
// so needn't add AC power check function in mCheckInfoList.
//
STATIC CAPSULE_UPDATE_CAPABILITY_CHECK_INFO mCheckInfoList[] = {
                                              {BatteryPowerCheck, ESRT_ERROR_INSUFFICIENT_BATTERY  , H2O_BDS_CP_CAPSULE_CHECK_ERROR_INSUFFICIENT_BATTERY  },
                                              {StorageCheck     , ESRT_ERROR_INSUFFICIENT_RESOURCES, H2O_BDS_CP_CAPSULE_CHECK_ERROR_INSUFFICIENT_RESOURCES},
                                              {IntegrityCheck   , ESRT_ERROR_INVALID_IMAGE_FORMAT  , H2O_BDS_CP_CAPSULE_CHECK_ERROR_INVALID_IMAGE_FORMAT  },
                                              {VersionCheck     , ESRT_ERROR_INCORRECT_VERSION     , H2O_BDS_CP_CAPSULE_CHECK_ERROR_INCORRECT_VERSION     },
                                              {SecurityCheck    , ESRT_ERROR_AUTHENTICATION        , H2O_BDS_CP_CAPSULE_CHECK_ERROR_AUTHENTICATION        },
                                              {NULL             , ESRT_ERROR_UNSUCCESSFUL          , H2O_BDS_CP_CAPSULE_CHECK_ERROR_UNSUCCESSFUL          },
                                              {NULL             , ESRT_ERROR_AC_NOT_CONNECTED      , H2O_BDS_CP_CAPSULE_CHECK_ERROR_AC_NOT_CONNECTED      }
                                              };

/**
 Convert capsule update capability error bits to error status.

 @param[in] ErrorBits              Capsule update capability error bits which is defined in H2OCp.h.

 @return Capsule update capability error status.
**/
ESRT_STATUS
EFIAPI
ConvertCapsuleErrorBitsToStatus (
  IN UINT32          ErrorBits
  )
{
  UINTN              Index;

  if (ErrorBits == 0) {
    return ESRT_SUCCESS;
  }

  for (Index = 0; Index < sizeof (mCheckInfoList) / sizeof (CAPSULE_UPDATE_CAPABILITY_CHECK_INFO); Index++) {
    if ((mCheckInfoList[Index].ErrorBit & ErrorBits) != 0) {
      return mCheckInfoList[Index].ErrorStatus;
    }
  }

  return ESRT_SUCCESS;
}

/**
 Check capsule update capability and return the error bits which is defined in H2OCp.h.

 @param[in] Capsule              Points to a capsule.

 @return Capsule update capability error bits or zero if all checks are pass.
**/
UINT32
EFIAPI
CheckCapsuleUpdateCapability (
  IN VOID            *Capsule
  )
{
  UINT32             ErrorBits;
  UINTN              Index;

  UpdatePlatformStatus();

  for (Index = 0, ErrorBits = 0; Index < sizeof (mCheckInfoList) / sizeof (CAPSULE_UPDATE_CAPABILITY_CHECK_INFO); Index++) {
    if (mCheckInfoList[Index].CheckFn != NULL && !mCheckInfoList[Index].CheckFn (Capsule)) {
      ErrorBits |= mCheckInfoList[Index].ErrorBit;
    }
  }
  return ErrorBits;
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
  EFI_CAPSULE_HEADER *Capsule
  )
{
  return ConvertCapsuleErrorBitsToStatus (CheckCapsuleUpdateCapability (Capsule));
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
  EFI_CAPSULE_HEADER *Capsule
  )
{
  return ESRT_SUCCESS;
}
