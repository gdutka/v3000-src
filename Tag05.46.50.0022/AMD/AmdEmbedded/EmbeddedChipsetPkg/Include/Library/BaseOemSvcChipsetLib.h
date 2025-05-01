/** @file
  Definition for Base OEM Services Chipset Lib.

;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _BASE_OEM_SVC_CHIPSET_LIB_H_
#define _BASE_OEM_SVC_CHIPSET_LIB_H_

#include <Uefi.h>

/**
 This function offers an interface to provide hook function for OEM to implement save and restore KBC
 
 @param[in]         SaveRestoreFlag     Save/Restore Flag. TRUE means restore Keyboard command byte; FALSE means Save Keyboard command byte.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcSaveRestoreKbc (
  IN  BOOLEAN                       SaveRestoreFlag
  );

/**
 Get power state from EC.  If power state cannot be determined,
 battery powered is assumed.
 
 @param[in, out]    PowerState          A boolean pointer. TRUE means AC power; FALSE means Battery power.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcPowerState (
  IN OUT  BOOLEAN                       *PowerState
  );

/**
 Get major/minor version from EC. 
 
 @param[out]        ReadEcVersionStatus The status of read EC version.
 @param[out]        MajorNum            EC major number.
 @param[out]        MinorNum            EC minor number.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcVersion (
  OUT UINT8        *FrontNum,
  OUT UINT8        *MediumNum,
  OUT UINT8        *BackNum
  );

/**
 Set DSW mode to EC. 
 
 @param[out]        SetDswModeStatus    The status of set DSW mode.
 @param[in]         DswMode             DSW mode.
                                          0x0 Disable DSW mode
                                          0x1 Enable DSW in S5 (DC)
                                          0x2 Enable DSW in S5 (Always)
                                          0x3 Enable DSW in S4-S5 (DC)
                                          0x4 Enable DSW in S4-S5 (Always)

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcSetDswMode (
  OUT EFI_STATUS   *SetDswModeStatus,
  IN  UINT8        DswMode
  );

/**
 Get Lid state from EC.
 
 @param[out]        EcGetLidState       The status of get Lid.
 @param[out]        LidIsOpen           TRUE: Lid is open; FALSE: Lid is close.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcGetLidState (
  OUT EFI_STATUS   *EcGetLidState,
  OUT UINT8        *LidIsOpen
  );

/**
 Send Critical thermal to EC.
 
 @param[in]         CriticalThermal     The temperature of Critical Thermal.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcSetCriticalThermal (
  IN UINT8         Temperature
  );

/**
 Set Low Power Mode to EC. 
 
 @param[in]         LowPowerMode        Low Power Mode.
                                          TRUE  Enable Low Power Mode
                                          FALSE Disable Low Power Mode

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcSetLowPowerMode (
  IN  BOOLEAN      LowPowerMode
  );

/**
 Detect EC Present.
 
 @param[out]        *CommandStatus      Command status.
 @param[out]        Present             TRUE: EC is presnet; FALSE: EC is absent.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcDetectEcPresent (
  OUT EFI_STATUS    *CommandStatus,
  OUT BOOLEAN       *Present
  );

/**
 Get PCIe dock status from EC.
 
 @param[out]        *CommandStatus      Command status.
 @param[out]        *Dock               Dock Status.
                                        The bit0 is PCIe Dock Status, 1 = docked

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcGetPcieDockStatus (
  OUT EFI_STATUS   *CommandStatus,
  OUT UINT8        *Dock
  );
/**
 Read the EC memory.

 @param[out]      *ReadEcRamStatus   Read EC ram status.
 @param[in, out]  *DataBuffer        For input, DataBuffer means offset, 
                                     while it means data for output.
                                    
 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcReadEcRam (
  OUT EFI_STATUS *ReadEcRamStatus,
  IN OUT UINT8   *DataBuffer
  );

#endif
