/** @file

  This module provides password relative functions.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "InternalUtilityLib.h"
#include "String.h"
#include <KernelSetupConfig.h>

#include <Protocol/SysPasswordService.h>
#include <Protocol/H2ODialog.h>

/**
  This function uses to check password.

  @retval TRUE     Insyde password is in locked state.
  @retval FALSE    Insyde password is in unloced state.
**/
BOOLEAN
EFIAPI
IsPasswordExist (
  VOID
  )
{
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL         *SysPasswordService;
  EFI_STATUS                                Status;

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = SysPasswordService->GetStatus (SysPasswordService, SystemSupervisor);

  return (BOOLEAN) (Status == EFI_SUCCESS ? TRUE : FALSE);
}

/**
 Check if need to do system password check.

 @retval TRUE     Need to do system password check.
 @retval FALSE    No need to do system password check.
**/
BOOLEAN
EFIAPI
NeedSysPasswordCheck (
  VOID
  )
{
  EFI_STATUS                              Status;
  SYS_PASSWORD_SETUP_INFO_DATA            SetupInfoBuffer;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL       *SysPasswordService;

  Status = gBS->LocateProtocol (&gEfiSysPasswordServiceProtocolGuid, NULL, (VOID **) &SysPasswordService);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = SysPasswordService->GetSysPswdSetupInfoData (SysPasswordService, &SetupInfoBuffer);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return SetupInfoBuffer.HaveSysPasswordCheck;
}

/**
 Clear flag to indicate that no need to do system password check.

 @retval EFI_SUCCESS          Success to disable system password check.
 @retval Other                Failed to find system password protocol instance or get/set setup info data.
**/
EFI_STATUS
EFIAPI
DisableSysPasswordCheck (
  VOID
  )
{
  EFI_STATUS                              Status;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL       *SysPasswordService;
  SYS_PASSWORD_SETUP_INFO_DATA            SetupInfoBuffer;

  Status = gBS->LocateProtocol (&gEfiSysPasswordServiceProtocolGuid, NULL, (VOID **) &SysPasswordService);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SysPasswordService->GetSysPswdSetupInfoData (SysPasswordService, &SetupInfoBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SetupInfoBuffer.HaveSysPasswordCheck = FALSE;
  Status = SysPasswordService->SetSysPswdSetupInfoData(
                                 SysPasswordService,
                                 &SetupInfoBuffer,
                                 (UINT32) SYS_PASSWORD_CHECK_BIT
                                 );
  return Status;
}

/**
  Set current password input is supervisor password or user password.

  @param  SysPasswordService     system password service protocol
  @param  CurrentUserType        systemSupervisorType or systemUserType

  @retval EFI_SUCCESS            set current user successful.

**/
EFI_STATUS
SetSysPasswordCurrentUser (
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL       *SysPasswordService, OPTIONAL
  CURRENT_USER_TYPE       CurrentUserType
  )
{
  EFI_STATUS                              Status;
  SYS_PASSWORD_SETUP_INFO_DATA            SetupInfoBuffer;
  UINT32                                  SetupBits;

  SetupBits = 0;

  if (SysPasswordService == NULL) {
    Status = gBS->LocateProtocol (
                    &gEfiSysPasswordServiceProtocolGuid,
                    NULL,
                    (VOID **) &SysPasswordService
                    );
    if (EFI_ERROR (Status)) {
      return FALSE;
    }
  }

  Status = SysPasswordService->GetSysPswdSetupInfoData(
                                 SysPasswordService,
                                 &SetupInfoBuffer
                                 );

  SetupInfoBuffer.CurrentUser = CurrentUserType;

  SetupBits = CURRENT_USER_BIT;

  Status = SysPasswordService->SetSysPswdSetupInfoData(
                                 SysPasswordService,
                                 &SetupInfoBuffer,
                                 SetupBits
                                 );

  return Status;
}

/**
  Change password state from unlocked state to locked state.

  @retval EFI_SUCCESS          Change password state to locked state.
  @retval EFI_UNSUPPORTED      The password doesn't saved in variable storage.
  @retval EFI_ALREADY_STARTED  The password state is already in locked state.
**/
EFI_STATUS
EFIAPI
LockPasswordState (
  VOID
  )
{
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL         *SysPasswordService;
  EFI_STATUS                                Status;

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SysPasswordService->LockPassword (SysPasswordService);

  return Status;

}

/**
  Change the Insyde password state to unlocked state.

  @param[in] Password          The null-terminated string that is the input password.
  @param[in] PasswordLength    The password length in bytes of PasswordPtr.

  @retval EFI_SUCCESS          Change password state to unlocked state.
  @retval EFI_UNSUPPORTED      The password doesn't saved in variable storage.
  @retval EFI_ALREADY_STARTED  The password state is already in unlocked state.
**/
EFI_STATUS
EFIAPI
UnlockPasswordState (
  IN  CHAR16                               *Password,
  IN  UINTN                                PasswordLength
  )
{
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL         *SysPasswordService;
  EFI_STATUS                                Status;

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = SysPasswordService->UnlockPassword (SysPasswordService, Password, PasswordLength);

  return Status;
}

/**
  Get the length of system password support.

  @param[in] SysPasswordService system password service protocol

  @retval                      The length of system password support
**/
UINTN
SysPasswordMaxNum (
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL    *SysPasswordService  OPTIONAL
  )
{
  EFI_STATUS                              Status;
  SYS_PASSWORD_SETUP_INFO_DATA            SetupInfoBuffer;


  if (SysPasswordService == NULL) {
    Status = gBS->LocateProtocol (
                    &gEfiSysPasswordServiceProtocolGuid,
                    NULL,
                    (VOID **) &SysPasswordService
                    );
    if (EFI_ERROR (Status)) {
      return FALSE;
    }
  }


  Status = SysPasswordService->GetSysPswdSetupInfoData(
                                 SysPasswordService,
                                 &SetupInfoBuffer
                                 );

  if (EFI_ERROR(Status)) {
    return 0;
  }

  return SetupInfoBuffer.MaxSysPasswordLength;
}

/**
  Check the input password is valid or invalid.

  If input password is valid, it will unlock password state and return EFI_SUCCESS.
  If input password is invalid and exceed retry times, it will show error message and do a dead loop.
  If system doesn't have password, return EFI_SUCCESS directly.

  @retval EFI_SUCCESS          Check input password is valid and change password state to unloced.
  @retval EFI_NOT_FOUND        gH2ODialogProtocolGuid protocol doesn't exist
  @retval EFI_ALREADY_STARTED  Input password is invalid.
**/
EFI_STATUS
EFIAPI
CheckPassword (
  VOID
  )
{
  H2O_DIALOG_PROTOCOL                    *H2ODialog;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL      *SysPasswordService;
  EFI_STATUS                             Status;
  CHAR16                                 *CheckPasswordStr;
  CHAR16                                 *InvalidPasswordStr;
  CHAR16                                 *Password;
  CHAR16                                 *ErrorStatusMsg;
  CHAR16                                 *ThreeIncorrectMsg;
  CHAR16                                 *RebootMsg;
  EFI_INPUT_KEY                          Key;
  UINTN                                  TryPasswordTimes;
  UINTN                                  SysPasswordMaximumNumber;

  if (!IsPasswordExist ()) {
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **) &SysPasswordService
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gH2ODialogProtocolGuid, NULL, (VOID **) &H2ODialog);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CheckPasswordStr = GenericUtilityLibGetStringById (STRING_TOKEN (STR_CHECK_PASSWORD_STRING));
  InvalidPasswordStr = GenericUtilityLibGetStringById (STRING_TOKEN (STR_INVALID_PASSWORD));
  gST->ConOut->ClearScreen (gST->ConOut);
  TryPasswordTimes = 0;

  SysPasswordMaximumNumber = SysPasswordMaxNum (SysPasswordService);

  Password = AllocatePool ((SysPasswordMaximumNumber + 1) * sizeof (CHAR16));
  if (Password == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  do {
    ZeroMem (Password, (SysPasswordMaximumNumber + 1) * sizeof (CHAR16));
    H2ODialog->PasswordDialog (
                 0,
                 FALSE,
                 (UINT32) (SysPasswordMaximumNumber + 1),
                 Password,
                 &Key,
                 CheckPasswordStr
                 );
    Status = SysPasswordService->CheckPassword (
                                   SysPasswordService,
                                   Password,
                                   (SysPasswordMaximumNumber + 1) * sizeof(CHAR16),
                                   SystemSupervisor
                                   );
    if (Status == EFI_SUCCESS) {
      SysPasswordService->UnlockPassword (
                            SysPasswordService,
                            Password,
                            (SysPasswordMaximumNumber + 1) * sizeof(CHAR16)
                            );
      SetSysPasswordCurrentUser (
          SysPasswordService,
          SystemSupervisorType
          );
    }
    TryPasswordTimes++;
    if (Status != EFI_SUCCESS && TryPasswordTimes < 3) {
      H2ODialog->ConfirmDialog (
                   2,
                   FALSE,
                   40,
                   NULL,
                   &Key,
                   InvalidPasswordStr
                   );
    }
  } while (Status != EFI_SUCCESS && TryPasswordTimes < 3);

  //
  // Display check password failed and do a dead loop in this function, if user inputs password is invalid.
  //
  if (Status != EFI_SUCCESS) {
    ErrorStatusMsg    = GenericUtilityLibGetStringById (STRING_TOKEN (STR_ERROR_STATUS));
    ThreeIncorrectMsg = GenericUtilityLibGetStringById (STRING_TOKEN (STR_THREE_ERRORS));
    RebootMsg         = GenericUtilityLibGetStringById (STRING_TOKEN (STR_RESTART_MSG));
    H2ODialog->CreateMsgPopUp (
                 40,
                 3,
                 ErrorStatusMsg,
                 ThreeIncorrectMsg,
                 RebootMsg
                 );

    FreePool (CheckPasswordStr);
    FreePool (Password);
    CpuDeadLoop ();
  }
  FreePool (InvalidPasswordStr);
  FreePool (CheckPasswordStr);
  FreePool (Password);
  return Status;
}

