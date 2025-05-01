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
#ifndef _DEVICE_INFO_HELPER_H
#define _DEVICE_INFO_HELPER_H

#include <Uefi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/DevicePath.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PciLib.h>
#include <Library/DeviceInfo2Lib.h>

EFI_STATUS
GetDevicePathPciLocation (
  IN CONST EFI_DEVICE_PATH   *DevicePath,
  OUT UINTN                  *BusNumber,
  OUT UINTN                  *DeviceNumber,
  OUT UINTN                  *FunctionNumber
  );

EFI_STATUS
GetDeviecInfoTokenNumByPciLocation (
  IN UINTN                   BusNumber,
  IN UINTN                   DeviceNumber,
  IN UINTN                   FunctionNumber,
  OUT UINTN                  *TokenNum
  );

#endif
