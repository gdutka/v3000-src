/** @file
  Memory Information

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MemoryInfoSetupUtilityDxe.h"

MEMORY_INFO_PRIVATE_DATA                    *mMemoryInfoData = NULL;
H2O_MEMORY_DEVICE_INFO_PROTOCOL             *mMemoryDeviceInfoProtocol = NULL;
MEMORY_DETAIL_DISPLAY_LIST                  mMemoryDetailDisplayList[] = {
                                              {FALSE, InitDeviceLocatorInfo,             UpdateDeviceLocatorInfo},
                                              {FALSE, InitBankLocatorInfo,               UpdateBankLocatorInfo},
                                              {FALSE, InitManufacturerInfo,              UpdateManufacturerInfo},
                                              {FALSE, InitMemoryTypeInfo,                UpdateMemoryTypeInfo},
                                              {FALSE, InitDramTypeInfo,                  UpdateDramTypeInfo},
                                              {FALSE, InitSerialNumberInfo,              UpdateSerialNumberInfo},
                                              {FALSE, InitPartNumberInfo,                UpdatePartNumberInfo},
                                              {FALSE, InitMemorySizeInfo,                UpdateMemorySizeInfo},
                                              {FALSE, InitMaximumCapableMemorySpeedInfo, UpdateMaximumCapableMemorySpeedInfo},
                                              {FALSE, InitConfigureMemorySpeedInfo,      UpdateConfigureMemorySpeedInfo},
                                              {FALSE, InitConfigureVoltageInfo,          UpdateConfigureVoltageInfo}
                                              };

#define MEMORY_DETAIL_ITEM_MAXIMUM          (sizeof (mMemoryDetailDisplayList) / sizeof (MEMORY_DETAIL_DISPLAY_LIST))

CHAR16                                      *mVariableName = H2O_MEMORY_INFO_SETUP_VARSTORE_NAME;
HII_VENDOR_DEVICE_PATH                      mHiiMemoryInfoVendorDevPath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    H2O_MEMORY_INFO_FORMSET_GUID
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
  Update the form data in the memory detail information form

  @param[in]  PrivateData               Points to the MEMORY_INFO_PRIVATE_DATA.

  @retval EFI_SUCCESS                   Update form data success
  @retval other                         Update form data fail
**/
EFI_STATUS
InitMemoryDetailInfoSetup (
  IN  MEMORY_INFO_PRIVATE_DATA          *PrivateData
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  VOID                                  *StartOpCodeHandle;
  VOID                                  *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                    *StartLabel;
  EFI_IFR_GUID_LABEL                    *EndLabel;
  UINT8                                 *DisplayList;
  UINT16                                DisplayListMaximum;
  UINT16                                DisplayListIndex;
  UINT8                                 ItemNumber;
  EFI_QUESTION_ID                       QuestionId;

  HiiHandle = PrivateData->HiiHandle;

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
  StartLabel->Number       = H2O_MEMORY_DETAIL_LABEL_START;

  EndLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = H2O_MEMORY_DETAIL_LABEL_END;

  DisplayList = (UINT8 *)PcdGetPtr (PcdH2OMemoryDetailDisplayList);
  DisplayListMaximum = PcdGetSize (PcdH2OMemoryDetailDisplayList);

  for (DisplayListIndex = 0; DisplayListIndex < DisplayListMaximum; DisplayListIndex++) {
    ItemNumber = DisplayList[DisplayListIndex];
    if (ItemNumber >= MEMORY_DETAIL_ITEM_MAXIMUM) {
      continue;
    }
    if (mMemoryDetailDisplayList[ItemNumber].ItemDisplay) {
      continue;
    }
    QuestionId = (H2O_MEMORY_INFO_DYNAMIC_DIMM_DETAIL_BASE_INFO_QUESTION_ID + ItemNumber);
    mMemoryDetailDisplayList[ItemNumber].InitItemFunction (StartOpCodeHandle, QuestionId);
    mMemoryDetailDisplayList[ItemNumber].ItemDisplay = TRUE;
  }

  Status = HiiUpdateForm (
             HiiHandle,
             &gH2OMemoryInfoFormsetGuid,
             H2O_MEMORY_INFO_DIMM_INFO_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return EFI_SUCCESS;
}

/**
  The function for update memory install status

  @param[in]  PrivateData               Points to the MEMORY_INFO_PRIVATE_DATA.
  @param[in]  TotalLanguageCount        Number Of Support Language
  @param[in]  LanguageString            Pointer Of Support Language String

  @retval EFI_SUCCESS                   Create memory goto item success
  @retval other                         Create memory goto item fail
**/
EFI_STATUS
UpdateMemoryInstallStatus (
  IN  MEMORY_INFO_PRIVATE_DATA          *PrivateData,
  IN  UINTN                             TotalLanguageCount,
  IN  CHAR8                             *LanguageString
  )
{
  EFI_HII_HANDLE                        HiiHandle;
  CHAR16                                TempString[MAX_STR_SIZE] = {0};
  CHAR16                                *MemoryString;
  CHAR16                                *MemoryInstalledString;
  CHAR16                                *MemoryNotInstalledString;
  UINTN                                 LanguageCount;
  CHAR8                                 *Lang;

  HiiHandle = PrivateData->HiiHandle;

  for (LanguageCount = 0; LanguageCount < TotalLanguageCount; LanguageCount++) {
    Lang = &LanguageString[LanguageCount * RFC_3066_ENTRY_SIZE];
    MemoryInstalledString    = HiiGetString (HiiHandle, STRING_TOKEN (STR_INSTALLED_STRING), Lang);
    MemoryNotInstalledString = HiiGetString (HiiHandle, STRING_TOKEN (STR_NOT_INSTALLED_STRING), Lang);

    //
    // Update Total Memory Number
    //
    MemoryString = HiiGetString (HiiHandle, STRING_TOKEN (STR_MEMORY_SLOT_STRING), Lang);
    UnicodeSPrint (
      TempString,
      MAX_STR_SIZE,
      L" %2d %s",
      PrivateData->NumberOfMemoryInfo,
      MemoryString
      );
    HiiSetString (HiiHandle, STRING_TOKEN (STR_TOTAL_OF_MEMORY_SLOT_STRING), TempString, Lang);
    FreePool (MemoryString);

    //
    // Update Total Install Memory Number
    //
    if (PrivateData->NumberOfInstallMemory > 1) {
      MemoryString = HiiGetString (HiiHandle, STRING_TOKEN (STR_MEMORIES_STRING), Lang);
    } else {
      MemoryString = HiiGetString (HiiHandle, STRING_TOKEN (STR_MEMORY_STRING), Lang);
    }

    UnicodeSPrint (
      TempString,
      MAX_STR_SIZE,
      L" %2d %s%s",
      PrivateData->NumberOfInstallMemory,
      MemoryString,
      MemoryInstalledString
      );
    HiiSetString (HiiHandle, STRING_TOKEN (STR_NUMBER_OF_MEMORY_INSTALLED_STRING), TempString, Lang);
    FreePool (MemoryString);

    //
    // Update Total Not Install Memory Number
    //
    if (PrivateData->NumberOfUnInstallMemory > 1) {
      MemoryString = HiiGetString (HiiHandle, STRING_TOKEN (STR_MEMORIES_STRING), Lang);
    } else {
      MemoryString = HiiGetString (HiiHandle, STRING_TOKEN (STR_MEMORY_STRING), Lang);
    }

    UnicodeSPrint (
      TempString,
      MAX_STR_SIZE,
      L" %2d %s%s",
      PrivateData->NumberOfUnInstallMemory,
      MemoryString,
      MemoryNotInstalledString
      );
    HiiSetString (HiiHandle, STRING_TOKEN (STR_NUMBER_OF_MEMORY_UNINSTALL_STRING), TempString, Lang);

    FreePool (MemoryString);
    FreePool (MemoryInstalledString);
    FreePool (MemoryNotInstalledString);
  }

  return EFI_SUCCESS;
}

/**
  The function for update ecc type info

  @param[in]  PrivateData               Points to the MEMORY_INFO_PRIVATE_DATA.
  @param[in]  TotalLanguageCount        Number Of Support Language
  @param[in]  LanguageString            Pointer Of Support Language String

  @retval EFI_SUCCESS                   Update ecc type info success
  @retval other                         Update ecc type info fail
**/
EFI_STATUS
UpdateEccTypeInfo (
  IN  MEMORY_INFO_PRIVATE_DATA          *PrivateData,
  IN  UINTN                             TotalLanguageCount,
  IN  CHAR8                             *LanguageString
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  CHAR16                                *String;
  UINTN                                 LanguageCount;
  CHAR8                                 *Lang;

  Status = EFI_UNSUPPORTED;
  HiiHandle = PrivateData->HiiHandle;

  //
  // Update Ecc Type
  //
  for (LanguageCount = 0; LanguageCount < TotalLanguageCount; LanguageCount++) {
    Lang = &LanguageString[LanguageCount * RFC_3066_ENTRY_SIZE];
    Status = mMemoryDeviceInfoProtocol->GetEccTypeString (mMemoryDeviceInfoProtocol, &String, Lang);
    if (EFI_ERROR (Status)) {
      continue;
    }

    HiiSetString (HiiHandle, STRING_TOKEN (STR_ECC_TYPE_STRING), String, Lang);
    FreePool (String);
  }

  return Status;
}

/**
  The function for update total memory size info

  @param[in]  PrivateData               Points to the MEMORY_INFO_PRIVATE_DATA.

  @retval EFI_SUCCESS                   Update total memory size info success
  @retval other                         Update total memory size info fail
**/
EFI_STATUS
UpdateTotalMemorySizeInfo (
  IN  MEMORY_INFO_PRIVATE_DATA          *PrivateData
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  CHAR16                                *String;
  UINT64                                TotalMemorySize;

  HiiHandle = PrivateData->HiiHandle;

  //
  // Update Memory Size
  //
  String = NULL;
  TotalMemorySize = 0;
  Status = mMemoryDeviceInfoProtocol->GetTotalMemorySize (mMemoryDeviceInfoProtocol, &TotalMemorySize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  String = TotalMemorySizeStringFormat (TotalMemorySize);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_TOTAL_MEMORY_SIZE_STRING), String, NULL);
  if (String != NULL) {
    FreePool (String);
  }

  return Status;
}

/**
  The function for create memory goto item

  @param[in]  PrivateData               Points to the MEMORY_INFO_PRIVATE_DATA.
  @param[in]  StartOpCodeHandle         The IFR lable
  @param[in]  TotalLanguageCount        Number Of Support Language
  @param[in]  LanguageString            Pointer Of Support Language String

  @retval EFI_SUCCESS                   Create memory goto item success
  @retval other                         Create memory goto item fail
**/
EFI_STATUS
CreateMemoryGotoItem (
  IN  MEMORY_INFO_PRIVATE_DATA          *PrivateData,
  IN  VOID						        *StartOpCodeHandle,
  IN  UINTN                             TotalLanguageCount,
  IN  CHAR8                             *LanguageString
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  UINTN                                 Index;
  CHAR16                                TempString[MAX_STR_SIZE] = {0};
  CHAR16                                *DeviceLocatorString;
  CHAR16                                *MemoryConnectString;
  CHAR16                                *MemoryInstalledString;
  CHAR16                                *MemoryNotInstalledString;
  EFI_FORM_ID                           MemoryFormId;
  EFI_QUESTION_ID                       MemoryQuestionId;
  EFI_STRING_ID                         MemoryPromptStrId;
  BOOLEAN                               MemoryInstalled;
  UINTN                                 LanguageCount;
  CHAR8                                 *Lang;

  Status = EFI_SUCCESS;
  HiiHandle = PrivateData->HiiHandle;

  for (Index = 0; Index < PrivateData->NumberOfMemoryInfo; Index++) {
    MemoryPromptStrId = 0;
    MemoryInstalled = FALSE;
    Status = mMemoryDeviceInfoProtocol->GetDeviceLocatorString (
                                          mMemoryDeviceInfoProtocol,
                                          Index,
                                          &DeviceLocatorString
                                          );
    if (EFI_ERROR (Status)) {
      continue;
    }

    for (LanguageCount = 0; LanguageCount < TotalLanguageCount; LanguageCount++) {
      Lang = &LanguageString[LanguageCount * RFC_3066_ENTRY_SIZE];
      MemoryInstalledString    = HiiGetString (HiiHandle, STRING_TOKEN (STR_INSTALLED_STRING), Lang);
      MemoryNotInstalledString = HiiGetString (HiiHandle, STRING_TOKEN (STR_NOT_INSTALLED_STRING), Lang);

      if (mMemoryDeviceInfoProtocol->GetMemoryInstallStatus (mMemoryDeviceInfoProtocol, Index)) {
        MemoryConnectString = MemoryInstalledString;
        MemoryInstalled = TRUE;
      } else {
        MemoryConnectString = MemoryNotInstalledString;
        MemoryInstalled = FALSE;
      }

      UnicodeSPrint (
        TempString,
        MAX_STR_SIZE,
        L"%s [%s]",
        DeviceLocatorString,
        MemoryConnectString
        );
      MemoryPromptStrId = HiiSetString (HiiHandle, MemoryPromptStrId, TempString, Lang);
      FreePool (MemoryInstalledString);
      FreePool (MemoryNotInstalledString);
    }

    FreePool (DeviceLocatorString);

    MemoryQuestionId = (EFI_QUESTION_ID)(H2O_MEMORY_INFO_DYNAMIC_DIMM_BASE_QUESTION_ID + Index);

    if (!MemoryInstalled) {
      HiiCreateSuppressIfOpCodeEx (StartOpCodeHandle, 1);
      HiiCreateIdEqualOpCodeEx (StartOpCodeHandle, H2O_MEMORY_INFO_HIDDEN_UNINSTALL_MEMORY_QUESTION_ID, TRUE);
    }
    //
    // Add One Memory Item.
    //
    MemoryFormId = H2O_MEMORY_INFO_DIMM_INFO_FORM_ID;
    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      MemoryFormId,
      MemoryPromptStrId,
      STRING_TOKEN (STR_MEMORY_DETAIL_INFO_FORMSET_HELP),
      EFI_IFR_FLAG_CALLBACK,
      MemoryQuestionId
      );

    if (!MemoryInstalled) {
      HiiCreateEndOpCode (StartOpCodeHandle);
    }
  }

  return Status;
}

/**
  Update the form data in the memory information form

  @param[in]   PrivateData                    Points to the MEMORY_INFO_PRIVATE_DATA.
  @param[in]   TotalLanguageCount             Number Of Support Language
  @param[in]   LanguageString                 Pointer Of Support Language String

  @retval EFI_SUCCESS                         Update form data success
  @retval other                               Update form data fail
**/
EFI_STATUS
InitMemoryInfoSetup (
  IN  MEMORY_INFO_PRIVATE_DATA                *PrivateData,
  IN  UINTN                                   TotalLanguageCount,
  IN  CHAR8                                   *LanguageString
  )
{
  EFI_STATUS                                  Status;
  EFI_HII_HANDLE                              HiiHandle;
  VOID                                        *StartOpCodeHandle;
  VOID                                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                          *StartLabel;
  EFI_IFR_GUID_LABEL                          *EndLabel;

  Status = EFI_SUCCESS;
  HiiHandle = PrivateData->HiiHandle;

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
  StartLabel->Number       = H2O_MEMORY_INFO_LABEL_START;

  EndLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = H2O_MEMORY_INFO_LABEL_END;

  Status = gBS->LocateProtocol (
                  &gH2OMemoryDeviceInfoProtocolGuid,
                  NULL,
                  (VOID **) &mMemoryDeviceInfoProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to locate protocol gH2OMemoryDeviceInfoProtocolGuid(%g), Status: %r\n",
      __FUNCTION__,
      gH2OMemoryDeviceInfoProtocolGuid,
      Status
      ));
    goto Exit;
  }

  Status = mMemoryDeviceInfoProtocol->GetAllMemoryDeviceInfo (
                                        mMemoryDeviceInfoProtocol,
                                        &PrivateData->NumberOfMemoryInfo,
                                        &PrivateData->NumberOfInstallMemory,
                                        &PrivateData->NumberOfUnInstallMemory,
                                        NULL
                                        );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Update Memory Install Status
  //
  UpdateMemoryInstallStatus (PrivateData, TotalLanguageCount, LanguageString);

  //
  // Update Ecc Type
  //
  UpdateEccTypeInfo (PrivateData, TotalLanguageCount, LanguageString);

  //
  // Update Total Memory Size
  //
  UpdateTotalMemorySizeInfo (PrivateData);

  //
  // Create Memory Goto Item
  //
  CreateMemoryGotoItem (PrivateData, StartOpCodeHandle, TotalLanguageCount, LanguageString);

  Status = HiiUpdateForm (
             HiiHandle,
             &gH2OMemoryInfoFormsetGuid,
             H2O_MEMORY_INFO_DIMM_LIST_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

Exit:
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return Status;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.


  @param This                                Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Request                             A null-terminated Unicode string in <ConfigRequest> format.
  @param Progress                            On return, points to a character in the Request string.
                                             Points to the string's null terminator if request was successful.
                                             Points to the most recent '&' before the first failing name/value
                                             pair (or the beginning of the string if the failure is in the
                                             first name/value pair) if the request was not successful.
  @param Results                             A null-terminated Unicode string in <ConfigAltResp> format which
                                             has all values filled in for the names in the Request string.
                                             String to be allocated by the called function.

  @retval  EFI_SUCCESS                       The Results is filled with the requested values.
  @retval  EFI_OUT_OF_RESOURCES              Not enough memory to store the results.
  @retval  EFI_INVALID_PARAMETER             Request is illegal syntax, or unknown name.
  @retval  EFI_NOT_FOUND                     Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
MemoryInfoFormExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      BufferSize;
  MEMORY_INFO_PRIVATE_DATA                   *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL            *HiiConfigRouting;
  EFI_STRING                                 ConfigRequest;
  EFI_STRING                                 ConfigRequestHdr;
  UINTN                                      Size;
  BOOLEAN                                    AllocatedRequest;
  CHAR16                                     *StrPointer;

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

  PrivateData = MEMORY_INFO_PRIVATE_DATA_FROM_THIS (This);
  HiiConfigRouting = PrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (MEMORY_INFO_SETUP);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OMemoryInfoSetupVarstoreGuid,
             &BufferSize,
             &PrivateData->Configuration
             );
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
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OMemoryInfoSetupVarstoreGuid, mVariableName, PrivateData->SeviceHandle);
    if (ConfigRequestHdr == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto ErrorExit;
    }
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto ErrorExit;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &gH2OMemoryInfoSetupVarstoreGuid, NULL)) {
      return EFI_NOT_FOUND;
    }
    //
    // Check whether request for EFI Varstore. EFI varstore get data
    // through hii database, not support in this path.
    //
    if (!HiiIsConfigHdrMatch (Request, &gH2OMemoryInfoSetupVarstoreGuid, mVariableName)) {
      return EFI_NOT_FOUND;
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
        if (ConfigRequest == NULL) {
          Status = EFI_OUT_OF_RESOURCES;
          goto ErrorExit;
        }
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64)BufferSize);
      }
    }
  }

  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *)&PrivateData->Configuration,
                               sizeof (MEMORY_INFO_SETUP),
                               Results,
                               Progress
                               );
  if (!EFI_ERROR (Status)) {
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OMemoryInfoSetupVarstoreGuid, mVariableName, PrivateData->SeviceHandle);
  }

ErrorExit:
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


  @param This                                Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Configuration                       A null-terminated Unicode string in <ConfigResp> format.
  @param Progress                            A pointer to a string filled in with the offset of the most
                                             recent '&' before the first failing name/value pair (or the
                                             beginning of the string if the failure is in the first
                                             name/value pair) or the terminating NULL if all was successful.

  @retval  EFI_SUCCESS                       The Results is processed successfully.
  @retval  EFI_INVALID_PARAMETER             Configuration is NULL.
  @retval  EFI_NOT_FOUND                     Routing data doesn't match any storage in this driver.

**/
EFI_STATUS
EFIAPI
MemoryInfoFormRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                                 Status;
  MEMORY_INFO_PRIVATE_DATA                   *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL            *HiiConfigRouting;
  UINTN                                      BufferSize;

  if (This == NULL || Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = MEMORY_INFO_PRIVATE_DATA_FROM_THIS (This);
  HiiConfigRouting = PrivateData->HiiConfigRouting;
  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch(Configuration, &gH2OMemoryInfoSetupVarstoreGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  BufferSize = sizeof (MEMORY_INFO_SETUP);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OMemoryInfoSetupVarstoreGuid,
             &BufferSize,
             &PrivateData->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  BufferSize = sizeof (MEMORY_INFO_SETUP);
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
  // Store broswer data Buffer Storage to EFI variable
  //
  Status = CommonSetVariable (
             mVariableName,
             &gH2OMemoryInfoSetupVarstoreGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             sizeof (MEMORY_INFO_SETUP),
             &PrivateData->Configuration
             );

  return Status;
}

/**
  This function processes the results of changes in configuration.


  @param This                                Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action                              Specifies the type of action taken by the browser.
  @param QuestionId                          A unique value which is sent to the original exporting driver
                                             so that it can identify the type of data to expect.
  @param Type                                The type of value for the question.
  @param Value                               A pointer to the data being sent to the original exporting driver.
  @param ActionRequest                       On return, points to the action requested by the callback function.

  @retval  EFI_SUCCESS                       The callback successfully handled the action.
  @retval  EFI_OUT_OF_RESOURCES              Not enough storage is available to hold the variable and its data.
  @retval  EFI_DEVICE_ERROR                  The variable could not be saved.
  @retval  EFI_UNSUPPORTED                   The specified Action is not supported by the callback.

**/
EFI_STATUS
EFIAPI
MemoryInfoFormCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                                 Status;
  MEMORY_INFO_PRIVATE_DATA                   *PrivateData;
  EFI_HII_HANDLE                             HiiHandle;
  UINTN                                      DisplayListIndex;
  UINTN                                      MemoryInfoIndex;
  CHAR16                                     TempString[MAX_STR_SIZE] = {0};
  CHAR16                                     *DeviceLocatorString;
  CHAR16                                     *MemoryConnectString;

  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }

  if ((Value == NULL) || (ActionRequest == NULL) || (mMemoryDeviceInfoProtocol == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = MEMORY_INFO_PRIVATE_DATA_FROM_THIS (This);
  HiiHandle = PrivateData->HiiHandle;

  if ((QuestionId < H2O_MEMORY_INFO_DYNAMIC_DIMM_BASE_QUESTION_ID) ||
      (QuestionId >= H2O_MEMORY_INFO_DYNAMIC_DIMM_BASE_QUESTION_ID + PrivateData->NumberOfMemoryInfo)) {
    return EFI_INVALID_PARAMETER;
  }

  MemoryInfoIndex = QuestionId - H2O_MEMORY_INFO_DYNAMIC_DIMM_BASE_QUESTION_ID;

  Status = mMemoryDeviceInfoProtocol->GetDeviceLocatorString (
                                        mMemoryDeviceInfoProtocol,
                                        MemoryInfoIndex,
                                        &DeviceLocatorString
                                        );
  if (!EFI_ERROR (Status)) {
    if (mMemoryDeviceInfoProtocol->GetMemoryInstallStatus (mMemoryDeviceInfoProtocol, MemoryInfoIndex)) {
      MemoryConnectString = HiiGetString (HiiHandle, STRING_TOKEN (STR_INSTALLED_STRING), NULL);
    } else {
      MemoryConnectString = HiiGetString (HiiHandle, STRING_TOKEN (STR_NOT_INSTALLED_STRING), NULL);
    }

    UnicodeSPrint (
      TempString,
      MAX_STR_SIZE,
      L"%s [%s]",
      DeviceLocatorString,
      MemoryConnectString
      );
    FreePool (DeviceLocatorString);
    FreePool (MemoryConnectString);

    HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_LOCATOR_TITLE), TempString, NULL);
  }

  for (DisplayListIndex = 0; DisplayListIndex < MEMORY_DETAIL_ITEM_MAXIMUM; DisplayListIndex++) {
    if (mMemoryDetailDisplayList[DisplayListIndex].ItemDisplay) {
      mMemoryDetailDisplayList[DisplayListIndex].DisplayItemFunction (HiiHandle, MemoryInfoIndex);
    }
  }

  return EFI_SUCCESS;
}


/**
  Free the private data if error occurs.

  @retval
  @return

**/
VOID
EFIAPI
FreePrivateData (
  )
{
  if (mMemoryInfoData == NULL) {
    return;
  }

  FreePool (mMemoryInfoData);

  mMemoryInfoData = NULL;
}

/**
  Unloads this driver and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
MemoryInfoUnload (
  IN EFI_HANDLE                   ImageHandle
  )
{
  if (mMemoryInfoData == NULL) {
    return EFI_NOT_READY;
  }

  if (mMemoryInfoData->SeviceHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mMemoryInfoData->SeviceHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiMemoryInfoVendorDevPath,
           &gEfiHiiConfigAccessProtocolGuid,
           &mMemoryInfoData->ConfigAccess,
           NULL
           );
    mMemoryInfoData->SeviceHandle = NULL;
  }

  if (mMemoryInfoData->HiiHandle != NULL) {
    HiiRemovePackages (mMemoryInfoData->HiiHandle);
  }

  FreePrivateData();

  return EFI_SUCCESS;
}

/**
  To collect data and initialize the Memory info formset after entering setup utility application

  @param[in]  Event                   Event
  @param[in]  Context                 Context

  No return value.
**/
VOID
MemorySetupCallback (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
  )
{
  EFI_STATUS                               Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL   *SetupUtilityApp;
  UINTN                                    DisplayListIndex;
  UINTN                                    TotalLanguageCount;
  CHAR8                                    *LanguageString;

  TotalLanguageCount = 0;
  LanguageString = NULL;
  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL,
                  (VOID **) &SetupUtilityApp
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    //
    // Get all supported languages
    //
    GetSupportLang (&TotalLanguageCount, (UINT8**)&LanguageString);
    if (TotalLanguageCount == 0 || LanguageString == NULL) {
      return;
    }
    Status = InitMemoryInfoSetup (mMemoryInfoData, TotalLanguageCount, LanguageString);
    Status = InitMemoryDetailInfoSetup (mMemoryInfoData);
  } else if (SetupUtilityApp->VfrDriverState == ShutdownSetupUtility) {
    for (DisplayListIndex = 0; DisplayListIndex < MEMORY_DETAIL_ITEM_MAXIMUM; DisplayListIndex++) {
      mMemoryDetailDisplayList[DisplayListIndex].ItemDisplay = FALSE;
    }
  }

  return;
}

/**
  The entry point for the driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
MemoryInfoEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Registration;
  EFI_HII_HANDLE                        HiiHandle;
  UINTN                                 BufferSize;
  EFI_STRING                            ConfigRequestHdr;

  //
  // Initialize driver private data
  //
  mMemoryInfoData = AllocateZeroPool (sizeof (MEMORY_INFO_PRIVATE_DATA));

  if (mMemoryInfoData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mMemoryInfoData->Signature = MEMORY_INFO_PRIVATE_SIGNATURE;
  mMemoryInfoData->ConfigAccess.ExtractConfig = MemoryInfoFormExtractConfig;
  mMemoryInfoData->ConfigAccess.RouteConfig   = MemoryInfoFormRouteConfig;
  mMemoryInfoData->ConfigAccess.Callback      = MemoryInfoFormCallback;

  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID**)&mMemoryInfoData->HiiConfigRouting
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mMemoryInfoData->SeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiMemoryInfoVendorDevPath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mMemoryInfoData->ConfigAccess,
                  NULL
                  );

  if (EFI_ERROR(Status)) {
    FreePrivateData ();
    return Status;
  }

  //
  // Publish our HII data
  //
  HiiHandle = HiiAddPackages (
                &gH2OMemoryInfoFormsetGuid,
                mMemoryInfoData->SeviceHandle,
                MemoryInfoSetupUtilityDxeStrings,
                MemoryInfoVfrBin,
                NULL
                );
  if (HiiHandle == NULL) {
    FreePrivateData ();
    return EFI_OUT_OF_RESOURCES;
  }

  mMemoryInfoData->HiiHandle = HiiHandle;

  //
  // Check the status of the setup variable. Note that it has a read-only
  // default value stored in the variable defaults region.
  //
  BufferSize = sizeof (MEMORY_INFO_SETUP);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OMemoryInfoSetupVarstoreGuid,
             &BufferSize,
             &mMemoryInfoData->Configuration
             );
  if (EFI_ERROR (Status) ||
      !IsVariableInVariableStoreRegion (mVariableName, &gH2OMemoryInfoSetupVarstoreGuid)) {
    BufferSize = sizeof (MEMORY_INFO_SETUP);
    Status = InitVarDefault (
               mVariableName,
               &gH2OMemoryInfoSetupVarstoreGuid,
               &BufferSize,
               &mMemoryInfoData->Configuration
               );
    if (EFI_ERROR (Status)) {
      MemoryInfoUnload (ImageHandle);
      return Status;
    }
  }

  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gH2OMemoryInfoSetupVarstoreGuid,
                       mVariableName,
                       mMemoryInfoData->SeviceHandle
                       );
  if (ConfigRequestHdr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiValidateSettings (ConfigRequestHdr)) {
    FreePool (ConfigRequestHdr);
    MemoryInfoUnload (ImageHandle);
    return EFI_INVALID_PARAMETER;
  }

  FreePool (ConfigRequestHdr);

  //
  // When execute Setup Utility application, install HII data
  //
  Status = EfiNamedEventListen (
             &gEfiSetupUtilityApplicationProtocolGuid,
             TPL_CALLBACK - 1,
             MemorySetupCallback,
             NULL,
             &Registration
             );

  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Fail to register MemorySetupCallback on gEfiSetupUtilityApplicationProtocolGuid, Status:%r\n", Status));
  }

  return EFI_SUCCESS;
}

