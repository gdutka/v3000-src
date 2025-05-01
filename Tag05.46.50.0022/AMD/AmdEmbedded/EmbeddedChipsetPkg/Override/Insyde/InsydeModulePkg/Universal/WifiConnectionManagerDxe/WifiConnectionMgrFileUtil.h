/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The file operation functions for wireless connection driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __EFI_WIFI_MGR_FILE_UTIL__
#define __EFI_WIFI_MGR_FILE_UTIL__

#include "WifiConnectionMgrDxe.h"

/**
  Worker function that prints an EFI_GUID into specified Buffer.

  @param[in]     Guid          Pointer to GUID to print.
  @param[in]     Buffer        Buffer to print Guid into.
  @param[in]     BufferSize    Size of Buffer.

  @retval    Number of characters printed.
**/
UINTN
GuidToString (
  IN  EFI_GUID          *Guid,
  IN  CHAR16            *Buffer,
  IN  UINTN             BufferSize
  );

/**
  Convert a String to Guid Value.

  @param[in]   Str        Specifies the String to be converted.
  @param[in]   StrLen     Number of Unicode Characters of String (exclusive \0)
  @param[out]  Guid       Return the result Guid value.

  @retval    EFI_SUCCESS           The operation is finished successfully.
  @retval    EFI_NOT_FOUND         Invalid string.
**/
EFI_STATUS
StringToGuid (
  IN   CHAR16           *Str,
  IN   UINTN            StrLen,
  OUT  EFI_GUID         *Guid
  );

/**
  Read file content into BufferPtr, the size of the allocate buffer 
  is *FileSize plus AddtionAllocateSize.

  @param[in]       FileHandle            The file to be read.
  @param[in, out]  BufferPtr             Pointers to the pointer of allocated buffer.
  @param[out]      FileSize              Size of input file
  @param[in]       AddtionAllocateSize   Addtion size the buffer need to be allocated. 
                                         In case the buffer need to contain others besides the file content.
  
  @retval   EFI_SUCCESS                  The file was read into the buffer.
  @retval   EFI_INVALID_PARAMETER        A parameter was invalid.
  @retval   EFI_OUT_OF_RESOURCES         A memory allocation failed.
  @retval   others                       Unexpected error.
**/
EFI_STATUS
ReadFileContent (
  IN      EFI_FILE_HANDLE           FileHandle,
  IN OUT  VOID                      **BufferPtr,
     OUT  UINTN                     *FileSize,
  IN      UINTN                     AddtionAllocateSize
  );

/**
  Get the expected cert from cert database.

  @param[in]    VariableName        The variable name of the vendor's signature database.
  @param[in]    ExpectCertGuid      The GUID of expected cert.
  @param[out]   CertSize            The size of the cert returned, can be set to NULL

  @return the expected cert or NULL.
**/
EFI_SIGNATURE_DATA*
GetFromCertDatabase (
  IN  CHAR16                         *VariableName,
  IN  EFI_GUID                       *ExpectCertGuid,
  OUT UINTN                          *CertSize
  );

/**
  Enroll Cert into cert database.

  @param[in] FileContext     A file hander points to the cert.
  @param[in] CertGuid        The GUID for this cert.
  @param[in] VariableName    Variable name of CA database.

  @retval   EFI_SUCCESS            New Cert enrolled successfully.
  @retval   EFI_INVALID_PARAMETER  The parameter is invalid.
  @retval   EFI_UNSUPPORTED        The Cert file is unsupported type.
  @retval   others                 Fail to enroll Cert data.
**/
EFI_STATUS
EnrollToCertDatabase (
  IN  WIFI_MGR_FILE_CONTEXT          *FileContext,
  IN  EFI_GUID                       *CertGuid,
  IN  CHAR16                         *VariableName
  );

/**
  Update the CA cert base on the input file path info.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  FilePath            Point to the file path.

  @retval TRUE   Exit caller function.
  @retval FALSE  Not exit caller function.
**/
BOOLEAN
UpdateCAFromFile (
  IN  WIFI_MGR_PRIVATE_DATA           *Private,
  IN  EFI_DEVICE_PATH_PROTOCOL        *FilePath
  );

/**
  Update the Private Key base on the input file path info.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  FilePath            Point to the file path.

  @retval TRUE   Exit caller function.
  @retval FALSE  Not exit caller function.
**/
BOOLEAN
UpdatePrivateKeyFromFile (
  IN  WIFI_MGR_PRIVATE_DATA           *Private,
  IN  EFI_DEVICE_PATH_PROTOCOL        *FilePath
  );

#endif
