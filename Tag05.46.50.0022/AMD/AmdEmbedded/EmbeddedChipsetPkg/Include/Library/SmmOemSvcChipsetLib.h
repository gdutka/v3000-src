/** @file
  Definition for Smm OEM Services Chipset Lib.

;******************************************************************************
;* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _SMM_OEM_SVC_CHIPSET_LIB_H_
#define _SMM_OEM_SVC_CHIPSET_LIB_H_
#include <Uefi.h>
#include <H2OIhisi.h>
#include <Protocol/LegacyBios.h>

typedef struct {
  UINTN               RegNum;
  UINT8               PciBus;
  UINT8               PciDev;
  UINT8               PciFun;
  UINT8               *PciRegTable;
  UINT32              *PciRegTableSave;
} SR_DEVICE;

typedef struct {
  UINT8               Bus;
  UINT8               Dev;
  UINT8               Fun;
} P2P_BRIDGE;

typedef struct {
  SR_DEVICE           Device;
  P2P_BRIDGE          P2PB;
} SR_OEM_DEVICE;

typedef struct {
  SR_DEVICE           *Chipset;
  SR_OEM_DEVICE       *Oem;
} SR_TABLE;

/**
 This function provides OEM to add additional PCI registers which is not listed 
 on global register table PciDeviceSubResList [].It will replace global register
 table PciDeviceOemSubResList[].

 Boot to OS/"S3 resume", call this function to get OEM PCI register table to 
 save/restore OEM PCI register Value. 

 @param[out]        *PciDeviceOemList   Points to the list of OEM PCI device registers which must be saved on an S3.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
*/
EFI_STATUS
OemSvcGetSaveRestorePciDeviceOemList (
  OUT SR_OEM_DEVICE                     **PciDeviceOemList
  );

/**
 This function provides an interface to get OemInt15VbiosFunctionHook. If function is 
 added on INT15 trigger, OemInt15VbiosFunctionlist and Size are updated. When user 
 triggered INT15, it compared EAX register and OemInt15VbiosFunctionlist data. If data 
 match, it will call VbiosHookCallBack function. 
 Caller will use this to install additional VbiosHookCallBack function.

 @param[out]        *OemInt15VbiosFunctionlist  Point to OemInt15VbiosFunctionHook array.
                                                This array record EAX register value on INT15 trigger
 @param[out]        *Size               The number of entries in OemInt15VbiosFunctionHook array.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
*/
EFI_STATUS
OemSvcGetOemInt15VbiosFunctionlist (
  IN OUT UINT16                         **OemInt15VbiosFunctionlist,
  OUT UINT16                            *Size
  );

/**
 This function provides an interface to do additional VbiosHookCallBack function that are list on 
 OemInt15VbiosFunctionHook array.

 @param[in]         Int15FunNum         Int15 function number.
 @param[in out]     CpuRegs             The structure containing CPU Registers (AX, BX, CX, DX etc.).
 @param[in]         Context             Context.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcVbiosHookCallBack (
  IN UINT32                             Int15FunNum,
  IN OUT EFI_IA32_REGISTER_SET          *CpuRegs,
  IN VOID                               *Context
  );



/**
 This function provides a hook for IHISI sub-function AH=12h and should be implemented to get OemFlashMap. 
 Please refer to Insyde H2O internal Soft-SMI interface (IHISI) Specification.

 @param[in out]     *MediaOemRomMapSize           On entry, the size of FBTS_PLATFORM_ROM_MAP matrix.
                                                  On exit , the size of updated FBTS_PLATFORM_ROM_MAP matrix.
 @param[in out]     **MediaOemRomMap              On entry, points to FBTS_PLATFORM_ROM_MAP matrix.
                                                  On exit , points to updated FBTS_PLATFORM_ROM_MAP matrix.
 @param[in out]     *MediaOemPrivateRomMapSize    On entry, the size of FBTS_PLATFORM_PRIVATE_ROM matrix.
                                                  On exit , the size of updated FBTS_PLATFORM_PRIVATE_ROM matrix.
 @param[in out]     **MediaOemPrivateRomMap       On entry, points to FBTS_PLATFORM_PRIVATE_ROM matrix.
                                                  On exit , points to updated FBTS_PLATFORM_PRIVATE_ROM matrix.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcIhisiS12HookGetOemFlashMap (
  IN OUT UINTN                          *MediaOemRomMapSize,
  IN OUT FBTS_PLATFORM_ROM_MAP          **MediaOemRomMap,
  IN OUT UINTN                          *MediaOemPrivateRomMapSize,
  IN OUT FBTS_PLATFORM_PRIVATE_ROM      **MediaOemPrivateRomMap
  );

#endif
