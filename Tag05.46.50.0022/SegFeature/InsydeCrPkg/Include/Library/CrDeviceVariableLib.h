/** @file
  This Library will install CrDeviceVariableLib for reference.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CR_DEVLIB_H_
#define _CR_DEVLIB_H_

#include <Protocol/ConsoleRedirectionService.h>

typedef enum {
  NORMAL_ITEM = 0,
  ASTERISK_ITEM,
  INVALID_ITEM
} ITEM_TYPE;

//
// === Public Struct ===
//
#pragma pack(1)

typedef struct _CR_DEVICE_SETTING {
  UINT8                     Exist;
  UINT8                     ItemType;
  CR_DEVICE_ATTRIBUTES      Attribute;
  CHAR16                    DevName[0];
  //
  //Note: DevName is follow by DevicePath. but not define in this struct.
  //      We can using GetDevicePathFromCrDevSetting() to get this column.
  //
  //EFI_DEVICE_PATH         DevPath;
  //
} CR_DEVICE_SETTING;

#pragma pack()

//[-start-220208-IB04880004-add]//
EFI_STATUS
CrDeviceSettingLoadDefault (
  IN     UINTN              DevIndex,
  IN OUT CR_DEVICE_SETTING  *Dev
  );
//[-end-220208-IB04880004-add]//

EFI_STATUS
EFIAPI
UpdateOrDeleteCrDevVarWithDevInfo (
  IN  UINT8                    Index,
  IN  CR_DEVICE_SETTING        *DevInfo
  );

EFI_DEVICE_PATH*
EFIAPI
GetDevicePathFromCrDevSetting (
  IN  CR_DEVICE_SETTING        *Buffer
  );

EFI_STATUS
EFIAPI
GetCrDevInfoFromVariable (
  IN  UINT8                    Index,
  OUT CR_DEVICE_SETTING        **DevInfo
  );

EFI_STATUS
EFIAPI
PcdDevicesToVariables (
  IN  BOOLEAN                  LoadDefault
  );

EFI_STATUS
EFIAPI
CreateNewCrDeviceVariable (
  IN  CHAR16                    *DevName,
  IN  EFI_DEVICE_PATH           *DevPath
  );

EFI_STATUS
EFIAPI
UpdateDeviceSettingToCrConfig (
  VOID
  );

/**

  For BIOS upgrade/downgrade case, if CrconFig variable size doesn't match with CR_CONFIGURATION structure size.
  Retrieve the default settings from default variable store region and create new CrConfig variable. Then, sync 
  the value from old CrConfig variale.

  @param [IN]*OldCrConfigVar  A pointer to old CrConfig data buffer.
  @param [IN]OldCrConfigSize  The size of old CrConfig variable.

  @retval EFI_SUCCESS    Successfully
  @retval Other          Fail.

**/
EFI_STATUS
DealCrConfigVariableUpdate (
  IN UINT8          *OldCrConfigVar,
  IN UINTN          OldCrConfigSize
  );

/**
  
  This function will lock CrDevVar### variables by EdkiiVariableLockProtocol..

  @retval EFI_SUCCESS    Successfully
  @retval Other          Fail.

**/
EFI_STATUS
LockCrDeviceVar (
  VOID
  );

#endif

