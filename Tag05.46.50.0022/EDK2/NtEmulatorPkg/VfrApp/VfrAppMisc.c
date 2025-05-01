/** @file
 VFR application

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#include "VfrAppMisc.h"

/**
 Update names of Name/Value storage to current language.

 @param PrivateData   Points to the driver private data.

 @retval EFI_SUCCESS   All names are successfully updated.
 @retval EFI_NOT_FOUND Failed to get Name from HII database.

**/
EFI_STATUS
LoadNameValueNames (
  IN VFR_APP_PRIVATE_DATA               *PrivateData
  )
{
  UINTN      Index;

  //
  // Get Name/Value name string of current language
  //
  for (Index = 0; Index < NAME_VALUE_NAME_NUMBER; Index++) {
    PrivateData->NameValueName[Index] = HiiGetString (
                                         PrivateData->HiiHandle,
                                         PrivateData->NameStringId[Index],
                                         NULL
                                         );
    if (PrivateData->NameValueName[Index] == NULL) {
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}

/**
  Get the value of <Number> in <BlockConfig> format, i.e. the value of OFFSET
  or WIDTH or VALUE.
  <BlockConfig> ::= 'OFFSET='<Number>&'WIDTH='<Number>&'VALUE'=<Number>

  This is a internal function.

  @param  StringPtr              String in <BlockConfig> format and points to the
                                 first character of <Number>.
  @param  Number                 The output value. Caller takes the responsibility
                                 to free memory.
  @param  Len                    Length of the <Number>, in characters.

  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to store neccessary
                                 structures.
  @retval EFI_SUCCESS            Value of <Number> is outputted in Number
                                 successfully.

**/
EFI_STATUS
GetValueOfNumber (
  IN EFI_STRING                    StringPtr,
  OUT UINT8                        **Number,
  OUT UINTN                        *Len
  )
{
  EFI_STRING               TmpPtr;
  UINTN                    Length;
  EFI_STRING               Str;
  UINT8                    *Buf;
  EFI_STATUS               Status;
  UINT8                    DigitUint8;
  UINTN                    Index;
  CHAR16                   TemStr[2];

  if (StringPtr == NULL || *StringPtr == L'\0' || Number == NULL || Len == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Buf = NULL;

  TmpPtr = StringPtr;
  while (*StringPtr != L'\0' && *StringPtr != L'&') {
    StringPtr++;
  }
  *Len   = StringPtr - TmpPtr;
  Length = *Len + 1;

  Str = (EFI_STRING) AllocateZeroPool (Length * sizeof (CHAR16));
  if (Str == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  CopyMem (Str, TmpPtr, *Len * sizeof (CHAR16));
  *(Str + *Len) = L'\0';

  Length = (Length + 1) / 2;
  Buf = (UINT8 *) AllocateZeroPool (Length);
  if (Buf == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Length = *Len;
  ZeroMem (TemStr, sizeof (TemStr));
  for (Index = 0; Index < Length; Index ++) {
    TemStr[0] = Str[Length - Index - 1];
    DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
    if ((Index & 1) == 0) {
      Buf [Index/2] = DigitUint8;
    } else {
      Buf [Index/2] = (UINT8) ((DigitUint8 << 4) + Buf [Index/2]);
    }
  }

  *Number = Buf;
  Status  = EFI_SUCCESS;

Exit:
  if (Str != NULL) {
    FreePool (Str);
  }

  return Status;
}

/**
  Create altcfg string.

  @param  Result               The request result string.
  @param  ConfigHdr            The request head info. <ConfigHdr> format.
  @param  Offset               The offset of the parameter int he structure.
  @param  Width                The width of the parameter.


  @retval  The string with altcfg info append at the end.
**/
EFI_STRING
CreateAltCfgString (
  IN     EFI_STRING     Result,
  IN     EFI_STRING     ConfigHdr,
  IN     UINTN          Offset,
  IN     UINTN          Width
  )
{
  EFI_STRING StringPtr;
  EFI_STRING TmpStr;
  UINTN      NewLen;

  NewLen = StrLen (Result);
  //
  // String Len = ConfigResp + AltConfig + AltConfig + 1("\0")
  //
  NewLen = (NewLen + ((1 + StrLen (ConfigHdr) + 8 + 4) + (8 + 4 + 7 + 4 + 7 + 4)) * 2 + 1) * sizeof (CHAR16);
  StringPtr = AllocateZeroPool (NewLen);
  if (StringPtr == NULL) {
    return NULL;
  }

  TmpStr = StringPtr;
  if (Result != NULL) {
    StrCpyS (StringPtr, NewLen / sizeof (CHAR16), Result);
    StringPtr += StrLen (Result);
    FreePool (Result);
  }

  UnicodeSPrint (
  StringPtr,
  (1 + StrLen (ConfigHdr) + 8 + 4 + 1) * sizeof (CHAR16),
  L"&%s&ALTCFG=%04x",
  ConfigHdr,
  EFI_HII_DEFAULT_CLASS_STANDARD
  );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
    StringPtr,
    (8 + 4 + 7 + 4 + 7 + 4 + 1) * sizeof (CHAR16),
    L"&OFFSET=%04x&WIDTH=%04x&VALUE=%04x",
    Offset,
    Width,
    DEFAULT_CLASS_STANDARD_VALUE
    );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
  StringPtr,
  (1 + StrLen (ConfigHdr) + 8 + 4 + 1) * sizeof (CHAR16),
  L"&%s&ALTCFG=%04x",
  ConfigHdr,
  EFI_HII_DEFAULT_CLASS_MANUFACTURING
  );
  StringPtr += StrLen (StringPtr);

  UnicodeSPrint (
    StringPtr,
    (8 + 4 + 7 + 4 + 7 + 4 + 1) * sizeof (CHAR16),
    L"&OFFSET=%04x&WIDTH=%04x&VALUE=%04x",
    Offset,
    Width,
    DEFAULT_CLASS_MANUFACTURING_VALUE
    );
  StringPtr += StrLen (StringPtr);

  return TmpStr;
}

/**
  Check whether need to add the altcfg string. if need to add, add the altcfg
  string.

  @param  RequestResult              The request result string.
  @param  ConfigRequestHdr           The request head info. <ConfigHdr> format.

**/
VOID
AppendAltCfgString (
  IN OUT EFI_STRING                       *RequestResult,
  IN     EFI_STRING                       ConfigRequestHdr
  )
{
  EFI_STRING                          StringPtr;
  UINTN                               Length;
  UINT8                               *TmpBuffer;
  UINTN                               Offset;
  UINTN                               Width;
  UINTN                               BlockSize;
  UINTN                               ValueOffset;
  UINTN                               ValueWidth;
  EFI_STATUS                          Status;

  StringPtr = *RequestResult;
  StringPtr = StrStr (StringPtr, L"OFFSET");
  BlockSize = sizeof (VFR_APP_CONFIGURATION);
  ValueOffset = OFFSET_OF (VFR_APP_CONFIGURATION, GetDefaultValueFromAccess);
  ValueWidth  = sizeof (((VFR_APP_CONFIGURATION *)0)->GetDefaultValueFromAccess);

  if (StringPtr == NULL) {
    return;
  }

  while (*StringPtr != 0 && StrnCmp (StringPtr, L"OFFSET=", StrLen (L"OFFSET=")) == 0) {
    StringPtr += StrLen (L"OFFSET=");
    //
    // Get Offset
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    Offset = 0;
    CopyMem (
     &Offset,
     TmpBuffer,
     (((Length + 1) / 2) < sizeof (UINTN)) ? ((Length + 1) / 2) : sizeof (UINTN)
     );
    FreePool (TmpBuffer);

    StringPtr += Length;
    if (StrnCmp (StringPtr, L"&WIDTH=", StrLen (L"&WIDTH=")) != 0) {
      return;
    }
    StringPtr += StrLen (L"&WIDTH=");

    //
    // Get Width
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    Width = 0;
    CopyMem (
     &Width,
     TmpBuffer,
     (((Length + 1) / 2) < sizeof (UINTN)) ? ((Length + 1) / 2) : sizeof (UINTN)
     );
    FreePool (TmpBuffer);

    StringPtr += Length;
    if (StrnCmp (StringPtr, L"&VALUE=", StrLen (L"&VALUE=")) != 0) {
      return;
    }
    StringPtr += StrLen (L"&VALUE=");

    //
    // Get Value
    //
    Status = GetValueOfNumber (StringPtr, &TmpBuffer, &Length);
    if (EFI_ERROR (Status)) {
      return;
    }
    FreePool (TmpBuffer);

    StringPtr += Length;

    //
    // Calculate Value and convert it to hex string.
    //
    if (Offset + Width > BlockSize) {
      return;
    }

    if (Offset <= ValueOffset && Offset + Width >= ValueOffset + ValueWidth) {
      *RequestResult = CreateAltCfgString(*RequestResult, ConfigRequestHdr, ValueOffset, ValueWidth);
      return;
    }
  }
}

