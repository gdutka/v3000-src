/** @file

;******************************************************************************
;* Copyright (c) 2016 - 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <UpdateMemInfo.h>

EFI_HII_STRING_PROTOCOL   *mIfrLibHiiString;

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
//[-start-220311-IB14740224-modify]//
STATIC
EFI_STATUS
CreateRamSlotInfo (
  IN        EFI_HII_HANDLE         SourceHiiHandle,
  IN        EFI_HII_HANDLE         DestinationHiiHandle,
  IN        UINTN                  NumSlot,
  IN        UPDATE_INFO_STRUCT     *TransferRamSlotInfo
  )
{
  VOID                                  *StartOpCodeHandle;
  VOID                                  *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                    *StartLabel;
  EFI_IFR_GUID_LABEL                    *EndLabel;
  UINTN                                 Index;
  EFI_STATUS                            Status;
  EFI_STRING_ID                         BlankString;
  EFI_STRING_ID                         OldChannelStringId;

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
  OldChannelStringId = BlankString;

  for (Index = 0; Index < NumSlot; Index++) {
    if (StrnCmp (HiiGetString (DestinationHiiHandle, TransferRamSlotInfo[Index].ChannelStringId, NULL),
                 HiiGetString (DestinationHiiHandle, OldChannelStringId, NULL),
                 StrLen (HiiGetString (DestinationHiiHandle, OldChannelStringId, NULL)))) {
      HiiCreateTextOpCode (
        StartOpCodeHandle,
        TransferRamSlotInfo[Index].ChannelStringId,
        0,
        BlankString
      );
      OldChannelStringId = TransferRamSlotInfo[Index].ChannelStringId;
    }
    HiiCreateTextOpCode (
      StartOpCodeHandle,
      TransferRamSlotInfo[Index].DimmStringId,
      0,
      TransferRamSlotInfo[Index].SizeStringId
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
//[-end-220311-IB14740224-modify]//

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
    StrSize           = AsciiStrnSizeS (OptionalStrStart, PcdGet32 (PcdMaximumAsciiStringLength));
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
//[-start-220311-IB14740224-modify]//
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
  UINT8                       StrIndex;
  UINTN                       NumSlot;
  UINTN                       Index;
  UINTN                       SlotIndex;
  CHAR16                      *NewString;
  CHAR16                      StrBuffer[40];
  SMBIOS_TABLE_TYPE17         *Type17Record;
  UPDATE_INFO_STRUCT          *LocalMainRamSlotInfo;

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

  NumSlot = 0;
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }
    if (Record->Type == EFI_SMBIOS_TYPE_MEMORY_DEVICE) {
      NumSlot++;
    }
  } while(Status == EFI_SUCCESS);

  LocalMainRamSlotInfo = AllocateZeroPool (NumSlot * sizeof(UPDATE_INFO_STRUCT));
  if (LocalMainRamSlotInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

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
      // Update memory device slot string
      //
      SlotIndex = Index;
      if (Type17Record->FormFactor >= MemoryFormFactorOther &&
          Type17Record->FormFactor <= MemoryFormFactorFbDimm) {
        StrIndex = Type17Record->BankLocator;
        Status = GetStringByIndex ((CHAR8*) ((UINT8*) Type17Record + Type17Record->Hdr.Length), StrIndex, &NewString);
        if (Status == EFI_SUCCESS) {
          LocalMainRamSlotInfo[SlotIndex].ChannelStringId = HiiSetString (LocalMainHiiHandle, (EFI_STRING_ID)(0), NewString, NULL);
          gBS->FreePool (NewString);
        }

        StrIndex = Type17Record->DeviceLocator;
        Status = GetStringByIndex ((CHAR8*) ((UINT8*) Type17Record + Type17Record->Hdr.Length), StrIndex, &NewString);
        if (Status == EFI_SUCCESS) {
          LocalMainRamSlotInfo[SlotIndex].DimmStringId = HiiSetString (LocalMainHiiHandle, (EFI_STRING_ID)(0), NewString, NULL);
          gBS->FreePool (NewString);
        }
      }

      if (Type17Record->Size != 0) {
        ZeroMem (StrBuffer, sizeof (StrBuffer));
        if (Type17Record->Size == 0xFFFF) {
          StrCatS (StrBuffer, sizeof (StrBuffer) / sizeof (CHAR16), L"Unknown");
        } else if (Type17Record->Size == 0x7FFFF) {
          UnicodeValueToStringS (StrBuffer, sizeof (StrBuffer), 0, (UINT64)(Type17Record->ExtendedSize & ~BIT31), 0);
          StrCatS (StrBuffer, sizeof (StrBuffer) / sizeof (CHAR16), L" MB");
        } else {
          UnicodeValueToStringS (StrBuffer, sizeof (StrBuffer), 0, (UINT64)(Type17Record->Size & ~BIT15), 0);
          if (Type17Record->Size & BIT15) {
            StrCatS (StrBuffer, sizeof (StrBuffer) / sizeof (CHAR16), L" KB");
          } else {
            StrCatS (StrBuffer, sizeof (StrBuffer) / sizeof (CHAR16), L" MB");
          }
        }
        LocalMainRamSlotInfo[SlotIndex].SizeStringId = HiiSetString (LocalMainHiiHandle, (EFI_STRING_ID)(0), StrBuffer, NULL);
      } else {
        LocalMainRamSlotInfo[SlotIndex].SizeStringId = HiiSetString (LocalMainHiiHandle, (EFI_STRING_ID)(0), L"[Not Installed]", NULL);
      }
      Index++;
    }
  } while (Status == EFI_SUCCESS);

  Status = CreateRamSlotInfo (
             LocalAdvanceHiiHandle,
             LocalMainHiiHandle,
             NumSlot,
             LocalMainRamSlotInfo
             );
  ASSERT_EFI_ERROR (Status);

  gBS->FreePool (LocalMainRamSlotInfo);
  return Status;
}
//[-end-220311-IB14740224-modify]//

