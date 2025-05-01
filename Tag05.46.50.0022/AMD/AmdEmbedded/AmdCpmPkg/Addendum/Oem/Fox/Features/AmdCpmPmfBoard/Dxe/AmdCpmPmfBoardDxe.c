/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>

EFI_GUID OEM_BOARD_PMF_SSDT_GUID = {
  0x3CBAFDA6, 0xCD23, 0x4A52, 0x82, 0xF3, 0x63, 0xBD, 0x2A, 0x41, 0x46, 0x85
};

VOID
EFIAPI
AmdCpmPmfBoardLateEntry (
  IN      EFI_EVENT           Event,
  IN      VOID                *Context
  );

/**
 *---------------------------------------------------------------------------------------
 *
 *  CpmDxeDriverEntryPoint
 *
 *  Description:
 *     Entry point for the cpu driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         PMODULE_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdCpmPmfDxeBoardEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);
  Status = gBS->CreateEventEx (
                              CPM_EVENT_NOTIFY_SIGNAL,
                              CPM_TPL_NOTIFY,
                              AmdCpmPmfBoardLateEntry,
                              NULL,
                              &gEfiEventReadyToBootGuid,
                              &ReadyToBootEvent
                              );

  return Status;
}

VOID
EFIAPI
AmdCpmPmfBoardLateEntry (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
/*++

Routine Description:

  Invoke AmdinitLate entry point. This function gets called
  each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled

Arguments & Return Values: Standard event handling function prototype

--*/
{
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;

  if (!InitlateInvoked) {
    Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                    NULL,
                                    (VOID**)&CpmTableProtocolPtr
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }
    //
    //  Install the sample SSDT for OEM PMF callback.
    //  OEM can put the related callback method in their platform ACPI directly.
    //
    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                      CpmTableProtocolPtr,
                                                      &OEM_BOARD_PMF_SSDT_GUID,
                                                      NULL,
                                                      NULL,
                                                      NULL
                                                      );
  }
  InitlateInvoked = TRUE;
  return;
}
