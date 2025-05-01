/** @file
  CPU information setup dxe header file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#ifndef _CPU_INFO_SETUP_UTILITY_DXE_H_
#define _CPU_INFO_SETUP_UTILITY_DXE_H_

#include <Uefi.h>

#include <Guid/MdeModuleHii.h>

#include <IndustryStandard/SmBios.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuDeviceInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiExLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/LanguageDataLib.h>

#include <Protocol/H2OCpuDeviceInfo.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/SetupUtilityApplication.h>
#include <Protocol/Smbios.h>

#include <Uefi/UefiInternalFormRepresentation.h>

#include "CpuInfoFormset.h"
//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8 CpuInfoVfrBin[];

#define MAX_STR_SIZE  0x100

EFI_STATUS
typedef
(EFIAPI *INIT_ITEM_FUNCTION) (
  IN     EFI_HII_HANDLE       HiiHandle
  );

EFI_STATUS
typedef
(EFIAPI *DISPLAY_ITEM_FUNCTION) (
  IN     UINTN                       Index,
     OUT CHAR16                      **String
  );

#define CPU_INFO_PRIVATE_SIGNATURE SIGNATURE_32 ('C', 'P', 'U', 'I')

typedef struct {
  UINT32                             Signature;
  EFI_HANDLE                         SeviceHandle;
  EFI_HII_HANDLE                     HiiHandle;

  //
  // Produced protocols
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL     ConfigAccess;
} CPU_INFO_PRIVATE_DATA;

#pragma pack(1)
typedef struct {
  UINT8                              ItemDisplay;
  EFI_STRING_ID                      Prompt;
  EFI_STRING_ID                      Help;
  EFI_STRING_ID                      TextTwo;
  DISPLAY_ITEM_FUNCTION              DisplayItemFunction;
} CPU_DISPLAY_LIST;

typedef struct {
  VENDOR_DEVICE_PATH                 VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL           End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

/**
  Update Processor Version string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     ProcessorVersionString   The Processor Version string. Caller should free it after use.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateProcessorVersionString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **ProcessorVersionString
  );

/**
  Update CPU ID string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuIdString              Get CPU ID string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuIdString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuIdString
  );

/**
  Update CPU speed string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuSpeedString           Get CPU speed string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuSpeedString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuSpeedString
  );

/**
  Update CPU TDP string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuTDPString             Get CPU TDP string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuTDPString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuTDPString
  );

/**
  Update CPU voltage string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuVoltageString         Get CPU voltage string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuVoltageString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuVoltageString
  );

/**
  Update CPU stepping string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuSteppingString        Get CPU stepping string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuSteppingString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuSteppingString
  );

/**
  Update core l1 data cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL1DataCacheString    Get core l1 data cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL1DataCacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL1DataCacheString
  );

/**
  Update core l1 insruction cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL1InsructionCacheString Get core l1 insruction cache string.

  @retval         EFI_SUCCESS                 Success.
**/
EFI_STATUS
UpdateCoreL1InsructionCacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL1InsructionCacheString
  );

/**
  Update core l2 cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL2CacheString        Get core l2 cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL2CacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL2CacheString
  );

/**
  Update core l3 cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL3CacheString        Get core l3 cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL3CacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL3CacheString
  );

/**
  Update core thread num string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreThreadNumString      Get core thread num string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreThreadNumString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreThreadNumString
  );

/**
  Update microcode version string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     MicrocodeVersionString   Get microcode version string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateMicrocodeVersionString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **MicrocodeVersionString
  );

#endif

