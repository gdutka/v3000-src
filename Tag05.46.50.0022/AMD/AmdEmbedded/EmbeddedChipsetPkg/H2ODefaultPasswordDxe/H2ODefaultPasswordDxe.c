/** @file
;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/
#include "H2ODefaultPasswordDxe.h"

EFI_HII_HANDLE      mHiiHandle;

STATIC
BOOLEAN
IsH2OSecureBootEnabled (
  VOID
  )
{
  UINT8      SecureBootEnabled;
  UINTN      DataSize;
  EFI_STATUS Status;

  DataSize = sizeof (SecureBootEnabled);
  Status = CommonGetVariable (
             EFI_SECURE_BOOT_MODE_NAME,
             &gEfiGlobalVariableGuid,
             &DataSize,
             &SecureBootEnabled
             );
  if (!EFI_ERROR (Status) && SecureBootEnabled == 1) {
    return TRUE;
  }
  return FALSE;

}

VOID
EFIAPI
EdkiiVariableLockNotifyCallback (
  IN     EFI_EVENT    Event,
  IN     VOID         *Context
  )
{
  EFI_STATUS                      Status;
  EDKII_VARIABLE_LOCK_PROTOCOL    *VariableLock;

  
  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &VariableLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[EdkiiVariableLockNotifyCallback] Fail to locate EdkiiVariableLockProtocol. (%r)\n", Status));
    goto Done;
  }

  Status = VariableLock->RequestToLock (VariableLock, H2O_DEFAULT_PASSWORD_VARIABLE_NAME, &gSystemConfigurationGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[EdkiiVariableLockNotifyCallback] Fail to lock %s variable. (%r)\n", H2O_DEFAULT_PASSWORD_VARIABLE_NAME, Status));
  }

Done:
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

VOID
LockDefaultPasswordVariable (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       Event;
  VOID                            *Registration;
  EDKII_VARIABLE_LOCK_PROTOCOL    *VariableLock;
  UINTN                           VarSize;
  UINT8                           PswExist = 0;
  
  VarSize = sizeof (UINT8);
  Status = gRT->GetVariable (
    H2O_DEFAULT_PASSWORD_VARIABLE_NAME,
    &gSystemConfigurationGuid,
    NULL,
    &VarSize,
    &PswExist
    );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &VariableLock);
  if (!EFI_ERROR (Status)) {
    EdkiiVariableLockNotifyCallback (NULL, NULL);
  } else {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    EdkiiVariableLockNotifyCallback,
                    NULL,
                    &Event
                    );
    if (Status == EFI_SUCCESS) {
      Status = gBS->RegisterProtocolNotify (
                      &gEdkiiVariableLockProtocolGuid,
                      Event,
                      &Registration
                      );
    }
    return;
  }
}

VOID
H2ODefaultPasswordPopUpMessage (
  IN     EFI_EVENT    Event,
  IN     VOID         *Context
  )
{
  CHAR16                               *String;
  EFI_INPUT_KEY                        Key;
  EFI_STATUS                           Status;
  H2O_DIALOG_PROTOCOL                  *H2ODialog;
  
  String = HiiGetString (mHiiHandle, STRING_TOKEN(STR_DEFAULT_PASSWORD_WARNING), NULL);
  if (String == NULL) {
    goto Done;
  }
  
  Status = gBS->LocateProtocol (
                  &gH2ODialogProtocolGuid,
                  NULL,
                  (VOID **) &H2ODialog
                  );
  if (EFI_ERROR (Status)) {
    goto Done;
  }
  
  gST->ConOut->ClearScreen (gST->ConOut);

  H2ODialog->ConfirmDialog (
    DlgOk,
    FALSE,
    0,
    NULL,
    &Key,
    String
    );
  
  H2OFreePool ((VOID**)&String);
  
Done:
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

VOID
CreateH2ODefaultPasswordPopUpMessage (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       Event;
  VOID                            *Registration;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  H2ODefaultPasswordPopUpMessage,
                  NULL,
                  &Event
                  );
  if (Status == EFI_SUCCESS) {
    Status = gBS->RegisterProtocolNotify (
                    &gEndOfBdsBootSelectionProtocolGuid,
                    Event,
                    &Registration
                    );
  }
  return;
}

VOID
EFIAPI
H2ODefaultPasswordNotifyCallback (
  IN     EFI_EVENT    Event,
  IN     VOID         *Context
  )
{
  EFI_STATUS                              Status;
  UINTN                                   VarSize;
  UINT8                                   PswExist = 0;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL       *SysPasswordService;
  UINTN                                   PswSize;
  CHAR16                                  DefaultSupervisorPw[FixedPcdGet32(PcdDefaultSysPasswordMaxLength) + 1];

  if (FixedPcdGetPtr (PcdH2ODefaultSupervisorPwString) == NULL) return;

  ZeroMem ((VOID *)DefaultSupervisorPw, sizeof (DefaultSupervisorPw));
  if (FixedPcdGetPtrSize(PcdH2ODefaultSupervisorPwString) - sizeof(CHAR16) > FixedPcdGet32(PcdDefaultSysPasswordMaxLength) * sizeof (CHAR16)) {
    CopyMem ((VOID *)DefaultSupervisorPw, (VOID *)FixedPcdGetPtr (PcdH2ODefaultSupervisorPwString), FixedPcdGet32(PcdDefaultSysPasswordMaxLength) * sizeof(CHAR16));
  } else {
    CopyMem ((VOID *)DefaultSupervisorPw, (VOID *)FixedPcdGetPtr (PcdH2ODefaultSupervisorPwString), FixedPcdGetPtrSize(PcdH2ODefaultSupervisorPwString));
  }

#if (FixedPcdGetBool (PcdH2ODefaultSupervisorPwSupported) == 0) && (FixedPcdGetBool (PcdH2ODefaultSecureBootPwSupported) == 1)
  if (!IsH2OSecureBootEnabled()) {
    goto Done;
  }
#endif

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  Status = SysPasswordService->GetStatus(SysPasswordService, SystemSupervisor);
  if (Status == EFI_SUCCESS) {
    goto Done;
  }

  VarSize = sizeof (UINT8);
  Status = gRT->GetVariable (
    H2O_DEFAULT_PASSWORD_VARIABLE_NAME,
    &gSystemConfigurationGuid,
    NULL,
    &VarSize,
    &PswExist
    );
  if (EFI_ERROR(Status)) {
    //
    // Default Password do not exist, set default password
    //
    if (SysPasswordService != NULL) {
      PswSize = sizeof (DefaultSupervisorPw);
      if (PcdGetBool (PcdSecureSysPasswordSupported)) {
        SysPasswordService->UnlockPassword (SysPasswordService, L"", 0);
        Status = SysPasswordService->SetPassword (SysPasswordService, DefaultSupervisorPw, PswSize, SystemSupervisor);
        SysPasswordService->LockPassword (SysPasswordService);
      } else {
        Status = SysPasswordService->SetPassword (SysPasswordService, DefaultSupervisorPw, PswSize, SystemSupervisor);
      }
      ASSERT_EFI_ERROR (Status);

      PswExist = 0x01;
      Status = gRT->SetVariable (
        H2O_DEFAULT_PASSWORD_VARIABLE_NAME,
        &gSystemConfigurationGuid,
        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
        VarSize,
        &PswExist
        );
      ASSERT_EFI_ERROR (Status);

      LockDefaultPasswordVariable();
      CreateH2ODefaultPasswordPopUpMessage();
    }
  }

Done:
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
}

/**
 Entrypoint of this module.

 @param[in]         ImageHandle         The firmware allocated handle for the EFI image.
 @param[in]         SystemTable         A pointer to the EFI System Table.

 @retval EFI_SUCCESS                    Module is loaded successfully.
 @retval EFI_UNSUPPORTED                Cannot locate H2O_IPMI_INTERFACE_PROTOCOL.
*/
EFI_STATUS
H2ODefaultPasswordDxeEntry (
  IN  EFI_HANDLE                        ImageHandle,
  IN  EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               Event;
  VOID                                    *Registration;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL       *SysPasswordService;
  
  mHiiHandle = HiiAddPackages (
    &gEfiCallerIdGuid,
    ImageHandle,
    H2ODefaultPasswordDxeStrings,
    NULL
    );
  if (mHiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  LockDefaultPasswordVariable();

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  if (!EFI_ERROR (Status)) {
    H2ODefaultPasswordNotifyCallback (NULL, NULL);
  } else {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    H2ODefaultPasswordNotifyCallback,
                    NULL,
                    &Event
                    );
    if (Status == EFI_SUCCESS) {
      Status = gBS->RegisterProtocolNotify (
                      &gEfiSysPasswordServiceProtocolGuid,
                      Event,
                      &Registration
                      );
    }
  } 
  return EFI_SUCCESS;
}

