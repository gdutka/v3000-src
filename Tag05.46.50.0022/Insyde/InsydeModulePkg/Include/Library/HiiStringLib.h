/** @file
  Public include file for the HII Database Library

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

Copyright (c) 2007 - 2013, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                            

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#ifndef __HII_STRING_LIB_H__
#define __HII_STRING_LIB_H__

#include <Protocol/HiiString.h>

extern EFI_HII_STRING_PROTOCOL *gHiiString;

/**
  This function creates a new string in String Package or updates an existing 
  string in a String Package.  If StringId is 0, then a new string is added to
  a String Package.  If StringId is not zero, then a string in String Package is
  updated.  If SupportedLanguages is NULL, then the string is added or updated
  for all the languages that the String Package supports.  If SupportedLanguages
  is not NULL, then the string is added or updated for the set of languages 
  specified by SupportedLanguages.
    
  If HiiHandle is NULL, then ASSERT().
  If String is NULL, then ASSERT().

  @param[in]  HiiHandle           A handle that was previously registered in the 
                                  HII Database.
  @param[in]  StringId            If zero, then a new string is created in the 
                                  String Package associated with HiiHandle.  If 
                                  non-zero, then the string specified by StringId 
                                  is updated in the String Package associated 
                                  with HiiHandle. 
  @param[in]  String              A pointer to the Null-terminated Unicode string 
                                  to add or update in the String Package associated 
                                  with HiiHandle.
  @param[in]  SupportedLanguages  A pointer to a Null-terminated ASCII string of 
                                  language codes.  If this parameter is NULL, then 
                                  String is added or updated in the String Package 
                                  associated with HiiHandle for all the languages 
                                  that the String Package supports.  If this 
                                  parameter is not NULL, then String is added 
                                  or updated in the String Package associated with 
                                  HiiHandle for the set of languages specified by 
                                  SupportedLanguages.  The format of 
                                  SupportedLanguages must follow the language 
                                  format assumed in the HII Database.

  @retval 0      The string could not be added or updated in the String Package.
  @retval Other  The EFI_STRING_ID of the newly added or updated string.

**/
EFI_STRING_ID
EFIAPI
HiiSetString (
  IN EFI_HII_HANDLE    HiiHandle,
  IN EFI_STRING_ID     StringId,            OPTIONAL
  IN CONST EFI_STRING  String,
  IN CONST CHAR8       *SupportedLanguages  OPTIONAL
  )
;

/**
  Retrieves a string from a string package in a specific language.  If the language
  is not specified, then a string from a string package in the current platform 
  language is retrieved.  If the string cannot be retrieved using the specified 
  language or the current platform language, then the string is retrieved from 
  the string package in the first language the string package supports.  The 
  returned string is allocated using AllocatePool().  The caller is responsible 
  for freeing the allocated buffer using FreePool().
  
  If HiiHandle is NULL, then ASSERT().
  If StringId is 0, then ASSERT().

  @param[in]  HiiHandle  A handle that was previously registered in the HII Database.
  @param[in]  StringId   The identifier of the string to retrieved from the string 
                         package associated with HiiHandle.
  @param[in]  Language   The language of the string to retrieve.  If this parameter 
                         is NULL, then the current platform language is used.  The 
                         format of Language must follow the language format assumed in
                         the HII Database.

  @retval NULL   The string specified by StringId is not present in the string package.
  @retval Other  The string was returned.

**/
EFI_STRING
EFIAPI
HiiGetString (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID   StringId,
  IN CONST CHAR8     *Language  OPTIONAL
  )
;

/**
  Retrieves a string from a string package named by GUID, in the specified language.  
  If the language is not specified, then a string from a string package in the 
  current platform  language is retrieved.  If the string cannot be retrieved 
  using the specified language or the current platform language, then the string 
  is retrieved from the string package in the first language the string package 
  supports.  The returned string is allocated using AllocatePool().  The caller 
  is responsible for freeing the allocated buffer using FreePool().
  
  If PackageListGuid is NULL, then ASSERT().
  If StringId is 0, then ASSERT().

  @param[in]  PackageListGuid  The GUID of a package list that was previously 
                               registered in the HII Database.
  @param[in]  StringId         The identifier of the string to retrieved from the 
                               string package associated with PackageListGuid.
  @param[in]  Language         The language of the string to retrieve.  If this 
                               parameter is NULL, then the current platform 
                               language is used.  The format of Language must 
                               follow the language format assumed in the HII Database.

  @retval NULL   The package list specified by PackageListGuid is not present in the
                 HII Database.
  @retval NULL   The string specified by StringId is not present in the string package.
  @retval Other  The string was returned.

**/
EFI_STRING
EFIAPI
HiiGetPackageString (
  IN CONST EFI_GUID  *PackageListGuid,
  IN EFI_STRING_ID   StringId,
  IN CONST CHAR8     *Language  OPTIONAL
  )
;

/**
  Retrieves a pointer to a Null-terminated ASCII string containing the list 
  of languages that an HII handle in the HII Database supports.  The returned 
  string is allocated using AllocatePool().  The caller is responsible for freeing
  the returned string using FreePool().  The format of the returned string follows
  the language format assumed in the HII Database.
  
  If HiiHandle is NULL, then ASSERT().

  @param[in]  HiiHandle  A handle that was previously registered in the HII Database.

  @retval NULL   HiiHandle is not registered in the HII database
  @retval NULL   There are not enough resources available to retrieve the suported 
                 languages.
  @retval NULL   The list of suported languages could not be retrieved.
  @retval Other  A pointer to the Null-terminated ASCII string of supported languages.

**/
CHAR8 *
EFIAPI
HiiGetSupportedLanguages (
  IN EFI_HII_HANDLE           HiiHandle
  )
;

#endif
