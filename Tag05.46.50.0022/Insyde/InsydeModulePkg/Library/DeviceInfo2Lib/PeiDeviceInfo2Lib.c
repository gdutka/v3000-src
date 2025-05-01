/** @file
  This library class provides helper functions to access the Device Information.

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
#include <PiPei.h>
#include <Library/HobLib.h>
#include "DeviceInfo2LibInternal.h"

/**
  Get valid device info PCD data.

  @param[in]  Guid          The GUID to match with in the HOB list.

  @return Pointer to the device info PCD data or NULL if not found.

**/
STATIC
VOID *
GetValidGuidHob (
  IN CONST EFI_GUID         *Guid
)
{
  EFI_HOB_HANDOFF_INFO_TABLE    *HandOffHob;
  VOID                          *GuidHob;
  VOID                          **DevInfoListPtr;

  GuidHob = GetFirstGuidHob (Guid);
  if (GuidHob == NULL) {
    return NULL;
  }

  HandOffHob = GetHobList ();
  if (HandOffHob == NULL) {
    return NULL;
  }

  //
  // Check if data in HOB is in the available range.
  //
  DevInfoListPtr = GET_GUID_HOB_DATA (GuidHob);
  if ((UINTN)(*DevInfoListPtr) < (UINTN)HandOffHob ||
      (UINTN)(*DevInfoListPtr) > (UINTN)HandOffHob->EfiMemoryTop) {
    return NULL;
  }

  return GuidHob;
}

/**
  Get device info PCD data.

  @param[in] PcdTokenNum         Unsigned integer that specifies the PCD token associated with a device's information.

  @return Pointer to the device info PCD data or NULL if not found.
**/
H2O_DEVICE_INFO_PCD_DATA *
GetDevInfoPcdData (
  IN UINTN                             PcdTokenNum
  )
{
  UINTN                                Index;
  EFI_HOB_GUID_TYPE                    *GuidHob;
  VOID                                 **DevInfoListPtr;
  H2O_DEVICE_INFO_PCD_DATA             *DevInfoList;

  //
  // Device info list pointer is saved in an HOB. All of library instances can access the same database.
  //
  GuidHob = GetValidGuidHob (&mInternalDeviceInfo2ListGuid);
  if (GuidHob == NULL) {
    EnumerateAllDevInfoPcd (&DevInfoList);
    GuidHob = GetFirstGuidHob (&mInternalDeviceInfo2ListGuid);
    DevInfoListPtr = (GuidHob == NULL) ? BuildGuidHob (&mInternalDeviceInfo2ListGuid, sizeof (VOID *)) : GET_GUID_HOB_DATA (GuidHob);
    if (DevInfoListPtr == NULL) {
      return NULL;
    }
    ASSERT (DevInfoListPtr != NULL);
    *DevInfoListPtr = (VOID *)(UINTN)DevInfoList;
  } else {
    DevInfoListPtr = GET_GUID_HOB_DATA (GuidHob);
    DevInfoList = (H2O_DEVICE_INFO_PCD_DATA *)(*DevInfoListPtr);
  }

  if (DevInfoList == NULL) {
    return NULL;
  }

  for (Index = 0; DevInfoList[Index].PcdTokenNum != 0; Index++) {
    if (DevInfoList[Index].PcdTokenNum == PcdTokenNum) {
      return &DevInfoList[Index];
    }
  }

  return NULL;
}

