/** @file


;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/Smbios.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg
#include <Protocol/H2OMemoryDeviceInfo.h>     // InsydePlatformInfoPkg

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryDeviceInfoLib.h>      // InsydePlatformInfoPkg
#include <Library/SmbiosCommonLib.h>          // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

/**
  Handler function for displaying memory information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
MemoryInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_STATUS                        Status;
  H2O_MEMORY_DEVICE_INFO_PROTOCOL   *MemoryInfo;
  UINTN                             NumberOfMemoryInfo;
  UINTN                             NumberOfInstallMemory;
  UINTN                             NumberOfUnInstallMemory;
  SMBIOS_TABLE_TYPE17               **SmbiosType17Records;
  UINTN                             FirstMemoryModuleIndex;
  UINTN                             Index;
  CHAR16                            *DeviceLocatorString;
  CHAR16                            *ManufacturerString;
  CHAR16                            *PartNumberString;
  CHAR16                            *MemoryTypeString;
  UINT32                            MemorySize;
  UINT32                            MemorySpeed;
  UINT64                            TotalMemorySize;
  CHAR16                            *TotalMemorySizeValue;
  CHAR16                            *ConfiguredMemorySpeedValue;
  UINT32                            ConfiguredMemorySpeed;

  INT32  SummaryLineAttributes[] = {
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK)
  };

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (!FeaturePcdGet (PcdH2OMemoryDeviceInfoSupported)) {
    return;
  }
  Status = gBS->LocateProtocol (
                  &gH2OMemoryDeviceInfoProtocolGuid,
                  NULL,
                  (VOID**) &MemoryInfo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_INFO,
      "%a() - Fail to locate protocol %g, Status: %r\n",
      __FUNCTION__,
      &gH2OMemoryDeviceInfoProtocolGuid,
      Status
      ));
    return;
  }

  Status = MemoryInfo->GetAllMemoryDeviceInfo (
                         MemoryInfo,
                         &NumberOfMemoryInfo,
                         &NumberOfInstallMemory,
                         &NumberOfUnInstallMemory,
                         &SmbiosType17Records)
                         ;
  ASSERT_EFI_ERROR (Status);

  //
  // Memory Modules
  //
  FirstMemoryModuleIndex = NumberOfMemoryInfo;
  for (Index = 0; Index < NumberOfMemoryInfo; Index++) {
    if (!MemoryInfo->GetMemoryInstallStatus (MemoryInfo, Index)) {
      continue;
    }
    if (FirstMemoryModuleIndex >= NumberOfMemoryInfo) {
      FirstMemoryModuleIndex = Index;
    }

    MemoryInfo->GetDeviceLocatorString (MemoryInfo, Index, &DeviceLocatorString);
    MemoryInfo->GetManufacturerString (MemoryInfo, Index, &ManufacturerString);
    MemoryInfo->GetPartNumberString (MemoryInfo, Index, &PartNumberString);
    MemoryInfo->GetMemorySize (MemoryInfo, Index, &MemorySize);
    MemoryInfo->GetMemoryTypeString (MemoryInfo, Index, &MemoryTypeString, NULL);
    MemoryInfo->GetMaximumCapableMemorySpeed (MemoryInfo, Index, &MemorySpeed);

    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_MEMORY_MODULE_INFORMATION_FORMAT),
                      NULL, // Language
                      DeviceLocatorString,
                      ManufacturerString,
                      PartNumberString,
                      (unsigned int) (MemorySize / (SIZE_1GB / SIZE_1MB)),
                      MemoryTypeString,
                      (unsigned int) MemorySpeed
                      );
     FreePoolIfNonNull (DeviceLocatorString);
     FreePoolIfNonNull (ManufacturerString);
     FreePoolIfNonNull (PartNumberString);
     FreePoolIfNonNull (MemoryTypeString);
  }

  //
  // Summary line
  //
  if (FirstMemoryModuleIndex >= NumberOfMemoryInfo) {
    ASSERT (FirstMemoryModuleIndex < NumberOfMemoryInfo);
    return;
  }
  Index = FirstMemoryModuleIndex;

  MemoryInfo->GetTotalMemorySize (MemoryInfo, &TotalMemorySize);
  TotalMemorySizeValue = TotalMemorySizeStringFormat (TotalMemorySize);

  MemoryInfo->GetConfigureMemorySpeed (MemoryInfo, Index, &ConfiguredMemorySpeed);
  ConfiguredMemorySpeedValue = MemorySpeedStringFormat (ConfiguredMemorySpeed);;

  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    ARRAY_SIZE (SummaryLineAttributes),
                    SummaryLineAttributes,
                    HiiHandle,
                    STRING_TOKEN (STR_MEMORY_SUMMARY_FORMAT),
                    NULL, // Language
                    TotalMemorySizeValue,
                    (unsigned int) NumberOfInstallMemory,
                    ConfiguredMemorySpeedValue
                    );
  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  FreePoolIfNonNull (TotalMemorySizeValue);
  FreePoolIfNonNull (ConfiguredMemorySpeedValue);

  FreePoolIfNonNull (SmbiosType17Records);

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}
