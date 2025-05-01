/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Advance.h>
EFI_CALLBACK_INFO                         *mAdvCallBackInfo;


/**
 This is the callback function for the Advance Menu.

 @param [in]   This
 @param [in]   Action
 @param [in]   QuestionId
 @param [in]   Type
 @param [in]   Value
 @param [out]  ActionRequest
**/
EFI_STATUS
AdvanceCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                                 Status;
  CHIPSET_CONFIGURATION                      *MyIfrNVData;
  EFI_CALLBACK_INFO                          *CallbackInfo;
  UINTN                                      BufferSize;
  EFI_GUID                                   VarStoreGuid = SYSTEM_CONFIGURATION_GUID;

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return AdvanceCallbackRoutineByAction (This, Action, QuestionId, Type, Value, ActionRequest);
  }

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  CallbackInfo = EFI_CALLBACK_INFO_FROM_THIS (This);
  BufferSize = GetVarStoreSize (CallbackInfo->HiiHandle, &CallbackInfo->FormsetGuid, &VarStoreGuid, "SystemConfig");
  Status = SetupVariableConfig (
             &VarStoreGuid,
             L"SystemConfig",
             BufferSize,
             (UINT8 *) gSUBrowser->SCBuffer,
             TRUE
             );
  MyIfrNVData = (CHIPSET_CONFIGURATION *)gSUBrowser->SCBuffer;
  Status    = EFI_SUCCESS;

  switch (QuestionId) {

  default:
    Status = HotKeyCallBack (
               This,
               Action,
               QuestionId,
               Type,
               Value,
               ActionRequest
               );
    break;
  }

  SetupVariableConfig (
    &VarStoreGuid,
    L"SystemConfig",
    BufferSize,
    (UINT8 *) gSUBrowser->SCBuffer,
    FALSE
    );

  return Status;
}

EFI_STATUS
AdvanceCallbackRoutineByAction (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                            Status;
  EFI_CALLBACK_INFO                     *CallbackInfo;
  UINTN                                 BufferSize;
  EFI_GUID                              VarStoreGuid = SYSTEM_CONFIGURATION_GUID;

  if ((This == NULL) ||
      ((Value == NULL) &&
       (Action != EFI_BROWSER_ACTION_FORM_OPEN) &&
       (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
      (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  CallbackInfo   = EFI_CALLBACK_INFO_FROM_THIS (This);
  BufferSize     = GetVarStoreSize (CallbackInfo->HiiHandle, &CallbackInfo->FormsetGuid, &VarStoreGuid, "SystemConfig");
  Status         = EFI_UNSUPPORTED;

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    if (QuestionId == 0) {
      Status = SetupVariableConfig (
                 &VarStoreGuid,
                 L"SystemConfig",
                 BufferSize,
                 (UINT8 *) gSUBrowser->SCBuffer,
                 FALSE
                 );
    }
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    if (QuestionId == 0) {
      Status = SetupVariableConfig (
                 &VarStoreGuid,
                 L"SystemConfig",
                 BufferSize,
                 (UINT8 *) gSUBrowser->SCBuffer,
                 TRUE
                 );
    }
    break;

  case EFI_BROWSER_ACTION_CHANGING:
    Status = EFI_SUCCESS;
    break;

  case EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING:
    if (QuestionId == KEY_SCAN_F9) {
      Status = HotKeyCallBack (
                This,
                Action,
                QuestionId,
                Type,
                Value,
                ActionRequest
                );
      SetupVariableConfig (
        &VarStoreGuid,
        L"SystemConfig",
        BufferSize,
        (UINT8 *) gSUBrowser->SCBuffer,
        FALSE
        );
    }
    //
    // avoid GetQuestionDefault execute ExtractConfig
    //
    return EFI_SUCCESS;

  default:
    break;
  }

  return Status;
}

EFI_STATUS
InitAdvanceMenu (
  IN EFI_HII_HANDLE                         HiiHandle
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
InstallAdvanceCallbackRoutine (
  IN EFI_HANDLE                             DriverHandle,
  IN EFI_HII_HANDLE                         HiiHandle
  )
{
  EFI_STATUS                                Status;
  EFI_GUID                                  FormsetGuid = FORMSET_ID_GUID_ADVANCE;

  mAdvCallBackInfo = AllocatePool (sizeof(EFI_CALLBACK_INFO));
  if (mAdvCallBackInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mAdvCallBackInfo->Signature                    = EFI_CALLBACK_INFO_SIGNATURE;
  mAdvCallBackInfo->DriverCallback.ExtractConfig = gSUBrowser->ExtractConfig;
  mAdvCallBackInfo->DriverCallback.RouteConfig   = gSUBrowser->RouteConfig;
  mAdvCallBackInfo->DriverCallback.Callback      = AdvanceCallbackRoutine;
  mAdvCallBackInfo->HiiHandle                    = HiiHandle;
  CopyGuid (&mAdvCallBackInfo->FormsetGuid, &FormsetGuid);

  //
  // Install protocol interface
  //
  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAdvCallBackInfo->DriverCallback
                  );
  ASSERT_EFI_ERROR (Status);

  Status = InitAdvanceMenu (HiiHandle);

  return Status;
}

EFI_STATUS
UninstallAdvanceCallbackRoutine (
  IN EFI_HANDLE                             DriverHandle
  )
{
  EFI_STATUS     Status;

  if (mAdvCallBackInfo == NULL) {
    return EFI_SUCCESS;
  }
  Status = gBS->UninstallProtocolInterface (
                  DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mAdvCallBackInfo->DriverCallback
                  );
  ASSERT_EFI_ERROR (Status);
  gBS->FreePool (mAdvCallBackInfo);
  mAdvCallBackInfo = NULL;
  return Status;
}

