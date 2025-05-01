/** @file
  This gets all information and implements all functions that will be used in
  Setup Utility.

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <SDEmmcInfoSetupUtilityDxe.h>

SD_EMMC_SETUP_UTILITY_PRIVATE_DATA  *mSetupPrivate                = NULL;
EFI_HANDLE                          mSDEmmcInfoHiiHandle          = NULL;
EFI_STRING_ID                       *mSDEmmcDeviceStringId        = NULL;
EFI_STRING_ID                       *mSDEmmcDeviceHelpStringId    = NULL;
EFI_STRING_ID                       *mSDEmmcPortNameStringId      = NULL;
EFI_STRING_ID                       *mSDEmmcPortNameHelpStringId  = NULL;
EFI_EVENT                           mSDEmmcRefreshEvent           = NULL;
CHAR16                              *mVariableName                = SD_EMMC_INFO_SETUP_VARSTORE_NAME;
BOOLEAN                             mRefreshFlag                  = FALSE;

UINTN                               mSDEmmcDeviceCount = 0;
SD_EMMC_INFO                        *mSDEmmcDeviceInfo = NULL;

UINTN                               mOldSDEmmcDeviceCount = 0;
SD_EMMC_INFO                        *mOldSDEmmcDeviceInfo = NULL;

CHAR16                              *mChangingDeviceName = NULL;
CHANGING_TYPE                       mChangingType = Unsupport;

UINTN                               mOemPortNameCount = 0;
OEM_PORT_NAME_INFO                  *mOemPortNameInfo = NULL;

UINTN                               mBufferLargestSizeEver = 0;

HII_VENDOR_DEVICE_PATH  mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SD_EMMC_INFO_SETUP_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

SD_EMMC_INFO_PARSER_TABLE mSDEmmcInfoParserTable[] = {
  { STRING_TOKEN (STR_SD_EMMC_DRIVENAME_PROMPT),    STRING_TOKEN (STR_SD_EMMC_DRIVENAME_HELP),    SDEmmcDeviceNameStr            },
  { STRING_TOKEN (STR_SD_EMMC_SERIAL_NUM_PROMPT),   STRING_TOKEN (STR_SD_EMMC_SERIAL_NUM_HELP),   SDEmmcDeviceSerialNumStr       },
  { STRING_TOKEN (STR_SD_EMMC_REVISION_PROMPT),     STRING_TOKEN (STR_SD_EMMC_REVISION_HELP),     SDEmmcDeviceRevisionStr        },
  { STRING_TOKEN (STR_SD_EMMC_OEM_APP_ID_PROMPT),   STRING_TOKEN (STR_SD_EMMC_OEM_APP_ID_HELP),   SDEmmcDeviceOemAppIdStr        },
  { STRING_TOKEN (STR_SD_EMMC_MANU_ID_PROMPT),      STRING_TOKEN (STR_SD_EMMC_MANU_ID_HELP),      SDEmmcDeviceManufactureIdStr   },
  { STRING_TOKEN (STR_SD_EMMC_MANU_DATE_PROMPT),    STRING_TOKEN (STR_SD_EMMC_MANU_DATE_HELP),    SDEmmcDeviceManufactureDateStr },
  { STRING_TOKEN (STR_SD_EMMC_CARD_TYPE_PROMPT),    STRING_TOKEN (STR_SD_EMMC_CARD_TYPE_HELP),    SDEmmcDeviceCardTypeStr        },
  { STRING_TOKEN (STR_SD_EMMC_CAPACITY_PROMPT),     STRING_TOKEN (STR_SD_EMMC_CAPACITY_HELP),     SDEmmcDeviceCapacityStr        },
  { STRING_TOKEN (STR_SD_EMMC_BLOCK_COUNT_PROMPT),  STRING_TOKEN (STR_SD_EMMC_BLOCK_COUNT_HELP),  SDEmmcDeviceBlockNumStr        },
  { STRING_TOKEN (STR_SD_EMMC_BLOCK_SIZE_PROMPT),   STRING_TOKEN (STR_SD_EMMC_BLOCK_SIZE_HELP),   SDEmmcDeviceBlockSizeStr       },
  { 0,                                              0,                                            NULL                           }
};

UINTN  mSDEmmcInfoParserTableSize = ARRAY_SIZE (mSDEmmcInfoParserTable);

EFI_STRING_ID mSDSubInfoPromptStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)]   = {0};
EFI_STRING_ID mSDSubInfoValueStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)]    = {0};
EFI_STRING_ID mSDSubInfoHelpStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)]     = {0};
EFI_STRING_ID mEmmcSubInfoPromptStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)] = {0};
EFI_STRING_ID mEmmcSubInfoValueStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)]  = {0};
EFI_STRING_ID mEmmcSubInfoHelpStringId[ARRAY_SIZE (mSDEmmcInfoParserTable)]   = {0};

/**
  Compare two device pathes to check if they are exactly same.

  @param DevicePath1    A pointer to the first device path data structure.
  @param DevicePath2    A pointer to the second device path data structure.

  @retval TRUE    They are same.
  @retval FALSE   They are not same.

**/
BOOLEAN
EfiCompareDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath1,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath2
  )
{
  UINTN Size1;
  UINTN Size2;

  Size1 = GetDevicePathSize (DevicePath1);
  Size2 = GetDevicePathSize (DevicePath2);

  if (Size1 != Size2) {
    return FALSE;
  }

  if (CompareMem (DevicePath1, DevicePath2, Size1) != 0) {
    return FALSE;
  }

  return TRUE;
}

/**

  This function update SD/eMMC goto opcode under H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_START label by card type.

  @param[in] VOID

  @retval VOID

**/
VOID
EFIAPI
SDEmmcLabelInitByType (
  VOID
)
{
  VOID                           *StartOpCodeHandle;
  VOID                           *EndOpCodeHandle;
  UINT16                         FormSetId;
  EFI_IFR_GUID_LABEL             *StartLabel;
  EFI_IFR_GUID_LABEL             *EndLabel;
  UINTN                          SDEmmcCount;
  SD_EMMC_INFO                   *SDEmmcInfo;
  UINT16                         Index;
  EFI_STATUS                     Status;
  CHAR16                         *NameString;
  CHAR16                         *HelpString;
  BOOLEAN                        NoDevice;

  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));

  NameString = NULL;
  HelpString = NULL;
  FormSetId  = H2O_SD_EMMC_INFO_LIST_FORM_ID;
  NoDevice   = TRUE;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      StartOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      EndOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_END;

  SDEmmcInfo  = mSDEmmcDeviceInfo;
  SDEmmcCount = mSDEmmcDeviceCount;

  HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_SD_EMMC_EMMC), 0, 0, 0);
  for (Index = 0; Index < SDEmmcCount; Index++) {
    // Emmc
    if (SDEmmcInfo[Index].CardInfo->CardType != MMC_CARD) {
      continue;
    }

    NoDevice   = FALSE;
    NameString = SDEmmcDeviceNameStr (&SDEmmcInfo[Index]);
    HelpString = ConvertDevicePathToText (SDEmmcInfo[Index].DevicePath, FALSE, FALSE);

    mSDEmmcDeviceStringId[Index]      = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcDeviceStringId[Index], NameString, NULL);
    mSDEmmcDeviceHelpStringId[Index]  = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcDeviceHelpStringId[Index], HelpString, NULL);

    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_FORM_ID,
      mSDEmmcDeviceStringId[Index],
      mSDEmmcDeviceHelpStringId[Index],
      EFI_IFR_FLAG_CALLBACK,
      (EFI_QUESTION_ID)(H20_SD_EMMC_INFO_DYNAMIC_EMMC_DEVICE_BASE_QUESTION_ID + Index)
      );

    FreePool (NameString);
    FreePool (HelpString);
  }
  if (NoDevice) {
    HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_NO_DEVICE), 0, 0, 0);
  }
  NoDevice = TRUE;

  HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_SD_EMMC_SD), 0, 0, 0);
  for (Index = 0; Index < SDEmmcCount; Index++) {
    // SD
    if (SDEmmcInfo[Index].CardInfo->CardType == MMC_CARD) {
      continue;
    }

    NoDevice   = FALSE;
    NameString = SDEmmcDeviceNameStr (&SDEmmcInfo[Index]);
    HelpString = ConvertDevicePathToText (SDEmmcInfo[Index].DevicePath, FALSE, FALSE);

    mSDEmmcDeviceStringId[Index]      = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcDeviceStringId[Index], NameString, NULL);
    mSDEmmcDeviceHelpStringId[Index]  = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcDeviceHelpStringId[Index], HelpString, NULL);

    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID,
      mSDEmmcDeviceStringId[Index],
      mSDEmmcDeviceHelpStringId[Index],
      EFI_IFR_FLAG_CALLBACK,
      (EFI_QUESTION_ID)(H20_SD_EMMC_INFO_DYNAMIC_SD_DEVICE_BASE_QUESTION_ID + Index)
      );

    if (NameString != NULL) {
      FreePool (NameString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }
  if (NoDevice) {
    HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_NO_DEVICE), 0, 0, 0);
  }

  Status = HiiUpdateForm (
             mSDEmmcInfoHiiHandle,             // HII handle
             &gH2OSDEmmcInfoSetupFormsetGuid,  // Formset GUID
             FormSetId,                        // Form ID
             StartOpCodeHandle,                // Label for where to insert opcodes
             EndOpCodeHandle                   // Replace data
             );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**

  This function update SD/eMMC goto opcode under H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_START label by OEM port name.

  @param[in] VOID

  @retval VOID

**/
VOID
EFIAPI
SDEmmcLabelInitByOemPort (
  VOID
)
{
  VOID                           *StartOpCodeHandle;
  VOID                           *EndOpCodeHandle;
  UINT16                         FormSetId;
  EFI_IFR_GUID_LABEL             *StartLabel;
  EFI_IFR_GUID_LABEL             *EndLabel;
  UINTN                          SDEmmcCount;
  SD_EMMC_INFO                   *SDEmmcInfo;
  UINTN                          OemPortNameCount;
  OEM_PORT_NAME_INFO             *OemPortNameInfo;
  EFI_FORM_ID                    FormId;
  EFI_QUESTION_ID                QuestionIdBase;
  UINT16                         Index;
  UINT16                         Index1;
  UINT16                         InfoIndex;
  EFI_STATUS                     Status;
  CHAR16                         *NameString;
  CHAR16                         *HelpString;
  CHAR16                         OemPortString[MAX_DATA_STRING] = {0};

  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));


  FormSetId   = H2O_SD_EMMC_INFO_LIST_FORM_ID;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      StartOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      EndOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = H2O_SD_EMMC_INFO_DEVICE_LIST_LABEL_END;

  SDEmmcInfo  = mSDEmmcDeviceInfo;
  SDEmmcCount = mSDEmmcDeviceCount;

  OemPortNameInfo   = mOemPortNameInfo;
  OemPortNameCount  = mOemPortNameCount;

  for (Index = 0; Index < OemPortNameCount; Index++) {
    NameString      = NULL;
    HelpString      = NULL;
    InfoIndex       = Index;
    FormId          = H2O_SD_EMMC_INFO_LIST_FORM_ID; // Current FormId
    QuestionIdBase  = H2O_SD_EMMC_INFO_UNSUPPORTED_QUESTION_ID;
    DEBUG((EFI_D_INFO, "%a (), Pcd Device Path: %s\n", __FUNCTION__, ConvertDevicePathToText(OemPortNameInfo[Index].DevicePath, FALSE, FALSE)));
    HelpString = ConvertDevicePathToText (OemPortNameInfo[Index].DevicePath, FALSE, FALSE);
    mSDEmmcPortNameHelpStringId[Index]  = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcPortNameHelpStringId[Index], HelpString, NULL);

    for (Index1 = 0; Index1 < SDEmmcCount; Index1++) {

      if (!EfiCompareDevicePath (OemPortNameInfo[Index].DevicePath, SDEmmcInfo[Index1].DevicePath)) {
        continue;
      }

      NameString = SDEmmcDeviceNameStr (&SDEmmcInfo[Index1]);
      UnicodeSPrint (
        OemPortString,
        MAX_DATA_STRING,
        L"%10s - [%6s]",
        OemPortNameInfo[Index].PortName,
        NameString
        );

      mSDEmmcPortNameStringId[Index]      = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcPortNameStringId[Index], OemPortString, NULL);

      switch (SDEmmcInfo[Index1].CardInfo->CardType) {
        case MMC_CARD:
          FormId          = H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_FORM_ID;
          QuestionIdBase  = H20_SD_EMMC_INFO_DYNAMIC_EMMC_DEVICE_BASE_QUESTION_ID;
          break;

        case SD_CARD:
        case SD_CARD_2:
        case SD_CARD_2_HIGH:
          FormId          = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID;
          QuestionIdBase  = H20_SD_EMMC_INFO_DYNAMIC_SD_DEVICE_BASE_QUESTION_ID;
          break;

        default:
          FormId          = H2O_SD_EMMC_INFO_LIST_FORM_ID; // Current FormId
          QuestionIdBase  = 0;
          DEBUG ((EFI_D_INFO, "CardType Unsupport.\n"));
          break;
      }
      InfoIndex = Index1;
      break; // break to keep index1
    }

    if ((NameString == NULL) && (Index1 == SDEmmcCount)) {
      UnicodeSPrint (
        OemPortString,
        MAX_DATA_STRING,
        L"%10s - [%13s]",
        OemPortNameInfo[Index].PortName,
        L"Not Installed"
        );

      mSDEmmcPortNameStringId[Index]     = HiiSetString (mSDEmmcInfoHiiHandle, mSDEmmcPortNameStringId[Index], OemPortString, NULL);
    }

    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      FormId,
      mSDEmmcPortNameStringId[Index],
      mSDEmmcPortNameHelpStringId[Index],
      EFI_IFR_FLAG_CALLBACK,
      (EFI_QUESTION_ID)(QuestionIdBase + InfoIndex)
      );

    if (NameString != NULL) {
      FreePool (NameString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  Status = HiiUpdateForm (
             mSDEmmcInfoHiiHandle,             // HII handle
             &gH2OSDEmmcInfoSetupFormsetGuid,  // Formset GUID
             FormSetId,                        // Form ID
             StartOpCodeHandle,                // Label for where to insert opcodes
             EndOpCodeHandle                   // Replace data
             );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**

  This function initialize SD/eMMC infomation root form.

  @param[in] VOID

  @retval VOID

**/
VOID
EFIAPI
SDEmmcInfoFormInit (
  VOID
  )
{
  switch (mSetupPrivate->Configuration.DisplayMode) {

  case ByCardType:
    SDEmmcLabelInitByType ();
    break;

  case ByOemPortName:
    SDEmmcLabelInitByOemPort ();
    break;

  default:
    DEBUG ((EFI_D_ERROR, "%a() - Invalid display mode: %d\n", __FUNCTION__, mSetupPrivate->Configuration.DisplayMode));
    break;
  }
}


/**
  This function update H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID form information.

  @param[in] QuestionId  A unique value which is sent to the original exporting driver so that it can identify
                         the type of SD/eMMC data to expect.

  @retval VOID

**/
VOID
SDSubFormUpdate (
  IN   EFI_QUESTION_ID        QuestionId
  )
{
  VOID                           *StartOpCodeHandle;
  VOID                           *EndOpCodeHandle;
  UINT16                         FormId;
  EFI_IFR_GUID_LABEL             *StartLabel;
  EFI_IFR_GUID_LABEL             *EndLabel;
  UINTN                          DeviceIndex;
  UINTN                          ParserIndex;
  UINT8                          *InfoSequence;
  UINTN                          InfoSequenceMaximum;
  UINTN                          Index;
  SD_EMMC_INFO                   *SDEmmcInfo;
  EFI_STATUS                     Status;
  EFI_STRING                     TempParserInfo;
  EFI_STRING                     TempPromptStr;
  EFI_STRING                     TempHelpStr;

  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));

  FormId              = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID;
  DeviceIndex         = (UINTN)(QuestionId & H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_SERIAL);
  SDEmmcInfo          = mSDEmmcDeviceInfo;

  if (DeviceIndex >= mSDEmmcDeviceCount){
    DEBUG ((EFI_D_INFO, "%a (), Out of device range, Device Index: %d, Device count: %d.\n", __FUNCTION__, DeviceIndex, mSDEmmcDeviceCount));
    return;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      StartOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      EndOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_END;

  InfoSequence         = (UINT8 *)PcdGetPtr (PcdH2OSDEmmcDetailDisplayList);
  InfoSequenceMaximum  = PcdGetSize (PcdH2OSDEmmcDetailDisplayList);

  for (Index = 0; Index < InfoSequenceMaximum; Index++) {
    for (ParserIndex = 0; ParserIndex < mSDEmmcInfoParserTableSize; ParserIndex++) {

      if ((mSDEmmcInfoParserTable[ParserIndex].PromptStringId == 0) ||
          (mSDEmmcInfoParserTable[ParserIndex].HelpStringId   == 0) ||
          (mSDEmmcInfoParserTable[ParserIndex].InfoParserFunction == NULL)) {
        continue;
      }

      if (ParserIndex != (UINTN)InfoSequence[Index]) {
        continue;
      }

      DEBUG ((EFI_D_ERROR, "ParserIndex: %d\n", ParserIndex));

      TempParserInfo = mSDEmmcInfoParserTable[ParserIndex].InfoParserFunction (&SDEmmcInfo[DeviceIndex]);
      if (TempParserInfo == NULL) {
        continue;
      }
      TempPromptStr = HiiGetString (mSDEmmcInfoHiiHandle, mSDEmmcInfoParserTable[ParserIndex].PromptStringId, NULL);
      TempHelpStr   = HiiGetString (mSDEmmcInfoHiiHandle, mSDEmmcInfoParserTable[ParserIndex].HelpStringId, NULL);

      mSDSubInfoPromptStringId[ParserIndex] = HiiSetString (mSDEmmcInfoHiiHandle, mSDSubInfoPromptStringId[ParserIndex], TempPromptStr, NULL);
      mSDSubInfoValueStringId[ParserIndex]  = HiiSetString (mSDEmmcInfoHiiHandle, mSDSubInfoValueStringId[ParserIndex], TempParserInfo, NULL);
      mSDSubInfoHelpStringId[ParserIndex]   = HiiSetString (mSDEmmcInfoHiiHandle, mSDSubInfoHelpStringId[ParserIndex], TempHelpStr, NULL);

      HiiCreateActionOpCodeEx (
        StartOpCodeHandle,
        (H20_SD_EMMC_INFO_DYNAMIC_SD_INFO_BASE_QUESTION_ID + (UINT16)ParserIndex),  // Question ID
        mSDSubInfoPromptStringId[ParserIndex],
        mSDSubInfoHelpStringId[ParserIndex],
        mSDSubInfoValueStringId[ParserIndex],
        0x00,
        STRING_TOKEN (0x0000)
        );

      FreePool (TempParserInfo);
      FreePool (TempPromptStr);
      FreePool (TempHelpStr);
    }
  }

  Status = HiiUpdateForm (
             mSDEmmcInfoHiiHandle,             // HII handle
             &gH2OSDEmmcInfoSetupFormsetGuid,  // Formset GUID
             FormId,                        // Form ID
             StartOpCodeHandle,                // Label for where to insert opcodes
             EndOpCodeHandle                   // Replace data
             );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function clear H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID form information.

  @param[in] QuestionId  A unique value which is sent to the original exporting driver so that it can identify
                         the type of SD/eMMC data to expect.

  @retval VOID

**/
VOID
SDSubFormClear (
  VOID
  )
{
  VOID                           *StartOpCodeHandle;
  VOID                           *EndOpCodeHandle;
  UINT16                         FormId;
  EFI_IFR_GUID_LABEL             *StartLabel;
  EFI_IFR_GUID_LABEL             *EndLabel;
  EFI_STATUS                     Status;
  H2O_FORM_BROWSER_PROTOCOL      *FormBrowserProtocol;


  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));

  FormId           = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      StartOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      EndOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = H2O_SD_EMMC_INFO_SD_DEVICE_INFO_LABEL_END;

  Status = gBS->LocateProtocol (
                  &gH2OFormBrowserProtocolGuid,
                  NULL,
                  (VOID **)&FormBrowserProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a () FormBrowserProtocol: %r.\n", __FUNCTION__, Status));
    return;
  }

  if (((EFI_FORM_ID)FormBrowserProtocol->CurrentP->PageId) == H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID) {
    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      (EFI_FORM_ID)FormBrowserProtocol->CurrentP->ParentPage->PageId,
      STRING_TOKEN (STR_GO_BACK),
      STRING_TOKEN (STR_GO_BACK_HELP),
      EFI_IFR_FLAG_CALLBACK,
      H2O_SD_EMMC_INFO_UNSUPPORTED_QUESTION_ID
      );
  }

  Status = HiiUpdateForm (
             mSDEmmcInfoHiiHandle,             // HII handle
             &gH2OSDEmmcInfoSetupFormsetGuid,  // Formset GUID
             FormId,                        // Form ID
             StartOpCodeHandle,                // Label for where to insert opcodes
             EndOpCodeHandle                   // Replace data
             );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function update H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_FORM_ID form information.

  @param[in] QuestionId  A unique value which is sent to the original exporting driver so that it can identify
                         the type of SD/eMMC data to expect.

  @retval VOID

**/
VOID
EmmcSubFormUpdate (
  IN   EFI_QUESTION_ID        QuestionId
  )
{
  VOID                           *StartOpCodeHandle;
  VOID                           *EndOpCodeHandle;
  UINT16                         FormId;
  EFI_IFR_GUID_LABEL             *StartLabel;
  EFI_IFR_GUID_LABEL             *EndLabel;
  UINTN                          DeviceIndex;
  UINTN                          ParserIndex;
  UINT8                          *InfoSequence;
  UINTN                          InfoSequenceMaximum;
  UINTN                          Index;
  SD_EMMC_INFO                   *SDEmmcInfo;
  EFI_STATUS                     Status;
  EFI_STRING                     TempParserInfo;
  EFI_STRING                     TempPromptStr;
  EFI_STRING                     TempHelpStr;

  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));

  FormId              = H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_FORM_ID;
  DeviceIndex         = (UINTN)(QuestionId & H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_SERIAL);
  SDEmmcInfo          = mSDEmmcDeviceInfo;

  if (DeviceIndex >= mSDEmmcDeviceCount){
    DEBUG ((EFI_D_INFO, "%a (), Out of device range, Device Index: %d, Device count: %d.\n", __FUNCTION__, DeviceIndex, mSDEmmcDeviceCount));
    return;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      StartOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_LABEL_START;
  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                                      EndOpCodeHandle,
                                                      &gEfiIfrTianoGuid,
                                                      NULL,
                                                      sizeof(EFI_IFR_GUID_LABEL)
                                                      );
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = H2O_SD_EMMC_INFO_EMMC_DEVICE_INFO_LABEL_END;


  InfoSequence         = (UINT8 *)PcdGetPtr (PcdH2OSDEmmcDetailDisplayList);
  InfoSequenceMaximum  = PcdGetSize (PcdH2OSDEmmcDetailDisplayList);

  for (Index = 0; Index < InfoSequenceMaximum; Index++) {
    for (ParserIndex = 0; ParserIndex < mSDEmmcInfoParserTableSize; ParserIndex++) {

      if ((mSDEmmcInfoParserTable[ParserIndex].PromptStringId == 0) ||
          (mSDEmmcInfoParserTable[ParserIndex].HelpStringId   == 0) ||
          (mSDEmmcInfoParserTable[ParserIndex].InfoParserFunction == NULL)) {
        continue;
      }

      if (ParserIndex != (UINTN)InfoSequence[Index]) {
        continue;
      }

      DEBUG ((EFI_D_ERROR, "ParserIndex: %d\n", ParserIndex));

      TempParserInfo = mSDEmmcInfoParserTable[ParserIndex].InfoParserFunction (&SDEmmcInfo[DeviceIndex]);
      if (TempParserInfo == NULL) {
        continue;
      }
      TempPromptStr = HiiGetString (mSDEmmcInfoHiiHandle, mSDEmmcInfoParserTable[ParserIndex].PromptStringId, NULL);
      TempHelpStr   = HiiGetString (mSDEmmcInfoHiiHandle, mSDEmmcInfoParserTable[ParserIndex].HelpStringId, NULL);

      mEmmcSubInfoPromptStringId[ParserIndex] = HiiSetString (mSDEmmcInfoHiiHandle, mEmmcSubInfoPromptStringId[ParserIndex], TempPromptStr, NULL);
      mEmmcSubInfoValueStringId[ParserIndex]  = HiiSetString (mSDEmmcInfoHiiHandle, mEmmcSubInfoValueStringId[ParserIndex], TempParserInfo, NULL);
      mEmmcSubInfoHelpStringId[ParserIndex]   = HiiSetString (mSDEmmcInfoHiiHandle, mEmmcSubInfoHelpStringId[ParserIndex], TempHelpStr, NULL);

      HiiCreateActionOpCodeEx (
        StartOpCodeHandle,
        (H20_SD_EMMC_INFO_DYNAMIC_EMMC_INFO_BASE_QUESTION_ID + (UINT16)ParserIndex),
        mEmmcSubInfoPromptStringId[ParserIndex],
        mEmmcSubInfoHelpStringId[ParserIndex],
        mEmmcSubInfoValueStringId[ParserIndex],
        0x00,
        STRING_TOKEN (0x0000)
        );

      FreePool (TempParserInfo);
      FreePool (TempPromptStr);
      FreePool (TempHelpStr);
    }
  }

  Status = HiiUpdateForm (
             mSDEmmcInfoHiiHandle,             // HII handle
             &gH2OSDEmmcInfoSetupFormsetGuid,  // Formset GUID
             FormId,                           // Form ID
             StartOpCodeHandle,                // Label for where to insert opcodes
             EndOpCodeHandle                   // Replace data
             );
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function processes the results of changes in configuration.
  (i.e. when SCU settings has been adjusted)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Action                   Specifies the type of action taken by the browser.
  @param[in]       QuestionId               A unique value which is sent to the original
                                            exporting driver so that it can identify the type
                                            of data to expect.
  @param[in]       Type                     The type of value for the question.
  @param[in]       Value                    A pointer to the data being sent to the original
                                            exporting driver.
  @param[out]      ActionRequest            On return, points to the action requested by the
                                            callback function.

  @retval          EFI_SUCCESS              The callback successfully handled the action.
  @retval          EFI_OUT_OF_RESOURCES     Not enough storage is available to hold the
                                            variable and its data.
  @retval          EFI_DEVICE_ERROR         The variable could not be saved.
  @retval          EFI_UNSUPPORTED          The specified Action is not supported by the
                                            callback.
**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN        EFI_BROWSER_ACTION                Action,
  IN        EFI_QUESTION_ID                   QuestionId,
  IN        UINT8                             Type,
  IN        EFI_IFR_TYPE_VALUE                *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST        *ActionRequest
  )
{
  SD_EMMC_SETUP_UTILITY_PRIVATE_DATA   *SDEmmcSetupPrivate;

  SDEmmcSetupPrivate = SD_EMMC_SETUP_UTILITY_PRIVATE_FROM_THIS (This);

  DEBUG ((EFI_D_INFO, "%a () Action: %d QuestionId: 0x%04x\n", __FUNCTION__, Action, QuestionId));

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    SDEmmcInfoFormInit ();
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    if (QuestionId == 0 && mRefreshFlag) {
      mRefreshFlag = FALSE;

      HiiGetBrowserData (
        &gH2OSDEmmcInfoSetupVarstoreGuid,
        mVariableName,
        sizeof (SD_EMMC_SETUP_CONFIGURATION),
        (UINT8 *) &SDEmmcSetupPrivate->Configuration
        );

      SDEmmcInfoFormInit ();

      SDEmmcSetupPrivate->Configuration.NumberOfOemPorts = (UINT8) mOemPortNameCount;
      HiiSetBrowserData (
        &gH2OSDEmmcInfoSetupVarstoreGuid,
        mVariableName,
        sizeof (SD_EMMC_SETUP_CONFIGURATION),
        (UINT8 *) &SDEmmcSetupPrivate->Configuration,
        NULL
        );
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    if (QuestionId == H2O_SD_EMMC_INFO_DISPLAY_MODE_QUESTION_ID) {
      mRefreshFlag = TRUE;
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_CHANGING:
    if (QuestionId == H2O_SD_EMMC_INFO_DISPLAY_MODE_QUESTION_ID) {
      SDEmmcSetupPrivate->Configuration.DisplayMode = Value->u8;
      SDEmmcInfoFormInit ();
    }
    switch ((QuestionId & H2O_SD_EMMC_INFO_QUESTION_ID_POLICY_TYPE)) {
      case H20_SD_EMMC_INFO_DYNAMIC_SD_DEVICE_BASE_QUESTION_ID:
        SDSubFormUpdate (QuestionId);
        break;

      case H20_SD_EMMC_INFO_DYNAMIC_EMMC_DEVICE_BASE_QUESTION_ID:
        EmmcSubFormUpdate (QuestionId);
        break;

      default:
        break;
    }
    break;

  default:
    break;
  }

  return EFI_UNSUPPORTED;
}

/**
  This function updates SD/eMMC global data.

  @retval EFI_SUCCESS Function execute successful.
  @retval Others.

**/
EFI_STATUS
UpdateGlobalSDEmmcData (
  VOID
  )
{
  H2O_SD_EMMC_INFO_PROTOCOL      *SDEmmcInfoProtocol;
  EFI_STATUS                     Status;

  DEBUG ((EFI_D_INFO, "%a()\n", __FUNCTION__));

  mSDEmmcDeviceInfo  = NULL;
  mSDEmmcDeviceCount = 0;
  SDEmmcInfoProtocol = mSetupPrivate->Protocol;

  Status = SDEmmcInfoProtocol->GetSDEmmcInfo (SDEmmcInfoProtocol, &mSDEmmcDeviceCount, &mSDEmmcDeviceInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a () Status: %r\n", __FUNCTION__, Status));
    if (mSDEmmcDeviceInfo != NULL) {
      FreePool (mSDEmmcDeviceInfo);
    }
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function finds out the changing SD/eMMC device by comparing current data and old data.

  @param[out] ChangingDeviceName    An Unicode string buffer of SD/eMMC changing device.
  @param[out] ChangingType          Changing type of SD/eMMC device.

**/
VOID
FindOutChangingSDDevice (
  OUT CHAR16          **ChangingDeviceName,
  OUT CHANGING_TYPE   *ChangingType
)
{
  UINTN       Index;
  UINTN       Index1;
  CHAR16      *TempStr;
  BOOLEAN     SDEmmcDataMatch;

  TempStr   = NULL;

  if (mOldSDEmmcDeviceCount > mSDEmmcDeviceCount) {
    for (Index = 0; Index < mOldSDEmmcDeviceCount; Index++) {
      SDEmmcDataMatch = FALSE;
      for (Index1 = 0; Index1 < mSDEmmcDeviceCount; Index1++) {
        if ((CompareMem(mOldSDEmmcDeviceInfo[Index].CardInfo, mSDEmmcDeviceInfo[Index1].CardInfo, sizeof (CARD_INFO)) == 0) &&
            EfiCompareDevicePath(mOldSDEmmcDeviceInfo[Index].DevicePath, mSDEmmcDeviceInfo[Index1].DevicePath)) {
          SDEmmcDataMatch = TRUE;
        }
      }

      if (!SDEmmcDataMatch) {
          TempStr = SDEmmcDeviceNameStr (&mOldSDEmmcDeviceInfo[Index]);
          break;
      }
    }
    (*ChangingDeviceName) = TempStr;
    (*ChangingType)       = PlugOut;

  } else if (mSDEmmcDeviceCount > mOldSDEmmcDeviceCount) {
    for (Index = 0; Index < mSDEmmcDeviceCount; Index++) {
      SDEmmcDataMatch = FALSE;
      for (Index1 = 0; Index1 < mOldSDEmmcDeviceCount; Index1++) {
        if ((CompareMem(mSDEmmcDeviceInfo[Index].CardInfo, mOldSDEmmcDeviceInfo[Index1].CardInfo, sizeof (CARD_INFO)) == 0) &&
            EfiCompareDevicePath(mSDEmmcDeviceInfo[Index].DevicePath, mOldSDEmmcDeviceInfo[Index1].DevicePath)) {
          SDEmmcDataMatch = TRUE;
        }
      }

      if (!SDEmmcDataMatch) {
          TempStr = SDEmmcDeviceNameStr (&mSDEmmcDeviceInfo[Index]);
          break;
      }
    }
    (*ChangingDeviceName) = TempStr;
    (*ChangingType)       = PlugIn;
  } else {
    (*ChangingType)       = Unsupport;
  }
}

/**
  This function update STR_DEVICE_CHANGING_STAT string ID when SD/eMMC devices is changing.
**/
VOID
UpdateDeviceChangingStat (
  VOID
)
{
  CHAR16        DeviceChangingStr[MAX_DATA_STRING] = {0};

  if ((mChangingDeviceName != NULL) && (mChangingType != Unsupport)) {
    switch (mChangingType) {
      case PlugIn:
        UnicodeSPrint (
          DeviceChangingStr,
          MAX_DATA_STRING,
          L"Device Connected - [%6s]",
          mChangingDeviceName
          );
        break;

      case PlugOut:
        UnicodeSPrint (
          DeviceChangingStr,
          MAX_DATA_STRING,
          L"Device Disconnected - [%6s]",
          mChangingDeviceName
          );
        break;

      default:
        break;
    }
    HiiSetString (mSDEmmcInfoHiiHandle, STRING_TOKEN (STR_DEVICE_CHANGING_STAT), DeviceChangingStr, NULL);
  }
}

/**
  This function keeps the old SD/eMMC information when device is changing.
**/
VOID
RemainOldSDEmmcData (
  VOID
)
{
  UINTN   Index;

  mOldSDEmmcDeviceInfo  = AllocateCopyPool ((sizeof (SD_EMMC_INFO) * mSDEmmcDeviceCount), mSDEmmcDeviceInfo);
  mOldSDEmmcDeviceCount = mSDEmmcDeviceCount;

  if (mOldSDEmmcDeviceInfo == NULL) {
    return;
  }
  for (Index = 0;Index < mOldSDEmmcDeviceCount; Index++) {
    mOldSDEmmcDeviceInfo[Index].CardInfo    = AllocateCopyPool(sizeof (CARD_INFO), mSDEmmcDeviceInfo[Index].CardInfo);
    mOldSDEmmcDeviceInfo[Index].DevicePath  = DuplicateDevicePath(mSDEmmcDeviceInfo[Index].DevicePath);
  }
}

/**
  This function free the allocated buffer which kept old data.
**/
VOID
FreeOldSDEmmcData (
  VOID
)
{
  UINTN   Index;

  for (Index = 0;Index < mOldSDEmmcDeviceCount; Index++) {
    if (mOldSDEmmcDeviceInfo[Index].CardInfo != NULL) {
      FreePool (mOldSDEmmcDeviceInfo[Index].CardInfo);
    }
    if (mOldSDEmmcDeviceInfo[Index].DevicePath != NULL) {
      FreePool (mOldSDEmmcDeviceInfo[Index].DevicePath);
    }
  }

  if (mOldSDEmmcDeviceInfo != NULL) {
    FreePool (mOldSDEmmcDeviceInfo);
  }

  return;
}

/**
  A timer event call back function detects SD card hotplug and refreshes H2O_SD_EMMC_INFO_SD_DEVICE_INFO_FORM_ID form.

  @param[IN]  Event
  @param[IN]  Context

**/
VOID
EFIAPI
SDEmmcRefreshCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                  Status;
  UINTN                       SDEmmcHandleCount;
  EFI_HANDLE                  *SDEmmcHandleBuffer;
  H2O_SD_EMMC_INFO_PROTOCOL   *Protocol;


  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gSdPassThruProtocolGuid,
                  NULL,
                  &SDEmmcHandleCount,
                  &SDEmmcHandleBuffer
                  );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    DEBUG ((EFI_D_ERROR, "%a LocateHandleBuffer: %r.\n", __FUNCTION__, Status));
    goto Exit;
  }

  if (mSDEmmcDeviceCount != SDEmmcHandleCount) {

    Protocol = mSetupPrivate->Protocol;

    // Keep Old Data
    if (mOldSDEmmcDeviceInfo != NULL) {
      FreeOldSDEmmcData();
      mOldSDEmmcDeviceInfo = NULL;
    }
    if (mChangingDeviceName != NULL) {
      FreePool (mChangingDeviceName);
      mChangingDeviceName = NULL;
    }

    RemainOldSDEmmcData();

    Status = Protocol->Refresh (Protocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a Refresh: %r.\n", __FUNCTION__, Status));
      goto Exit;
    }

    if ((SDEmmcHandleCount > mSDEmmcDeviceCount) && (SDEmmcHandleCount > mBufferLargestSizeEver)) {
      mSDEmmcDeviceStringId     = (EFI_STRING_ID *) ReallocatePool (
                                                      (sizeof (EFI_STRING_ID) * mSDEmmcDeviceCount),
                                                      (sizeof (EFI_STRING_ID) * SDEmmcHandleCount),
                                                      mSDEmmcDeviceStringId
                                                      );
      mSDEmmcDeviceHelpStringId = (EFI_STRING_ID *) ReallocatePool (
                                                      (sizeof (EFI_STRING_ID) * mSDEmmcDeviceCount),
                                                      (sizeof (EFI_STRING_ID) * SDEmmcHandleCount),
                                                      mSDEmmcDeviceHelpStringId
                                                      );
      if ((mSDEmmcDeviceStringId == NULL) || (mSDEmmcDeviceHelpStringId == NULL)) {
        DEBUG ((EFI_D_ERROR, "%a EFI_OUT_OF_RESOURCE.\n", __FUNCTION__));
        goto Exit;
      }
      mBufferLargestSizeEver = SDEmmcHandleCount;
    }

    UpdateGlobalSDEmmcData ();
    FindOutChangingSDDevice (&mChangingDeviceName, &mChangingType);
    UpdateDeviceChangingStat ();
    SDEmmcInfoFormInit ();
    SDSubFormClear ();
  }

Exit:
  if (SDEmmcHandleBuffer != NULL) {
    FreePool (SDEmmcHandleBuffer);
  }
}

/**
  This function updates global OEM port name information.

  @retval EFI_SUCCESS Function execute successful.
  @retval Others

**/
EFI_STATUS
UpdateGlobalOemPortNameData (
  VOID
  )
{
  H2O_SD_EMMC_INFO_PROTOCOL      *SDEmmcInfoProtocol;
  EFI_STATUS                     Status;

  DEBUG ((EFI_D_INFO, "%a()\n", __FUNCTION__));
  SDEmmcInfoProtocol = mSetupPrivate->Protocol;

  Status = SDEmmcInfoProtocol->GetOemPortNameInfo (SDEmmcInfoProtocol, &mOemPortNameCount, &mOemPortNameInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a () Status: %r\n", __FUNCTION__, Status));
    if (mOemPortNameInfo != NULL) {
      FreePool (mOemPortNameInfo);
    }
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function updates setup variable to latest data.

  @retval EFI_SUCCESS Function execute successful.
  @retval Others.

**/
EFI_STATUS
UpdateSetupVariable (
  VOID
  )
{
  EFI_STATUS                    Status;
  SD_EMMC_SETUP_CONFIGURATION   *Configuration;
  UINTN                         VariableSize;

  DEBUG ((EFI_D_INFO, "%a()\n", __FUNCTION__));

  Configuration = NULL;
  VariableSize  = 0;

  Status = CommonGetVariableDataAndSize  (
             mVariableName,
             &gH2OSDEmmcInfoSetupVarstoreGuid,
             &VariableSize,
             (VOID **) &Configuration
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Configuration != NULL) {
    Configuration->NumberOfOemPorts = (UINT8) mOemPortNameCount;
    Status = CommonSetVariable (
              mVariableName,
              &gH2OSDEmmcInfoSetupVarstoreGuid,
              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
              VariableSize,
              Configuration
              );
    FreePool (Configuration);
  } else {
    return EFI_NOT_FOUND;
  }

  return Status;
}

/**
  A event that notify by gEfiSetupUtilityApplicationProtocolGuid initializes SD/eMMC information form and SetupUtility driver private data.

  @param  Event        Event instance (unused)
  @param  Context      Event Context (It is not used)

**/
VOID
EFIAPI
SDEmmcSetupInfoInitialEvent (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                               Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL   *SetupUtilityApp;

  DEBUG ((EFI_D_INFO, "%a () Start.\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL,
                  (VOID **)&SetupUtilityApp
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO, "%a LocateProtocol: %r.\n", __FUNCTION__, Status));
    return ;
  }

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    DEBUG ((EFI_D_INFO, "%a InitializeSetupUtility.\n", __FUNCTION__));

    if (mSetupPrivate->Protocol == NULL) {
      Status = gBS->LocateProtocol (
                      &gH2OSDEmmcInfoProtocolGuid,
                      NULL,
                      (VOID **)&mSetupPrivate->Protocol
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "%a LocateProtocol: %r\n", __FUNCTION__, Status));
        return;
      }
    }

    mSetupPrivate->Protocol->Refresh (mSetupPrivate->Protocol);

    Status = UpdateGlobalSDEmmcData ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a UpdateGlobalSDEmmcData: %r\n", __FUNCTION__, Status));
      return;
    }
    Status = UpdateGlobalOemPortNameData ();
    if (Status == EFI_NOT_FOUND) {
      DEBUG ((EFI_D_VERBOSE, "%a(), No Oem Port Name has been defined!\n", __FUNCTION__));
    } else if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a UpdateGlobalOemPortNameData: %r\n", __FUNCTION__, Status));
      return;
    }
    Status = UpdateSetupVariable ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a UpdateSetupVariable: %r\n", __FUNCTION__, Status));
      return;
    }

    if (mSDEmmcDeviceStringId == NULL) {
      mSDEmmcDeviceStringId = (EFI_STRING_ID*) AllocateZeroPool (sizeof (EFI_STRING_ID) * mSDEmmcDeviceCount);
    }

    if (mSDEmmcDeviceHelpStringId == NULL) {
      mSDEmmcDeviceHelpStringId = (EFI_STRING_ID*) AllocateZeroPool (sizeof (EFI_STRING_ID) * mSDEmmcDeviceCount);
    }

    if ((mSDEmmcDeviceStringId       == NULL) ||
        (mSDEmmcDeviceHelpStringId   == NULL)
        ) {
      DEBUG ((EFI_D_ERROR, "%a Out of Resource\n", __FUNCTION__));
      return;
    }
    if (mOemPortNameCount != 0){
      if (mSDEmmcPortNameStringId == NULL) {
        mSDEmmcPortNameStringId = (EFI_STRING_ID*) AllocateZeroPool (sizeof (EFI_STRING_ID) * mOemPortNameCount);
      }

      if (mSDEmmcPortNameHelpStringId == NULL) {
        mSDEmmcPortNameHelpStringId = (EFI_STRING_ID*) AllocateZeroPool (sizeof (EFI_STRING_ID) * mOemPortNameCount);
      }
      if ((mSDEmmcPortNameStringId       == NULL) ||
          (mSDEmmcPortNameHelpStringId   == NULL)
         ) {
        DEBUG ((EFI_D_ERROR, "%a Out of Resource\n", __FUNCTION__));
        return;
      }
    }

    mBufferLargestSizeEver = mSDEmmcDeviceCount;

    SDEmmcInfoFormInit ();
    HiiSetString (mSDEmmcInfoHiiHandle, STRING_TOKEN (STR_DEVICE_CHANGING_STAT), L"", NULL);

    if (mSDEmmcRefreshEvent == NULL) {

      // Register SD/eMMC device refresh event.
      Status = gBS->CreateEvent (
                      (EVT_TIMER | EVT_NOTIFY_SIGNAL),
                      TPL_CALLBACK,
                      SDEmmcRefreshCallback,
                      NULL,
                      &mSDEmmcRefreshEvent
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "%a CreateEvent: %r.\n", __FUNCTION__, Status));
        return;
      }

      Status = gBS->SetTimer (
                      mSDEmmcRefreshEvent,
                      TimerPeriodic,
                      5000000
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_INFO, "%a SetTimer: %r.\n", __FUNCTION__, Status));
        return;
      }
    }
    return;

  } else if (SetupUtilityApp->VfrDriverState == ShutdownSetupUtility) {

    DEBUG ((EFI_D_INFO, "%a ShutdownSetupUtility.\n", __FUNCTION__));

    gBS->CloseEvent (mSDEmmcRefreshEvent);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "%a CloseEvent: %r.\n", __FUNCTION__, Status));
      return;
    }
    mSDEmmcRefreshEvent = NULL;

    return;
  }
}

/**
  This function processes the results of changes in configuration.
  (i.e. [F10] -> "Save and exit" has been pressed)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Configuration            A null-terminated Unicode string in <ConfigResp>
                                            format.
  @param[out]      Progress                 A pointer to a string filled in with the offset of
                                            the most recent '&' before the first failing
                                            name/value pair (or the beginning of the string if
                                            the failure is in the first name/value pair) or
                                            the terminating NULL if all was successful.

  @retval          EFI_SUCCESS              The Results is processed successfully.
  @retval          EFI_INVALID_PARAMETER    Configuration is NULL.
  @retval          EFI_NOT_FOUND            Routing data doesn't match any storage in this
                                            driver.
*/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Configuration,
  OUT       EFI_STRING                        *Progress
  )
{
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  EFI_HII_CONFIG_ROUTING_PROTOCOL       *HiiConfigRouting;
  SD_EMMC_SETUP_UTILITY_PRIVATE_DATA    *SDEmmcSetupPrivate;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SDEmmcSetupPrivate = SD_EMMC_SETUP_UTILITY_PRIVATE_FROM_THIS (This);
  HiiConfigRouting  = SDEmmcSetupPrivate->HiiConfigRouting;
  *Progress         = Configuration;

  if (!HiiIsConfigHdrMatch(Configuration, &gH2OSDEmmcInfoSetupVarstoreGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (SD_EMMC_SETUP_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSDEmmcInfoSetupVarstoreGuid,
             &BufferSize,
             &SDEmmcSetupPrivate->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Check if configuring Name/Value storage
  //
  if (StrStr (Configuration, L"OFFSET") == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (SD_EMMC_SETUP_CONFIGURATION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               (UINT8 *) &SDEmmcSetupPrivate->Configuration,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to ConfigToBlock, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = CommonSetVariable (
             mVariableName,
             &gH2OSDEmmcInfoSetupVarstoreGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (SD_EMMC_SETUP_CONFIGURATION),
             &SDEmmcSetupPrivate->Configuration
             );

  DEBUG ((EFI_D_INFO, "%a() - End, Status = %r\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.
  (i.e. head in "Setup Utility" from front page)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Request                  A null-terminated Unicode string in <ConfigRequest> format.
  @param[out]      Progress                 On return, points to a character in the Request
                                            string. Points to the string's null terminator if
                                            request was successful. Points to the most recent
                                            '&' before the first failing name/value pair (or
                                            the beginning of the string if the failure is in
                                            the first name/value pair) if the request was not
                                            successful.
  @param[out]      Results                  A null-terminated Unicode string in
                                            <ConfigAltResp> format which has all values filled
                                            in for the names in the Request string. String to
                                            be allocated by the called function.

  @retval          EFI_SUCCESS              The Results is filled with the requested values.
  @retval          EFI_OUT_OF_RESOURCES     Not enough memory to store the results.
  @retval          EFI_INVALID_PARAMETER    Request is illegal syntax, or unknown name.
  @retval          EFI_NOT_FOUND            Routing data doesn't match any storage in this
                                            driver.
**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Request,
  OUT       EFI_STRING                        *Progress,
  OUT       EFI_STRING                        *Results
  )
{
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  EFI_HII_CONFIG_ROUTING_PROTOCOL       *HiiConfigRouting;
  EFI_STRING                            ConfigRequest;
  EFI_STRING                            ConfigRequestHdr;
  UINTN                                 Size;
  CHAR16                                *StrPointer;
  BOOLEAN                               AllocatedRequest;
  SD_EMMC_SETUP_UTILITY_PRIVATE_DATA    *SDEmmcSetupPrivate;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = Request;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;

  SDEmmcSetupPrivate = SD_EMMC_SETUP_UTILITY_PRIVATE_FROM_THIS (This);
  HiiConfigRouting  = SDEmmcSetupPrivate->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (SD_EMMC_SETUP_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSDEmmcInfoSetupVarstoreGuid,
             &BufferSize,
             &SDEmmcSetupPrivate->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  if (Request == NULL) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OSDEmmcInfoSetupVarstoreGuid, mVariableName, SDEmmcSetupPrivate->DriverHandle);
    if (ConfigRequestHdr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64) BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &gH2OSDEmmcInfoSetupVarstoreGuid, mVariableName)) {
      DEBUG ((EFI_D_INFO, "%a() - ConfigHdr is not matched with this driver\n", __FUNCTION__));
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
      // Check Request Element does exist in Request String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest    = AllocateZeroPool (Size);
        if (ConfigRequest == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64) BufferSize);
      }
    }
  }

  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // If requesting Name/Value storage, return not found.
    //
    return EFI_NOT_FOUND;
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *) &SDEmmcSetupPrivate->Configuration,
                               BufferSize,
                               Results,
                               Progress
                               );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
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

EFI_STATUS
EFIAPI
SDEmmcInfoSetupUtilityDxeEntry (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  UINTN                     BufferSize;
  EFI_STRING                ConfigRequestHdr;
  EFI_STATUS                Status;
  VOID                      *Registration;

  DEBUG ((EFI_D_INFO, "%a () Start!!\n", __FUNCTION__));

  mSetupPrivate = AllocateZeroPool (sizeof (SD_EMMC_SETUP_UTILITY_PRIVATE_DATA));
  if (mSetupPrivate == NULL) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  mSetupPrivate->Signature                  = SD_EMMC_SETUP_UTILITY_PRIVATE_SIGNATURE;
  mSetupPrivate->ConfigAccess.ExtractConfig = ExtractConfig;
  mSetupPrivate->ConfigAccess.RouteConfig   = RouteConfig;
  mSetupPrivate->ConfigAccess.Callback      = DriverCallback;

  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **) &mSetupPrivate->HiiConfigRouting
                  );
  if (EFI_ERROR (Status)) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mSetupPrivate->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mSetupPrivate->ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return Status;
  }

  mSetupPrivate->HiiHandle = HiiAddPackages (
                               &gH2OSDEmmcInfoSetupFormsetGuid,
                               mSetupPrivate->DriverHandle,
                               SDEmmcInfoSetupUtilityDxeVfrBin,
                               SDEmmcInfoSetupUtilityDxeStrings,
                               NULL
                               );
  if (mSetupPrivate->HiiHandle == NULL) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  mSDEmmcInfoHiiHandle = mSetupPrivate->HiiHandle;

  //
  // Check the status of the setup variable. Note that it has a read-only
  // default value stored in the variable defaults region.
  //
  BufferSize = sizeof (SD_EMMC_SETUP_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSDEmmcInfoSetupVarstoreGuid,
             &BufferSize,
             &mSetupPrivate->Configuration
             );
  if (EFI_ERROR (Status) ||
      !IsVariableInVariableStoreRegion (mVariableName, &gH2OSDEmmcInfoSetupVarstoreGuid)) {
    BufferSize = sizeof (SD_EMMC_SETUP_CONFIGURATION);
    Status = InitVarDefault (
               mVariableName,
               &gH2OSDEmmcInfoSetupVarstoreGuid,
               &BufferSize,
               &mSetupPrivate->Configuration
               );
    if (EFI_ERROR (Status)) {
      SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
      return Status;
    }
  }

  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gH2OSDEmmcInfoSetupVarstoreGuid,
                       mVariableName,
                       mSetupPrivate->DriverHandle
                       );
  if (ConfigRequestHdr == NULL) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  if (!HiiValidateSettings (ConfigRequestHdr)) {
    FreePool (ConfigRequestHdr);
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return EFI_INVALID_PARAMETER;
  }

  FreePool (ConfigRequestHdr);

  //
  // Register a callback function.
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SDEmmcSetupInfoInitialEvent,
                  NULL,
                  &mSetupPrivate->SetupUtilityEvent
                  );
  if (EFI_ERROR (Status)) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  mSetupPrivate->SetupUtilityEvent,
                  &Registration
                  );
  if (EFI_ERROR (Status)) {
    SDEmmcInfoSetupUtilityDxeUnload (ImageHandle);
    return Status;
  }

  DEBUG ((EFI_D_INFO, "%a () End!!\n", __FUNCTION__));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SDEmmcInfoSetupUtilityDxeUnload (
  IN EFI_HANDLE                   ImageHandle
  )
{
  if (mSetupPrivate == NULL) {
    return EFI_SUCCESS;
  }

  gBS->CloseEvent (mSetupPrivate->SetupUtilityEvent);

  HiiRemovePackages (mSetupPrivate->HiiHandle);

  gBS->UninstallMultipleProtocolInterfaces (
          mSetupPrivate->DriverHandle,
          &gEfiDevicePathProtocolGuid,
          &mHiiVendorDevicePath0,
          &gEfiHiiConfigAccessProtocolGuid,
          &mSetupPrivate->ConfigAccess,
          NULL
          );

  if (mSetupPrivate != NULL) {
    FreePool (mSetupPrivate);
  }
  if (mSDEmmcDeviceStringId != NULL) {
    FreePool (mSDEmmcDeviceStringId);
  }
  if (mSDEmmcDeviceHelpStringId != NULL) {
    FreePool (mSDEmmcDeviceHelpStringId);
  }
  if (mSDEmmcPortNameStringId != NULL) {
    FreePool (mSDEmmcPortNameStringId);
  }
  if (mSDEmmcPortNameHelpStringId != NULL) {
    FreePool (mSDEmmcPortNameHelpStringId);
  }
  if (mSDEmmcDeviceInfo != NULL) {
    FreePool (mSDEmmcDeviceInfo);
  }
  if (mOemPortNameInfo != NULL) {
    FreePool (mOemPortNameInfo);
  }

  FreeOldSDEmmcData();

  return EFI_SUCCESS;
}