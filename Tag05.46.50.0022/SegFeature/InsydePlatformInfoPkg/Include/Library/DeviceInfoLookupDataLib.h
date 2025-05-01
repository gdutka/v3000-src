/** @file
  Routine for collecting data for Device Info Lookup Protocol.

;******************************************************************************
;* Copyright (c) 2021 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_DEVICE_INFO_LOOKUP_DATA_LIB_H_
#define H2O_DEVICE_INFO_LOOKUP_DATA_LIB_H_

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/H2ODeviceInfoLookup.h>

#include <Library/HiiExLib.h>

/**
  Finds a Device Path node of the given type, sub-type and length.

  @param[in]  DevicePath  Pointer to the start of a Device Path.
  @param[in]  Type        Device Path type to find.
  @param[in]  SubType     Device Path sub-type to find. To search for a
                          Device Path node without a specified sub-type,
                          specify ((UINTN) -1) to this parameter. Values other
                          than [0, 255] and -1 are reserved.
  @param[in]  Length      Length of the Device Path node in bytes. The Length
                          parameter is ignored if less than the minimum size of
                          a Device Path node (4 bytes) is specified. 
  @param[out] Offset      Returns the offset to the matching Device Path node,
                          in bytes, from the start of the Device Path. If the
                          Device Path node matching the type is not found,
                          returns an offset to the End Device Path node or the
                          first Device Path node that is considered invalid.

  @retval EFI_SUCCESS     A Device Path node matching the type is found.
  @retval EFI_NOT_FOUND   Device Path node matching the type is not found.

**/
EFI_STATUS
FindDevicePathNodeByType (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN UINT8                           Type,
  IN UINTN                           SubType,
  IN UINT16                          Length,
  OUT UINTN                          *Offset OPTIONAL
  );

/**
  Internal function called by LoadDeviceInfoLookupData to process Device Info or
  Device Info 2 PCDs.

  @param  Protocol              Device Info Lookup protocol instance.
  @param  HiiHandle             HII handle that registered string packages.
  @param  StringTokenNameToIds  Table from AutoGen.c that maps string token
                                names to their IDs.

  @retval EFI_SUCCESS           Success.
  @retval Other                 An error occurred.

**/
EFI_STATUS
EFIAPI
LoadDeviceInfoLookupDataInternal (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *Protocol,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN PCD_STRING_NAME_TO_ID                *StringTokenNameToIds
  );

/**

**/
VOID
InitDeviceInfoLookupProtocol (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  **DeviceInfoLookupInterface
  );

#endif // H2O_DEVICE_INFO_LOOKUP_DATA_LIB_H_
