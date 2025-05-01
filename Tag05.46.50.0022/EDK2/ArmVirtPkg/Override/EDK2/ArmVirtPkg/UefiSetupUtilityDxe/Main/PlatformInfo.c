/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PlatformInfo.h>

#define StringBufferSize 40

PLATFORM_INFO_DISPLAY_TABLE mDisplayPlatformInfoFunction[] = {
  PLATFORM_INFO_DISPLAY_OPTION_TABLE_LIST
  };

EFI_HII_STRING_PROTOCOL   *mIfrLibHiiString;

STATIC CHAR16                      *mMemFormFactorStr[] = {L"Other",
                                                           L"Unknown",
                                                           L"SIMM",
                                                           L"SIP",
                                                           L"Chip",
                                                           L"DIP",
                                                           L"ZIP",
                                                           L"Proprietary Card",
                                                           L"DIMM",
                                                           L"TSOP",
                                                           L"Row of chips",
                                                           L"RIMM",
                                                           L"SODIMM",
                                                           L"SRIMM",
                                                           L"FB-DIMM"
                                                           };

/**
  Create Ramslot information. And save string tokens into TransferRamSlotInfo for destination Hii handle
  after conversion.

  @param[in]       SourceHiiHandle        Input source Hii handle.
  @param[in]       DestinationHiiHandle   Input destination Hii handle.
  @param[in]       NumSlot                Number of memory slots.
  @param[in]       RamSlotInfo            Pointer to the start of Ram slot information arrary.
                                          This arrary saved string tokens for source Hii handle.
  @param[in, out]  TransferRamSlotInfo    Pointer to the start of Ram slot information arrary.
                                          This arrary saved string tokens for destination Hii handle
                                          after conversion.

  @retval          EFI_SUCCESS            If the create Ramslot and save string tokens could be successfully.
  @retval          Others                 Failed to update Form.

**/
STATIC
EFI_STATUS
CreateRamSlotInfo (
  IN        EFI_HII_HANDLE         SourceHiiHandle,
  IN        EFI_HII_HANDLE         DestinationHiiHandle,
  IN        UINTN                  NumSlot,
  IN        UPDATE_INFO_STRUCT     *RamSlotInfo,
  IN OUT    UPDATE_INFO_STRUCT     *TransferRamSlotInfo
  )
{
  VOID                                  *StartOpCodeHandle;
  VOID                                  *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                    *StartLabel;
  EFI_IFR_GUID_LABEL                    *EndLabel;
  UINTN                                 Index;
  EFI_STATUS                            Status;
  EFI_STRING_ID                         BlankString;
  EFI_STRING_ID                         StrToken1;
  EFI_STRING_ID                         StrToken2;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = UPDATE_INFO_RAM_SLOT_LABEL;

  EndLabel                 = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = MAIN_PAGE_PLATFORM_INFO_LABEL;

  NewStringToHandle (
    SourceHiiHandle,
    STRING_TOKEN (STR_BLANK_STRING),
    DestinationHiiHandle,
    &BlankString
    );
  //
  // Channel A
  //
  NewStringToHandle (
    SourceHiiHandle,
    STRING_TOKEN (STR_CHANNELA_STRING),
    DestinationHiiHandle,
    &StrToken1
    );

  HiiCreateTextOpCode (
    StartOpCodeHandle,
    StrToken1,
    0,
    BlankString
    );

    for (Index = 0; Index < NumSlot; Index++) {
      if (Index == NumSlot / 2 ) {
        //
        // Channel B
        //
        NewStringToHandle (
          SourceHiiHandle,
          STRING_TOKEN (STR_CHANNELB_STRING),
          DestinationHiiHandle,
          &StrToken1
          );
        HiiCreateTextOpCode (
          StartOpCodeHandle,
          StrToken1,
          0,
          BlankString
        );
      }

      NewStringToHandle (
        SourceHiiHandle,
        RamSlotInfo[Index].TokenToUpdate1,
        DestinationHiiHandle,
        &StrToken1
        );
      TransferRamSlotInfo[Index].TokenToUpdate1 = StrToken1;

      NewStringToHandle (
        SourceHiiHandle,
        RamSlotInfo[Index].TokenToUpdate2,
        DestinationHiiHandle,
        &StrToken2
        );
      TransferRamSlotInfo[Index].TokenToUpdate2 = StrToken2;

      HiiCreateTextOpCode (
        StartOpCodeHandle,
        StrToken1,
        0,
        StrToken2
        );
    }
  //
  // Add Text op-code
  //
  Status = HiiUpdateForm (
             DestinationHiiHandle,
             NULL,
             ROOT_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

   HiiFreeOpCodeHandle (StartOpCodeHandle);
   HiiFreeOpCodeHandle (EndOpCodeHandle);

   return Status;
}

/**

 Update new string to Hii Handle.

 @param [in]   SrcHiiHandle   New Hii Handle.
 @param [in]   SrcStringId    New String Token.
 @param [in]   DstHiiHandle   Hii Handle of the package to be updated.
 @param [out]   DstStringId   String Token to be updated.

 @retval EFI_SUCCESS          String update successfully.

**/
EFI_STATUS
NewStringToHandle (
  IN  EFI_HII_HANDLE           SrcHiiHandle,
  IN  EFI_STRING_ID            SrcStringId,
  IN  EFI_HII_HANDLE           DstHiiHandle,
  OUT EFI_STRING_ID            *DstStringId
  )
{
  EFI_STATUS                   Status;
  CHAR16                       *String;
  UINTN                        Size;
  UINTN                        StringSize;
  CHAR8                        *Languages;
  CHAR8                        *LangStrings;
  CHAR8                        Lang[RFC_3066_ENTRY_SIZE];


  StringSize = 0x200;
  String = AllocateZeroPool (StringSize);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Use english string as the default string.
  //
  Size = StringSize;
  Status = mIfrLibHiiString->GetString (
                               mIfrLibHiiString,
                               "en-US",
                               SrcHiiHandle,
                               SrcStringId,
                               String,
                               &Size,
                               NULL
                               );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    gBS->FreePool (String);
    StringSize = Size;
    String = AllocateZeroPool (StringSize);
    Status = mIfrLibHiiString->GetString (
                                 mIfrLibHiiString,
                                 "eng",
                                 SrcHiiHandle,
                                 SrcStringId,
                                 String,
                                 &Size,
                                 NULL
                                 );
    if (EFI_ERROR (Status)) {
      gBS->FreePool (String);
      return Status;
    }
  } else if (EFI_ERROR (Status)) {
    return Status;
  }

  *DstStringId = HiiSetString (DstHiiHandle, 0, String, NULL);

  //
  // Update string by each language.
  //
  Languages = HiiGetSupportedLanguages (DstHiiHandle);
  if (Languages == NULL) {
    gBS->FreePool (String);
    return EFI_NOT_FOUND;
  }
  LangStrings = Languages;
  while (*LangStrings != 0) {
    SetupUtilityLibGetNextLanguage (&LangStrings, Lang);

    Size = StringSize;
    Status = mIfrLibHiiString->GetString (
                                 mIfrLibHiiString,
                                 Lang,
                                 SrcHiiHandle,
                                 SrcStringId,
                                 String,
                                 &Size,
                                 NULL
                                 );
    if (!EFI_ERROR (Status)) {
      mIfrLibHiiString->SetString (
                          mIfrLibHiiString,
                          DstHiiHandle,
                          *DstStringId,
                          Lang,
                          String,
                          NULL
                          );
    }
  }

  gBS->FreePool (String);
  gBS->FreePool (Languages);

  return EFI_SUCCESS;
}

/**
  Acquire the string associated with the Index from smbios structure and return it.
  The caller is responsible for free the string buffer.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS            Get index string successfully.
  @retval   EFI_INVALID_PARAMETER  Index is zero. It is invalid value.
  @retval   EFI_ABORTED            Get missing string fail .
  @retval   EFI_OUT_OF_RESOURCES   Allocate memory fail.
  @retval   Other                  Get setup browser data fail.

**/
EFI_STATUS
GetStringByIndex (
  IN  CHAR8                        *OptionalStrStart,
  IN  UINT8                        Index,
  OUT CHAR16                       **String
  )
{
  UINTN                                 StrSize;
  CHAR16                                *StringBuffer;

  if (Index == 0) {
    return EFI_INVALID_PARAMETER;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart);
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    //
    return EFI_ABORTED;
  }

  StringBuffer = AllocatePool (StrSize * sizeof (CHAR16));
  if (StringBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  AsciiStrToUnicodeStrS (OptionalStrStart, StringBuffer, StrSize);

  *String = StringBuffer;

  return EFI_SUCCESS;
}

/**
  Update Memory Infomation.

  @param    SUBrowser              Setup utility bowser data point

  @retval   EFI_SUCCESS            If the Memory Infomation could be successfully updated.
  @retval   Other                  Failed to update Memory Infomation.

**/
EFI_STATUS
UpdateMemoryInfo (
  IN  SETUP_UTILITY_BROWSER_DATA            *SUBrowser
  )
{
  EFI_SMBIOS_TABLE_HEADER     *Record;
  EFI_SMBIOS_HANDLE           SmbiosHandle;
  EFI_SMBIOS_PROTOCOL         *Smbios;
  EFI_HII_HANDLE              LocalMainHiiHandle;
  EFI_HII_HANDLE              LocalAdvanceHiiHandle;
  EFI_STATUS                  Status;
  UINT8                       ChannelBase;
  UINT8                       SlotNumChar;
  UINT8                       StrIndex;
  UINTN                       NumSlot;
  UINTN                       Index;
  UINTN                       SlotIndex;
  CHAR16                      *NewString;
  CHAR16                      StrBuffer[StringBufferSize];
  EFI_STRING_ID               TokenToUpdate;
  SMBIOS_TABLE_TYPE17         *Type17Record;
  UPDATE_INFO_STRUCT          *LocalMainRamSlotInfo;
  BOOLEAN                     IsFormFactorStrUpdated;
  UPDATE_INFO_STRUCT          UpdateRamSlotInfo[] = {
                                {1, STRING_TOKEN(STR_CHA_DIMM0_STRING), STRING_TOKEN(STR_CHA_DIMM0_STRING2)},
                                {2, STRING_TOKEN(STR_CHA_DIMM1_STRING), STRING_TOKEN(STR_CHA_DIMM1_STRING2)},
                                {3, STRING_TOKEN(STR_CHB_DIMM0_STRING), STRING_TOKEN(STR_CHB_DIMM0_STRING2)},
                                {4, STRING_TOKEN(STR_CHB_DIMM1_STRING), STRING_TOKEN(STR_CHB_DIMM1_STRING2)}
                              };

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **)&mIfrLibHiiString);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  LocalMainHiiHandle      = SUBrowser->SUCInfo->MapTable[MainHiiHandle].HiiHandle;
  LocalAdvanceHiiHandle   = SUBrowser->SUCInfo->MapTable[AdvanceHiiHandle].HiiHandle;

  NumSlot = sizeof (UpdateRamSlotInfo) /sizeof (UPDATE_INFO_STRUCT);
  LocalMainRamSlotInfo = AllocateZeroPool(sizeof (UpdateRamSlotInfo));
  if (LocalMainRamSlotInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = CreateRamSlotInfo (
             LocalAdvanceHiiHandle,
             LocalMainHiiHandle,
             NumSlot,
             UpdateRamSlotInfo,
             LocalMainRamSlotInfo
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    gBS->FreePool (LocalMainRamSlotInfo);
    return Status;
  }

  IsFormFactorStrUpdated = FALSE;
  Index = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Record->Type == EFI_SMBIOS_TYPE_MEMORY_DEVICE) {
      Type17Record = (SMBIOS_TABLE_TYPE17 *) Record;
      //
      // Get slot index and update all memory device slot string at first time.
      //
      SlotIndex = Index;
      if (Type17Record->FormFactor >= MemoryFormFactorOther &&
          Type17Record->FormFactor <= MemoryFormFactorFbDimm) {
        StrIndex = Type17Record->DeviceLocator;
        Status = GetStringByIndex ((CHAR8*) ((UINT8*) Type17Record + Type17Record->Hdr.Length), StrIndex, &NewString);
        if (Status == EFI_SUCCESS && NewString != NULL) {
          if (!StrnCmp (NewString, L"Channel", StrLen (L"Channel"))) {
            ChannelBase = (UINT8) ((NewString[StrLen (L"Channel")] - 'A') * 2);
          } else {
            ChannelBase = 0;
          }

          SlotNumChar = *(UINT8 *) (NewString + StrLen (NewString) - 1);
          SlotNumChar += ChannelBase;
          if (SlotNumChar < '0' || SlotNumChar >= NumSlot + '0') {
            SlotIndex = Index;
          } else {
            SlotIndex = SlotNumChar - '0';
          }

          gBS->FreePool (NewString);
        }

        if (!IsFormFactorStrUpdated && Type17Record->Size != 0 && Type17Record->Size != 0xFFFF) {
          UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s 0", mMemFormFactorStr[Type17Record->FormFactor - 1]);
          HiiSetString (LocalMainHiiHandle, LocalMainRamSlotInfo[0].TokenToUpdate1, StrBuffer, NULL);
          HiiSetString (LocalMainHiiHandle, LocalMainRamSlotInfo[2].TokenToUpdate1, StrBuffer, NULL);
          UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s 1", mMemFormFactorStr[Type17Record->FormFactor - 1]);
          HiiSetString (LocalMainHiiHandle, LocalMainRamSlotInfo[1].TokenToUpdate1, StrBuffer, NULL);
          HiiSetString (LocalMainHiiHandle, LocalMainRamSlotInfo[3].TokenToUpdate1, StrBuffer, NULL);
          IsFormFactorStrUpdated = TRUE;
        }
      }

      if (Type17Record->Size != 0) {
        ZeroMem (StrBuffer, sizeof (StrBuffer));
        if (Type17Record->Size == 0xFFFF) {
          StrCatS (StrBuffer, StringBufferSize, L"Unknown");
        } else if (Type17Record->Size == 0x7FFFF) {
          UnicodeValueToStringS (StrBuffer, StringBufferSize * sizeof (CHAR16), 0, (UINT64)(Type17Record->ExtendedSize & ~BIT31), 0);
          StrCatS (StrBuffer, StringBufferSize, L" MB");
        } else {
          UnicodeValueToStringS (StrBuffer, StringBufferSize * sizeof (CHAR16),0, (UINT64)(Type17Record->Size & ~BIT15), 0);
          if (Type17Record->Size & BIT15) {
            StrCatS (StrBuffer, StringBufferSize, L" KB");
          } else {
            StrCatS (StrBuffer, StringBufferSize, L" MB");
          }
        }
        TokenToUpdate = (EFI_STRING_ID) (LocalMainRamSlotInfo[SlotIndex].TokenToUpdate2);
        HiiSetString (LocalMainHiiHandle, TokenToUpdate, StrBuffer, NULL);
      }
      Index++;
    }
  } while(Status == EFI_SUCCESS);

  gBS->FreePool (LocalMainRamSlotInfo);
  return EFI_SUCCESS;
}


EFI_STATUS
DisplayPlatformInfo (
  IN  SETUP_UTILITY_BROWSER_DATA            *SUBrowser
  )
{
  EFI_STATUS                Status;
  CHAR16                    *StringBuffer;
  EFI_HII_HANDLE            LocalMainHiiHandle;
  EFI_HII_HANDLE            LocalAdvanceHiiHandle;
  EFI_STRING_ID             PlatformInfoTitleString;
  EFI_STRING_ID             PlatformInfoHelp;
  EFI_STRING_ID             BlankString;
  VOID                      *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL        *StartLabel;

  Status                  = EFI_SUCCESS;
  StringBuffer            = NULL;
  PlatformInfoHelp        = 0;
  PlatformInfoTitleString = 0;
  LocalMainHiiHandle      = SUBrowser->SUCInfo->MapTable[MainHiiHandle].HiiHandle;
  LocalAdvanceHiiHandle   = SUBrowser->SUCInfo->MapTable[AdvanceHiiHandle].HiiHandle;

  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **)&mIfrLibHiiString);
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate space for creation of Buffer
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = MAIN_PAGE_PLATFORM_INFO_LABEL;

  //
  // In order to prevent to grayout the label MAIN_PAGE_PLATFORM_INFO_LABEL,
  // the below precedure
  //
  NewStringToHandle (
    LocalAdvanceHiiHandle,
    STRING_TOKEN (STR_PLATFORM_CONFIG_FORM_TITLE),
    LocalMainHiiHandle,
    &PlatformInfoTitleString
    );

  NewStringToHandle (
    LocalAdvanceHiiHandle,
    STRING_TOKEN (STR_PLATFORM_CONFIG_FORM_HELP),
    LocalMainHiiHandle,
    &PlatformInfoHelp
    );

  NewStringToHandle (
    LocalAdvanceHiiHandle,
    STRING_TOKEN (STR_BLANK_STRING),
    LocalMainHiiHandle,
    &BlankString
    );

  HiiCreateTextOpCode (StartOpCodeHandle,PlatformInfoTitleString, PlatformInfoHelp, BlankString );

  PlatformInfoInit (StartOpCodeHandle, LocalMainHiiHandle, LocalAdvanceHiiHandle);
  HiiUpdateForm (
    LocalMainHiiHandle,
    NULL,
    ROOT_FORM_ID,
    StartOpCodeHandle,
    NULL
    );
  gBS->FreePool (StringBuffer);
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  return Status;
}

/**
 Init Platform Infomation Setup Form
 To display the following Data of platform infomation:
 1. CPUID
 2. Number of Core
 3. Number of Thread per core
 4. Microcode Version
 5. TXT
 6. VT-d
 7. VT-x
 8. PCH Reversion
 9. SA Reversion
 10.VBIOS Version
 11.EC Version

 @param [in]   OpCodeHandle
 @param [in]   MainHiiHandle
 @param [in]   AdvanceHiiHandle

 @retval EFI_SUCCESS

**/
EFI_STATUS
PlatformInfoInit (
  IN  VOID                         *OpCodeHandle,
  IN  EFI_HII_HANDLE               MainHiiHandle,
  IN  EFI_HII_HANDLE               AdvanceHiiHandle
  )
{
  EFI_STATUS             Status;
  UINTN                  Index;
  CHAR16                 *StringDataBuffer;

  Status           = EFI_SUCCESS;
  StringDataBuffer = NULL;

  StringDataBuffer = AllocateZeroPool (0x100);
  if (StringDataBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; mDisplayPlatformInfoFunction[Index].DisplayPlatformInfoFunction != NULL; Index++) {
    if (mDisplayPlatformInfoFunction[Index].Option == DISPLAY_ENABLE) {
      ZeroMem(StringDataBuffer, 0x100);
      mDisplayPlatformInfoFunction[Index].DisplayPlatformInfoFunction (OpCodeHandle, MainHiiHandle, AdvanceHiiHandle, StringDataBuffer);
    }
  }

  gBS->FreePool (StringDataBuffer);
  return Status;
}

