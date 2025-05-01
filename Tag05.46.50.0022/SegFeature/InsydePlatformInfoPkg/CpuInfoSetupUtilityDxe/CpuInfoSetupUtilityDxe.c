/** @file
  CPU Information

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "CpuInfoSetupUtilityDxe.h"

CPU_INFO_PRIVATE_DATA         *mCpuInfoData = NULL;
H2O_CPU_DEVICE_INFO_PROTOCOL  *mCpuDeviceInfoProtocol = NULL;
CPU_DISPLAY_LIST              mCpuTokenList[] = {
  {0, STRING_TOKEN (STR_PROCESSOR_VERSION_PROMPT),     STRING_TOKEN (STR_PROCESSOR_VERSION_HELP),     STRING_TOKEN (STR_PROCESSOR_VERSION_TEXT),     UpdateProcessorVersionString},
  {0, STRING_TOKEN (STR_CPUID_PROMPT),                 STRING_TOKEN (STR_CPUID_HELP),                 STRING_TOKEN (STR_CPUID_TEXT),                 UpdateCpuIdString},
  {0, STRING_TOKEN (STR_CPU_SPEED_PROMPT),             STRING_TOKEN (STR_CPU_SPEED_HELP),             STRING_TOKEN (STR_CPU_SPEED_TEXT),             UpdateCpuSpeedString},
  {0, STRING_TOKEN (STR_CPU_TDP_PROMPT),               STRING_TOKEN (STR_CPU_TDP_HELP),               STRING_TOKEN (STR_CPU_TDP_TEXT),               UpdateCpuTDPString},
  {0, STRING_TOKEN (STR_CPU_VOLTAGE_PROMPT),           STRING_TOKEN (STR_CPU_VOLTAGE_HELP),           STRING_TOKEN (STR_CPU_VOLTAGE_TEXT),           UpdateCpuVoltageString},
  {0, STRING_TOKEN (STR_CPU_STEPPING_PROMPT),          STRING_TOKEN (STR_CPU_STEPPING_HELP),          STRING_TOKEN (STR_CPU_STEPPING_TEXT),          UpdateCpuSteppingString},
  {0, STRING_TOKEN (STR_DATA_CACHE_PROMPT),            STRING_TOKEN (STR_DATA_CACHE_HELP),            STRING_TOKEN (STR_DATA_CACHE_TEXT),            UpdateCoreL1DataCacheString},
  {0, STRING_TOKEN (STR_INSTRUCTION_CACHE_PROMPT),     STRING_TOKEN (STR_INSTRUCTION_CACHE_HELP),     STRING_TOKEN (STR_INSTRUCTION_CACHE_TEXT),     UpdateCoreL1InsructionCacheString},
  {0, STRING_TOKEN (STR_L2_CACHE_PROMPT),              STRING_TOKEN (STR_L2_CACHE_HELP),              STRING_TOKEN (STR_L2_CACHE_TEXT),              UpdateCoreL2CacheString},
  {0, STRING_TOKEN (STR_L3_CACHE_PROMPT),              STRING_TOKEN (STR_L3_CACHE_HELP),              STRING_TOKEN (STR_L3_CACHE_TEXT),              UpdateCoreL3CacheString},
  {0, STRING_TOKEN (STR_NUM_OF_CORE_THREAD_PROMPT),    STRING_TOKEN (STR_NUM_OF_CORE_THREAD_HELP),    STRING_TOKEN (STR_NUM_OF_CORE_THREAD_TEXT),    UpdateCoreThreadNumString},
  {0, STRING_TOKEN (STR_MICROCODE_PATCH_LEVEL_PROMPT), STRING_TOKEN (STR_MICROCODE_PATCH_LEVEL_HELP), STRING_TOKEN (STR_MICROCODE_PATCH_LEVEL_TEXT), UpdateMicrocodeVersionString}
};

#define CPU_DETAIL_ITEM_MAXIMUM          (sizeof (mCpuTokenList) / sizeof (CPU_DISPLAY_LIST))

HII_VENDOR_DEVICE_PATH        mHiiCpuInfoVendorDevPath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    H2O_CPU_INFO_FORMSET_GUID
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

/**
  The entry point for the driver.

  @param[in] HiiHandle               Hii handle
  @param[in] StartOpCodeHandle       Pointer to the opcode handle

  @retval EFI_SUCCESS                The entry point is executed successfully.
**/
EFI_STATUS
UpdateCpuInfo (
  IN           EFI_HII_HANDLE                  HiiHandle,
  IN           VOID                            *StartOpCodeHandle
  )
{
  EFI_STATUS       Status;
  UINTN            SmbiosIndex;
  CHAR16           StringfBuffer[MAX_STR_SIZE] = {0};
  EFI_STRING_ID    StringToken;
  UINTN            TotalSmbiosType4;
  UINTN            TotalInstallCpu;
  CHAR16           *String = NULL;
  CHAR16           *CpuString;
  CHAR16           *InstalledString;
  CHAR16           *IndexString;
  UINT8            *DisplayList;
  UINT16           DisplayListMaximum;
  UINT16           DisplayListIndex;
  UINT8            ItemNumber;
  EFI_QUESTION_ID  QuestionId;
  UINTN            TotalLanguageCount;
  CHAR8            *LanguageString;
  UINTN            LanguageCount;
  CHAR8            *Lang;

  TotalLanguageCount = 0;
  LanguageString     = NULL;
  DisplayList        = (UINT8 *)PcdGetPtr (PcdH2OCpuDetailDisplayList);
  DisplayListMaximum = PcdGetSize (PcdH2OCpuDetailDisplayList);

  if (HiiHandle == 0 || StartOpCodeHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gH2OCpuDeviceInfoProtocolGuid,
                  NULL,
                  (VOID **) &mCpuDeviceInfoProtocol
                  );
  DEBUG ((EFI_D_INFO, "LocateProtocol gH2OCpuDeviceInfoProtocolGuid(%g) - (%r)\n", &gH2OCpuDeviceInfoProtocolGuid, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mCpuDeviceInfoProtocol->GetAllCpuDeviceInfo (
                                     mCpuDeviceInfoProtocol,
                                     &TotalSmbiosType4,
                                     &TotalInstallCpu,
                                     NULL
                                     );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get all supported languages
  //
  Status = GetSupportLang (&TotalLanguageCount, (UINT8**)&LanguageString);
  if (EFI_ERROR (Status) || (TotalLanguageCount == 0) || (LanguageString == NULL)) {
    return Status;
  }

  for (LanguageCount = 0; LanguageCount < TotalLanguageCount; LanguageCount++) {
    Lang = &LanguageString[LanguageCount * RFC_3066_ENTRY_SIZE];
    if (TotalInstallCpu > 1) {
      CpuString = HiiGetString (HiiHandle, STRING_TOKEN (STR_CPUS_STRING), Lang);
    } else {
      CpuString = HiiGetString (HiiHandle, STRING_TOKEN (STR_CPU_STRING), Lang);
    }
    InstalledString = HiiGetString (HiiHandle, STRING_TOKEN (STR_INSTALLED_STRING), Lang);
    UnicodeSPrint (
      StringfBuffer,
      MAX_STR_SIZE,
      L" %d %s %s",
      TotalInstallCpu,
      CpuString,
      InstalledString
      );
    HiiSetString (HiiHandle, STRING_TOKEN (STR_NUMBER_OF_CPU_INSTALLED_STRING), StringfBuffer, Lang);
    FreePool (CpuString);
    FreePool (InstalledString);
  }

  for (SmbiosIndex = 0; SmbiosIndex < TotalSmbiosType4; SmbiosIndex++) {
    if (!mCpuDeviceInfoProtocol->GetCpuInstallStatus (mCpuDeviceInfoProtocol, SmbiosIndex)) {
      continue;
    }

    HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_BLANK_STRING), 0, 0, 0);

    IndexString = HiiGetString (HiiHandle, STRING_TOKEN (STR_INDEX_OF_CPU_STRING), NULL);
    UnicodeSPrint (
      StringfBuffer,
      MAX_STR_SIZE,
      L"%s %d:",
      IndexString,
      SmbiosIndex + 1
      );
    StringToken = HiiSetString (HiiHandle, 0, StringfBuffer, NULL);
    HiiCreateSubTitleOpCode (StartOpCodeHandle, StringToken, 0, 0, 0);

    for (DisplayListIndex = 0; DisplayListIndex < DisplayListMaximum; DisplayListIndex++) {
      ItemNumber = DisplayList[DisplayListIndex];
      if (ItemNumber >= CPU_DETAIL_ITEM_MAXIMUM) {
        continue;
      }

      if (mCpuTokenList[ItemNumber].ItemDisplay > SmbiosIndex) {
        continue;
      }

      QuestionId = (H2O_CPU_INFO_DYNAMIC_DETAIL_BASE_QUESTION_ID + ((UINT16)SmbiosIndex << 8) + DisplayListIndex);
      Status = mCpuTokenList[ItemNumber].DisplayItemFunction (SmbiosIndex, &String);
      if (EFI_ERROR (Status)) {
        continue;
      }

      StringToken = HiiSetString (HiiHandle, 0, String, NULL);
      if (StringToken == 0) {
        StringToken = mCpuTokenList[ItemNumber].TextTwo;
      }

      HiiCreateActionOpCodeEx (
        StartOpCodeHandle,
        QuestionId,
        mCpuTokenList[ItemNumber].Prompt,
        mCpuTokenList[ItemNumber].Help,
        StringToken,
        0x00,
        STRING_TOKEN (0x0000)
        );
      mCpuTokenList[ItemNumber].ItemDisplay++;
    }
  }

  return EFI_SUCCESS;
}

/**
  Update the form data in the Cpu information form

  @param[in]      HiiHandle                Hii Handle

  @retval         EFI_SUCCESS              Update form data success
**/
EFI_STATUS
InitCpuInfoSetup (
  IN     EFI_HII_HANDLE       HiiHandle
  )
{
  EFI_STATUS                  Status;
  VOID                        *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL          *StartLabel;
  VOID                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL          *EndLabel;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_CPU_INFO_LABEL_START;

  EndLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = H2O_CPU_INFO_LABEL_END;

  UpdateCpuInfo (HiiHandle, StartOpCodeHandle);

  Status = HiiUpdateForm (
             HiiHandle,
             &gH2OCpuInfoFormsetGuid,
             H2O_CPU_INFO_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return EFI_SUCCESS;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Request                A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress               On return, points to a character in the Request string.
                                     Points to the string's null terminator if request was successful.
                                     Points to the most recent '&' before the first failing name/value
                                     pair (or the beginning of the string if the failure is in the
                                     first name/value pair) if the request was not successful.
  @param[out] Results                A null-terminated Unicode string in <ConfigAltResp> format which
                                     has all values filled in for the names in the Request string.
                                     String to be allocated by the called function.

  @retval EFI_INVALID_PARAMETER      Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND              Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
FakeExtractConfig (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN     CONST EFI_STRING                      Request,
     OUT       EFI_STRING                      *Progress,
     OUT       EFI_STRING                      *Results
  )
{
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration          A null-terminated Unicode string in <ConfigResp> format.
  @param[out] Progress               A pointer to a string filled in with the offset of the most
                                     recent '&' before the first failing name/value pair (or the
                                     beginning of the string if the failure is in the first
                                     name/value pair) or the terminating NULL if all was successful.

  @retval EFI_INVALID_PARAMETER      Configuration is NULL.
  @retval EFI_NOT_FOUND              Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
FakeRouteConfig (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN     CONST EFI_STRING                      Configuration,
     OUT       EFI_STRING                      *Progress
  )
{
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_NOT_FOUND;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action                 Specifies the type of action taken by the browser.
  @param[in]  QuestionId             A unique value which is sent to the original exporting driver
                                     so that it can identify the type of data to expect.
  @param[in]  Type                   The type of value for the question.
  @param[in]  Value                  A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest          On return, points to the action requested by the callback function.

  @retval     EFI_UNSUPPORTED        The specified Action is not supported by the callback.
  @retval     EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval     EFI_SUCCESS            The callback successfully handled the action.
**/
EFI_STATUS
EFIAPI
CpuInfoFormCallback (
  IN     CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN           EFI_BROWSER_ACTION              Action,
  IN           EFI_QUESTION_ID                 QuestionId,
  IN           UINT8                           Type,
  IN           EFI_IFR_TYPE_VALUE              *Value,
     OUT       EFI_BROWSER_ACTION_REQUEST      *ActionRequest
  )
{
  EFI_HII_HANDLE HiiHandle;

  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }

  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  HiiHandle = mCpuInfoData->HiiHandle;

  return EFI_SUCCESS;
}

/**
  Free the private data if error occurs.

  @return
**/
VOID
EFIAPI
FreePrivateData (
  )
{
  if (mCpuInfoData == NULL) {
    return;
  }

  FreePool (mCpuInfoData);

  mCpuInfoData = NULL;
}

/**
  Free the Nvm data and the Hii Handle.

  @return
**/
VOID
EFIAPI
DestoryScuCpuInfoData (
  )
{
  UINTN  DisplayListIndex;

  for (DisplayListIndex = 0; DisplayListIndex < CPU_DETAIL_ITEM_MAXIMUM; DisplayListIndex++) {
    mCpuTokenList[DisplayListIndex].ItemDisplay = 0;
  }

  if (mCpuInfoData == NULL) {
    return;
  }

  if (mCpuInfoData->SeviceHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mCpuInfoData->SeviceHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiCpuInfoVendorDevPath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mCpuInfoData->ConfigAccess,
           NULL
           );
    mCpuInfoData->SeviceHandle = NULL;
  }

  if (mCpuInfoData->HiiHandle != NULL) {
    HiiRemovePackages (mCpuInfoData->HiiHandle);
  }

  FreePrivateData ();

  return;
}

/**
  Unloads this driver and its installed protocol.

  @param[in]      ImageHandle              Handle that identifies the image to be unloaded.

  @retval         EFI_SUCCESS              The image has been unloaded.
**/
EFI_STATUS
EFIAPI
CpuInfoUnload (
  IN     EFI_HANDLE           ImageHandle
  )
{
  DestoryScuCpuInfoData ();

  return EFI_SUCCESS;
}

/**
  To collect data and initialize the Cpu info formset after entering setup utility application

  @param[in]      Event                    Event
  @param[in]      Context                  Context

  No return value.
**/
VOID
CpuSetupCallback (
  IN     EFI_EVENT            Event,
  IN     VOID                 *Context
  )
{
  EFI_STATUS                             Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL *SetupUtilityApp;
  EFI_HII_HANDLE                         HiiHandle;

  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL,
                  (VOID **) &SetupUtilityApp
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    if (mCpuInfoData == NULL) {
      //
      // Initialize driver private data
      //
      mCpuInfoData = AllocateZeroPool (sizeof (CPU_INFO_PRIVATE_DATA));

      if (mCpuInfoData == NULL) {
        return;
      }

      mCpuInfoData->Signature                  = CPU_INFO_PRIVATE_SIGNATURE;
      mCpuInfoData->ConfigAccess.ExtractConfig = FakeExtractConfig;
      mCpuInfoData->ConfigAccess.RouteConfig   = FakeRouteConfig;
      mCpuInfoData->ConfigAccess.Callback      = CpuInfoFormCallback;


      Status = gBS->InstallMultipleProtocolInterfaces (
                      &mCpuInfoData->SeviceHandle,
                      &gEfiDevicePathProtocolGuid,
                      &mHiiCpuInfoVendorDevPath,
                      &gEfiHiiConfigAccessProtocolGuid,
                      &mCpuInfoData->ConfigAccess,
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        FreePrivateData ();
        return;
      }

      //
      // Publish our HII data
      //
      HiiHandle = HiiAddPackages (
                    &gH2OCpuInfoFormsetGuid,
                    mCpuInfoData->SeviceHandle,
                    CpuInfoSetupUtilityDxeStrings,
                    CpuInfoVfrBin,
                    NULL
                    );
      if (HiiHandle == NULL) {
        FreePrivateData ();
        return;
      }

      mCpuInfoData->HiiHandle = HiiHandle;
    }

    Status = InitCpuInfoSetup (mCpuInfoData->HiiHandle);

  } else if (SetupUtilityApp->VfrDriverState == ShutdownSetupUtility) {
    DestoryScuCpuInfoData ();
  }

  return;
}

/**
  The entry point for the driver.

  @param[in]      ImageHandle              The firmware allocated handle for the EFI image.
  @param[in]      SystemTable              A pointer to the EFI System Table.

  @retval         EFI_SUCCESS              The entry point is executed successfully.
**/
EFI_STATUS
EFIAPI
CpuInfoEntry (
  IN     EFI_HANDLE           ImageHandle,
  IN     EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                  Status;
  VOID                        *Registration;

  mCpuInfoData = NULL;

  //
  // When execute setup utility application, install HII data
  //
  Status = EfiNamedEventListen (
             &gEfiSetupUtilityApplicationProtocolGuid,
             TPL_CALLBACK - 1,
             CpuSetupCallback,
             NULL,
             &Registration
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Fail to register CpuSetupCallback on gEfiSetupUtilityApplicationProtocolGuid, Status:%r\n", Status));
  }

  return EFI_SUCCESS;
}

