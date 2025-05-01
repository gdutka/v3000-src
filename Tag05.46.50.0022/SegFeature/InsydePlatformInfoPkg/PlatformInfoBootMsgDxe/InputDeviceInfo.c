/** @file


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

#include <IndustryStandard/Usb.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HidDescriptorLib.h>         // InsydeModulePkg
#include <Library/UsbHandleRoutineLib.h>      // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

/**
  Handler function for displaying USB (peripheral) information.

  @param[in]  SimpleTextOut   A pointer to the instance of Simple Text
                              Output protocol.
  @param[in]  MaxRow          Maximum row count of current mode excluding reserved rows for
                              badging strings and hot key information.

  @retval EFI_SUCCESS   Successfully display system information.

**/
VOID
InputDeviceInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *Console,
  IN UINTN                              MaxColumn,
  IN UINTN                              MaxRow,
  IN H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg,
  IN EFI_HII_HANDLE                     HiiHandle
  )
{
  EFI_STATUS  Status;
  UINTN       NumberOfHandles;
  EFI_HANDLE  *HandleBuffer;
  UINTN       NumberOfKeyboards;
  UINTN       NumberOfMice;

  INT32  SummaryLineAttributes[] = {
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK),
    (EFI_WHITE      | EFI_BACKGROUND_BLACK)
  };

  HandleBuffer      = NULL;
  NumberOfKeyboards = 0;
  NumberOfMice      = 0;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = LocateUsbHandlesByClassCode (
             USB_DESC_TYPE_INTERFACE,
             CLASS_HID,
             SUBCLASS_BOOT,
             PROTOCOL_KEYBOARD,
             &NumberOfHandles,
             &HandleBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Fail to LocateUsbHandlesByClassCode, Status: %r\n", Status));
  } else {
    NumberOfKeyboards = NumberOfHandles;
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
    HandleBuffer = NULL;
  }

  Status = LocateUsbHandlesByClassCode (
             USB_DESC_TYPE_INTERFACE,
             CLASS_HID,
             SUBCLASS_BOOT,
             PROTOCOL_MOUSE,
             &NumberOfHandles,
             &HandleBuffer
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Fail to LocateUsbHandlesByClassCode, Status: %r\n", Status));
  } else {
    NumberOfMice = NumberOfHandles;
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  ConsoleBootMsg->ConsolePrintHiiString (
                    Console,
                    MaxColumn,
                    MaxRow,
                    ARRAY_SIZE (SummaryLineAttributes),
                    SummaryLineAttributes,
                    HiiHandle,
                    STRING_TOKEN (STR_INPUT_DEVICE_SUMMARY_FORMAT),
                    NULL, // Language
                    (unsigned int) NumberOfKeyboards,
                    (unsigned int) NumberOfMice
                    );
  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}