/** @file
  Definition for DXE Oem Services Chipset Lib.

;******************************************************************************
;* Copyright (c) 2013 - 2015, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _DXE_OEM_SVC_CHIPSET_LIB_H_
#define _DXE_OEM_SVC_CHIPSET_LIB_H_

#include <Uefi.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/UsbLegacyPlatform.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/BiosRegionLock.h>
#include <ChipsetSetupConfig.h>
#include <Library/BadgingSupportLib.h>

#define SCU_ACTION_LOAD_DEFAULT BIT0

//
// OemSvc function prototypes
//

/**
 This function provides an interface to hook before and after DxePlatformEntryPoint (PlatformDxe.inf).PlatformDxe.inf
 is responsible for setting up any platform specific policy or initialization information.

 @param[in out]     *SetupVariable      On entry, points to CHIPSET_CONFIGURATION instance.
                                        On exit , points to updated CHIPSET_CONFIGURATION instance.
 @param[in out]     *PciRootBridgeIo    On entry, points to EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL instance.
                                        On exit , points to updated EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL instance.
 @param[in out]     Flag                TRUE : Before DxePlatformEntryPoint.
                                        FALSE: After DxePlatformEntryPoint.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcHookPlatformDxe (
  IN  CHIPSET_CONFIGURATION            *SetupVariable,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  IN OUT  BOOLEAN                      Flag
  );

/**
  This function provides an interface for Oem to run code after all PCI device ready

  @param             VOID                Based on OEM design.

  @retval            EFI_UNSUPPORTED     Returns unsupported by default.
  @retval            EFI_SUCCESS         The service is customized in the project.
  @retval            Others              Depends on customization.
**/
EFI_STATUS
OemSvcDxePlatformAfterAllPciInitFinished (
  VOID
  );

/**
 This function provides an interface to modify USB legacy options.

 @param[in out]     *UsbLegacyModifiers On entry, points to USB_LEGACY_MODIFIERS instance.
                                        On exit , points to updated USB_LEGACY_MODIFIERS instance.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
*/
EFI_STATUS
OemSvcSetUsbLegacyPlatformOptions (
  IN OUT  USB_LEGACY_MODIFIERS                *UsbLegacyModifiers
  );

/**
 This function provides an interface to update GlobalNvs table content.

 @param[in, out]    mGlobalNvsArea      On entry, points to EFI_GLOBAL_NVS_AREA  instance.
                                        On exit , points to updated EFI_GLOBAL_NVS_AREA  instance.
 @param[in, out]    mOemGlobalNvsArea   On entry, points to EFI_OEM_GLOBAL_NVS_AREA  instance.
                                        On exit , points to updated EFI_OEM_GLOBAL_NVS_AREA  instance.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcUpdateGlobalNvs (
  EFI_GLOBAL_NVS_AREA                   *mGlobalNvsArea,
  EFI_OEM_GLOBAL_NVS_AREA               *mOemGlobalNvsArea
  );

/**
  Customize BIOS protect region before boot to OS. 

  @param[out]        *BiosRegionTable    Pointer to BiosRegion Table.
  @param[out]        ProtectRegionNum    The number of Bios protect region instances.
  
  @retval            EFI_UNSUPPORTED     Returns unsupported by default, OEM did not implement this function.
  @retval            EFI_MEDIA_CHANGED   Provide table for kernel to set protect region and lock flash program registers. 
**/
EFI_STATUS
OemSvcUpdateBiosProtectTable (
  OUT BIOS_PROTECT_REGION           **BiosRegionTable,
  OUT UINT8                         *ProtectRegionNum
  );
  
/**
  This function provides an interface to hook GenericRouteConfig.

  @param[in,out]     ScBuffer            A pointer to CHIPSET_CONFIGURATION struct.
  @param[in]         BufferSize          System configuration size.
  @param[in]         ScuRecord           The bit mask of the SCU record.
                                           Bit 0 = 1 (SCU_ACTION_LOAD_DEFAULT), It indicates system do load default action.

  @retval            EFI_UNSUPPORTED     This function is a pure hook; Chipset code don't care return status.
  @retval            EFI_SUCCESS         This function is a pure hook; Chipset code don't care return status. 
**/
EFI_STATUS
OemSvcHookRouteConfig (
  IN OUT CHIPSET_CONFIGURATION          *ScBuffer, 
  IN     UINTN                          BufferSize,
  IN     UINT32                         ScuRecord
  );
/**
 This function provides an interface to modify OEM Logo and POST String.

 @param[in out]     *EFI_OEM_BADGING_LOGO_DATA                   On entry, points to a structure that specifies image data.
                                                                 On exit , points to updated structure.
 @param[in out]     *BadgingDataSize    On entry, the size of EFI_OEM_BADGING_LOGO_DATA matrix.
                                        On exit , the size of updated EFI_OEM_BADGING_LOGO_DATA matrix.
 @param[in out]     *OemBadgingString   On entry, points to OEM_BADGING_STRING matrix.
                                        On exit , points to updated OEM_BADGING_STRING matrix.
 @param[in out]     *OemBadgingStringInTextMode                  On entry, points to OEM_BADGING_STRING matrix in text mode.
                                                                 On exit , points to updated OEM_BADGING_STRING matrix in text mode.
 @param[in out]     *StringCount        The number is POST string count.
                                        On entry, base on SetupVariable->QuietBoot
                                                  1 : The number of entries in OemBadgingString,
                                                  0 : The number of entries in OemBadgingStringInTextMode.
                                        On exit , base on SetupVariable->QuietBoot
                                                  1 : The number of entries in updated OemBadgingString,
                                                  0 : The number of entries in updated OemBadgingStringInTextMode.
 @param[in out]     *OemBadgingStringAfterSelectWithMe           On entry, points to OEM_BADGING_STRING matrix after selected.
                                                                 On exit , points to updated OEM_BADGING_STRING matrix after selected.
 @param[in out]     *OemBadgingStringAfterSelectWithMeInTextMode On entry, points to OEM_BADGING_STRINGmatrix after selected  in text mode.
                                                                 On exit , points to updated OEM_BADGING_STRING matrix after selected  in text mode.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcUpdateOemBadgingLogoData (
  IN OUT EFI_OEM_BADGING_LOGO_DATA            **EfiOemBadgingLogoData,
  IN OUT UINTN                                *BadgingDataSize,
  IN OUT OEM_BADGING_STRING                   **OemBadgingString,
  IN OUT OEM_BADGING_STRING                   **OemBadgingStringInTextMode,
  IN OUT UINTN                                *StringCount,
  IN OUT OEM_BADGING_STRING                   **OemBadgingStringAfterSelectWithMe,
  IN OUT OEM_BADGING_STRING                   **OemBadgingStringAfterSelectWithMeInTextMode
  );

#endif
