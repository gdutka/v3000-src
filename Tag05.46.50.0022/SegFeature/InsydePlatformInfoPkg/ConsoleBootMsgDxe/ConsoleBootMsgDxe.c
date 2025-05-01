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

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>

#include <Guid/H2OCp.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/H2OCpLib.h>                 // InsydeModulePkg
#include <Library/KernelConfigLib.h>          // InsydeModulePkg

#include "ConsoleBootMsgDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

//
// H2O checkpoint handles. These are never un-registered.
//
STATIC H2O_CP_HANDLE mBdsCpHandleDisplayBefore = NULL;
STATIC H2O_CP_HANDLE mBdsCpHandleDisplayStringBefore = NULL;

H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA mConsoleBootMsgPrivateData = {
  H2O_CONSOLE_BOOT_MSG_PRIVATE_DATA_SIGNATURE,
  { // Protocol
    ConsoleBootMsgRegisterDriver,
    ConsoleBootMsgAppendOutput,
    ConsoleBootMsgDispatch,
    ConsoleOutputLine,
    ConsoleVaPrint,
    ConsolePrint,
    ConsoleVaPrintHiiString,
    ConsolePrintHiiString
  },
  {
    NULL // RegistrationList
  },
  0      // HighestPriority
};

/**
  Handler function for checkpoint gH2OBdsCpDisplayBeforeGuid.

  @implements H2O_CP_HANDLER

  @param[in]  Event   Pointer to this event
  @param[in]  Handle  The handle associated with a previously
                      registered checkpoint handler.

**/
VOID
EFIAPI
DisplayBeforeHandler (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_STATUS                      Status;
  H2O_BDS_CP_DISPLAY_BEFORE_DATA  *DisplayBeforeData;

  Status = H2OCpLookup (Handle, (VOID **) &DisplayBeforeData, &gH2OBdsCpDisplayBeforeGuid);
  if (!EFI_ERROR (Status)) {
    //
    // Suppress display of system information by H2O kernel so that we can
    // display our own information (not now but later).
    //
    DisplayBeforeData->Features &= ~((UINT32) H2O_BDS_CP_DISPLAY_INFO);
  }
}

/**
  Handler function for checkpoint gH2OBdsCpDisplayStringBeforeGuid.

  This function will be triggered after displaying badging string but
  earlier then hot key information.

  When in text mode, badging string will be placed at (MaxRow - 2), and
  ESC hot key information will be placed at MaxRow by default. MaxRow
  is the maximum row number of current resolution.

  @implements H2O_CP_HANDLER

  @param[in]  Event   Pointer to this event
  @param[in]  Handle  The handle associated with a previously
                      registered checkpoint handler.

**/
VOID
EFIAPI
DisplayStringBeforeHandler (
  IN  EFI_EVENT       Event,
  IN  H2O_CP_HANDLE   Handle
  )
{
  EFI_STATUS            Status;
  KERNEL_CONFIGURATION  Configuration;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = GetKernelConfiguration (&Configuration);
  if (!EFI_ERROR (Status)) {
    if (Configuration.QuietBoot != 0) {
      DEBUG ((
        EFI_D_INFO,
        "%a() - System information not displayed during Quiet Boot.\n",
        __FUNCTION__
        ));
      goto End;
    }

    if (gST->ConOut == NULL) {
      DEBUG ((EFI_D_ERROR, "%a() - Error!! The ConOut is NULL\n", __FUNCTION__));
      goto End;
    }

    (VOID) gST->ConOut->SetCursorPosition (
                          gST->ConOut,
                          0, // Column
                          0  // Row
                          );

    if (gST->ConOut->Mode == NULL) {
      DEBUG ((EFI_D_ERROR, "%a() - Error!! The ConOut->Mode is NULL\n", __FUNCTION__));
      goto End;
    }

    //
    // If Quick Boot is disabled, reserve 5 rows for Memory Test status.
    //
    if (gST->ConOut->Mode->CursorRow < 5) {
      (VOID) gST->ConOut->SetCursorPosition (
                            gST->ConOut,
                            0, // Column
                            5  // Row
                            );
    }
  } else {
    DEBUG ((EFI_D_WARN, "%a() - Fail to GetKernelConfiguration. Status: %r\n", __FUNCTION__, Status));
  }

  ConsoleBootMsgDispatch (&mConsoleBootMsgPrivateData.Protocol);

End:
  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Entry point of this driver.

  @param[in] ImageHandle  Image handle this driver.
  @param[in] SystemTable  Pointer to SystemTable.

**/
EFI_STATUS
ConsoleBootMsgDxeEntry (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                         Status;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gH2OConsoleBootMessageProtocolGuid,
                  NULL, // No registration
                  (VOID **) &ConsoleBootMsg
                  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Protocol exists.\n", __FUNCTION__));
    Status = EFI_ALREADY_STARTED;
    goto End;
  }

  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpDisplayBeforeGuid,
               DisplayBeforeHandler,
               H2O_CP_MEDIUM,
               &mBdsCpHandleDisplayBefore
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a() - Failed to register handler for %a, Status: %r\n",
        __FUNCTION__,
        "H2OBdsCpDisplayBefore",
        Status
        ));
      goto End;
    }
  } else {
    DEBUG ((
      EFI_D_WARN,
      "%a() - %a is unsupported.\n",
      __FUNCTION__,
      "H2OBdsCpDisplayBefore"
      ));
  }

  if (FeaturePcdGet (PcdH2OBdsCpDisplayStringBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpDisplayStringBeforeGuid,
               DisplayStringBeforeHandler,
               H2O_CP_MEDIUM_HIGH,
               &mBdsCpHandleDisplayStringBefore
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a() - Failed to register handler for %a, Status: %r\n",
        __FUNCTION__,
        "H2OBdsCpDisplayStringBefore",
        Status
        ));
      goto End;
    }
  } else {
    DEBUG ((
      EFI_D_WARN,
      "%a() - %a is unsupported.\n",
      __FUNCTION__,
      "H2OBdsCpDisplayStringBefore"
      ));
  }

  InitializeListHead (&mConsoleBootMsgPrivateData.RegistrationList);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gH2OConsoleBootMessageProtocolGuid,
                  &mConsoleBootMsgPrivateData.Protocol,
                  (EFI_GUID *) NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Failed to install protocol interface, Status: %r\n",
      __FUNCTION__,
      Status
      ));
  }

End:

  DEBUG ((EFI_D_INFO, "%a() - End, Status: %r\n", __FUNCTION__, Status));
  return Status;
}
