/** @file
  

;*******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/VarApcbSyncProtocol.h>
#include <Guid/EventGroup.h>

EFI_GUID *mApcbSyncDoneProtocolGuid[] = {
           &gH2OCbsApcbSyncDoneProtocolGuid,
           &gH2OAodApcbSyncDoneProtocolGuid
           };

VOID
EFIAPI
EndOfDxeApcbSync (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  EFI_STATUS       Status;
  UINTN            Index;
  VOID             *DummyProtocol;

  for (Index = 0; Index < (sizeof(mApcbSyncDoneProtocolGuid) / sizeof(EFI_GUID*)); Index++) {
    Status = gBS->LocateProtocol (
                    mApcbSyncDoneProtocolGuid[Index],
                    NULL,
                    (VOID **)&DummyProtocol
                    );
    if (!EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Reset system after APCB Sync\n"));
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  }

  return;
}

VOID
EFIAPI
ApcbSyncDoneCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS          Status;
  UINTN               Index;
  VOID                *DummyProtocol;

  for (Index = 0; Index < (sizeof(mApcbSyncDoneProtocolGuid) / sizeof(EFI_GUID*)); Index++) {
    Status = gBS->LocateProtocol (
                    mApcbSyncDoneProtocolGuid[Index],
                    NULL,
                    (VOID **)&DummyProtocol
                    );
    if (EFI_ERROR (Status)) {
      gBS->CloseEvent (Event);
      return;
    }
  }

  DEBUG ((EFI_D_ERROR, "Reset system after all APCB Sync done\n"));
  gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  return;
}

EFI_STATUS
VarApcbSyncEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             Event;
  VOID                  *Registration;
  UINTN                 Index;
  VOID                  *DummyProtocol;
  BOOLEAN               AllProtocolIsFound;
  EFI_EVENT             EndOfDxeEvent;

  AllProtocolIsFound = TRUE;

  for (Index = 0; Index < (sizeof(mApcbSyncDoneProtocolGuid) / sizeof(EFI_GUID*)); Index++) {
    Status = gBS->LocateProtocol (
                    mApcbSyncDoneProtocolGuid[Index],
                    NULL,
                    (VOID **)&DummyProtocol
                    );
    if (EFI_ERROR (Status)) {
      AllProtocolIsFound = FALSE;
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      ApcbSyncDoneCallback,
                      NULL,
                      &Event
                      );
      ASSERT_EFI_ERROR (Status);
      //
      // Register for protocol notifications on this event
      //
      Status = gBS->RegisterProtocolNotify (
                      mApcbSyncDoneProtocolGuid[Index],
                      Event,
                      &Registration
                      );
      ASSERT_EFI_ERROR (Status);
    }
 }

  if (AllProtocolIsFound) {
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EndOfDxeApcbSync,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Create a EndOfDxeApcbSync function in End of DXE Failure!\n"));
  } 

  return Status;
}
