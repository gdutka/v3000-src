/** @file
 VFR app Legacy BIOS implementation

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#include "VfrAppLegacyBios.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE       mOrgGopSetMode              = NULL;
EFI_HANDLE                                  mVfrAppLegacyBiosHandle     = NULL;
HII_VENDOR_DEVICE_PATH                      mVfrAppLegacyBiosDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    VFR_APP_LEGACY_BIOS_VENDOR_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};


BOOLEAN
EFIAPI
FakeLegacyBiosInt86 (
  IN  EFI_LEGACY_BIOS_PROTOCOL      *This,
  IN  UINT8                         BiosInt,
  IN  EFI_IA32_REGISTER_SET         *Regs
  )
{
  return TRUE;
}

BOOLEAN
EFIAPI
FakeLegacyBiosFarCall86 (
  IN  EFI_LEGACY_BIOS_PROTOCOL        *This,
  IN  UINT16                          Segment,
  IN  UINT16                          Offset,
  IN  EFI_IA32_REGISTER_SET           *Regs,
  IN  VOID                            *Stack,
  IN  UINTN                           StackSize
  )
{
  return TRUE;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosCheckPciRom (
  IN  EFI_LEGACY_BIOS_PROTOCOL          *This,
  IN  EFI_HANDLE                        PciHandle,
  OUT VOID                              **RomImage, OPTIONAL
  OUT UINTN                             *RomSize, OPTIONAL
  OUT UINTN                             *Flags
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosInstallPciRom (
  IN  EFI_LEGACY_BIOS_PROTOCOL          *This,
  IN  EFI_HANDLE                        PciHandle,
  IN  VOID                              **RomImage,
  OUT UINTN                             *Flags,
  OUT UINT8                             *DiskStart, OPTIONAL
  OUT UINT8                             *DiskEnd, OPTIONAL
  OUT VOID                              **RomShadowAddress, OPTIONAL
  OUT UINT32                            *RomShadowedSize OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosLegacyBoot (
  IN  EFI_LEGACY_BIOS_PROTOCOL          *This,
  IN  BBS_BBS_DEVICE_PATH               *BbsDevicePath,
  IN  UINT32                            LoadOptionsSize,
  IN  VOID                              *LoadOptions
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosUpdateKeyboardLedStatus (
  IN EFI_LEGACY_BIOS_PROTOCOL           *This,
  IN  UINT8                             Leds
  )
{
  return EFI_UNSUPPORTED;
}


EFI_STATUS
EFIAPI
FakeLegacyBiosGetBbsInfo (
  IN EFI_LEGACY_BIOS_PROTOCOL          *This,
  OUT UINT16                           *HddCount,
  OUT HDD_INFO                         **HddInfo,
  OUT UINT16                           *BbsCount,
  OUT BBS_TABLE                        **BbsTable
  )
{
  UINTN                                LocalBbsTableCount;
  BBS_TABLE                            *LocalBbsTable;
  UINTN                                LocalHddInfoCount;
  HDD_INFO                             *LocalHddInfo;
  UINTN                                Index;

  LocalBbsTableCount = mVfrAppPrivate->Configuration.FloppyBootDevNum  +
                       mVfrAppPrivate->Configuration.HddBootDevNum     +
                       mVfrAppPrivate->Configuration.CdRomBootDevNum   +
                       mVfrAppPrivate->Configuration.PcmciaBootDevNum  +
                       mVfrAppPrivate->Configuration.UsbBootDevNum     +
                       mVfrAppPrivate->Configuration.NetworkBootDevNum +
                       mVfrAppPrivate->Configuration.BevBootDevNum;
  LocalBbsTable = AllocateZeroPool (sizeof (BBS_TABLE) * LocalBbsTableCount);
  if (LocalBbsTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LocalHddInfoCount = 4;
  LocalHddInfo      = AllocateZeroPool (sizeof (HDD_INFO) * LocalHddInfoCount);
  if (LocalHddInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LocalBbsTableCount = 0;
  for (Index = 0; Index < mVfrAppPrivate->Configuration.FloppyBootDevNum ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_FLOPPY;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.HddBootDevNum    ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_HARDDISK;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.CdRomBootDevNum  ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_CDROM;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.PcmciaBootDevNum ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_PCMCIA;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.UsbBootDevNum    ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_USB;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.NetworkBootDevNum; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_EMBED_NETWORK;}
  for (Index = 0; Index < mVfrAppPrivate->Configuration.BevBootDevNum    ; Index++) {LocalBbsTable[LocalBbsTableCount++].DeviceType   = BBS_BEV_DEVICE;}

  *HddCount = (UINT16) LocalHddInfoCount;
  *HddInfo  = LocalHddInfo;
  *BbsCount = (UINT16) LocalBbsTableCount;
  *BbsTable = LocalBbsTable;
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosShadowAllLegacyOproms (
  IN EFI_LEGACY_BIOS_PROTOCOL        *This
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosPrepareToBootEfi (
  IN EFI_LEGACY_BIOS_PROTOCOL         *This,
  OUT UINT16                          *BbsCount,
  OUT BBS_TABLE                       **BbsTable
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosGetLegacyRegion (
  IN    EFI_LEGACY_BIOS_PROTOCOL *This,
  IN    UINTN                    LegacyMemorySize,
  IN    UINTN                    Region,
  IN    UINTN                    Alignment,
  OUT   VOID                     **LegacyMemoryAddress
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosCopyLegacyRegion (
  IN EFI_LEGACY_BIOS_PROTOCOL *This,
  IN    UINTN                 LegacyMemorySize,
  IN    VOID                  *LegacyMemoryAddress,
  IN    VOID                  *LegacyMemorySourceAddress
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
FakeLegacyBiosBootUnconventionalDevice (
  IN EFI_LEGACY_BIOS_PROTOCOL         *This,
  IN UDC_ATTRIBUTES                   Attributes,
  IN UINTN                            BbsEntry,
  IN VOID                             *BeerData,
  IN VOID                             *ServiceAreaData
  )
{
  return EFI_INVALID_PARAMETER;
}

EFI_LEGACY_BIOS_PROTOCOL          mVfrAppLegacyBios = {
                                    FakeLegacyBiosInt86,
                                    FakeLegacyBiosFarCall86,
                                    FakeLegacyBiosCheckPciRom,
                                    FakeLegacyBiosInstallPciRom,
                                    FakeLegacyBiosLegacyBoot,
                                    FakeLegacyBiosUpdateKeyboardLedStatus,
                                    FakeLegacyBiosGetBbsInfo,
                                    FakeLegacyBiosShadowAllLegacyOproms,
                                    FakeLegacyBiosPrepareToBootEfi,
                                    FakeLegacyBiosGetLegacyRegion,
                                    FakeLegacyBiosCopyLegacyRegion,
                                    FakeLegacyBiosBootUnconventionalDevice
                                    };

EFI_STATUS
EFIAPI
VfrAppBiosGraphicsOutputSetMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL     *This,
  IN  UINT32                           ModeNumber
  )
{
  EFI_STATUS                           Status;

  gBS->UninstallProtocolInterface (mVfrAppLegacyBiosHandle, &gEfiLegacyBiosProtocolGuid, &mVfrAppLegacyBios);
  Status = mOrgGopSetMode (This, ModeNumber);
  gBS->InstallProtocolInterface (&mVfrAppLegacyBiosHandle, &gEfiLegacyBiosProtocolGuid, EFI_NATIVE_INTERFACE, &mVfrAppLegacyBios);
  return Status;
}

VOID
EFIAPI
VfrAppLegacyBiosGopProtocolCallback (
  IN EFI_EVENT                         Event,
  IN VOID                              *Context
  )
{
  EFI_STATUS                           Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL         *GraphicsOutput;

  if (mOrgGopSetMode != NULL) {
    return;
  }

  Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiGraphicsOutputProtocolGuid, (VOID **) &GraphicsOutput);
  if (!EFI_ERROR (Status)) {
    mOrgGopSetMode = GraphicsOutput->SetMode;
    GraphicsOutput->SetMode = VfrAppBiosGraphicsOutputSetMode;
  }
}

STATIC
BOOLEAN
IsLegacyBiosSupported (
  VOID
  )
{
  KERNEL_CONFIGURATION                 *KernelConfig;
  BOOLEAN                              Supported;

  Supported    = FALSE;
  KernelConfig = CommonGetVariableData (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
  if (KernelConfig != NULL) {
    Supported = (BOOLEAN) (KernelConfig->BootType != EFI_BOOT_TYPE);
    FreePool (KernelConfig);
  }

  return Supported;
}

EFI_STATUS
VfrAppLegacyBiosInit (
  VOID
  )
{
  VOID                                 *Registration;
  EFI_STATUS                           Status;
  EFI_LEGACY_BIOS_PROTOCOL             *LegacyBios;

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **) &LegacyBios);
  if (!EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }
  if (!IsLegacyBiosSupported ()) {
    return EFI_UNSUPPORTED;
  }

  mVfrAppLegacyBiosHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mVfrAppLegacyBiosHandle,
                  &gEfiLegacyBiosProtocolGuid,
                  &mVfrAppLegacyBios,
                  &gEfiDevicePathProtocolGuid,
                  &mVfrAppLegacyBiosDevicePath,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  EfiCreateProtocolNotifyEvent (
    &gEfiGraphicsOutputProtocolGuid,
    TPL_CALLBACK,
    VfrAppLegacyBiosGopProtocolCallback,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}

