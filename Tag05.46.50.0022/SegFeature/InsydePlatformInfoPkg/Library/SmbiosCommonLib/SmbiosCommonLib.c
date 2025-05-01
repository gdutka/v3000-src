/** @file
  Implementation of SmbiosCommonLib.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmbiosCommonLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Smbios.h>

#include <IndustryStandard/SmBios.h>

STATIC EFI_SMBIOS_PROTOCOL *mSmbios = NULL;

/**
  Trim leading and trailing spaces in the Null-terminated ASCII string.

  @param[in]  AsciiStr  An Null-terminated ASCII string.

  @retval A newly allocated buffer with the result string.
          Caller should free it after use.

**/
STATIC
CHAR8 *
AsciiTrimString (
  IN  CONST CHAR8   *AsciiStr
  )
{
  UINTN   Length;
  UINTN   Index;
  UINTN   Index2;
  CHAR8   *TrimmedAsciiStr;

  Length = AsciiStrLen (AsciiStr);

  for (Index = 0; Index < Length && AsciiStr[Index] == ' '; Index++);
  for (Index2 = Length - 1; Index2 > Index && AsciiStr[Index2] == ' '; Index2--);
  Index2++;

  TrimmedAsciiStr = (CHAR8 *) AllocateZeroPool ((Index2 - Index + 1) * sizeof (CHAR8));
  if (TrimmedAsciiStr == NULL) {
    return NULL;
  }

  AsciiStrnCpyS (TrimmedAsciiStr, (Index2 - Index + 1) * sizeof (CHAR8), AsciiStr + Index, Index2 - Index);

  return TrimmedAsciiStr;
}

/**
  Helper function to get text string by string number from given SMBIOS structure.

  @param[in]  Header  SMBIOS structure header.
  @param[in]  Number  String Number.

  @retval The callee allocated text string or NULL if not found.

**/
CHAR16 *
SmbiosGetStringByNumber (
  IN  EFI_SMBIOS_TABLE_HEADER   *Header,
  IN  EFI_SMBIOS_STRING         Number
  )
{
  CHAR8   *AsciiStringPtr;
  UINTN   Index;
  CHAR8   *TrimmedAsciiString;
  UINTN   Length;
  CHAR16  *UnicodeString;

  if (Header == NULL || Number == 0) {
    return NULL;
  }

  AsciiStringPtr = (CHAR8 *)((UINT8 *) Header + Header->Length);
  for (Index = 1; (CHAR16 *) AsciiStringPtr != NULL; Index++) {
    if (Number == Index) {
      TrimmedAsciiString = AsciiTrimString (AsciiStringPtr);
	  if (TrimmedAsciiString == NULL) {
	  	return NULL;
	  }
      Length = AsciiStrLen (TrimmedAsciiString);
      UnicodeString = AllocateZeroPool ((Length + 1) * sizeof (CHAR16));
      if (UnicodeString == NULL) {
        return NULL;
      }
      AsciiStrToUnicodeStrS (TrimmedAsciiString, UnicodeString, Length + 1);

      FreePool (TrimmedAsciiString);

      return UnicodeString;
    }

    AsciiStringPtr = (CHAR8 *)((UINT8 *) AsciiStringPtr + AsciiStrSize (AsciiStringPtr));
  }

  return NULL;
}

/**
  Return the first SMBIOS record that matches the handle and the type.

  @param[in]  SmbiosHandle  On entry it means return the next SMBIOS record of handle SmbiosHandle. If a NULL
                            is passed in or it is FFFEh, then the first SMBIOS record handle will be returned.
  @param[in]  SmbiosType    On entry it means return the next SMBIOS record of type Type. If a NULL is passed
                            in, it will be ignored.
  @param[out] Record        On exit, points to the SMBIOS Record consisting of the formatted area followed by
                            the unformatted area. The unformatted area optionally contains text strings.

  @retval EFI_SUCCESS             SMBIOS record information was successfully returned in Record.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_NOT_FOUND           None of SMBIOS record matches the handle and/or the type.

**/
EFI_STATUS
LocateSmbiosRecord (
  IN  EFI_SMBIOS_HANDLE       *SmbiosHandle  OPTIONAL,
  IN  EFI_SMBIOS_TYPE         *SmbiosType    OPTIONAL,
  OUT EFI_SMBIOS_TABLE_HEADER **Record
  )
{
  EFI_HANDLE                ProducerHandle;
  EFI_SMBIOS_HANDLE         TempSmbiosHandle;
  EFI_STATUS                Status;

  if (Record == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (SmbiosHandle != NULL && *SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED) {
    TempSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    do {
      Status = mSmbios->GetNext (mSmbios, &TempSmbiosHandle, SmbiosType, Record, &ProducerHandle);
      if ((*Record)->Handle == *SmbiosHandle) {
        break;
      }
    } while (TempSmbiosHandle != SMBIOS_HANDLE_PI_RESERVED);
  } else {
    TempSmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->GetNext (mSmbios, &TempSmbiosHandle, SmbiosType, Record, &ProducerHandle);
  }

  return Status;
}

/**
  Function returns an array of records that matches the requested type in a buffer
  allocated from pool.

  @param[in]  SmbiosType        Provides the type to search by.
  @param[out] NumberOfRecords   The number of records returned in RecordBuffer.
  @param[out] RecordBuffer      A pointer to the buffer to return the requested array of records which matches
                                SmbiosType.

  @retval EFI_SUCCESS             The result array of records was returned.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_NOT_FOUND           No records match the search.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
LocateSmbiosRecordBuffer (
  IN      EFI_SMBIOS_TYPE           SmbiosType,
  IN OUT  UINTN                     *NumberOfRecords,
  IN OUT  EFI_SMBIOS_TABLE_HEADER   ***RecordBuffer
  )
{
  EFI_SMBIOS_HANDLE         SmbiosHandle;
  EFI_STATUS                Status;
  EFI_SMBIOS_TABLE_HEADER   *Record;
  UINTN                     TempNumberOfRecords;
  EFI_SMBIOS_TABLE_HEADER   **TempRecordBuffer;

  if (NumberOfRecords == NULL || RecordBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SmbiosHandle        = SMBIOS_HANDLE_PI_RESERVED;
  TempNumberOfRecords = 0;
  TempRecordBuffer    = NULL;
  do {
    Status = mSmbios->GetNext (mSmbios, &SmbiosHandle, &SmbiosType, &Record, NULL);
    if (!EFI_ERROR (Status)) {
      TempRecordBuffer = (EFI_SMBIOS_TABLE_HEADER **) ReallocatePool (
                                                        TempNumberOfRecords * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                        (TempNumberOfRecords + 1) * sizeof (EFI_SMBIOS_TABLE_HEADER *),
                                                        (VOID *) TempRecordBuffer
                                                        );
      if (TempRecordBuffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      TempRecordBuffer[TempNumberOfRecords++] = Record;
    }
  } while (SmbiosHandle != SMBIOS_HANDLE_PI_RESERVED);

  *NumberOfRecords  = TempNumberOfRecords;
  *RecordBuffer     = TempRecordBuffer;

  return EFI_SUCCESS;
}

/**
  Constructor of SMBIOS Common Library Instance.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmbiosCommonLibConstructor (
  IN  EFI_HANDLE        FileHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &mSmbios);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to locate protocol %g, Status: %r\n",
      __FUNCTION__,
      &gEfiSmbiosProtocolGuid,
      Status
      ));
  }

  return EFI_SUCCESS;
}