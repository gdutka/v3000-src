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

#include "VfrApp.h"
#include "VfrAppMisc.h"
#include "VfrAppConOut.h"
#include "VfrAppLegacyBios.h"
#include <Guid/H2ODisplayEngineType.h>
#include <Protocol/Bds.h>
#include <Protocol/H2ODisplayEngine.h>
#include <Protocol/H2ODialog.h>
#include <Protocol/H2OFormBrowser.h>
#include <Protocol/HiiPopup.h>

VFR_APP_PRIVATE_DATA            *mVfrAppPrivate       = NULL;
EFI_GUID                        mVfrAppFormSetGuid    = VFR_APP_FORMSET_GUID;
CHAR16                          mVariableName[]       = L"VfrAppVar";
CHAR16                          mMyEfiVar[]           = L"VfrAppEfiVar";
EFI_HANDLE                      *mH2ODisplayEngineHandleList = NULL;
H2O_DISPLAY_ENGINE_PROTOCOL     **mH2ODisplayEngineList      = NULL;
UINTN                           mH2ODisplayEngineCount       = 0;
EFI_BDS_ENTRY                   mOriginalBdsEntry            = NULL;
HII_VENDOR_DEVICE_PATH          mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    VFR_APP_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
  };
EFI_EVENT                       mEvent;

STATIC
EFI_STATUS
SendGotoHotKeyNotify (
  IN UINT16                               FormId,
  IN UINT16                               QuestionId
  )
{
  EFI_STATUS                              Status;
  H2O_FORM_BROWSER_PROTOCOL               *FBProtocol;
  H2O_DISPLAY_ENGINE_EVT_HOT_KEY          HotKeyNotify;

  Status = gBS->LocateProtocol (&gH2OFormBrowserProtocolGuid, NULL, (VOID **) &FBProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (&HotKeyNotify, sizeof (HotKeyNotify));
  HotKeyNotify.Hdr.Size   = sizeof (H2O_DISPLAY_ENGINE_EVT_HOT_KEY);
  HotKeyNotify.Hdr.Type   = H2O_DISPLAY_ENGINE_EVT_TYPE_HOT_KEY;
  HotKeyNotify.Hdr.Target = H2O_DISPLAY_ENGINE_EVT_TARGET_FORM_BROWSER;

  HotKeyNotify.HotKeyAction           = HotKeyGoTo;
  HotKeyNotify.HotKeyTargetFormId     = FormId;
  HotKeyNotify.HotKeyTargetQuestionId = QuestionId;

  return FBProtocol->Notify (FBProtocol, &HotKeyNotify.Hdr);
}
/**
  Notification function for keystrokes.

  @param[in] KeyData    The key that was pressed.

  @retval EFI_SUCCESS   The operation was successful.
**/
EFI_STATUS
EFIAPI
NotificationFunction(
  IN EFI_KEY_DATA *KeyData
  )
{
  gBS->SignalEvent (mEvent);

  return EFI_SUCCESS;
}

/**
  Function to start monitoring for CTRL-C using SimpleTextInputEx.

  @retval EFI_SUCCESS           The feature is enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough mnemory available.
**/
EFI_STATUS
EFIAPI
InternalStartMonitor(
  VOID
  )
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_KEY_DATA                      KeyData;
  EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  EFI_HANDLE                        NotifyHandle;

  Status = gBS->LocateHandleBuffer (
              ByProtocol,
              &gEfiSimpleTextInputExProtocolGuid,
              NULL,
              &HandleCount,
              &Handles
              );
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    ASSERT_EFI_ERROR (Status);

    KeyData.KeyState.KeyToggleState = 0;
    KeyData.Key.ScanCode            = 0;
    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_LEFT_CONTROL_PRESSED;
    KeyData.Key.UnicodeChar         = L'c';

    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_RIGHT_CONTROL_PRESSED;
    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  return EFI_SUCCESS;
}

/**
  Function to stop monitoring for CTRL-C using SimpleTextInputEx.

  @retval EFI_SUCCESS           The feature is enabled.
  @retval EFI_OUT_OF_RESOURCES  There is not enough mnemory available.
**/
EFI_STATUS
EFIAPI
InternalStopMonitor(
  VOID
  )
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleEx;
  EFI_STATUS                        Status;
  EFI_HANDLE                        *Handles;
  EFI_KEY_DATA                      KeyData;
  UINTN                             HandleCount;
  UINTN                             HandleIndex;
  EFI_HANDLE                        NotifyHandle;

  Status = gBS->LocateHandleBuffer (
                ByProtocol,
                &gEfiSimpleTextInputExProtocolGuid,
                NULL,
                &HandleCount,
                &Handles
                );
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (Handles[HandleIndex], &gEfiSimpleTextInputExProtocolGuid, (VOID **) &SimpleEx);
    ASSERT_EFI_ERROR (Status);

    KeyData.KeyState.KeyToggleState = 0;
    KeyData.Key.ScanCode            = 0;
    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_LEFT_CONTROL_PRESSED;
    KeyData.Key.UnicodeChar         = L'c';

    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (!EFI_ERROR (Status)) {
      Status = SimpleEx->UnregisterKeyNotify (SimpleEx, NotifyHandle);
    }

    KeyData.KeyState.KeyShiftState  = EFI_SHIFT_STATE_VALID|EFI_RIGHT_CONTROL_PRESSED;
    Status = SimpleEx->RegisterKeyNotify(
      SimpleEx,
      &KeyData,
      NotificationFunction,
      &NotifyHandle);
    if (!EFI_ERROR (Status)) {
      Status = SimpleEx->UnregisterKeyNotify (SimpleEx, NotifyHandle);
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Request,
  OUT       EFI_STRING                        *Progress,
  OUT       EFI_STRING                        *Results
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       BufferSize;
  VFR_APP_PRIVATE_DATA                        *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL             *HiiConfigRouting;
  EFI_STRING                                  ConfigRequest;
  EFI_STRING                                  ConfigRequestHdr;
  UINTN                                       Size;
  EFI_STRING                                  Value;
  UINTN                                       ValueStrLen;
  CHAR16                                      BackupChar;
  CHAR16                                      *StrPointer;
  BOOLEAN                                     AllocatedRequest;

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = NULL;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;
  PrivateData       = VFR_APP_PRIVATE_FROM_THIS (This);
  HiiConfigRouting  = PrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (VFR_APP_CONFIGURATION);
  Status = gRT->GetVariable (mVariableName, &mVfrAppFormSetGuid, NULL, &BufferSize, &PrivateData->Configuration);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  if (Request == NULL) {
    //
    // Request is set to NULL, construct full request string.
    //

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&mVfrAppFormSetGuid, mVariableName, PrivateData->DriverHandle);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &mVfrAppFormSetGuid, NULL)) {
      return EFI_NOT_FOUND;
    }
    //
    // Check whether request for EFI Varstore. EFI varstore get data
    // through hii database, not support in this path.
    //
    if (HiiIsConfigHdrMatch(Request, &mVfrAppFormSetGuid, mMyEfiVar)) {
      return EFI_UNSUPPORTED;
    }
    //
    // Set Request to the unified request string.
    //
    ConfigRequest = Request;
    //
    // Check whether Request includes Request Element.
    //
    if (StrStr (Request, L"OFFSET") == NULL) {
      //
      // Check Request Element does exist in Reques String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest    = AllocateZeroPool (Size);
        ASSERT (ConfigRequest != NULL);
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64)BufferSize);
      }
    }
  }

  //
  // Check if requesting Name/Value storage
  //
  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // Update Name/Value storage Names
    //
    Status = LoadNameValueNames (PrivateData);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Allocate memory for <ConfigResp>, e.g. Name0=0x11, Name1=0x1234, Name2="ABCD"
    // <Request>   ::=<ConfigHdr>&Name0&Name1&Name2
    // <ConfigResp>::=<ConfigHdr>&Name0=11&Name1=1234&Name2=0041004200430044
    //
    BufferSize = (StrLen (ConfigRequest) +
      1 + sizeof (PrivateData->Configuration.NameValueVar0) * 2 +
      1 + sizeof (PrivateData->Configuration.NameValueVar1) * 2 +
      1 + sizeof (PrivateData->Configuration.NameValueVar2) * 2 + 1) * sizeof (CHAR16);
    *Results = AllocateZeroPool (BufferSize);
    ASSERT (*Results != NULL);
    StrCpyS (*Results, BufferSize / sizeof (CHAR16), ConfigRequest);
    Value = *Results;

    //
    // Append value of NameValueVar0, type is UINT8
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[0])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[0]);
      ValueStrLen = ((sizeof (PrivateData->Configuration.NameValueVar0) * 2) + 1);
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      BackupChar = Value[ValueStrLen];
      *Value++   = L'=';
      UnicodeValueToStringS (
        Value,
        BufferSize - ((UINTN)Value - (UINTN)*Results),
        PREFIX_ZERO | RADIX_HEX,
        PrivateData->Configuration.NameValueVar0,
        sizeof (PrivateData->Configuration.NameValueVar0) * 2
        );
      Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      *Value = BackupChar;
    }

    //
    // Append value of NameValueVar1, type is UINT16
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[1])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[1]);
      ValueStrLen = ((sizeof (PrivateData->Configuration.NameValueVar1) * 2) + 1);
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      BackupChar = Value[ValueStrLen];
      *Value++   = L'=';
      UnicodeValueToStringS (
        Value,
        BufferSize - ((UINTN)Value - (UINTN)*Results),
        PREFIX_ZERO | RADIX_HEX,
        PrivateData->Configuration.NameValueVar1,
        sizeof (PrivateData->Configuration.NameValueVar1) * 2
        );
      Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      *Value = BackupChar;
    }

    //
    // Append value of NameValueVar2, type is CHAR16 *
    //
    if ((Value = StrStr (*Results, PrivateData->NameValueName[2])) != NULL) {
      Value += StrLen (PrivateData->NameValueName[2]);
      ValueStrLen = StrLen (PrivateData->Configuration.NameValueVar2) * 4 + 1;
      CopyMem (Value + ValueStrLen, Value, StrSize (Value));

      *Value++ = L'=';
      //
      // Convert Unicode String to Config String, e.g. "ABCD" => "0041004200430044"
      //
      StrPointer = (CHAR16 *) PrivateData->Configuration.NameValueVar2;
      for (; *StrPointer != L'\0'; StrPointer++) {
        UnicodeValueToStringS (
          Value,
          BufferSize - ((UINTN)Value - (UINTN)*Results),
          PREFIX_ZERO | RADIX_HEX,
          *StrPointer,
          4
          );
        Value += StrnLenS (Value, (BufferSize - ((UINTN)Value - (UINTN)*Results)) / sizeof (CHAR16));
      }
    }

    Status = EFI_SUCCESS;
  } else {
    //
    // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
    //
    Status = HiiConfigRouting->BlockToConfig (
                                  HiiConfigRouting,
                                  ConfigRequest,
                                  (UINT8 *) &PrivateData->Configuration,
                                  BufferSize,
                                  Results,
                                  Progress
                                  );
    if (!EFI_ERROR (Status)) {
      ConfigRequestHdr = HiiConstructConfigHdr (&mVfrAppFormSetGuid, mVariableName, PrivateData->DriverHandle);
      AppendAltCfgString(Results, ConfigRequestHdr);
    }
  }

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
  }

  if (ConfigRequestHdr != NULL) {
    FreePool (ConfigRequestHdr);
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}


/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Configuration          A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param  Progress               A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  VFR_APP_PRIVATE_DATA             *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  CHAR16                           *Value;
  CHAR16                           *StrPtr;
  CHAR16                           TemStr[5];
  UINT8                            *DataBuffer;
  UINT8                            DigitUint8;
  UINTN                            Index;
  CHAR16                           *StrBuffer;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = VFR_APP_PRIVATE_FROM_THIS (This);
  HiiConfigRouting = PrivateData->HiiConfigRouting;
  *Progress = Configuration;

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if (!HiiIsConfigHdrMatch (Configuration, &mVfrAppFormSetGuid, NULL)) {
    return EFI_NOT_FOUND;
  }

  //
  // Check whether request for EFI Varstore. EFI varstore get data
  // through hii database, not support in this path.
  //
  if (HiiIsConfigHdrMatch(Configuration, &mVfrAppFormSetGuid, mMyEfiVar)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (VFR_APP_CONFIGURATION);
  Status = gRT->GetVariable (
            mVariableName,
            &mVfrAppFormSetGuid,
            NULL,
            &BufferSize,
            &PrivateData->Configuration
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Check if configuring Name/Value storage
  //
  if (StrStr (Configuration, L"OFFSET") == NULL) {
    //
    // Update Name/Value storage Names
    //
    Status = LoadNameValueNames (PrivateData);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // Convert value for NameValueVar0
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[0])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[0]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Value to Buffer data
      //
      DataBuffer = (UINT8 *) &PrivateData->Configuration.NameValueVar0;
      ZeroMem (TemStr, sizeof (TemStr));
      for (Index = 0, StrPtr --; StrPtr >= Value; StrPtr --, Index ++) {
        TemStr[0] = *StrPtr;
        DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
        if ((Index & 1) == 0) {
          DataBuffer [Index/2] = DigitUint8;
        } else {
          DataBuffer [Index/2] = (UINT8) ((UINT8) (DigitUint8 << 4) + DataBuffer [Index/2]);
        }
      }
    }

    //
    // Convert value for NameValueVar1
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[1])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[1]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Value to Buffer data
      //
      DataBuffer = (UINT8 *) &PrivateData->Configuration.NameValueVar1;
      ZeroMem (TemStr, sizeof (TemStr));
      for (Index = 0, StrPtr --; StrPtr >= Value; StrPtr --, Index ++) {
        TemStr[0] = *StrPtr;
        DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
        if ((Index & 1) == 0) {
          DataBuffer [Index/2] = DigitUint8;
        } else {
          DataBuffer [Index/2] = (UINT8) ((UINT8) (DigitUint8 << 4) + DataBuffer [Index/2]);
        }
      }
    }

    //
    // Convert value for NameValueVar2
    //
    if ((Value = StrStr (Configuration, PrivateData->NameValueName[2])) != NULL) {
      //
      // Skip "Name="
      //
      Value += StrLen (PrivateData->NameValueName[2]);
      Value++;
      //
      // Get Value String
      //
      StrPtr = StrStr (Value, L"&");
      if (StrPtr == NULL) {
        StrPtr = Value + StrLen (Value);
      }
      //
      // Convert Config String to Unicode String, e.g "0041004200430044" => "ABCD"
      //
      StrBuffer = (CHAR16 *) PrivateData->Configuration.NameValueVar2;
      ZeroMem (TemStr, sizeof (TemStr));
      while (Value < StrPtr) {
        StrnCpyS (TemStr, sizeof (TemStr) / sizeof (CHAR16), Value, 4);
        *(StrBuffer++) = (CHAR16) StrHexToUint64 (TemStr);
        Value += 4;
      }
      *StrBuffer = L'\0';
    }

    //
    // Store Buffer Storage back to EFI variable
    //
    Status = gRT->SetVariable(
      mVariableName,
      &mVfrAppFormSetGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
      sizeof (VFR_APP_CONFIGURATION),
      &PrivateData->Configuration
      );

    return Status;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (VFR_APP_CONFIGURATION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               (UINT8 *) &PrivateData->Configuration,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = gRT->SetVariable(
                  mVariableName,
                  &mVfrAppFormSetGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (VFR_APP_CONFIGURATION),
                  &PrivateData->Configuration
                  );

  return Status;
}

EFI_STATUS
VfrAppUpdateSetupPageHighlightFormLabel (
  IN VFR_APP_PRIVATE_DATA                     *Private
  )
{
  VOID                                        *StartOpCodeHandle;
  VOID                                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                          *StartLabel;
  EFI_IFR_GUID_LABEL                          *EndLabel;
  EFI_GUID                                    FormSetGuid = VFR_APP_FORMSET_GUID;

  StartOpCodeHandle   = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle     = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    return EFI_ABORTED;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LABEL_UPDATE_SELECTABLE_QUESTION_LIST_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = LABEL_UPDATE_SELECTABLE_QUESTION_LIST_END;

  HiiCreateGotoOpCode (StartOpCodeHandle, VFR_APP_FORM_SETUP_PAGE_PANEL, STRING_TOKEN (STR_QUESTION_ONE_SAME_QUESTION_ID)  , STRING_TOKEN (STR_QUESTION_ONE_SAME_QUESTION_ID)  , 0, VFR_APP_KEY_SAME_QUESTION_ID);
  HiiCreateGotoOpCode (StartOpCodeHandle, VFR_APP_FORM_SETUP_PAGE_PANEL, STRING_TOKEN (STR_QUESTION_TWO_SAME_QUESTION_ID)  , STRING_TOKEN (STR_QUESTION_TWO_SAME_QUESTION_ID)  , 0, VFR_APP_KEY_SAME_QUESTION_ID);
  HiiCreateGotoOpCode (StartOpCodeHandle, VFR_APP_FORM_SETUP_PAGE_PANEL, STRING_TOKEN (STR_QUESTION_THREE_QUESTION_ID_ZERO), STRING_TOKEN (STR_QUESTION_THREE_QUESTION_ID_ZERO), 0, 0);

  HiiUpdateForm (
    Private->HiiHandle,
    &FormSetGuid,
    VFR_APP_FORM_SETUP_PAGE_PANEL_HIGHLIGHT_TEST,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  return EFI_SUCCESS;
}


EFI_STATUS
VfrAppRefreshIdUpdateMenuLabel (
  IN VFR_APP_PRIVATE_DATA                     *Private
  )
{
  VOID                                        *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL                          *StartLabel;
  EFI_GUID                                    FormSetGuid = VFR_APP_FORMSET_GUID;
  EFI_STATUS                                  Status;
  CHAR16                                      VariableName[]       = L"VfrAppVar";

  Status = EFI_UNSUPPORTED;
  //
  // We will reach here once the Question is refreshed
  //

  //
  // Initialize the container for dynamic opcodes
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);
  if (StartOpCodeHandle == NULL ) {
    return EFI_ABORTED;
  }

  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LABEL_UPDATE3;
  Private->Configuration.RefreshGuidCount++;

  HiiCreateActionOpCode (
    StartOpCodeHandle,                // Container for dynamic created opcodes
    VFR_APP_Q_ID_VFR_EXIT,            // Question ID
    STRING_TOKEN(STR_EXIT_TEXT),      // Prompt text
    STRING_TOKEN(STR_EXIT_TEXT),      // Help text
    EFI_IFR_FLAG_CALLBACK,            // Question flag
    0                                 // Action String ID
  );

  HiiUpdateForm (
    Private->HiiHandle,               // HII handle
    &FormSetGuid,                     // Formset GUID
    VFR_APP_FORM_REFRESH_OPCODE,      // Form ID
    StartOpCodeHandle,                // Label for where to insert opcodes
    NULL                              // Insert data
  );

  HiiFreeOpCodeHandle (StartOpCodeHandle);

  //
  // Refresh the Question value
  //
  Status = gRT->SetVariable(
                        VariableName,
                        &FormSetGuid,
                        EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof (VFR_APP_CONFIGURATION),
                        &Private->Configuration
                        );
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DriverCallbackActionRetrieve (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                                 Status;
  CHAR16                                     String[30];
  STATIC UINT8                               RefreshInterval1 = 0;
  STATIC UINT8                               RefreshInterval2 = 0;

  Status = EFI_SUCCESS;

  switch (QuestionId) {

  case VFR_APP_Q_ID_FB_FUNC_VFR_REFRESH_INTERVAL_ONE:
    RefreshInterval1++;
    Value->u8 = RefreshInterval1;

    UnicodeSPrint (String, sizeof (String), L"Count 1: %d", RefreshInterval1);
    HiiSetString (
      mVfrAppPrivate->HiiHandle,
      STRING_TOKEN(STR_FB_FUNC_VFR_REFRESH_INTERVAL_S_ONE),
      String,
      NULL
      );
    break;

  case VFR_APP_Q_ID_FB_FUNC_VFR_REFRESH_INTERVAL_TWO:
    RefreshInterval2--;
    Value->u8 = RefreshInterval2;

    UnicodeSPrint (String, sizeof (String), L"Count 2: %d", RefreshInterval2);
    HiiSetString (
      mVfrAppPrivate->HiiHandle,
      STRING_TOKEN(STR_FB_FUNC_VFR_REFRESH_INTERVAL_S_TWO),
      String,
      NULL
      );
    break;

  case VFR_APP_Q_ID_VFR_REFRESH_ID:
    VfrAppRefreshIdUpdateMenuLabel (mVfrAppPrivate);
    Status = EFI_SUCCESS;
    break;

  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                                 Status;

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    Status = EFI_UNSUPPORTED;
    if (QuestionId == VFR_APP_KEY_HIDDEN_QUESTION) {
      VfrAppConOutUpdateDevLabel (mVfrAppPrivate);
      VfrAppUpdateSetupPageHighlightFormLabel (mVfrAppPrivate);
      Status = EFI_SUCCESS;
    }
    if (QuestionId == VFR_APP_Q_ID_VFR_REFRESH_ID) {
      Status = InternalStartMonitor ();
      ASSERT_EFI_ERROR (Status);
    }
    break;

  case EFI_BROWSER_ACTION_RETRIEVE:
    Status = DriverCallbackActionRetrieve (This, Action, QuestionId, Type, Value, ActionRequest);
    break;

  case H2O_BROWSER_ACTION_HOT_KEY_CALLBACK:
    Status = EFI_UNSUPPORTED;
    if (QuestionId == 0x400B || QuestionId == 0x400C) {
      EFI_INPUT_KEY                              Key;
      H2O_DIALOG_PROTOCOL                        *H2ODialog;
      UINT8                                      *QuestionValuePtr;

      HiiGetBrowserData (&mVfrAppFormSetGuid, L"VfrAppVar", sizeof (mVfrAppPrivate->Configuration), (UINT8 *) &mVfrAppPrivate->Configuration);
      QuestionValuePtr = (QuestionId == 0x400B) ? &mVfrAppPrivate->Configuration.HotKeyPanelNumeric0011 :
                                                  &mVfrAppPrivate->Configuration.HotKeyPanelNumeric0012;
      *QuestionValuePtr = (*QuestionValuePtr >= 99) ? 0 : *QuestionValuePtr + 1;
      HiiSetBrowserData (&mVfrAppFormSetGuid, L"VfrAppVar", sizeof (mVfrAppPrivate->Configuration), (UINT8 *) &mVfrAppPrivate->Configuration, NULL);

      Status = gBS->LocateProtocol (&gH2ODialogProtocolGuid, NULL, (VOID **)&H2ODialog);
      if (!EFI_ERROR(Status)) {
        if (QuestionId == 0x400B) {
          H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, L"Hot Key Callback: Question 1 value should be increased by 1.");
        } else {
          H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, L"Hot Key Callback: Question 2 value should be increased by 1 and change highlight question to Question 1.");
          SendGotoHotKeyNotify (0, 0x400B);
        }
      }
      Status = EFI_SUCCESS;
    }
    break;

  case EFI_BROWSER_ACTION_CHANGED:
    Status = EFI_UNSUPPORTED;
    switch (QuestionId) {
      case VFR_APP_Q_ID_VFR_EXIT:
        //
        // User press "Exit now", request Browser to exit
        //
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_EXIT;
        Status = EFI_SUCCESS;
        break;
    }
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    Status = EFI_UNSUPPORTED;
    if (QuestionId == VFR_APP_Q_ID_VFR_REFRESH_ID) {
      Status = InternalStopMonitor ();
      ASSERT_EFI_ERROR (Status);
    }
    break;

  default:
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
VfrAppUnload (
  VOID
  )
{
  UINTN Index;

  if (mVfrAppPrivate == NULL) {
    return EFI_SUCCESS;
  }

  if (mVfrAppPrivate->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
            mVfrAppPrivate->DriverHandle,
            &gEfiDevicePathProtocolGuid,
            &mHiiVendorDevicePath0,
            &gEfiHiiConfigAccessProtocolGuid,
            &mVfrAppPrivate->ConfigAccess,
            NULL
           );
    mVfrAppPrivate->DriverHandle = NULL;
  }

  if (mVfrAppPrivate->HiiHandle != NULL) {
    HiiRemovePackages (mVfrAppPrivate->HiiHandle);
  }

  for (Index = 0; Index < NAME_VALUE_NAME_NUMBER; Index++) {
    if (mVfrAppPrivate->NameValueName[Index] != NULL) {
      FreePool (mVfrAppPrivate->NameValueName[Index]);
    }
  }
  FreePool (mVfrAppPrivate);
  mVfrAppPrivate = NULL;
  gBS->CloseEvent (mEvent);

  return EFI_SUCCESS;
}

EFI_STATUS
VfrAppInitializePrivateData (
  VOID
  )
{
  EFI_STATUS                                  Status;
  VFR_APP_PRIVATE_DATA                        *Private;

  Private = AllocateZeroPool (sizeof (VFR_APP_PRIVATE_DATA));
  if (Private == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mVfrAppPrivate = Private;

  gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid     , NULL, (VOID **) &Private->HiiDatabase     );
  gBS->LocateProtocol (&gEfiHiiStringProtocolGuid       , NULL, (VOID **) &Private->HiiString       );
  gBS->LocateProtocol (&gEfiFormBrowser2ProtocolGuid    , NULL, (VOID **) &Private->FormBrowser2    );
  gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &Private->HiiConfigRouting);
  if (Private->HiiDatabase  == NULL || Private->HiiString        == NULL ||
      Private->FormBrowser2 == NULL || Private->HiiConfigRouting == NULL) {
    Status = EFI_ABORTED;
    goto Exit;
  }

  Private->Signature                  = VFR_APP_PRIVATE_SIGNATURE;
  Private->PasswordState              = BROWSER_STATE_VALIDATE_PASSWORD;
  Private->ConfigAccess.ExtractConfig = ExtractConfig;
  Private->ConfigAccess.RouteConfig   = RouteConfig;
  Private->ConfigAccess.Callback      = DriverCallback;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  NULL
                  );
  if (Status != EFI_SUCCESS) {
    goto Exit;
  }

  Private->HiiHandle = HiiAddPackages (
                         &mVfrAppFormSetGuid,
                         Private->DriverHandle,
                         VfrAppStrings,
                         VfrAppVfrBin,
                         NULL
                         );
  if (Private->HiiHandle == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status)) {
    VfrAppUnload ();
  }

  return Status;
}

EFI_STATUS
VfrAppInitializeConfigData (
  VOID
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       BufferSize;
  VFR_APP_CONFIGURATION                       *Configuration;
  BOOLEAN                                     ActionFlag;
  EFI_STRING                                  ConfigRequestHdr;
  EFI_STRING                                  NameRequestHdr;
  VFR_APP_EFI_VARSTORE_DATA                   *VarStoreConfig;

  //
  // Initialize configuration data
  //
  Configuration = &mVfrAppPrivate->Configuration;
  ZeroMem (Configuration, sizeof (VFR_APP_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr (&mVfrAppFormSetGuid, mVariableName, mVfrAppPrivate->DriverHandle);
  NameRequestHdr   = HiiConstructConfigHdr (&mVfrAppFormSetGuid, NULL         , mVfrAppPrivate->DriverHandle);
  if (ConfigRequestHdr == NULL || NameRequestHdr == NULL) {
    return EFI_ABORTED;
  }

  BufferSize = sizeof (VFR_APP_CONFIGURATION);
  Status     = gRT->GetVariable (mVariableName, &mVfrAppFormSetGuid, NULL, &BufferSize, Configuration);
  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable(
                    mVariableName,
                    &mVfrAppFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (VFR_APP_CONFIGURATION),
                    Configuration
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (NameRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }

    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (NameRequestHdr);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }

    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  //
  // Initialize efi varstore configuration data
  //
  VarStoreConfig = &mVfrAppPrivate->VarStoreConfig;
  ZeroMem (VarStoreConfig, sizeof (VFR_APP_EFI_VARSTORE_DATA));

  ConfigRequestHdr = HiiConstructConfigHdr (&mVfrAppFormSetGuid, mMyEfiVar, mVfrAppPrivate->DriverHandle);
  if (ConfigRequestHdr == NULL) {
    return EFI_ABORTED;
  }

  BufferSize = sizeof (VFR_APP_EFI_VARSTORE_DATA);
  Status     = gRT->GetVariable (mMyEfiVar, &mVfrAppFormSetGuid, NULL, &BufferSize, VarStoreConfig);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data to EFI variable Storage.
    //
    Status = gRT->SetVariable(
                    mMyEfiVar,
                    &mVfrAppFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (VFR_APP_EFI_VARSTORE_DATA),
                    VarStoreConfig
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  return EFI_SUCCESS;
}

EFI_STATUS
VfrAppUpdateDisplayEngineList (
  VOID
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       NumberOfHandle;
  EFI_HANDLE                                  *HandleBuffer;
  UINT32                                      Index;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gH2ODisplayEngineProtocolGuid,
                  NULL,
                  &NumberOfHandle,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || NumberOfHandle == 0 || HandleBuffer == NULL) {
    return EFI_NOT_FOUND;
  }

  mH2ODisplayEngineList = AllocateZeroPool (NumberOfHandle * sizeof (H2O_DISPLAY_ENGINE_PROTOCOL *));
  if (mH2ODisplayEngineList == NULL) {
    FreePool (HandleBuffer);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < NumberOfHandle; Index++) {
    gBS->HandleProtocol (HandleBuffer[Index], &gH2ODisplayEngineProtocolGuid, (VOID **) &mH2ODisplayEngineList[Index]);
  }
  mH2ODisplayEngineHandleList = HandleBuffer;
  mH2ODisplayEngineCount      = NumberOfHandle;

  return EFI_SUCCESS;
}

VOID
EFIAPI
VfrAppControlDisplayEngine (
  VOID
  )
{
  EFI_STATUS                                  Status;
  UINT8                                       ActiveDE;
  UINT32                                      Index;
  EFI_HANDLE                                  DisplayEngineHandle;
  H2O_DISPLAY_ENGINE_PROTOCOL                 *DisplayEngine;
  H2O_DISPLAY_ENGINE_PROTOCOL                 *ProtocolInstance;

  if (mH2ODisplayEngineList == NULL) {
    VfrAppUpdateDisplayEngineList ();
  }

  if (mH2ODisplayEngineList == NULL || mH2ODisplayEngineCount < 2) {
    return;
  }

  //
  // Install active display engine protocol and uninstall unnecessary display engine protocol.
  //
  ActiveDE = mVfrAppPrivate->Configuration.ActiveDisplayEngine;

  for (Index = 0; Index < mH2ODisplayEngineCount; Index++) {
    DisplayEngine       = mH2ODisplayEngineList[Index];
    DisplayEngineHandle = mH2ODisplayEngineHandleList[Index];
    if (DisplayEngine == NULL || DisplayEngineHandle == NULL) {
      continue;
    }

    if ((ActiveDE == VFR_APP_ACTIVE_DISPLAY_ENGINE_ALL) ||
        (ActiveDE == VFR_APP_ACTIVE_DISPLAY_ENGINE_METRO && CompareGuid (&DisplayEngine->Id, &gH2ODisplayEngineLocalMetroGuid)) ||
        (ActiveDE == VFR_APP_ACTIVE_DISPLAY_ENGINE_TEXT  && CompareGuid (&DisplayEngine->Id, &gH2ODisplayEngineLocalTextGuid))) {
      Status = gBS->HandleProtocol (DisplayEngineHandle, &gH2ODisplayEngineProtocolGuid, (VOID **) &ProtocolInstance);
      if (EFI_ERROR (Status)) {
        gBS->InstallProtocolInterface (
               &DisplayEngineHandle,
               &gH2ODisplayEngineProtocolGuid,
               EFI_NATIVE_INTERFACE,
               DisplayEngine
               );
      }
    } else {
      Status = gBS->HandleProtocol (DisplayEngineHandle, &gH2ODisplayEngineProtocolGuid, (VOID **) &ProtocolInstance);
      if (!EFI_ERROR (Status)) {
        gBS->UninstallProtocolInterface (
               DisplayEngineHandle,
               &gH2ODisplayEngineProtocolGuid,
               DisplayEngine
               );
      }
    }
  }
}

VOID
VfrAppTestHiiPopupDialogFunc (
  VOID
  )
{
  EFI_STATUS                                  Status;
  EFI_HII_POPUP_PROTOCOL                      *HiiPopup;
  EFI_HII_POPUP_SELECTION                     UserSelection;

  mVfrAppPrivate->Configuration.TestHiiPopupDialog = FALSE;
  gRT->SetVariable(
          mVariableName,
          &mVfrAppFormSetGuid,
          EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
          sizeof (VFR_APP_CONFIGURATION),
          &mVfrAppPrivate->Configuration
          );

  Status = gBS->LocateProtocol (&gEfiHiiPopupProtocolGuid, NULL, (VOID **) &HiiPopup);
  if (EFI_ERROR (Status)) {
    return;
  }
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleInfo, EfiHiiPopupTypeOk         , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_INFO), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleInfo, EfiHiiPopupTypeOkCancel   , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_INFO), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleInfo, EfiHiiPopupTypeYesNo      , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_INFO), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleInfo, EfiHiiPopupTypeYesNoCancel, mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_INFO), &UserSelection);

  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleWarning, EfiHiiPopupTypeOk         , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_WARNING), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleWarning, EfiHiiPopupTypeOkCancel   , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_WARNING), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleWarning, EfiHiiPopupTypeYesNo      , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_WARNING), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleWarning, EfiHiiPopupTypeYesNoCancel, mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_WARNING), &UserSelection);

  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleError, EfiHiiPopupTypeOk         , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_ERROR), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleError, EfiHiiPopupTypeOkCancel   , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_ERROR), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleError, EfiHiiPopupTypeYesNo      , mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_ERROR), &UserSelection);
  HiiPopup->CreatePopup (HiiPopup, EfiHiiPopupStyleError, EfiHiiPopupTypeYesNoCancel, mVfrAppPrivate->HiiHandle, STRING_TOKEN(STR_HII_POP_UP_MESSAGE_ERROR), &UserSelection);
}

VOID
VfrAppTestH2ODialogFunc (
  VOID
  )
{
  EFI_STATUS                                  Status;
  EFI_INPUT_KEY                               Key;
  H2O_DIALOG_PROTOCOL                         *H2ODialog;
  UINT32                                      Index;
  UINT64                                      Value;
  CHAR16                                      StringBuffer[20];
  CHAR16                                      *OptionBuffer[]  = {L"Option 1",L"Option 2", L"Option 3"};


  Status = gBS->LocateProtocol (&gH2ODialogProtocolGuid, NULL, (VOID **)&H2ODialog);
  if (EFI_ERROR(Status)) {
    return;
  }

  mVfrAppPrivate->Configuration.TestH2ODialog = FALSE;
  gRT->SetVariable(
          mVariableName,
          &mVfrAppFormSetGuid,
          EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
          sizeof (VFR_APP_CONFIGURATION),
          &mVfrAppPrivate->Configuration
          );

  H2ODialog->ConfirmDialog (2, FALSE, 0, NULL, &Key, L"Test H2ODialog->ConfirmDialog");
  H2ODialog->PasswordDialog (0, FALSE, (UINT32) (sizeof (StringBuffer) / sizeof (CHAR16) - 1), StringBuffer, &Key, L"Test H2ODialog->PasswordDialog");
  H2ODialog->OneOfOptionDialog (3, FALSE, NULL, &Key, 20, L"Test H2ODialog->OneOfOptionDialog", &Index, OptionBuffer, 0);
  H2ODialog->ShowPageInfo (L"Test H2ODialog->ShowPageInfo", L"Page Info String");
  H2ODialog->NumericDialog (L"Test H2ODialog->NumericDialog", 0, 100, 1, FALSE, &Value);
  H2ODialog->ConfirmPageDialog (1, L"Test H2ODialog->ConfirmPageDialog", L"Comform String 1", L"Comform String 2", &Key);
  H2ODialog->CreateMsgPopUp (20, 2, L"Test H2ODialog->CreateMsgPopUp", L"Please restart system");
}

VOID
EFIAPI
VfrAppReadyToBootEvtFunc (
  IN EFI_EVENT                                Event,
  IN VOID                                     *Context
  )
{
  VfrAppControlDisplayEngine ();
  VfrAppConOutUpdateDESetting (mVfrAppPrivate);

  if (mVfrAppPrivate->Configuration.TestHiiPopupDialog) {
    VfrAppTestHiiPopupDialogFunc ();
  }
  if (mVfrAppPrivate->Configuration.TestH2ODialog) {
    VfrAppTestH2ODialogFunc ();
  }
}

VOID
EFIAPI
VfrAppBdsEntryFunc (
  IN EFI_BDS_ARCH_PROTOCOL                    *This
  )
{
  VfrAppControlDisplayEngine ();
  VfrAppConOutDevInit ();

  mOriginalBdsEntry (This);
}

EFI_STATUS
EFIAPI
VfrAppMain (
  IN EFI_HANDLE                               ImageHandle,
  IN EFI_SYSTEM_TABLE                         *SystemTable
  )
{
  EFI_STATUS                                  Status;
  EFI_EVENT                                   Event;
  EFI_BDS_ARCH_PROTOCOL                       *Bds;

  Status = VfrAppInitializePrivateData ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VfrAppInitializeConfigData ();
  VfrAppLegacyBiosInit ();

  Status = gBS->CreateEventEx (
        EVT_NOTIFY_SIGNAL,
        TPL_NOTIFY,
        EfiEventEmptyFunction,
        NULL,
        &gEfiIfrRefreshIdOpGuid,
        &mEvent
        );
  ASSERT_EFI_ERROR (Status);

  EfiCreateEventReadyToBootEx (TPL_CALLBACK, VfrAppReadyToBootEvtFunc, NULL, &Event);

  Status = gBS->LocateProtocol (&gEfiBdsArchProtocolGuid, NULL, (VOID **) &Bds);
  if (!EFI_ERROR (Status)) {
    mOriginalBdsEntry = Bds->Entry;
    Bds->Entry = VfrAppBdsEntryFunc;
  }

  return EFI_SUCCESS;
}

