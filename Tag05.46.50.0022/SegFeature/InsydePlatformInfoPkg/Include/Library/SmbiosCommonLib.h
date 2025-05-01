/** @file
  SMBIOS Common Library Definition.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __SMBIOS_COMMON_LIB_H__
#define __SMBIOS_COMMON_LIB_H__

#include <Uefi.h>

#include <Protocol/Smbios.h>

/**
  Helper function to get text string by string number from given SMBIOS structure.

  @param[in]  Header  SMBIOS structure header.
  @param[in]  Number  String Number.

  @retval The callee allocated text string or NULL if not found.

**/
CHAR16 *
SmbiosGetStringByNumber (
  IN  EFI_SMBIOS_TABLE_HEADER   *Header,
  IN  EFI_SMBIOS_STRING         StringNumber
  );

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
  );

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
  IN  EFI_SMBIOS_TYPE           SmbiosType,
  OUT UINTN                     *NumberOfRecords,
  OUT EFI_SMBIOS_TABLE_HEADER   ***RecordBuffer
  );

#endif