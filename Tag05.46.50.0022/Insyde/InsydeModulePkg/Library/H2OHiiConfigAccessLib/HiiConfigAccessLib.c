/** @file
  HII Library implementation that uses DXE protocols and services.

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

  Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>

#include <Protocol/HiiConfigAccess.h>


#define GUID_CONFIG_STRING_TYPE 0x00
#define NAME_CONFIG_STRING_TYPE 0x01
#define PATH_CONFIG_STRING_TYPE 0x02

/**
  Converts all hex dtring characters in range ['A'..'F'] to ['a'..'f'] for 
  hex digits that appear between a '=' and a '&' in a config string.

  If ConfigString is NULL, then ASSERT().

  @param[in] ConfigString  Pointer to a Null-terminated Unicode string.

  @return  Pointer to the Null-terminated Unicode result string.

**/
EFI_STRING
EFIAPI
InternalHiiLowerConfigString (
  IN EFI_STRING  ConfigString
  )
{
  EFI_STRING  String;
  BOOLEAN     Lower;

  ASSERT (ConfigString != NULL);
  if (ConfigString == NULL) {
    return NULL;
  }

  //
  // Convert all hex digits in range [A-F] in the configuration header to [a-f]
  //
  for (String = ConfigString, Lower = FALSE; *String != L'\0'; String++) {
    if (*String == L'=') {
      Lower = TRUE;
    } else if (*String == L'&') {
      Lower = FALSE;
    } else if (Lower && *String >= L'A' && *String <= L'F') {
      *String = (CHAR16) (*String - L'A' + L'a');
    }
  }

  return ConfigString;
}

/**
  Allocates and returns a Null-terminated Unicode <ConfigHdr> string using routing 
  information that includes a GUID, an optional Unicode string name, and a device
  path.  The string returned is allocated with AllocatePool().  The caller is 
  responsible for freeing the allocated string with FreePool().
  
  The format of a <ConfigHdr> is as follows:

    GUID=<HexCh>32&NAME=<Char>NameLength&PATH=<HexChar>DevicePathSize<Null>

  @param[in]  Guid          Pointer to an EFI_GUID that is the routing information
                            GUID.  Each of the 16 bytes in Guid is converted to 
                            a 2 Unicode character hexadecimal string.  This is
                            an optional parameter that may be NULL.
  @param[in]  Name          Pointer to a Null-terminated Unicode string that is 
                            the routing information NAME.  This is an optional 
                            parameter that may be NULL.  Each 16-bit Unicode 
                            character in Name is converted to a 4 character Unicode 
                            hexadecimal string.
  @param[in]  DriverHandle  The driver handle which supports a Device Path Protocol
                            that is the routing information PATH.  Each byte of
                            the Device Path associated with DriverHandle is converted
                            to a 2 Unicode character hexadecimal string.

  @retval NULL   DriverHandle does not support the Device Path Protocol.
  @retval Other  A pointer to the Null-terminate Unicode <ConfigHdr> string

**/
EFI_STRING
EFIAPI
HiiConstructConfigHdr (
  IN CONST EFI_GUID  *Guid,  OPTIONAL
  IN CONST CHAR16    *Name,  OPTIONAL
  IN EFI_HANDLE      DriverHandle
  )
{
  UINTN                     NameLength;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     DevicePathSize;
  CHAR16                    *String;
  CHAR16                    *ReturnString;
  UINTN                     Index;
  UINT8                     *Buffer;
  UINTN                     MaxLen;

  //
  // Compute the length of Name in Unicode characters.  
  // If Name is NULL, then the length is 0.
  //
  NameLength = 0;
  if (Name != NULL) {
    NameLength = StrLen (Name);
  }

  DevicePath = NULL;
  DevicePathSize = 0;
  //
  // Retrieve DevicePath Protocol associated with DriverHandle
  //
  if (DriverHandle != NULL) {
    DevicePath = DevicePathFromHandle (DriverHandle);
    if (DevicePath == NULL) {
      return NULL;
    }
    //
    // Compute the size of the device path in bytes
    //
    DevicePathSize = GetDevicePathSize (DevicePath);
  }

  //
  // GUID=<HexCh>32&NAME=<Char>NameLength&PATH=<HexChar>DevicePathSize <Null>
  // | 5 | sizeof (EFI_GUID) * 2 | 6 | NameStrLen*4 | 6 | DevicePathSize * 2 | 1 |
  //
  MaxLen = 5 + sizeof (EFI_GUID) * 2 + 6 + NameLength * 4 + 6 + DevicePathSize * 2 + 1;
  String = AllocateZeroPool (MaxLen * sizeof (CHAR16));
  if (String == NULL) {
    return NULL;
  }

  //
  // Start with L"GUID="
  //
  StrCpyS (String, MaxLen, L"GUID=");
  ReturnString = String;
  String += StrLen (String);

  if (Guid != NULL) {
    //
    // Append Guid converted to <HexCh>32
    //
    for (Index = 0, Buffer = (UINT8 *)Guid; Index < sizeof (EFI_GUID); Index++) {
      UnicodeValueToStringS (
        String,
        MaxLen * sizeof (CHAR16) - ((UINTN)String - (UINTN)ReturnString),
        PREFIX_ZERO | RADIX_HEX,
        *(Buffer++),
        2
        );
      String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
    }
  }
  
  //
  // Append L"&NAME="
  //
  StrCatS (ReturnString, MaxLen, L"&NAME=");
  String += StrLen (String);

  if (Name != NULL) {
    //
    // Append Name converted to <Char>NameLength
    //
    for (; *Name != L'\0'; Name++) {
      UnicodeValueToStringS (
        String,
        sizeof (CHAR16) * MaxLen - ((UINTN)String - (UINTN)ReturnString),
        PREFIX_ZERO | RADIX_HEX,
        *Name,
        4
        );
      String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
    }
  }

  //
  // Append L"&PATH="
  //
  StrCatS (ReturnString, MaxLen, L"&PATH=");
  String += StrLen (String);

  //
  // Append the device path associated with DriverHandle converted to <HexChar>DevicePathSize
  //
  for (Index = 0, Buffer = (UINT8 *)DevicePath; Index < DevicePathSize; Index++) {
    UnicodeValueToStringS (
      String,
      sizeof (CHAR16) * MaxLen - ((UINTN)String - (UINTN)ReturnString),
      PREFIX_ZERO | RADIX_HEX,
      *(Buffer++),
      2
      );
    String += StrnLenS (String, MaxLen - ((UINTN)String - (UINTN)ReturnString) / sizeof (CHAR16));
  }

  //
  // Null terminate the Unicode string
  //
  *String = L'\0';

  //
  // Convert all hex digits in range [A-F] in the configuration header to [a-f]
  //
  return InternalHiiLowerConfigString (ReturnString);
}

/**
  Convert the hex UNICODE encoding string of UEFI GUID, NAME or device path 
  to binary buffer from <ConfigHdr>.

  This is a internal function.

  @param  String                 UEFI configuration string.
  @param  Flag                   Flag specifies what type buffer will be retrieved.
  @param  Buffer                 Binary of Guid, Name or Device path.

  @retval EFI_INVALID_PARAMETER  Any incoming parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Lake of resources to store neccesary structures.
  @retval EFI_SUCCESS            The buffer data is retrieved and translated to
                                 binary format.

**/
EFI_STATUS
InternalHiiGetBufferFromString (
  IN  EFI_STRING                 String,
  IN  UINT8                      Flag,
  OUT UINT8                      **Buffer
  )
{
  UINTN      Length;
  EFI_STRING ConfigHdr;
  CHAR16     *StringPtr;
  UINT8      *DataBuffer;
  CHAR16     TemStr[5];
  UINTN      Index;
  UINT8      DigitUint8;

  if (String == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  DataBuffer = NULL;
  StringPtr  = NULL;
  ConfigHdr  = String;
  //
  // The content between 'GUID', 'NAME', 'PATH' of <ConfigHdr> and '&' of next element
  // or '\0' (end of configuration string) is the UNICODE %02x bytes encoding string.
  //
  for (Length = 0; *String != 0 && *String != L'&'; String++, Length++);

  switch (Flag) {
  case GUID_CONFIG_STRING_TYPE:
  case PATH_CONFIG_STRING_TYPE:
    //
    // The data in <ConfigHdr> is encoded as hex UNICODE %02x bytes in the same order
    // as the device path and Guid resides in RAM memory.
    // Translate the data into binary.
    //
    DataBuffer = (UINT8 *) AllocateZeroPool ((Length + 1) / 2);
    if (DataBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Convert binary byte one by one
    //
    ZeroMem (TemStr, sizeof (TemStr));
    for (Index = 0; Index < Length; Index ++) {
      TemStr[0] = ConfigHdr[Index];
      DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
      if ((Index & 1) == 0) {
        DataBuffer [Index/2] = DigitUint8;
      } else {
        DataBuffer [Index/2] = (UINT8) ((DataBuffer [Index/2] << 4) + DigitUint8);
      }
    }
    
    *Buffer = DataBuffer;
    break;

  case NAME_CONFIG_STRING_TYPE:
    //
    // Convert Config String to Unicode String, e.g. "0041004200430044" => "ABCD"
    // 

    //
    // Add the tailling char L'\0'
    //
    DataBuffer = (UINT8 *) AllocateZeroPool ((Length/4 + 1) * sizeof (CHAR16));
    if (DataBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    //
    // Convert character one by one
    //
    StringPtr = (CHAR16 *) DataBuffer;
    ZeroMem (TemStr, sizeof (TemStr));
    for (Index = 0; Index < Length; Index += 4) {
      StrnCpyS (TemStr, sizeof (TemStr) / sizeof (CHAR16), ConfigHdr + Index, 4);
      StringPtr[Index/4] = (CHAR16) StrHexToUint64 (TemStr);
    }
    //
    // Add tailing L'\0' character
    //
    StringPtr[Index/4] = L'\0';

    *Buffer = DataBuffer;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}


/**
  Get the value of <Number> in <BlockConfig> format, i.e. the value of OFFSET
  or WIDTH or VALUE.
  <BlockConfig> ::= 'OFFSET='<Number>&'WIDTH='<Number>&'VALUE'=<Number>

  @param  ValueString            String in <BlockConfig> format and points to the
                                 first character of <Number>.
  @param  ValueData              The output value. Caller takes the responsibility
                                 to free memory.
  @param  ValueLength            Length of the <Number>, in characters.

  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to store neccessary
                                 structures.
  @retval EFI_SUCCESS            Value of <Number> is outputted in Number
                                 successfully.
  @retval EFI_INVALID_PARAMETER  Invalid parameter.

**/
EFI_STATUS
EFIAPI
InternalHiiGetValueOfNumber (
  IN  EFI_STRING           ValueString,
  OUT UINT8                **ValueData,
  OUT UINTN                *ValueLength
  )
{
  EFI_STRING               StringPtr;
  UINTN                    Length;
  UINT8                    *Buf;
  UINT8                    DigitUint8;
  UINTN                    Index;
  CHAR16                   TemStr[2];

  ASSERT (ValueString != NULL && ValueData != NULL && ValueLength != NULL);
  if (ValueString == NULL || ValueData == NULL || ValueLength == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ASSERT (*ValueString != L'\0');

  //
  // Get the length of value string
  //
  StringPtr = ValueString;
  while (*StringPtr != L'\0' && *StringPtr != L'&') {
    StringPtr++;
  }
  Length = StringPtr - ValueString;
  
  //
  // Allocate buffer to store the value
  //
  Buf = (UINT8 *) AllocateZeroPool ((Length + 1) / 2);
  if (Buf == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  
  //
  // Convert character one by one to the value buffer
  //
  ZeroMem (TemStr, sizeof (TemStr));
  for (Index = 0; Index < Length; Index ++) {
    TemStr[0] = ValueString[Length - Index - 1];
    DigitUint8 = (UINT8) StrHexToUint64 (TemStr);
    if ((Index & 1) == 0) {
      Buf [Index/2] = DigitUint8;
    } else {
      Buf [Index/2] = (UINT8) ((DigitUint8 << 4) + Buf [Index/2]);
    }
  }
  
  //
  // Set the converted value and string length.
  //
  *ValueData    = Buf;
  *ValueLength  = Length;
  return EFI_SUCCESS;
}

/**
  Get value from config request resp string.

  @param ConfigElement           ConfigResp string contains the current setting.
  @param VarName                 The variable name which need to get value.
  @param VarValue                The return value.
  
  @retval EFI_SUCCESS            Get the value for the VarName
  @retval EFI_OUT_OF_RESOURCES   The memory is not enough.
**/
EFI_STATUS
GetValueFromRequest (
  IN CHAR16                       *ConfigElement,
  IN CHAR16                       *VarName,
  OUT UINT64                      *VarValue
  )
{
  UINT8                        *TmpBuffer;
  CHAR16                       *StringPtr;
  UINTN                        Length;
  EFI_STATUS                   Status;

  //
  // Find VarName related string.
  //
  StringPtr = StrStr (ConfigElement, VarName);
  ASSERT (StringPtr != NULL);

  //
  // Skip the "VarName=" string
  //
  StringPtr += StrLen (VarName) + 1;

  //
  // Get Offset
  //
  Status = InternalHiiGetValueOfNumber (StringPtr, &TmpBuffer, &Length);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *VarValue = 0;
  CopyMem (VarValue, TmpBuffer, (((Length + 1) / 2) < sizeof (UINT64)) ? ((Length + 1) / 2) : sizeof (UINT64));

  FreePool (TmpBuffer);

  return EFI_SUCCESS;
}
