/** @file
  PlatformBdsLib

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/**
  Head file for BDS Platform specific code

Copyright (c) 2004 - 2008, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _BDS_PLATFORM_H_
#define _BDS_PLATFORM_H_
#include <Uefi.h>
#include <PiDxe.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/Pci.h>
#include <Uefi/UefiInternalFormRepresentation.h>
#include <MeChipset.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/SetupUtility.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol/ConOutDevStarted.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyRegion2.h>
#include <Protocol/ConsoleRedirectionService.h>
#include <Protocol/OEMBadgingSupport.h>
#include <Protocol/UgaSplash.h>
#include <Protocol/ConsoleControl.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/H2ODialog.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/SkipScanRemovableDev.h>
#include <Protocol/GopPolicy.h>
#include <PlatformGopVbt.h>
#include <Protocol/IsaAcpi.h>
#include <Protocol/SerialIo.h>
#include <Protocol/TerminalEscCode.h>
#include <Protocol/AcpiS3Save.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/LoadFile.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/ConsoleControl.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/Smbios.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/BootLogo.h>
#include <Protocol/MePolicy.h>
#include <Protocol/SimpleTextIn.h>
#include <MeBiosPayloadData.h>
#include <Protocol/AmtPolicy.h>

#include <Guid/GlobalVariable.h>
#include <Guid/DebugMask.h>
#include <Guid/CapsuleVendor.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/BdsHii.h>
//#include <Guid/ConsoleRedirectionSupport.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/OemGraphicsLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PlatformBdsLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/GenericUtilityLib.h>
#include <Library/IoLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeServicesLib.h>
#include <PostCode.h>
#include <Library/DxeOemSvcKernelLib.h>
#include <ChipsetSetupConfig.h>
#include <PlatformBoardId.h>
#include <Library/PchPcieRpLib.h>
#include <IntelRemapPwd.h>
#include <UefiBootManagerLib.h>

extern EFI_GUID                  gHaloMd2VariableGuid;
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformRootBridges [];
extern BDS_CONSOLE_CONNECT_ENTRY gPlatformConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossibleIgfxConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePegConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePcieConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePciConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectSequence [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformDriverOption [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformBootOption [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectSata[];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectLastBoot[];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossibleIgfxConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePegConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePcieConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformAllPossiblePciConsole [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectSataController[];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectIrstStorage [];
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectXhciController[];

//[-start-190321-IB11790295-add]//
#define EfiNetworkAll    1
#define EfiNetworkWifi   2
#define INTEL_VENDOR_ID          0x8086  // Intel(R) vendor ID
//[-end-190321-IB11790295-add]//

typedef struct {
  UINTN  Seg;
  UINTN  Bus;
  UINTN  Dev;
  UINTN  Fun;
} PCI_DEV_DEF;
#define CONSOLE_LOCK_VARIABLE_NAME        (L"ConsoleLock")

#define LOCK_CONSOLE    1
#define NO_LOCK_CONSOLE 0

#ifndef PCICMD
#define PCICMD                          (0x4)
#endif

#define gPciRootBridge \
  {\
    ACPI_DEVICE_PATH,\
    ACPI_DP,\
    (UINT8)(sizeof(ACPI_HID_DEVICE_PATH)),\
    (UINT8)((sizeof(ACPI_HID_DEVICE_PATH)) >> 8),\
    EISA_PNP_ID(0x0A03),\
    0\
  }

#define gEndEntire \
  {\
    END_DEVICE_PATH_TYPE,\
    END_ENTIRE_DEVICE_PATH_SUBTYPE,\
    END_DEVICE_PATH_LENGTH,\
    0\
  }

//
// Temporary console variables used in the ConnectConsole
//
#define CON_OUT_CANDIDATE_NAME    L"ConOutCandidateDev"
#define CON_IN_CANDIDATE_NAME     L"ConInCandidateDev"
#define ERR_OUT_CANDIDATE_NAME    L"ErrOutCandidateDev"
#define HaloMd2Variable           L"HaloMd2Variable"
//
// Platform Root Bridge
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ROOT_BRIDGE_DEVICE_PATH;

//
// Below is the platform console device path
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Keyboard;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_KEYBOARD_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Mouse;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_MOUSE_DEVICE_PATH;

typedef struct {
  USB_CLASS_DEVICE_PATH     Usb;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_USB_HID_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           OnboardVga;
  ACPI_ADR_DEVICE_PATH      DisplayDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ONBOARD_VGA_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           AgpBridge;
  PCI_DEVICE_PATH           AgpDevice;
  ACPI_ADR_DEVICE_PATH      DisplayDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PLUG_IN_VGA_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  SATA_DEVICE_PATH          SataBridge;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_SATA_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      IsaSerial;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ISA_SERIAL_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           SerialOverLAN;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ISA_SERIAL_OVER_LAN_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           SataController;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_SATA_CONTROLLER_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           XhciController;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_XHCI_CONTROLLER_PATH;


//
// Below is the boot option device path
//
typedef struct {
  BBS_BBS_DEVICE_PATH             LegacyHD;
  EFI_DEVICE_PATH_PROTOCOL        End;
} LEGACY_HD_DEVICE_PATH;



typedef enum {
  Igfx  = 0,
  Peg,
  Pcie,
  Pci,
  PossibleVgaTypeMax
} POSSIBLE_VGA_TYPE;

typedef enum {
  DisplayModeIgfx  = 0,
  DisplayModePeg,
  DisplayModePci,
  DisplayModeAuto,
  DisplayModeHg
} PRIMARY_DISPLAY;

typedef struct {
  EFI_HANDLE                   Handle;
  POSSIBLE_VGA_TYPE            VgaType;
  UINT8                        Priority;
} VGA_DEVICE_INFO;

typedef struct {
  UINTN                        VgaHandleConut;
  EFI_HANDLE                   PrimaryVgaHandle;
  EFI_HANDLE                   *VgaHandleBuffer;
} VGA_HANDLES_INFO;

VOID
ChipsetPrepareReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

EFI_STATUS
PlatformBdsNoConsoleAction (
  VOID
  );

EFI_STATUS
PlatformBdsBootDisplayDevice (
  IN CHIPSET_CONFIGURATION        *SetupNVRam,
  IN SA_SETUP                     *SaSetupNVRam
  );

EFI_STATUS
PlatformBdsBootSelection (
  UINT16                                    Selection,
  UINT16                                    Timeout
  );

EFI_STATUS
PlatformBdsConnectDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL    *HandleDevicePath,
  IN  BOOLEAN                     ConnectChildHandle,
  IN  BOOLEAN                     DispatchPossibleChild
  );

EFI_STATUS
UpdateLastBootDevicePath (
  IN   UINT16                     *BootPrevious,
  OUT  EFI_DEVICE_PATH_PROTOCOL   **LastBootDevicePath
  );

EFI_STATUS
PlatformBdsConnectConsoleMini (
  IN  BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  );


VOID
PlatformAlertAT (
  VOID
  );

VOID
InvokeMebxHotKey (
  VOID
  );

VOID
InvokeRemoteAsstHotKey (
  VOID
  );

EFI_STATUS
EFIAPI
SetVgaCommandRegReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

UINT8
IsvtCall (
  IN UINT8 CheckPoint
  );

BOOLEAN
FoundUSBConsoleOutput (
  OUT EFI_DEVICE_PATH_PROTOCOL  **UsbConsoleOuputDevPath
  );

VOID
SetUsbConsoleOutToConOutVar (
  IN EFI_DEVICE_PATH_PROTOCOL  *UsbConsoleOuputDevPath
  );

EFI_STATUS
BdsMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  * Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  * Single,
  IN  EFI_DEVICE_PATH_PROTOCOL  **NewDevicePath OPTIONAL,
  IN  BOOLEAN                   Delete
  );
VOID
UpdateIntelFwEsrt (
  VOID
  );


VOID
BdsProcessSecureErase(
  VOID
  );

VOID
EFIAPI
PlatformBdsTriggerEndOfDxeEvent (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  );

VOID
EFIAPI
PlatformBdsTriggerSmmReadyToLock (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  );

EFI_STATUS
PlatformBdsEnableSmmReadyToLock(
  VOID  
  );

/**
Function to Save the current GOP Mode before launching PBA during TDS flow
**/
VOID
EFIAPI
SaveGopMode (
  VOID
  );

/**
Function to Restore the current GOP Mode at the end of PBA during TDS flow
**/
VOID
EFIAPI
RestoreGopMode (
  VOID
  );

#endif
