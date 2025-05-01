/** @file
  Implementation of PlatformInfoBootMsgDxe.

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/GenericBdsLib.h>            // InsydeModulePkg

#include "PlatformInfoBootMsgDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

extern EFI_GUID gEfiCallerIdGuid; // From AutoGen.c

extern UINT8 PlatformInfoBootMsgDxeStrings[];

H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  mPlatformInfoBootMessageContext = {
  NULL,  // ConsoleBootMsg
  NULL,  // HiiHandle
  FALSE, // IsUefiFastBootActive
  NULL,  // DeviceInfoLookup
  NULL,  // SataDriveInfoEvent
  NULL,  // NvmeInfoEvent
  NULL,  // SdEmmcInfoEvent
  NULL,  // SataDriveInfoRegistration
  NULL,  // NvmeInfoRegistration
  NULL,  // SdEmmcInfoRegistration
  FALSE, // RecordCursorPosition
  0,     // RecordTailCursorCol
  0,     // RecordTailCursorRow
  FALSE, // IsSataDeviceExist
  FALSE, // IsNvmeDeviceExist
  FALSE  // IsSdEmmcExist
};

VOID
FreePoolIfNonNull (
  IN VOID  *Buffer
  )
{
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
}

/**
**/
STATIC
EFI_STATUS
EFIAPI
PlatformInfoBootMsgInit (
  IN OUT VOID  *Context OPTIONAL
  )
{
  H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *PlatformInfoBootMsgContext;
  EFI_HII_HANDLE                          HiiHandle;

  PlatformInfoBootMsgContext =
    (H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context;

  //
  // Add HII string package
  //
  if (PlatformInfoBootMsgContext->HiiHandle != NULL) {
    return EFI_ALREADY_STARTED;
  }

  HiiHandle = HiiAddPackages (
                &gEfiCallerIdGuid,
                gImageHandle,
                PlatformInfoBootMsgDxeStrings,
                NULL
                );
  if (HiiHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a: HiiAddPackages failed.\n", gEfiCallerBaseName));
    return EFI_ABORTED;
  }
  PlatformInfoBootMsgContext->HiiHandle = HiiHandle;

  //
  // Detect UEFI OS Fast Boot. Routine provided by GenericBdsLib.
  //
  PlatformInfoBootMsgContext->IsUefiOsFastBootActive =
                                BdsLibIsWin8FastBootActive ();

  return EFI_SUCCESS;
}

/**
**/
STATIC
EFI_STATUS
EFIAPI
PlatformInfoBootMsgOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context OPTIONAL
  )
{
  H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *PlatformInfoBootMsgContext;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL       *ConsoleBootMsg;
  EFI_HII_HANDLE                          HiiHandle;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  PlatformInfoBootMsgContext =
    (H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context;

  ConsoleBootMsg = PlatformInfoBootMsgContext->ConsoleBootMsg;
  HiiHandle      = PlatformInfoBootMsgContext->HiiHandle;

  BasicSystemInfoOutput (
    Console,
    MaxColumn,
    MaxRow,
    ConsoleBootMsg,
    HiiHandle
    );
  CpuInfoOutput (
    Console,
    MaxColumn,
    MaxRow,
    ConsoleBootMsg,
    HiiHandle
    );
  MemoryInfoOutput (
    Console,
    MaxColumn,
    MaxRow,
    ConsoleBootMsg,
    HiiHandle
    );
#ifdef INSYDEIPMIPKG_IMPORTED
  BmcInfoOutput (
    Console,
    MaxColumn,
    MaxRow,
    ConsoleBootMsg,
    HiiHandle
    );
#endif

  if (PlatformInfoBootMsgContext->IsUefiOsFastBootActive) {
    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      0,    // AttributeCount
                      NULL, // Attributes
                      HiiHandle,
                      STRING_TOKEN (STR_UEFI_OS_FAST_BOOT_ACTIVE),
                      NULL  // Language
                      );
  } else {
    InputDeviceInfoOutput (
      Console,
      MaxColumn,
      MaxRow,
      ConsoleBootMsg,
      HiiHandle
      );

    SataDriveInfoOutput (Console, MaxColumn, MaxRow, Context);
    NvmeInfoOutput (Console, MaxColumn, MaxRow, Context);
    SdEmmcInfoOutput (Console, MaxColumn, MaxRow, Context);
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Entry point of this driver.

  @param[in] ImageHandle  Image handle this driver.
  @param[in] SystemTable  Pointer to SystemTable.

**/
EFI_STATUS
EFIAPI
PlatformInfoBootMsgDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg;

  Status = gBS->LocateProtocol (
                  &gH2OConsoleBootMessageProtocolGuid,
                  NULL,
                  (VOID **) &ConsoleBootMsg
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Failed to locate %a: %r\n",
      gEfiCallerBaseName,
      "H2OConsoleBootMessageProtocol",
      Status
      ));
    return Status;
  }
  mPlatformInfoBootMessageContext.ConsoleBootMsg = ConsoleBootMsg;

  Status = ConsoleBootMsg->RegisterDriver (
                             ConsoleBootMsg,
                             H2OConsoleBootMessagePriorityHigh,
                             PlatformInfoBootMsgInit,
                             PlatformInfoBootMsgOutput,
                             &mPlatformInfoBootMessageContext
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Failed to register, Status: %r\n",
      __FUNCTION__,
      Status
      ));
  }

  return Status;
}