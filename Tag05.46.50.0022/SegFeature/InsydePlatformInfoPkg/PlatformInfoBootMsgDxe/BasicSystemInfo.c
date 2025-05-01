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

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmbiosCommonLib.h>          // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

/**
  Displays common system information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
BasicSystemInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_SMBIOS_HANDLE   SmbiosHandle;
  EFI_SMBIOS_TYPE     SmbiosType;
  EFI_STATUS          Status;
  SMBIOS_TABLE_TYPE0  *SmbiosTableType0;
  SMBIOS_TABLE_TYPE1  *SmbiosTableType1;
  CHAR16              *ProductName;
  CHAR16              *BiosVersion;
  CHAR16              *BiosReleaseDate;
  INT32               LastLineAttribute;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  SmbiosHandle  = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType    = SMBIOS_TYPE_BIOS_INFORMATION;     // Type 00h
  Status        = LocateSmbiosRecord (&SmbiosHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &SmbiosTableType0);
  ASSERT_EFI_ERROR (Status);

  SmbiosHandle  = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType    = SMBIOS_TYPE_SYSTEM_INFORMATION;   // Type 01h
  Status        = LocateSmbiosRecord (&SmbiosHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &SmbiosTableType1);
  ASSERT_EFI_ERROR (Status);

  //
  // Trademark
  //
  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    0,    // AttributeCount
                    NULL, // Attributes
                    HiiHandle,
                    STRING_TOKEN (STR_TRADEMARK),
                    NULL  // Language
                    );

  if (SmbiosTableType1 != NULL) {
    //
    // Product Name
    //
    ProductName = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType1, SmbiosTableType1->ProductName);
    ConsoleBootMsg->ConsolePrint (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      L"%s\n",
                      ProductName
                      );
    FreePool (ProductName);

    //
    // UUID
    //
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_SYSTEM_UUID_TEXT_FORMAT),
                      NULL, // Language
                      &SmbiosTableType1->Uuid
                      );
  }

  //
  // BIOS Version and Release Date
  //
  if (SmbiosTableType0 != NULL) {
    BiosVersion     = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType0, SmbiosTableType0->BiosVersion);
    BiosReleaseDate = SmbiosGetStringByNumber ((EFI_SMBIOS_TABLE_HEADER *) SmbiosTableType0, SmbiosTableType0->BiosReleaseDate);

    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_BIOS_VERSION_TEXT_FORMAT),
                      NULL, // Language
                      BiosVersion,
                      BiosReleaseDate
                      );

    FreePool (BiosVersion);
    FreePool (BiosReleaseDate);
  }

  LastLineAttribute = (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
  ConsoleBootMsg->ConsolePrint (
                    Console,
                    MaxColumn,
                    MaxRow,
                    1, // AttributeCount
                    &LastLineAttribute,
                    L"\n"
                    );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}