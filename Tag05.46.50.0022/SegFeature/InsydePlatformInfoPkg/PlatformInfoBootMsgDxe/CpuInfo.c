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

//
// Enable corrected field names of <IndustryStandard/SmBios.h>.
// SMBIOS_CORRECT_TYPO is introduced in H2O Kernel Tag 05.43.08 (IB19060002)
// and obsolete in Tag 05.43.16 (IB07300543).
//
#define SMBIOS_CORRECT_TYPO

#include <Uefi.h>

#include <IndustryStandard/SmBios.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/Smbios.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg
#include <Protocol/H2OCpuDeviceInfo.h>        // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CpuDeviceInfoLib.h>         // InsydePlatformInfoPkg
#include <Library/SmbiosCommonLib.h>          // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

/**
  Helper function to get installed size from SMBIOS Table Type 07h.

  @param[in]  SmbiosTableType7  A pointer points to header of SMBIOS Table Type 07h.

  @retval   A formatted string to express the size.

**/
STATIC
CHAR16 *
SmbiosGetInstalledSizeString (
  IN  UINT32  InstalledCacheSize
  )
{
  CHAR16  StringBuffer[MAX_STRING_SIZE];

  if (InstalledCacheSize >= (SIZE_1MB / SIZE_1KB)) {
    UnicodeSPrint (
      StringBuffer,
      sizeof (StringBuffer), L"%d KiB (%d MiB)",
      InstalledCacheSize,
      (InstalledCacheSize / (SIZE_1MB / SIZE_1KB))
      );
  } else {
    UnicodeSPrint (StringBuffer, sizeof (StringBuffer), L"%d KiB", InstalledCacheSize);
  }

  return AllocateCopyPool (sizeof (StringBuffer), StringBuffer);
}

/**
  Handler function for displaying processor and cache information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
CpuInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_STATUS                    Status;
  H2O_CPU_DEVICE_INFO_PROTOCOL  *CpuDeviceInfo;
  UINTN                         TotalSmbiosType4;
  UINTN                         TotalInstallCpu;
  SMBIOS_TABLE_TYPE4            **SmbiosTableType4;
  UINTN                         Index;
  BOOLEAN                       SocketPopulated;
  UINTN                         EnabledCoreCount;
  UINTN                         FirstSocketIndex;

  UINT32                        ProcessorSignatureValue;
  UINT16                        ProcessorSpeedValue;

  CHAR16                        *ProcessorVersionString;
  CHAR16                        *ProcessorManufacturerString;
  CHAR16                        *ProcessorSignatureString;
  CHAR16                        *ProcessorSpeedString;
  UINT32                        InstalledCacheSizeL1;
  UINT32                        InstalledCacheSizeL2;
  UINT32                        InstalledCacheSizeL3;
  CHAR16                        *L1InstalledCacheSizeString;
  CHAR16                        *L2InstalledCacheSizeString;
  CHAR16                        *L3InstalledCacheSizeString;

  INT32  SummaryLineAttributes[] = {
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK)
  };

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (!FeaturePcdGet (PcdH2OCpuDeviceInfoSupported)) {
    return;
  }
  Status = gBS->LocateProtocol (
                  &gH2OCpuDeviceInfoProtocolGuid,
                  NULL,
                  (VOID**) &CpuDeviceInfo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_INFO,
      "%a() - Fail to locate protocol %g, Status: %r\n",
      __FUNCTION__,
      &gH2OCpuDeviceInfoProtocolGuid,
      Status
      ));
    return;
  }

  Status = CpuDeviceInfo->GetAllCpuDeviceInfo (
                            CpuDeviceInfo,
                            &TotalSmbiosType4,
                            &TotalInstallCpu,
                            &SmbiosTableType4
                            );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Processor Information (Version, Manufacturer, Signature, Speed)
  //
  EnabledCoreCount = 0;
  FirstSocketIndex = TotalSmbiosType4;
  for (Index = TotalSmbiosType4 - 1; Index != (UINTN)(-1); Index--) {
    //
    // The bits of SMBIOS_TABLE_TYPE4.Status is defined as type
    // PROCESSOR_STATUS_DATA in <IndustryStandard/SmBios.h> but the type is not
    // defined in older versions of the header.
    // The SocketPopulated bit is BIT6.
    //
    SocketPopulated = ((SmbiosTableType4[Index]->Status & BIT6) != 0);
    if (SocketPopulated) {
      if (SmbiosTableType4[Index]->EnabledCoreCount != 0xFF) {
        EnabledCoreCount += SmbiosTableType4[Index]->EnabledCoreCount;
      } else {
        EnabledCoreCount += SmbiosTableType4[Index]->EnabledCoreCount2;
      }
      FirstSocketIndex = Index;
    }
  }
  if (FirstSocketIndex >= TotalSmbiosType4) {
    ASSERT (FirstSocketIndex < TotalSmbiosType4);
    return;
  }

  Index = FirstSocketIndex;

  //
  // Processor Version
  //
  ProcessorVersionString = SmbiosGetStringByNumber (
                             (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType4[Index],
                             SmbiosTableType4[Index]->ProcessorVersion
                             );
  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    0,    // AttributeCount
                    NULL, // Attributes
                    HiiHandle,
                    STRING_TOKEN (STR_PROCESSOR_VERSION_FORMAT),
                    NULL, // Language
                    ProcessorVersionString
                    );
  FreePool (ProcessorVersionString);

  //
  // Processor Manufacturer
  //
  ProcessorManufacturerString = SmbiosGetStringByNumber (
                                  (EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType4[Index],
#if FixedPcdGet32 (PcdH2OCcbVersion) >= 0x05430800
                                  // Struct field name corrected in Tag 05.43.08 (IB19060002)
                                  SmbiosTableType4[Index]->ProcessorManufacturer
#else
                                  SmbiosTableType4[Index]->ProcessorManufacture
#endif
                                  );
  if (ProcessorManufacturerString != NULL) {
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_PROCESSOR_MANUFACTURER_FORMAT),
                      NULL, // Language
                      ProcessorManufacturerString
                      );
    FreePool (ProcessorManufacturerString);
  }

  //
  // Processor ID
  //
  CpuDeviceInfo->GetCpuId (CpuDeviceInfo, Index, &ProcessorSignatureValue);
  ProcessorSignatureString = CpuIdStringFormat (ProcessorSignatureValue);
  if (ProcessorSignatureString != NULL) {
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_PROCESSOR_ID_FORMAT),
                      NULL, // Language
                      ProcessorSignatureString
                      );
    FreePool (ProcessorSignatureString);
  }

  //
  // Cache Information (L1/L2/L3 Installed Cache Size)
  //
  CpuDeviceInfo->GetInstalledCacheSize (CpuDeviceInfo, Index, EfiCacheLevel1, &InstalledCacheSizeL1);
  CpuDeviceInfo->GetInstalledCacheSize (CpuDeviceInfo, Index, EfiCacheLevel2, &InstalledCacheSizeL2);
  CpuDeviceInfo->GetInstalledCacheSize (CpuDeviceInfo, Index, EfiCacheLevel3, &InstalledCacheSizeL3);
  L1InstalledCacheSizeString = SmbiosGetInstalledSizeString (InstalledCacheSizeL1);
  L2InstalledCacheSizeString = SmbiosGetInstalledSizeString (InstalledCacheSizeL2);
  L3InstalledCacheSizeString = SmbiosGetInstalledSizeString (InstalledCacheSizeL3);

  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    0,    // AttributeCount
                    NULL, // Attributes
                    HiiHandle,
                    STRING_TOKEN (STR_CACHE_SIZES_FORMAT),
                    NULL, // Language
                    L1InstalledCacheSizeString,
                    L2InstalledCacheSizeString,
                    L3InstalledCacheSizeString
                    );
  FreePoolIfNonNull (L1InstalledCacheSizeString);
  FreePoolIfNonNull (L2InstalledCacheSizeString);
  FreePoolIfNonNull (L3InstalledCacheSizeString);

  //
  // Summary line
  //
  CpuDeviceInfo->GetCurrentSpeed (CpuDeviceInfo, Index, &ProcessorSpeedValue);
  ProcessorSpeedString = CurrentSpeedStringFormat (ProcessorSpeedValue);

  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    ARRAY_SIZE (SummaryLineAttributes),
                    SummaryLineAttributes,
                    HiiHandle,
                    STRING_TOKEN (STR_PROCESSOR_SUMMARY_FORMAT),
                    NULL, // Language
                    (unsigned int) EnabledCoreCount,
                    (unsigned int) TotalInstallCpu,
                    ProcessorSpeedString
                    );
  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  FreePoolIfNonNull (ProcessorSpeedString);

  FreePool (SmbiosTableType4);

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}
