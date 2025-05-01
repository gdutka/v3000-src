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

#ifndef __HII_DB_LIB_H__
#define __HII_DB_LIB_H__

#include <Protocol/HiiDatabase.h>

extern EFI_HII_DATABASE_PROTOCOL *gHiiDatabase;

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// HiiLib Functions
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/**
  Registers a list of packages in the HII Database and returns the HII Handle
  associated with that registration.  If an HII Handle has already been registered
  with the same PackageListGuid and DeviceHandle, then NULL is returned.  If there
  are not enough resources to perform the registration, then NULL is returned.
  If an empty list of packages is passed in, then NULL is returned.  If the size of
  the list of package is 0, then NULL is returned.

  The variable arguments are pointers that point to package headers defined 
  by UEFI VFR compiler and StringGather tool.

  #pragma pack (push, 1)
  typedef struct {
    UINT32                  BinaryLength;
    EFI_HII_PACKAGE_HEADER  PackageHeader;
  } EDKII_AUTOGEN_PACKAGES_HEADER;
  #pragma pack (pop)
  
  @param[in]  PackageListGuid  The GUID of the package list.
  @param[in]  DeviceHandle     If not NULL, the Device Handle on which 
                               an instance of DEVICE_PATH_PROTOCOL is installed.
                               This Device Handle uniquely defines the device that 
                               the added packages are associated with.
  @param[in]  ...              The variable argument list that contains pointers 
                               to packages terminated by a NULL.

  @retval NULL   An HII Handle has already been registered in the HII Database with
                 the same PackageListGuid and DeviceHandle.
  @retval NULL   The HII Handle could not be created.
  @retval NULL   An empty list of packages was passed in.
  @retval NULL   All packages are empty.
  @retval Other  The HII Handle associated with the newly registered package list.

**/
EFI_HII_HANDLE
EFIAPI
HiiAddPackages (
  IN CONST EFI_GUID    *PackageListGuid,
  IN       EFI_HANDLE  DeviceHandle  OPTIONAL,
  ...
  )
;

/**
  Removes a package list from the HII database.

  If HiiHandle is NULL, then ASSERT().
  If HiiHandle is not a valid EFI_HII_HANDLE in the HII database, then ASSERT().

  @param[in]  HiiHandle   The handle that was previously registered in the HII database

**/
VOID
EFIAPI
HiiRemovePackages (
  IN      EFI_HII_HANDLE      HiiHandle
  )
;


/**
  Retrieves the array of all the HII Handles or the HII handles of a specific
  package list GUID in the HII Database.
  This array is terminated with a NULL HII Handle.
  This function allocates the returned array using AllocatePool().
  The caller is responsible for freeing the array with FreePool().

  @param[in]  PackageListGuid  An optional parameter that is used to request 
                               HII Handles associated with a specific
                               Package List GUID.  If this parameter is NULL,
                               then all the HII Handles in the HII Database
                               are returned.  If this parameter is not NULL,
                               then zero or more HII Handles associated with 
                               PackageListGuid are returned.

  @retval NULL   No HII handles were found in the HII database
  @retval NULL   The array of HII Handles could not be retrieved
  @retval Other  A pointer to the NULL terminated array of HII Handles

**/
EFI_HII_HANDLE *
EFIAPI
HiiGetHiiHandles (
  IN CONST EFI_GUID  *PackageListGuid  OPTIONAL
  )
;


#endif
