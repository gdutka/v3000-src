/** @file
  Internal header file for DeviceInfo2Lib instances.

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _DEVICE_INFO_2_LIB_INTERNAL_H_
#define _DEVICE_INFO_2_LIB_INTERNAL_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/DevicePath.h>

#define IS_PCI_ROOT(DevNodePtr)          (((ACPI_HID_DEVICE_PATH *) DevNodePtr)->HID == EFI_PNP_ID (0x0a03))

typedef struct _H2O_DEVICE_INFO_PCD_DATA {
  UINTN                                PcdTokenNum;
  UINT32                               AttribCount;
  CHAR8                                **AttribAsciiStrPtrArray;
  CHAR8                                **ValueAsciiStrPtrArray;
  EFI_DEVICE_PATH_PROTOCOL             *DevPath;
  UINTN                                DevPathBufferSize;
} H2O_DEVICE_INFO_PCD_DATA;

/**
 Enumerate all device info PCDs in gH2ODeviceInfo2TokenSpaceGuid.

 @param[out] DevInfoList        Pointer to a returned pointer to a device info data list

 @retval EFI_SUCCESS            Enumerate all device info PCDs successfully.
 @retval EFI_OUT_OF_RESOURCES   Allocate memory failed.
 @retval Other                  Fail to locate EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL.
**/
EFI_STATUS
EnumerateAllDevInfoPcd (
  OUT H2O_DEVICE_INFO_PCD_DATA         **DevInfoList
  );

/**
 Get device info PCD data.

 @param[in] PcdTokenNum         Unsigned integer that specifies the PCD token associated with a device's information.

 @return Pointer to the device info PCD data or NULL if not found.
**/
H2O_DEVICE_INFO_PCD_DATA *
GetDevInfoPcdData (
  IN UINTN                             PcdTokenNum
  );

extern EFI_GUID mInternalDeviceInfo2ListGuid;
#endif
