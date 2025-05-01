/** @file
  HII Library implementation that uses DXE protocols and services.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
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
#include <Library/HiiDbLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>

#include <Protocol/HiiDatabase.h>

//
// Template used to mark the end of a list of packages 
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST EFI_HII_PACKAGE_HEADER  mEndOfPakageList = {
  sizeof (EFI_HII_PACKAGE_HEADER),
  EFI_HII_PACKAGE_END
};

/**
  Extract Hii package list GUID for given HII handle.

  If HiiHandle could not be found in the HII database, then ASSERT.
  If Guid is NULL, then ASSERT.

  @param  Handle              Hii handle
  @param  Guid                Package list GUID

  @retval EFI_SUCCESS         Successfully extract GUID from Hii database.

**/
EFI_STATUS
EFIAPI
InternalHiiExtractGuidFromHiiHandle (
  IN      EFI_HII_HANDLE      Handle,
  OUT     EFI_GUID            *Guid
  )
{
  EFI_STATUS                   Status;
  UINTN                        BufferSize;
  EFI_HII_PACKAGE_LIST_HEADER  *HiiPackageList;
  EFI_HII_DATABASE_PROTOCOL    *HiiDatabase;  

  ASSERT (Guid != NULL);
  ASSERT (Handle != NULL);

  //
  // Get HII PackageList
  //
  BufferSize = 0;
  HiiPackageList = NULL;

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = HiiDatabase->ExportPackageLists (HiiDatabase, Handle, &BufferSize, HiiPackageList);
  ASSERT (Status != EFI_NOT_FOUND);
  if (Status != EFI_BUFFER_TOO_SMALL){
    return EFI_NOT_FOUND;
  }

  HiiPackageList = AllocatePool (BufferSize);
  ASSERT (HiiPackageList != NULL);
  if (HiiPackageList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = HiiDatabase->ExportPackageLists (HiiDatabase, Handle, &BufferSize, HiiPackageList);
  if (EFI_ERROR (Status)) {
    FreePool (HiiPackageList);
    return EFI_NOT_FOUND;
  }

  //
  // Extract GUID
  //
  CopyGuid (Guid, &HiiPackageList->PackageListGuid);

  FreePool (HiiPackageList);

  return EFI_SUCCESS;
}

/**
  Registers a list of packages in the HII Database and returns the HII Handle
  associated with that registration.  If an HII Handle has already been registered
  with the same PackageListGuid and DeviceHandle, then NULL is returned.  If there
  are not enough resources to perform the registration, then NULL is returned.
  If an empty list of packages is passed in, then NULL is returned.  If the size of
  the list of package is 0, then NULL is returned.

  The variable arguments are pointers which point to package header that defined 
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

  @retval NULL   A HII Handle has already been registered in the HII Database with
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
{
  EFI_STATUS                   Status;
  VA_LIST                      Args;
  UINT32                       *Package;
  EFI_HII_PACKAGE_LIST_HEADER  *PackageListHeader;
  EFI_HII_HANDLE               HiiHandle;
  UINT32                       Length;
  UINT8                        *Data;
  EFI_HII_DATABASE_PROTOCOL    *HiiDatabase;    

  ASSERT (PackageListGuid != NULL);
  if (PackageListGuid == NULL) {
    return NULL;
  }

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  //
  // Calculate the length of all the packages in the variable argument list
  //
  ZeroMem (&Args, sizeof (Args));
  for (Length = 0, VA_START (Args, DeviceHandle); (Package = VA_ARG (Args, UINT32 *)) != NULL; ) {
    Length += (ReadUnaligned32 (Package) - sizeof (UINT32));
  }
  VA_END (Args);

  //
  // If there are no packages in the variable argument list or all the packages 
  // are empty, then return a NULL HII Handle
  //
  if (Length == 0) {
    return NULL;
  }

  //
  // Add the length of the Package List Header and the terminating Package Header 
  //
  Length += sizeof (EFI_HII_PACKAGE_LIST_HEADER) + sizeof (EFI_HII_PACKAGE_HEADER);

  //
  // Allocate the storage for the entire Package List
  //
  PackageListHeader = AllocateZeroPool (Length);

  //
  // If the Package List can not be allocated, then return a NULL HII Handle
  //
  if (PackageListHeader == NULL) {
    return NULL;
  }

  //
  // Fill in the GUID and Length of the Package List Header
  //
  CopyGuid (&PackageListHeader->PackageListGuid, PackageListGuid);
  PackageListHeader->PackageLength = Length;

  //
  // Initialize a pointer to the beginning if the Package List data
  //
  Data = (UINT8 *)(PackageListHeader + 1);

  //
  // Copy the data from each package in the variable argument list
  //
  for (VA_START (Args, DeviceHandle); (Package = VA_ARG (Args, UINT32 *)) != NULL; ) {
    Length = ReadUnaligned32 (Package) - sizeof (UINT32);
    CopyMem (Data, Package + 1, Length);
    Data += Length;
  }
  VA_END (Args);

  //
  // Append a package of type EFI_HII_PACKAGE_END to mark the end of the package list
  //
  CopyMem (Data, &mEndOfPakageList, sizeof (mEndOfPakageList));

  //
  // Register the package list with the HII Database
  //
  Status = HiiDatabase->NewPackageList (
                           HiiDatabase, 
                           PackageListHeader, 
                           DeviceHandle, 
                           &HiiHandle
                           );
  if (EFI_ERROR (Status)) {
    HiiHandle = NULL;
  }

  //
  // Free the allocated package list
  //
  FreePool (PackageListHeader);

  //
  // Return the new HII Handle
  //
  return HiiHandle;
}

/**
  Removes a package list from the HII database.

  If HiiHandle is NULL, then ASSERT.
  If HiiHandle is not a valid EFI_HII_HANDLE in the HII database, then ASSERT.

  @param[in]  HiiHandle   The handle that was previously registered in the HII database

**/
VOID
EFIAPI
HiiRemovePackages (
  IN      EFI_HII_HANDLE      HiiHandle
  )
{
  EFI_STATUS                   Status;
  EFI_HII_DATABASE_PROTOCOL    *HiiDatabase;  

  ASSERT (HiiHandle != NULL);

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR(Status)) {
    return;
  }
  
  Status = HiiDatabase->RemovePackageList (HiiDatabase, HiiHandle);
  ASSERT_EFI_ERROR (Status);
}


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
{
  EFI_STATUS                   Status;
  UINTN                        HandleBufferLength;
  EFI_HII_HANDLE               TempHiiHandleBuffer;
  EFI_HII_HANDLE               *HiiHandleBuffer;
  EFI_GUID                     Guid;
  UINTN                        Index1;
  UINTN                        Index2;
  EFI_HII_DATABASE_PROTOCOL    *HiiDatabase;  


  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR(Status)) {
    return NULL;
  }

  //
  // Retrieve the size required for the buffer of all HII handles.
  //
  HandleBufferLength = 0;
  Status = HiiDatabase->ListPackageLists (
                           HiiDatabase,
                           EFI_HII_PACKAGE_TYPE_ALL,
                           NULL,
                           &HandleBufferLength,
                           &TempHiiHandleBuffer
                           );

  //
  // If ListPackageLists() returns EFI_SUCCESS for a zero size, 
  // then there are no HII handles in the HII database.  If ListPackageLists() 
  // returns an error other than EFI_BUFFER_TOO_SMALL, then there are no HII 
  // handles in the HII database.
  //
  if (Status != EFI_BUFFER_TOO_SMALL) {
    //
    // Return NULL if the size can not be retrieved, or if there are no HII 
    // handles in the HII Database
    //
    return NULL;
  }

  //
  // Allocate the array of HII handles to hold all the HII Handles and a NULL terminator
  //
  HiiHandleBuffer = AllocateZeroPool (HandleBufferLength + sizeof (EFI_HII_HANDLE));
  if (HiiHandleBuffer == NULL) {
    //
    // Return NULL if allocation fails.
    //
    return NULL;
  }

  //
  // Retrieve the array of HII Handles in the HII Database
  //
  Status = HiiDatabase->ListPackageLists (
                           HiiDatabase,
                           EFI_HII_PACKAGE_TYPE_ALL,
                           NULL,
                           &HandleBufferLength,
                           HiiHandleBuffer
                           );
  if (EFI_ERROR (Status)) {
    //
    // Free the buffer and return NULL if the HII handles can not be retrieved.
    //
    FreePool (HiiHandleBuffer);
    return NULL;
  }

  if (PackageListGuid == NULL) {
    //
    // Return the NULL terminated array of HII handles in the HII Database
    //
    return HiiHandleBuffer;
  } else {
    for (Index1 = 0, Index2 = 0; HiiHandleBuffer[Index1] != NULL; Index1++) {
      Status = InternalHiiExtractGuidFromHiiHandle (HiiHandleBuffer[Index1], &Guid);
      ASSERT_EFI_ERROR (Status);
      if (CompareGuid (&Guid, PackageListGuid)) {
        HiiHandleBuffer[Index2++] = HiiHandleBuffer[Index1];       
      }
    }
    if (Index2 > 0) {
      HiiHandleBuffer[Index2] = NULL;
      return HiiHandleBuffer;
    } else {
      FreePool (HiiHandleBuffer);
      return NULL;
    }
  }
}