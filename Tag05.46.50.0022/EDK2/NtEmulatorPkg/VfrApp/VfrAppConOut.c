/** @file
 VFR application

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

#include "VfrAppConOut.h"

EFI_HANDLE              mConOutDevList[VFR_APP_MAX_CON_OUT_DEV_NUM] = {0};
UINTN                   mConOutDevCount       = 0;
EFI_GUID                *mConOutDevGuidList[] = {&gEfiGraphicsOutputProtocolGuid,
                                                 &gEfiSimpleTextOutProtocolGuid
                                                 };

STATIC
EFI_STATUS
AddConOutDevToList (
  IN EFI_HANDLE                               Handle
  )
{
  UINTN                                       Index;

  if (mConOutDevCount >= VFR_APP_MAX_CON_OUT_DEV_NUM) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < mConOutDevCount; Index++) {
    if (mConOutDevList[Index] == Handle) {
      return EFI_ALREADY_STARTED;
    }
  }

  mConOutDevList[mConOutDevCount] = Handle;
  mConOutDevCount++;
  return EFI_SUCCESS;
}

STATIC
CHAR16 *
GetDeviceStr (
  IN CONST EFI_HANDLE                         Handle
  )
{
  EFI_STATUS                                  Status;
  EFI_HANDLE                                  *HandleBuffer;
  UINTN                                       NumberOfHandles;
  UINTN                                       Index;
  EFI_COMPONENT_NAME2_PROTOCOL                *ComponentName2;
  CHAR16                                      *DeviceStr;


  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiComponentName2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiComponentName2ProtocolGuid, (VOID **)&ComponentName2);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = ComponentName2->GetControllerName (ComponentName2, Handle, NULL, "en", &DeviceStr);
    if (EFI_ERROR (Status)) {
      continue;
    }

    FreePool (HandleBuffer);
    return AllocateCopyPool (StrSize (DeviceStr), DeviceStr);
  }

  FreePool (HandleBuffer);
  return NULL;
}

STATIC
CHAR16 *
GetDevicePathStr (
  IN CONST EFI_HANDLE                         Handle
  )
{
  EFI_STATUS                                  Status;
  EFI_DEVICE_PATH_PROTOCOL                    *DevPath;
  EFI_DEVICE_PATH_TO_TEXT_PROTOCOL            *DevicePathToText;

  Status = gBS->HandleProtocol (Handle, &gEfiDevicePathProtocolGuid, (VOID **)&DevPath);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (VOID **)&DevicePathToText);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return DevicePathToText->ConvertDevicePathToText (DevPath, FALSE, FALSE);
}

STATIC
EFI_STATUS
InstallDEProtocol (
  IN EFI_HANDLE                               *Handle,
  IN EFI_GUID                                 *Protocol
  )
{
  EFI_STATUS                                  Status;
  VOID                                        *Instance;

  if (Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*Handle != NULL) {
    Status = gBS->HandleProtocol (*Handle, Protocol, &Instance);
    if (!EFI_ERROR (Status)) {
      return Status;
    }
  }

  return gBS->InstallProtocolInterface (Handle, Protocol, EFI_NATIVE_INTERFACE, NULL);
}

STATIC
EFI_STATUS
UninstallDEProtocol (
  IN EFI_HANDLE                               Handle,
  IN EFI_GUID                                 *Protocol
  )
{
  EFI_STATUS                                  Status;
  VOID                                        *Instance;

  Status = gBS->HandleProtocol (Handle, Protocol, &Instance);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  return gBS->UninstallProtocolInterface (Handle, Protocol, NULL);
}

VOID
EFIAPI
VfrAppConOutDevNotidyFunc (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  )
{
  EFI_STATUS                                  Status;
  EFI_HANDLE                                  *HandleBuffer;
  UINTN                                       NumberOfHandles;
  UINTN                                       Index;
  VOID                                        *Instance;
  EFI_GUID                                    *ConOutDevGuid;

  if (Context == NULL) {
    return;
  }

  ConOutDevGuid = (EFI_GUID *) Context;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  ConOutDevGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], ConOutDevGuid, &Instance);
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDevicePathProtocolGuid, &Instance);
    if (EFI_ERROR (Status)) {
      continue;
    }

    AddConOutDevToList (HandleBuffer[Index]);
  }
  FreePool (HandleBuffer);
}

EFI_STATUS
VfrAppConOutUpdateDESetting (
  IN VFR_APP_PRIVATE_DATA                     *Private
  )
{
  UINTN                                       Index;
  EFI_HII_HANDLE                              Handle;

  for (Index = 0; Index < mConOutDevCount; Index++) {
    Handle = mConOutDevList[Index];
    if (Handle == NULL) {
      continue;
    }

    switch (Private->Configuration.ConOutDESetting[Index]) {

    case VFR_APP_ACTIVE_DISPLAY_ENGINE_METRO:
      InstallDEProtocol (&Handle, &gH2ODisplayEngineLocalMetroGuid);
      UninstallDEProtocol (Handle, &gH2ODisplayEngineLocalTextGuid);
      break;

    case VFR_APP_ACTIVE_DISPLAY_ENGINE_TEXT:
      InstallDEProtocol (&Handle, &gH2ODisplayEngineLocalTextGuid);
      UninstallDEProtocol (Handle, &gH2ODisplayEngineLocalMetroGuid);
      break;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
VfrAppConOutUpdateDevLabel (
  IN VFR_APP_PRIVATE_DATA                     *Private
  )
{
  UINTN                                       Index;
  EFI_STRING_ID                               Token;
  EFI_STRING_ID                               TokenHelp;
  EFI_HII_HANDLE                              HiiHandle;
  CHAR16                                      *String;
  VOID                                        *StartOpCodeHandle;
  VOID                                        *EndOpCodeHandle;
  VOID                                        *OptionsOpCodeHandle;
  EFI_IFR_GUID_LABEL                          *StartLabel;
  EFI_IFR_GUID_LABEL                          *EndLabel;
  UINT8                                       Type;
  EFI_GUID                                    FormSetGuid = VFR_APP_FORMSET_GUID;


  StartOpCodeHandle   = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle     = HiiAllocateOpCodeHandle ();
  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL || EndOpCodeHandle == NULL || OptionsOpCodeHandle == NULL) {
    return EFI_ABORTED;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LABEL_UPDATE_CON_OUT_LIST_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = LABEL_UPDATE_CON_OUT_LIST_END;

  Type  = EFI_IFR_NUMERIC_SIZE_1;
  HiiCreateOneOfOptionOpCode (OptionsOpCodeHandle, STRING_TOKEN(STR_ACTIVE_DISPLAY_ENGINE_METRO), EFI_IFR_OPTION_DEFAULT, Type, VFR_APP_ACTIVE_DISPLAY_ENGINE_METRO);
  HiiCreateOneOfOptionOpCode (OptionsOpCodeHandle, STRING_TOKEN(STR_ACTIVE_DISPLAY_ENGINE_TEXT) , 0                     , Type, VFR_APP_ACTIVE_DISPLAY_ENGINE_TEXT );

  HiiHandle = Private->HiiHandle;
  for (Index = 0; Index < mConOutDevCount; Index++) {
    Token  = STRING_TOKEN(STR_UNKNOWN_DEV);
    String = GetDeviceStr (mConOutDevList[Index]);
    if (String != NULL) {
      Token = HiiSetString (HiiHandle, 0, String, NULL);
      FreePool (String);
    }

    TokenHelp = STRING_TOKEN(STR_UNKNOWN_DEV);
    String    = GetDevicePathStr (mConOutDevList[Index]);
    if (String != NULL) {
      TokenHelp = HiiSetString (HiiHandle, 0, String, NULL);
      FreePool (String);
    }

    HiiCreateOneOfOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID) (VFR_APP_KEY_CON_OUT_DEV_BASE + Index),
      VFR_APP_CONFIGURATION_VARSTORE_ID,
      (UINT16) ((UINTN) &Private->Configuration.ConOutDESetting[Index] - (UINTN) &Private->Configuration),
      Token,
      TokenHelp,
      0,
      Type,
      OptionsOpCodeHandle,
      NULL
      );
  }

  HiiUpdateForm (
    HiiHandle,
    &FormSetGuid,
    VFR_APP_FORM_ROOT,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  HiiFreeOpCodeHandle (OptionsOpCodeHandle);
  return EFI_SUCCESS;
}

EFI_STATUS
VfrAppConOutDevInit (
  VOID
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       Index;
  EFI_EVENT                                   Event;
  VOID                                        *Registration;

  for (Index = 0; Index < sizeof (mConOutDevGuidList) / sizeof (EFI_GUID *); Index++) {
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK - 1,  // Because DisplayType's level is TPL_CALLBACK, set lower level to override it.
                    VfrAppConOutDevNotidyFunc,
                    (VOID *) mConOutDevGuidList[Index],
                    &Event
                    );
    if (!EFI_ERROR (Status)) {
      gBS->RegisterProtocolNotify (
             mConOutDevGuidList[Index],
             Event,
             &Registration
             );
    }
  }

  return EFI_SUCCESS;
}
