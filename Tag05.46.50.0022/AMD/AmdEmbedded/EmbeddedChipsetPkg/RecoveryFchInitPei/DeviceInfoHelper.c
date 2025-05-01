/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "DeviceInfoHelper.h"

BOOLEAN
EFIAPI
IsDevicePathAcpiType (
  IN CONST EFI_DEVICE_PATH   *Node
  )
{
  ASSERT (Node != NULL);
  return (BOOLEAN) ((DevicePathType (Node) == ACPI_DEVICE_PATH) && (DevicePathSubType (Node) == ACPI_DP));
}

BOOLEAN
EFIAPI
IsDevicePathPciRootType (
  IN CONST EFI_DEVICE_PATH   *Node
  )
{
  ACPI_HID_DEVICE_PATH       *AcpiNode;

  ASSERT (Node != NULL);
  if (IsDevicePathAcpiType (Node)) {
    AcpiNode = (ACPI_HID_DEVICE_PATH*)Node;
    if (AcpiNode->HID == EFI_PNP_ID (0x0a03)) {
      return TRUE;
    }
  }

  return FALSE;
}

BOOLEAN
EFIAPI
IsDevicePathPciType (
  IN CONST EFI_DEVICE_PATH  *Node
  )
{
  ASSERT (Node != NULL);
  return (BOOLEAN) ((DevicePathType (Node) == HARDWARE_DEVICE_PATH) && (DevicePathSubType (Node) == HW_PCI_DP));
}

EFI_STATUS
GetDevicePathPciLocation (
  IN CONST EFI_DEVICE_PATH   *DevicePath,
  OUT UINTN                  *BusNumber,
  OUT UINTN                  *DeviceNumber,
  OUT UINTN                  *FunctionNumber
  )
{
  EFI_STATUS                 Status;
  CONST EFI_DEVICE_PATH      *TmpDevicePath;
  PCI_DEVICE_PATH            *PciDevicePath;
  UINTN                      BridgeSecondaryBus;
  UINTN                      Bus;
  UINTN                      Device;
  UINTN                      Function;
  UINT8                      BaseClassCode;
  UINT8                      SubClassCode;

  Status = EFI_NOT_FOUND;
  TmpDevicePath = DevicePath;
  BridgeSecondaryBus = 0;
  Bus = 0;
//[-start-230817-IB20840016-add]//
  Device = 0;
  Function = 0;
//[-end-230817-IB20840016-add]//

  while (!IsDevicePathEnd (TmpDevicePath)) {
    if (IsDevicePathEndInstance (TmpDevicePath)) {
      //
      // If DevicePath is a multi-instance device path,
      // the function will operate on the first instance
      //
      break;
    }

    Status = EFI_SUCCESS;
    Bus = BridgeSecondaryBus;
    if ((!IsDevicePathPciRootType (TmpDevicePath)) && (!IsDevicePathPciType (TmpDevicePath))) {
      break;
    }
    if (IsDevicePathPciType (TmpDevicePath)) {
      PciDevicePath = (PCI_DEVICE_PATH*)TmpDevicePath;
      Device = PciDevicePath->Device;
      Function = PciDevicePath->Function;
      BaseClassCode = PciRead8 ((UINTN)PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 0x02));
      SubClassCode = PciRead8 ((UINTN)PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 0x01));
      if (BaseClassCode == PCI_CLASS_BRIDGE && SubClassCode == PCI_CLASS_BRIDGE_P2P) {
        BridgeSecondaryBus = PciRead8 ((UINTN)PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
      }
    }
    TmpDevicePath = NextDevicePathNode (TmpDevicePath);
  }

  if (Status == EFI_SUCCESS) {
    *BusNumber = Bus;
    *DeviceNumber = Device;
    *FunctionNumber = Function;
  }

  return Status;
}

EFI_STATUS
GetDeviecInfoTokenNumByPciLocation (
  IN UINTN                   BusNumber,
  IN UINTN                   DeviceNumber,
  IN UINTN                   FunctionNumber,
  OUT UINTN                  *TokenNum
  )
{
  EFI_STATUS                 Status;
  UINTN                      TmpTokenNum;
  UINT32                     DeviceAttribCount;  
  EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  UINTN                      Bus;
  UINTN                      Device;
  UINTN                      Function;

  Status = EFI_NOT_FOUND;

  for (TmpTokenNum = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, 0)
       ; TmpTokenNum != 0
       ; TmpTokenNum = LibPcdGetNextToken (&gH2ODeviceInfo2TokenSpaceGuid, TmpTokenNum)
       ) {

    Status = H2OGetDeviceInfo (TmpTokenNum, &DevicePath, &DeviceAttribCount);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = GetDevicePathPciLocation (DevicePath, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if ((BusNumber == Bus) && (DeviceNumber == Device) && (FunctionNumber == Function)) {
      *TokenNum = TmpTokenNum;
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}



