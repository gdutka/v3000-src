/** @file
  CrVariableUpdate Header

;******************************************************************************
;* Copyright (c) 2012 - 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CR_VARIABLE_UPDATE_H_
#define _CR_VARIABLE_UPDATE_H_

#include "CrServiceDxe.h"

extern CHAR16                *mConInVarName;
extern CHAR16                *mConOutVarName;
extern EFI_GUID              *mConVarGuid;

//[-start-211214-IB18180097-remove]//
//#define  CR_ACTIVE_VARIABLE_NAME   L"CrActiveDevice"
//[-end-211214-IB18180097-remove]//

VOID
CreateCrDeviceVarList (
  VOID
  );

VOID
UpdateCrDeviceSetting (
  VOID
  );

VOID
FreeCrDevicesVarList (
  VOID
  );

VOID
CheckAndSaveConnectResult (
  IN  CHAR16                    *ConVarName,
  IN  EFI_GUID                  *ConVarGuid
  );

//[-start-211214-IB18180097-remove]//
//VOID
//CreateNewActiveCrDevice (
//  VOID
//  );
//[-end-211214-IB18180097-remove]//

VOID
FindAsteriskDevicePath (
  VOID
  );

VOID
InitializeCrDeviceList (
  VOID
  );

VOID
UpdateDevicesExistStatus (
  VOID
  );

VOID
//[-start-211214-IB18180097-modify]//
UpdateConInConOutVar (
//[-end-211214-IB18180097-modify]//
  VOID
  );

EFI_STATUS
UpdateConsoleVariable (
  IN  CHAR16                    *ConVarName,
  IN  EFI_GUID                  *ConVarGuid,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
  );

//[-start-211209-IB18180097-add]//
BOOLEAN
IsCrDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL  *UartDevPath
  );

BOOLEAN
IsUartDevice (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevPath
  );

BOOLEAN
IsDpHeadMatch (
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *BaseDevPath
  );

BOOLEAN
IsUartDeviceSame (
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath,
  IN  EFI_DEVICE_PATH_PROTOCOL  *CmpToDevPath
  );
//[-end-211209-IB18180097-add]//

BOOLEAN
IsInclusiveOfSelectDevPath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevPathPool,
  IN  EFI_DEVICE_PATH_PROTOCOL  *SelectDevPath
  );

BOOLEAN
IsEqualDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath1,
  IN  EFI_DEVICE_PATH_PROTOCOL  *DevicePath2
  );

#endif

