/** @file
 Implementation for HII Popup Protocol.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/
#include "InternalH2OFormBrowser.h"

/**
 Displays a popup window.

 @param[in]  This           A pointer to the EFI_HII_POPUP_PROTOCOL instance.
 @param[in]  PopupStyle     Popup style to use.
 @param[in]  PopupType      Type of the popup to display.
 @param[in]  HiiHandle      HII handle of the string pack containing Message
 @param[in]  Message        A message to display in the popup box.
 @param[out] UserSelection  User selection.

 @retval EFI_SUCCESS            The popup box was successfully displayed.
 @retval EFI_INVALID_PARAMETER  HiiHandle and Message do not define a valid HII string.
 @retval EFI_INVALID_PARAMETER  PopupType is not one of the values defined by this specification.
 @retval EFI_OUT_OF_RESOURCES   There are not enough resources available to display the popup box.
**/
EFI_STATUS
EFIAPI
CreatePopup (
  IN  EFI_HII_POPUP_PROTOCOL          *This,
  IN  EFI_HII_POPUP_STYLE             PopupStyle,
  IN  EFI_HII_POPUP_TYPE              PopupType,
  IN  EFI_HII_HANDLE                  HiiHandle,
  IN  EFI_STRING_ID                   Message,
  OUT EFI_HII_POPUP_SELECTION         *UserSelection OPTIONAL
  )
{
  EFI_STATUS                          Status;
  CHAR16                              *Str;
  EFI_STRING                          TitleString;
  EFI_STRING                          ButtonStringArray[4];
  EFI_HII_VALUE                       ButtonHiiValueArray[4];
  H2O_FORM_BROWSER_D                  Dialog;
  UINT32                              ButtonCount;
  BOOLEAN                             ConsoleInitComplete;
  H2O_FORM_BROWSER_PRIVATE_DATA       *Private;
  UINTN                               Index;

  if ((PopupType < EfiHiiPopupTypeOk) || (PopupType > EfiHiiPopupTypeYesNoCancel)) {
    return EFI_INVALID_PARAMETER;
  }

  if((HiiHandle == NULL) || (Message == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Str = HiiGetString (HiiHandle, Message, NULL);
  if(Str == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  ConsoleInitComplete = FALSE;
  Private = NULL;
  FBIsConsoleInit (&ConsoleInitComplete, &Private);
  if (!ConsoleInitComplete) {
    Status = FBInitConsoles (Private);
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  switch (PopupStyle) {

  case EfiHiiPopupStyleWarning:
    TitleString = GetString (STRING_TOKEN (HII_POP_UP_TITLE_WARNING), mHiiHandle);
    break;

  case EfiHiiPopupStyleError:
    TitleString = GetString (STRING_TOKEN (HII_POP_UP_TITLE_ERROR), mHiiHandle);
    break;

  case EfiHiiPopupStyleInfo:
  default:
    TitleString = GetString (STRING_TOKEN (HII_POP_UP_TITLE_INFO), mHiiHandle);
    break;
  }

  switch (PopupType) {

  case EfiHiiPopupTypeOkCancel:
    ButtonCount          = 2;
    ButtonStringArray[0] = GetString (STRING_TOKEN (SCU_STR_OK_TEXT), mHiiHandle);
    ButtonStringArray[1] = GetString (STRING_TOKEN (SCU_STR_CANCEL_TEXT), mHiiHandle);
    CreateValueAsUint64 (ButtonHiiValueArray    , ButtonActionYes);
    CreateValueAsUint64 (ButtonHiiValueArray + 1, ButtonActionCancel);
    break;

  case EfiHiiPopupTypeYesNo:
    ButtonCount          = 2;
    ButtonStringArray[0] = GetString (STRING_TOKEN (SCU_STR_YES_TEXT), mHiiHandle);
    ButtonStringArray[1] = GetString (STRING_TOKEN (SCU_STR_NO_TEXT), mHiiHandle);
    CreateValueAsUint64 (ButtonHiiValueArray    , ButtonActionYes);
    CreateValueAsUint64 (ButtonHiiValueArray + 1, ButtonActionNo);
    break;

  case EfiHiiPopupTypeYesNoCancel:
    ButtonCount          = 3;
    ButtonStringArray[0] = GetString (STRING_TOKEN (SCU_STR_YES_TEXT), mHiiHandle);
    ButtonStringArray[1] = GetString (STRING_TOKEN (SCU_STR_NO_TEXT), mHiiHandle);
    ButtonStringArray[2] = GetString (STRING_TOKEN (SCU_STR_CANCEL_TEXT), mHiiHandle);
    CreateValueAsUint64 (ButtonHiiValueArray    , ButtonActionYes);
    CreateValueAsUint64 (ButtonHiiValueArray + 1, ButtonActionNo);
    CreateValueAsUint64 (ButtonHiiValueArray + 2, ButtonActionCancel);
    break;

  case EfiHiiPopupTypeOk:
  default:
    ButtonCount          = 1;
    ButtonStringArray[0] = GetString (STRING_TOKEN (SCU_STR_OK_TEXT), mHiiHandle);
    CreateValueAsUint64 (ButtonHiiValueArray, ButtonActionYes);
    break;
  }

  ZeroMem (&Dialog, sizeof (H2O_FORM_BROWSER_D));
  CreateValueAsUint64 (&Dialog.ConfirmHiiValue, ButtonActionYes);

  Status = CreateNewDialog (
             H2O_FORM_BROWSER_D_TYPE_CONFIRM,
             0,
             TitleString,
             1,
             0,
             ButtonCount,
             &Str,
             NULL,
             ButtonStringArray,
             NULL,
             ButtonHiiValueArray,
             &Dialog
             );
  if (UserSelection != NULL) {
    switch (PopupType) {

    case EfiHiiPopupTypeOkCancel:
      if (!EFI_ERROR (Status) && Dialog.ConfirmHiiValue.Value.u8 == ButtonActionYes) {
        *UserSelection = EfiHiiPopupSelectionOk;
      } else {
        *UserSelection = EfiHiiPopupSelectionCancel;
      }
      break;

    case EfiHiiPopupTypeYesNo:
      if (!EFI_ERROR (Status) && Dialog.ConfirmHiiValue.Value.u8 == ButtonActionYes) {
        *UserSelection = EfiHiiPopupSelectionYes;
      } else {
        *UserSelection = EfiHiiPopupSelectionNo;
      }
      break;

    case EfiHiiPopupTypeYesNoCancel:
      if (!EFI_ERROR (Status) && Dialog.ConfirmHiiValue.Value.u8 == ButtonActionYes) {
        *UserSelection = EfiHiiPopupSelectionYes;
      } else if (!EFI_ERROR (Status) && Dialog.ConfirmHiiValue.Value.u8 == ButtonActionNo) {
        *UserSelection = EfiHiiPopupSelectionNo;
      } else {
        *UserSelection = EfiHiiPopupSelectionCancel;
      }
      break;

    case EfiHiiPopupTypeOk:
    default:
      *UserSelection = EfiHiiPopupSelectionOk;
      break;
    }
  }

  if (!ConsoleInitComplete) {
    FBDetachConsoles (Private);
  }
  for (Index = 0; Index < ButtonCount; Index++) {
    FreePool (ButtonStringArray[Index]);
  }
  FreePool (Str);
  return EFI_SUCCESS;
}

/**
 Install the HII popup protocol.

 @param[in] ImageHandle          ImageHandle of the loaded driver.

 @retval  EFI_SUCCESS            Install the HII popup protocol successfully.
 @retval  EFI_OUT_OF_RESOURCES   Allocate memory fail.
 @retval  Other                  Install protocol interface fail
**/
EFI_STATUS
InstallHiiPopupProtocol (
  IN EFI_HANDLE                  ImageHandle
  )
{
  EFI_STATUS                     Status;
  EFI_HII_POPUP_PROTOCOL         *HiiPopup;

  HiiPopup = AllocateZeroPool (sizeof (EFI_HII_POPUP_PROTOCOL));
  if (HiiPopup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  HiiPopup->Revision    = EFI_HII_POPUP_PROTOCOL_REVISION;
  HiiPopup->CreatePopup = CreatePopup;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiHiiPopupProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  HiiPopup
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

