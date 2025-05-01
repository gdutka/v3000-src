/** @file
  Provides global variables that are pointers to the UEFI HII related protocols.
  All of the UEFI HII related protocols are optional, so the consumers of this
  library class must verify that the global variable pointers are not NULL before
  use.   

Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under 
the terms and conditions of the BSD License that accompanies this distribution.  
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.                                            

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __DXETAGVERSION_LIB_H__
#define __DXETAGVERSION_LIB_H__


#include <Protocol/Pcd.h>
#include <Protocol/PiPcd.h>

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>


typedef struct {
  CHAR8     *FeatureStr;
  CHAR8     *VersionStr;
} FEATURE_VERSION;

typedef struct {
  UINTN               Count;
  FEATURE_VERSION     *Table;
} H2O_FEATURES_VER_TABLE_PROTOCOL;

/**
  Get the table that contains name string pointer and version string pointer.
  If count != NULL, get the number of name and version pairs.

  @param[IN] Table       FEATURE_VERSION pointer.
  @param[IN] Count       The number of name and version pairs.

  @retval    EFI_SUCCESS Get table successfully.
**/
EFI_STATUS
EFIAPI
GetFeatureVersionTable (
  IN FEATURE_VERSION  **Table,
  IN UINTN            *Count OPTIONAL
  );

/**
  Get the string pointer of version.

  @param[IN] FeatureName  Name string of package.

  @retval    The pointer of version string. 
**/
CHAR8 *
EFIAPI
GetFeatureVersionStr (
  IN CHAR8  *FeatureName
  );

/**
  Get version string and convert it into hex UINTN.

  @param[IN] FeatureName  Name string of package.

  @retval    Decimal UINTN of the version string.   
**/
UINTN
EFIAPI
GetFeatureVersionUint (
  IN CHAR8  *FeatureName
  );

extern EFI_GUID gSegFeatureVerTokenSpaceGuid;
extern EFI_GUID gH2OSegFeatureVersionProtocolGuid;
#endif
