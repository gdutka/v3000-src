/** @file
  SecurityEventDxe

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>
#include <Guid/H2OSecurityEventHob.h>
#include <Guid/H2OStatusCodeDataTypeId.h>
#include <Protocol/H2ODialog.h>
#include <Protocol/HiiConfigAccess.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/OemGraphicsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/HiiDbLib.h>
#include <Library/HiiStringLib.h>
#include <Library/HiiConfigAccessLib.h>
#include <Library/VariableLib.h>
#include <Library/H2OCpLib.h>
#include <Library/H2OLib.h>
#include <Library/ResetSystemLib.h>

STATIC EFI_HII_HANDLE mStringPackHandle = NULL;

extern UINT8   SecurityEventDxeStrings[];

EFI_GUID mSecurityEventStringPackGuid = {
  0x77107459, 0x675d, 0x4922, {0xae, 0xcc, 0x0b, 0x50, 0x9a, 0x90, 0xc4, 0xfc}
};

/**
  Append Event String.

  @param[in]     EventClass         Event class.
  @param[in]     EventDesc          Event description.
  @param[in out] WholeEventStr      Whole event string.

  @retval    EFI_SUCCESS            Function completed successfully.
  @retval    EFI_INVALID_PARAMETER  EventDesc or WholeEventStr is NULL.
  @retval    EFI_OUT_OF_RESOURCES   Out of resource.
**/
STATIC
EFI_STATUS
AppendEventStr (
  IN     UINT32  EventClass,
  IN     CHAR8   *EventDesc,
  IN OUT CHAR16  **WholeEventStr
  )
{
  CHAR16         *EventClassStr;
  CHAR16         *Buffer;

  if (EventDesc == NULL || WholeEventStr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  EventClassStr = AllocatePool(AsciiStrSize(EventDesc) * sizeof (CHAR16));
  if (EventClassStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  AsciiStrToUnicodeStrS (EventDesc, EventClassStr, AsciiStrSize(EventDesc));

  Buffer = CatSPrint (NULL, L"%sEvent Type:0x%02x %s\n", (*WholeEventStr == NULL? L"" : *WholeEventStr), EventClass, EventClassStr);
  H2OFreePool ((VOID **) &EventClassStr);
  H2OFreePool ((VOID **) WholeEventStr);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *WholeEventStr = Buffer;

  return EFI_SUCCESS;
}

/**
  Display security event message dialog.

  @param[in] WholeEventStr         Whole event string.

  @retval    EFI_SUCCESS           Function completed successfully.
  @retval    EFI_OUT_OF_RESOURCES  Out of resource.
**/
EFI_STATUS
DisplaySecurityEventDialog (
  IN CHAR16  *WholeEventStr
  )
{
  EFI_STATUS                      Status;
  H2O_DIALOG_PROTOCOL             *H2ODialog;
  CHAR16                          *ChangeStr;
  CHAR16                          *ActionStr;
  CHAR16                          *String;
  EFI_INPUT_KEY                   Key;

  Status = gBS->LocateProtocol (
                  &gH2ODialogProtocolGuid,
                  NULL,
                  (VOID **) &H2ODialog
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ChangeStr = HiiGetString (mStringPackHandle, STRING_TOKEN (STR_SECURITY_EVENT), NULL);
  if (ChangeStr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ActionStr = HiiGetString (mStringPackHandle, STRING_TOKEN (STR_EXIT_DISPLAY), NULL);
  if (ActionStr == NULL) {
    H2OFreePool ((VOID **) &ChangeStr);
    return EFI_OUT_OF_RESOURCES;
  }

  String = CatSPrint (NULL, L"%s\n%s\n%s", ChangeStr, WholeEventStr, ActionStr);
  H2OFreePool ((VOID **) &ChangeStr);
  H2OFreePool ((VOID **) &ActionStr);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DisableQuietBoot ();
  H2ODialog->ConfirmDialog (
               DlgYesNo,
               FALSE,
               0,
               NULL,
               &Key,
               String
               );
  if (Key.UnicodeChar != CHAR_CARRIAGE_RETURN) {
    ResetShutdown ();
  }

  H2OFreePool ((VOID **) &String);

  return EFI_SUCCESS;
}

/**
  To display the security event if the action bitmask is set for display.

  @retval    EFI_SUCCESS           Function completed successfully.
  @retval    Others                An error occurred when displaying security event.
**/
EFI_STATUS
DisplaySecurityEvent (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_HOB_GUID_TYPE       *GuidHob;
  H2O_SECURITY_EVENT_HOB  *SecurityEventHob;
  CHAR16                  *WholeEventStr;
  CHAR8                   *DescPtr;

  Status = EFI_SUCCESS;
  WholeEventStr = NULL;
  GuidHob = GetFirstGuidHob (&gH2OSecurityEventHobGuid);
  while (GuidHob != NULL) {
    SecurityEventHob = (H2O_SECURITY_EVENT_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if ((SecurityEventHob->Event.Event.Action & H2O_SECURITY_EVENT_ACTION_DISPLAY) != 0) {
      DescPtr = (CHAR8 *) ((UINTN) SecurityEventHob + sizeof (EFI_HOB_GUID_TYPE) + SecurityEventHob->Event.Size + (UINTN) SecurityEventHob->Event.Event.EventDataSize);
      Status = AppendEventStr (SecurityEventHob->Event.Event.Class, DescPtr, &WholeEventStr);
      if (EFI_ERROR (Status)) {
        goto Exit;
      }
    }
    GuidHob = GetNextGuidHob (&gH2OSecurityEventHobGuid, GET_NEXT_HOB (GuidHob));
  }

  if (WholeEventStr != NULL) {
    Status = DisplaySecurityEventDialog(WholeEventStr);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }
  }

Exit:
  H2OFreePool ((VOID **) &WholeEventStr);

  return Status;
}
/**
  Display security event and erase variable data.

  @param[in]  Event   Pointer to this event
  @param[in]  Handle  The handle associated with a previously registered checkpoint handler.

**/
VOID
DisplayBeforeCallback (
  IN EFI_EVENT         Event,
  IN H2O_CP_HANDLE     Handle
  )
{
  if (FeaturePcdGet (PcdH2OSecurityEventDisplaySupported)) {
    DisplaySecurityEvent();
  }

  CommonSetVariable (
    H2O_SECURITY_EVENT_VARIABLE_NAME,
    &gH2OSecurityEventHobGuid,
    0,
    0,
    NULL
    );

  H2OCpUnregisterHandler (Handle);
}

/**

  Security Event Entry Point

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval  EFI_SUCEESS  SecurityEventDxe has finished initializing.

  @retval  Other        Return status from DisplaySecurityEvent or EraseSecurityEvent handler

**/
EFI_STATUS
EFIAPI
SecurityEventDxeEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  mStringPackHandle = HiiAddPackages (
                        &mSecurityEventStringPackGuid,
                        gImageHandle,
                        SecurityEventDxeStrings,
                        NULL
                        );
  ASSERT (mStringPackHandle != NULL);

  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    H2O_CP_HANDLE       CpHandle;

    Status = H2OCpRegisterHandler (
              &gH2OBdsCpDisplayBeforeGuid,
              DisplayBeforeCallback,
              H2O_CP_MEDIUM,
              &CpHandle
              );
    if (EFI_ERROR (Status)) {
      DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpDisplayBeforeGuid, Status));
      return Status;
    }
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpDisplayBeforeGuid, Status));
  }

  return Status;
}
