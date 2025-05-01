/** @file


;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/DevicePath.h>

#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>

STATIC
BOOLEAN
DevicePathNodeMatchesType (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode,
  IN UINT8                           Type,
  IN UINTN                           SubType,
  IN UINT16                          Length
  )
{
  if (DevicePathNode == NULL) {
    return FALSE;
  }
  if (DevicePathType (DevicePathNode) != Type) {
    return FALSE;
  }
  if ((SubType <= 0xFF) && (DevicePathSubType (DevicePathNode) != SubType)) {
    return FALSE;
  }
  if ((Length >= sizeof (EFI_DEVICE_PATH_PROTOCOL)) && DevicePathNodeLength (DevicePathNode) != Length) {
    return FALSE;
  }
  return TRUE;
}

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
  )
{
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node;

  Node = DevicePath;
  while (TRUE) {
    if (Offset != NULL) {
      *Offset = (UINTN) ((CONST UINT8 *) Node - (CONST UINT8 *) DevicePath);
    }
    if (DevicePathNodeMatchesType (Node, Type, SubType, Length)) {
      return EFI_SUCCESS;
    }
    if (IsDevicePathEnd (Node) ||
        DevicePathNodeLength (Node) < sizeof (EFI_DEVICE_PATH_PROTOCOL))
    {
      break;
    }
    Node = NextDevicePathNode (Node);
  }
  //
  // Note: The Offset parameter is designed to never return a null value.
  // Whether a Device Path node is found is indicated by the return status.
  //
  return EFI_NOT_FOUND;
}
