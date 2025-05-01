/** @file
 This file include all platform action which can be customized by IBV/OEM.

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

Copyright (c) 2004 - 2008, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

*/

#include <BdsPlatform.h>
#include <OemHotKey.h>
#include <VideoOutputPortSelection.h>
#include <Library/DxeOemSvcChipsetLib.h>
#include <BootDevicesDisplay.h>
#include <AsfSupport.h>
#include <UefiBootManagerLib.h>
#include <PchAccess.h>
#include <PchResetPlatformSpecific.h>
#include <Protocol/AmtPolicy.h>
#include <Library/FdSupportLib.h>
#include <Guid/H2OBdsCheckPoint.h>
#include <Library/H2OCpLib.h>
#include <Guid/EventGroup.h>
#include <Guid/H2OBdsCheckPoint.h>
#include <ChipsetAccess.h>
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <SetupVariable.h>
#include <Library/DxeInsydeChipsetLib.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/EndOfBdsBootSelection.h>
#include "WaitForMePlatformReadyToBootEvent.h"
#include <Protocol/AmtWrapperProtocol.h>
#include <Protocol/AmtReadyToBoot.h>
#include <Protocol/SmmLockEnablePoint.h>
#include <Protocol/SmmAccess2.h>

EFI_STATUS
AtEmptyEventFuntion (
  IN  EFI_EVENT        Event,
  IN  void             *ParentImageHandle
  );
EFI_HANDLE                        mAmtMEBxHandle = NULL;

#include <Protocol/SwitchableGraphicsEvent.h>

EFI_HANDLE                        mAlertATHandle = NULL;

extern PLATFORM_ISA_SERIAL_OVER_LAN_DEVICE_PATH   gSerialOverLANDevicePath;
#define DP_TYPE_MESSAGING  0x03
#define DP_SUBTYPE_USB     0x05

typedef struct {
  UINT16  VendorId;
  UINT16  DeviceId;
  UINT16  SubsystemVendorID;
  UINT16  SubsystemID;
} CHECK_VGA_ID;

CHECK_VGA_ID CheckVgaId[] = {
  {
    0x10DE,
    0x06E4,
    0x0000,
    0x0000
  }
};


BOOLEAN
IsIntelRapidStorageFastBootEnable (
  IN CHIPSET_CONFIGURATION         *SystemConfiguration,
  IN PCH_SETUP                     *PchSetup
);

EFI_STATUS
ReconnectSataDevice ();

VOID
AmtBdsBoot (
  VOID
  );

//BOOLEAN mIntelRapidStorageFastBootEnabled;

//VOID
//EfiRaidSpecialCheck ();

extern EFI_GUID gBdsAllDriversConnectedProtocolGuid;
#define AMT_SOL_LEGACY_VGA_REDIR_ROM_FILE_GUID \
   { \
     0x346b4547, 0xfef7, 0x49d4, 0x9a, 0x4a, 0xf6, 0x66, 0x82, 0x5e, 0x25, 0xb7 \
   }


// /**
//   Locate and Initialize a PCIe SSD
// **/
// VOID
// LocateAndInitializePcieSsd (
//   )
// {
//   EFI_STATUS                  Status;
//   UINTN                       Index;
//   UINTN                       HandleCount;
//   EFI_HANDLE                  *HandleBuffer;
//   // PCI_TYPE00                  PciData;
//   EFI_PCI_IO_PROTOCOL         *PciIo;
//   UINTN                       Segment;
//   UINTN                       Bus;
//   UINTN                       Device;
//   UINTN                       Function;
// //  BOOLEAN                     MD2Exist = FALSE;
// //  BOOLEAN                     MD2ExistPrevious;
// //  UINTN                       DataSize;
// //  UINTN                       RpIndex;
// //  UINT16                      LSTS;

//   DEBUG ((DEBUG_INFO, "LocateAndInitializePcieSsd() - Start\n"));

//   Status = gBS->LocateHandleBuffer (
//                   ByProtocol,
//                   &gEfiPciIoProtocolGuid,
//                   NULL,
//                   &HandleCount,
//                   &HandleBuffer
//                   );

//   if (EFI_ERROR(Status)) {
//     HandleCount = 0;
//   }

//   for (Index = 0; Index < HandleCount; Index++) {
//     Status = gBS->HandleProtocol (
//                     HandleBuffer[Index],
//                     &gEfiPciIoProtocolGuid,
//                     (VOID *) &PciIo
//                     );
//     ASSERT_EFI_ERROR(Status);
//     PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);

//     //
//     // RVP10/16 M.2 power GPIO fix, detecting whether Root Port 0x0C/0x10 having device
//     //
//     // if (mPlatformBoardConfig->HaloMd2SpecificInit) {
//       //  It should get the HaloMd2SpecificInit from BOARD_CONFIG_BLOCK_DXE,
//       //  But the ConfigBlockDxe doesn't merge in our code base,
//       //  So, I have to replace mPlatformBoardConfig->HaloMd2SpecificInit by PlatofrmInfo->BoardId.
// //    if ((PlatformInfo->BoardId == BoardIdSkylakeHaloLpddr3Rvp16) || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeHaloDdr4Rvp11) || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeHaloDdr4Rvp11Ppv)  || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeSdlBrk)) {
// //
// //      if ((Bus == DEFAULT_PCI_BUS_NUMBER_PCH) &&
// //         ((Device == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_1) ||
// //          (Device == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_2) ||
// //          (Device == PCI_DEVICE_NUMBER_PCH_PCIE_DEVICE_3))) {
// //        Status = GetPchPcieRpNumber (Device, Function, &RpIndex);
// //        if (!EFI_ERROR(Status) && ((RpIndex == 0x0C) || (RpIndex == 0x10))) {
// //          Status = PciIo->Pci.Read (
// //                    PciIo,
// //                    EfiPciIoWidthUint16,
// //                    R_PCH_PCIE_CFG_LSTS,
// //                    sizeof (UINT16),
// //                    &LSTS
// //                    );
// //          if (LSTS & B_PCIE_LSTS_LA) {
// //            MD2Exist = TRUE;
// //            DEBUG ((DEBUG_INFO, "LocateAndInitializePcieSsd() - MD2Exist = TRUE.\n"));
// //          }
// //        }
// //      }
// //    }

//     // if (Bus != 0) {
//     //   Status = PciIo->Pci.Read (
//     //                 PciIo,
//     //                 EfiPciIoWidthUint8,
//     //                 0,
//     //                 sizeof (PciData),
//     //                 &PciData
//     //                 );
//     //   if ((PciData.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) &&
//     //       ((PciData.Hdr.ClassCode[1] == PCI_CLASS_MASS_STORAGE_SATADPA) || (PciData.Hdr.ClassCode[1] == PCI_CLASS_MASS_STORAGE_SOLID_STATE))) {
//     //     gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
//     //   }
//     // }
//   }
//   if (HandleBuffer) {
//     FreePool (HandleBuffer);
//   }

// //  // if (mPlatformBoardConfig->HaloMd2SpecificInit) {
// //    //  The removed reason as before.
// //  if ((PlatformInfo->BoardId == BoardIdSkylakeHaloLpddr3Rvp16) || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeHaloDdr4Rvp11) || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeHaloDdr4Rvp11Ppv)  || \
// //        (PlatformInfo->BoardId == BoardIdSkylakeSdlBrk)) {
// //
// //    Status = gRT->GetVariable (
// //                    HaloMd2Variable,
// //                    &gHaloMd2VariableGuid,
// //                    NULL,
// //                    &DataSize,
// //                    &MD2ExistPrevious
// //                    );
// //
// //    if ((MD2Exist != MD2ExistPrevious) || (Status == EFI_NOT_FOUND)) {
// //      Status = gRT->SetVariable (
// //                      HaloMd2Variable,
// //                      &gHaloMd2VariableGuid,
// //                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
// //                      sizeof (BOOLEAN),
// //                      &MD2Exist
// //                      );
// //    }
// //  }

//   DEBUG ((DEBUG_INFO, "LocateAndInitializePcieSsd() - End\n"));
// }

EFI_STATUS
DummyDisconnectCrsTerminal (
  EFI_CONSOLE_REDIRECTION_SERVICE_PROTOCOL  *This,
  UINTN                                     TerminalNumber
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
DummyconnectCrsTerminal (
  EFI_CONSOLE_REDIRECTION_SERVICE_PROTOCOL  *This,
  UINTN                                     TerminalNumber
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
QueryVgaScreenBuffer (
  IN      EFI_CONSOLE_REDIRECTION_SERVICE_PROTOCOL    *This,
  IN OUT  UINTN                                       *ScreenBuffer,
  IN OUT  UINTN                                       *ScreenAttributes
  )
{

  return EFI_SUCCESS;
}
EFI_STATUS
BdsEmptyCallbackFuntion (
  IN  EFI_EVENT        Event,
  IN  void             *ParentImageHandle
  )
{
  return EFI_SUCCESS;
}

/**
 This routine install SOL option ROM if needed

*/
static
VOID
InstallSOLOptionROM (
  VOID
  )
{
  BOOLEAN                       AMTEnable;
  BOOLEAN                       SOLFound;
  UINTN                         NumHandles;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  EFI_STATUS                    Status;
  PCI_TYPE00                    PciConfigHeader00;
  UINT8                         *VgaRedirIrqBda;
  UINT16                        *VgaRedirIoBaseBda;
  VOID                          *LocalRomImage;
  UINTN                         LocalRomSize;
  EFI_IA32_REGISTER_SET         RegisterSet;
  UINTN                         Flags;
  EFI_LEGACY_BIOS_PROTOCOL      *LegacyBios;
  AMT_WRAPPER_PROTOCOL          *AmtWrapper;
  EFI_GUID                      AmtSolVgaRedirRomFileGuid = AMT_SOL_LEGACY_VGA_REDIR_ROM_FILE_GUID;
//  VOID                                      *CrSupport;
  EFI_CONSOLE_REDIRECTION_SERVICE_PROTOCOL  *CRService;
  EFI_HANDLE                                ImageHandle = NULL;
  EFI_CONSOLE_REDIRECTION_SERVICE_PROTOCOL  DummyCRService = {NULL,
                                                              QueryVgaScreenBuffer,
                                                              DummyDisconnectCrsTerminal,
                                                              DummyconnectCrsTerminal
                                                              };

  AMTEnable = FALSE;
  SOLFound = FALSE;
  NumHandles = 0;
  VgaRedirIrqBda = NULL;
  VgaRedirIoBaseBda = NULL;

  //
  // Amt Library Init
  //

  Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);
  if (!EFI_ERROR (Status)) {
    if (AmtWrapper->IsSolEnabled ()) {
        AMTEnable =  TRUE;
    }
  }
  if (!AMTEnable) {
    return;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NumHandles,
                  &HandleBuffer
                  );
  for (Index = 0; Index < NumHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    ASSERT_EFI_ERROR (Status);

    Status = PciIo->Pci.Read (
                      PciIo,
                      EfiPciIoWidthUint32,
                      0,
                      sizeof (PciConfigHeader00) / sizeof (UINT32),
                      &PciConfigHeader00
                      );
    ASSERT_EFI_ERROR (Status);

    if ((PciConfigHeader00.Hdr.VendorId == V_ME_SOL_VENDOR_ID) &&
         (IS_PCH_SOL_DEVICE_ID(PciConfigHeader00.Hdr.DeviceId))
         ) {
      SOLFound = TRUE;
      //
      // Special make up for SOL option ROM
      //
      VgaRedirIrqBda     = (UINT8 *)(UINTN)0x4A6;
      VgaRedirIoBaseBda  = (UINT16 *)(UINTN)0x4A4;
      *VgaRedirIrqBda    = PciConfigHeader00.Device.InterruptLine;
      *VgaRedirIoBaseBda = (UINT16)(PciConfigHeader00.Device.Bar[0] & 0xFFFE);
      break;
    }
  }

  if (!SOLFound) {
    return;
  }

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
  if (EFI_ERROR (Status)) {
    return;
  }

  LocalRomImage = NULL;
  LocalRomSize = 0;
  Status = GetSectionFromAnyFv (
             &AmtSolVgaRedirRomFileGuid,
             EFI_SECTION_RAW,
             0,
             &LocalRomImage,
             (UINTN*)&LocalRomSize
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  LegacyBios->InstallPciRom (
                LegacyBios,
                NULL,
                &LocalRomImage,
                &Flags,
                NULL,
                NULL,
                NULL,
                NULL
                );

  // Need to fixup and test int10h clear screen stuff
  // Need to use int10h now as vgaredir oprom code has hooked int10h and int16h kybrd stuff
  // so can use console spilitter stuff anymore.

  //
  //  Need to add a clear screen msg after this for when oprom display is turned on.
  //
  //  Legacy INT 10h required for screen clear as we have already enabled console redir at this point.
  //
  // Set Video mode to cloer and 80X25
  //
  ZeroMem (&RegisterSet, sizeof (EFI_IA32_REGISTER_SET));
  RegisterSet.X.AX = 0x0003;

  Status = LegacyBios->Int86 (LegacyBios, 0x10, &RegisterSet);

  //
  // Scroll text upward to clear screen
  //
  ZeroMem (&RegisterSet, sizeof (EFI_IA32_REGISTER_SET));
  RegisterSet.X.AX = 0x0600;
  RegisterSet.H.BH = 0x00;
  RegisterSet.X.CX = 0x0000;
  RegisterSet.X.DX = 0x1950;

  Status = LegacyBios->Int86 (LegacyBios, 0x10, &RegisterSet);


  Status = gBS->LocateProtocol (&gConsoleRedirectionServiceProtocolGuid, NULL, (VOID **)&CRService);
  if (EFI_ERROR (Status)) {
    //
    // Install Dummy gConsoleRedirectionServiceProtocolGuid protocol, if SOL is working.
    //
    gBS->InstallProtocolInterface (
           &ImageHandle,
           &gConsoleRedirectionServiceProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &DummyCRService
           );
  }
}


VOID
InvokeMebxHotKey (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT8                                 AmtbxHotkeyPressed;
  AMT_POLICY_PROTOCOL                   *AMTPlatformPolicy;
  AMT_DXE_CONFIG                        *AmtDxeConfig;

  AMTPlatformPolicy = NULL;
  AMTPlatformPolicy = AllocateZeroPool (sizeof (AMT_POLICY_PROTOCOL));
  ASSERT (AMTPlatformPolicy != NULL);

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **)&AMTPlatformPolicy);

  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) AMTPlatformPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
    ASSERT_EFI_ERROR (Status);

    AmtDxeConfig->AmtbxHotkeyPressed = 1;
    Status = gBS->InstallProtocolInterface (
                    &mAmtMEBxHandle,
                    &gDxeAmtPolicyGuid,
                    EFI_NATIVE_INTERFACE,
                    AMTPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    AmtbxHotkeyPressed = 1;

    Status = gRT->SetVariable (
                    L"AmtbxHotkeyPressed",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof (AmtbxHotkeyPressed),
                    &AmtbxHotkeyPressed
                    );
    ASSERT_EFI_ERROR (Status);
  }

  TriggerCpReadyToBootBefore ();
  //
  // PostCode = 0x2E, Last Chipset initial before boot to OS
  //
  POST_CODE (BDS_READY_TO_BOOT_EVENT);
  //
  // Signal the EVT_SIGNAL_READY_TO_BOOT event
  //
  EfiSignalEventReadyToBoot();
  TriggerCpReadyToBootAfter ();

}

VOID
InvokeRemoteAsstHotKey (
  VOID
  )
{
  EFI_STATUS                                Status;
  UINT8                                     iRemoteAssistHotkeyPressed;
  AMT_POLICY_PROTOCOL                   *AMTPlatformPolicy;
  AMT_DXE_CONFIG                        *AmtDxeConfig;

  AMTPlatformPolicy = NULL;
  AMTPlatformPolicy = AllocateZeroPool (sizeof (AMT_POLICY_PROTOCOL));
  ASSERT (AMTPlatformPolicy != NULL);

  Status = gBS->LocateProtocol (&gDxeAmtPolicyGuid, NULL, (VOID **)&AMTPlatformPolicy);
  if (!EFI_ERROR (Status)) {
    Status = GetConfigBlock ((VOID *) AMTPlatformPolicy, &gAmtDxeConfigGuid, (VOID *) &AmtDxeConfig);
    ASSERT_EFI_ERROR (Status);

    AmtDxeConfig->CiraRequest = 1;
    Status = gBS->InstallProtocolInterface (
                    &mAmtMEBxHandle,
                    &gDxeAmtPolicyGuid,
                    EFI_NATIVE_INTERFACE,
                    AMTPlatformPolicy
                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    iRemoteAssistHotkeyPressed = 1;

    Status = gRT->SetVariable (
                    L"iRemoteAsstHotkeyPressed",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof (iRemoteAssistHotkeyPressed),
                    &iRemoteAssistHotkeyPressed
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

EFI_STATUS
EFIAPI
SetVgaCommandRegEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  UINTN                                 IndexGop;
  UINTN                                 IndexController;
  BOOLEAN                               PrimaryExit;
  EFI_HANDLE                            PrimaryHandle;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  EFI_STATUS                            Status;
  UINT8                                 Command;
  VGA_HANDLES_INFO                      *VgaHandlesInfo;
  UINTN                                 GopHandleCount;
  EFI_HANDLE                            *GopHandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL              *ControllerDevicePath;
  EFI_DEVICE_PATH_PROTOCOL              *GopDevicePath;

  VgaHandlesInfo = (VGA_HANDLES_INFO*) Context;

  if (VgaHandlesInfo == NULL) {
    return EFI_NOT_FOUND;
  }
  //
  // Make sure Primary Handle exist.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &GopHandleCount,
                  &GopHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PrimaryHandle = NULL;
  PrimaryExit = FALSE;

  for (IndexController = 0; IndexController < VgaHandlesInfo->VgaHandleConut; IndexController++) {
    if (PrimaryExit) {
      break;
    }

    Status = gBS->HandleProtocol (
                    VgaHandlesInfo->VgaHandleBuffer[IndexController],
                    &gEfiDevicePathProtocolGuid,
                    (VOID*)&ControllerDevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    for (IndexGop = 0; IndexGop < GopHandleCount; IndexGop++) {
      Status = gBS->HandleProtocol (GopHandleBuffer[IndexGop], &gEfiDevicePathProtocolGuid, (VOID*)&GopDevicePath);
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (CompareMem (ControllerDevicePath, GopDevicePath, GetDevicePathSize (ControllerDevicePath) - END_DEVICE_PATH_LENGTH) == 0) {
        PrimaryHandle = VgaHandlesInfo->VgaHandleBuffer[IndexController];
        if (PrimaryHandle == VgaHandlesInfo->PrimaryVgaHandle) {
          PrimaryExit = TRUE;
          break;
        }
      }
    }
  }

  if (PrimaryHandle == NULL) {
    goto EventExit;
  }

  Command = 0;

  for (IndexController = 0; IndexController < VgaHandlesInfo->VgaHandleConut; IndexController++) {
    if (VgaHandlesInfo->VgaHandleBuffer[IndexController] != PrimaryHandle) {
      //
      // Disconnect the driver of VgaHandle.
      //
      gBS->DisconnectController (VgaHandlesInfo->VgaHandleBuffer[IndexController], NULL, NULL);
      //
      // Clear the command register of Vga device.
      //
      Status = gBS->HandleProtocol (VgaHandlesInfo->VgaHandleBuffer[IndexController], &gEfiPciIoProtocolGuid, (VOID **)&PciIo);

      if (!EFI_ERROR (Status)) {
        PciIo->Pci.Write (PciIo, EfiPciIoWidthUint16, 0x04, 1, &Command);
      }
    }
  }

EventExit:

  gBS->FreePool (GopHandleBuffer);

  return EFI_SUCCESS;
}


//
// BDS Platform Functions
//
/**
  Platform Bds init. Include the platform firmware vendor, revision
  and so crc check.

**/
VOID
EFIAPI
PlatformBdsInit (
  VOID
  )
{
  UINT8                       Port84Data;

if (FeaturePcdGet (PcdAmtEnable)) {
  CreateWaitForMePlatformReadyToBootEvent ();
}
  //
  // Intel Silicon View Technology (ISVT) IO Reading port 0x99 with AH = 0x20 for End of DXE
  //
  Port84Data = IsvtCall (PcdGet8 (PcdIsvtCheckPoint2AhPreloadValue));

}


/**
 Connect RootBridge

 @param             none

 @retval            EFI_SUCCESS         Connect RootBridge successfully.
 @retval            EFI_STATUS          Connect RootBridge fail.
*/
EFI_STATUS
ConnectRootBridge (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                RootHandle;

  //
  // Make all the PCI_IO protocols on PCI Seg 0 show up
  //
  BdsLibConnectDevicePath (gPlatformRootBridges[0]);

  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &gPlatformRootBridges[0],
                  &RootHandle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // PostCode = 0x13, PCI enumeration
  //
  POST_CODE (BDS_PCI_ENUMERATION_START);
  Status = gBS->ConnectController (RootHandle, NULL, NULL, FALSE);
  //
  // PostCode = 0x15, PCI enumeration complete
  //
  POST_CODE (BDS_PCI_ENUMERATION_END);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}


EFI_STATUS
GetAllVgaHandles (
  OUT EFI_HANDLE  **VgaHandleBuffer,
  OUT UINTN       *VgaHandleCount
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 HandleCount;
  UINTN                                 HandleIndex;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_TYPE00                            Pci;
  EFI_HANDLE                            *TempVgaHandleBuffer;
  UINTN                                 BufferIndex;
  HandleBuffer        = NULL;
  HandleCount         = 0;
  BufferIndex         = 0;
  //
  // Start PciBus.
  //
  ConnectRootBridge ();
  if (FeaturePcdGet (PcdSwitchableGraphicsSupported)) {
    EFI_HANDLE                          SgHandle;
    //
    // Install Switchable Graphics Protocol to trigger
    // Switchable Graphics DXE driver registered callback function.
    //
    SgHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &SgHandle,
                    &gH2OSwitchableGraphicsEventProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
  }
  //
  // check all the pci io to find all possible VGA devices
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  (HandleCount * sizeof(EFI_HANDLE)),
                  (VOID **) &TempVgaHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (HandleBuffer[HandleIndex], &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
    if (!EFI_ERROR (Status)) {
      //
      // Check for all VGA device
      //
      Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0,
                        sizeof (Pci) / sizeof (UINT32),
                        &Pci
                        );
      if (EFI_ERROR (Status)) {
        continue;
      }

      if (!IS_PCI_DISPLAY (&Pci)) {
        continue;
      }

      TempVgaHandleBuffer[BufferIndex] = HandleBuffer[HandleIndex];
      BufferIndex++;
    }
  }

  if (BufferIndex == 0) {
    return EFI_NOT_FOUND;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  (BufferIndex * sizeof (EFI_HANDLE)),
                  (VOID **) VgaHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CopyMem (*VgaHandleBuffer, TempVgaHandleBuffer, (BufferIndex * sizeof(EFI_HANDLE)));

  *VgaHandleCount = BufferIndex;

  gBS->FreePool (HandleBuffer);
  gBS->FreePool (TempVgaHandleBuffer);

  return EFI_SUCCESS;
}


EFI_STATUS
GetGopDevicePath (
   IN  EFI_DEVICE_PATH_PROTOCOL *PciDevicePath,
   OUT EFI_DEVICE_PATH_PROTOCOL **GopDevicePath
   )
{
  UINTN                           Index;
  EFI_STATUS                      Status;
  EFI_HANDLE                      PciDeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *TempPciDevicePath;
  UINTN                           GopHandleCount;
  EFI_HANDLE                      *GopHandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath;

  RemainingDevicePath = NULL;

  if (PciDevicePath == NULL || GopDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the GopDevicePath to be PciDevicePath
  //
  *GopDevicePath    = PciDevicePath;
  TempPciDevicePath = PciDevicePath;

  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &TempPciDevicePath,
                  &PciDeviceHandle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Try to connect this handle, so that GOP driver could start on this
  // device and create child handles with GraphicsOutput Protocol installed
  // on them, then we get device paths of these child handles and select
  // them as possible console device.
  //
  PlatformBdsDisplayPortSelection (PciDeviceHandle, &RemainingDevicePath);
  Status = gBS->ConnectController (PciDeviceHandle, NULL, RemainingDevicePath, FALSE);
  if (EFI_ERROR (Status)) {
    //
    // In uefi boot mode If RemainingDevicePath != NULL, and connected failed.
    //
    PlatformBdsVgaConnectedFailCallback (PciDeviceHandle, RemainingDevicePath);
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &GopHandleCount,
                  &GopHandleBuffer
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Add all the child handles as possible Console Device
    //
    for (Index = 0; Index < GopHandleCount; Index++) {
      Status = gBS->HandleProtocol (GopHandleBuffer[Index], &gEfiDevicePathProtocolGuid, (VOID*)&TempDevicePath);
      if (EFI_ERROR (Status)) {
        continue;
      }
      if (CompareMem (
            PciDevicePath,
            TempDevicePath,
            GetDevicePathSize (PciDevicePath) - END_DEVICE_PATH_LENGTH
            ) == 0) {
        //
        // In current implementation, we only enable one of the child handles
        // as console device, i.e. sotre one of the child handle's device
        // path to variable "ConOut"
        // In futhure, we could select all child handles to be console device
        //

        *GopDevicePath = TempDevicePath;

        //
        // Delete the PCI device's path that added by GetPlugInPciVgaDevicePath()
        // Add the integrity GOP device path.
        //
        BdsLibUpdateConsoleVariable (L"ConOutDev", NULL, PciDevicePath);
        BdsLibUpdateConsoleVariable (L"ConOutDev", TempDevicePath, NULL);
        BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME, NULL, PciDevicePath);
        BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME, TempDevicePath, NULL);
      }
    }
    gBS->FreePool (GopHandleBuffer);
  }

  return EFI_SUCCESS;
}


/**
 Add PCI VGA to ConOut.
 PCI VGA: 03 00 00

 @param[in]         DeviceHandle        Handle of PCIIO protocol.

 @retval            EFI_SUCCESS         PCI VGA is added to ConOut.
 @retval            EFI_STATUS          No PCI VGA device is added.
*/
EFI_STATUS
PreparePciVgaDevicePath (
  IN EFI_HANDLE                DeviceHandle,
  IN EFI_DEVICE_PATH_PROTOCOL  **GopDevicePath
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  DevicePath = NULL;
  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID*)&DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GetGopDevicePath (DevicePath, GopDevicePath);

  return EFI_SUCCESS;
}


EFI_HANDLE
SelectVgaHandle (
  IN VGA_DEVICE_INFO                    *VgaDeviceList,
  IN UINTN                              VgaHandleCount
  )
{
  EFI_STATUS                            Status;
  UINT8                                 CheckSequenceIndex;
  UINT8                                 SelectedVgaIndex;
  UINT8                                 ListIndex;
  SA_SETUP                              *SaSetup;
  UINTN                                 SetupSize;
  POSSIBLE_VGA_TYPE                     CheckSequence[] = {Igfx, Peg, Pcie, Pci};

  SaSetup          = NULL;
  SelectedVgaIndex = 0xFF;

  SetupSize = sizeof (SA_SETUP);
  SaSetup = AllocateZeroPool (SetupSize);
  if (SaSetup == NULL) {
    return NULL;
  }
  Status = gRT->GetVariable (
                SA_SETUP_VARIABLE_NAME,
                &gSaSetupVariableGuid,
                NULL,
                &SetupSize,
                SaSetup
                );
  if (EFI_ERROR (Status)) {
    FreePool (SaSetup);
    return NULL;
  }

  if ((SaSetup->PrimaryDisplay == DisplayModeIgfx) ||
      (SaSetup->PrimaryDisplay == DisplayModeSg)) {
    CheckSequence[0] = Igfx;
    CheckSequence[1] = Peg;
    CheckSequence[2] = Pcie;
    CheckSequence[3] = Pci;
  }
  if ((SaSetup->PrimaryDisplay == DisplayModePeg) ||
      (SaSetup->PrimaryDisplay == DisplayModeAuto)) {
    CheckSequence[0] = Peg;
    CheckSequence[1] = Pcie;
    CheckSequence[2] = Pci;
    CheckSequence[3] = Igfx;
  }
  if (SaSetup->PrimaryDisplay == DisplayModePci) {
    CheckSequence[0] = Pci;
    CheckSequence[1] = Pcie;
    CheckSequence[2] = Peg;
    CheckSequence[3] = Igfx;
  }

  for (CheckSequenceIndex = 0; CheckSequenceIndex < PossibleVgaTypeMax; CheckSequenceIndex++) {
    for (ListIndex = 0; ListIndex < VgaHandleCount; ListIndex++) {
      if ((VgaDeviceList[ListIndex].VgaType == CheckSequence[CheckSequenceIndex]) &&
          ((SelectedVgaIndex == 0xFF) ||
           ((SelectedVgaIndex != 0xFF) &&
            (VgaDeviceList[ListIndex].Priority < VgaDeviceList[SelectedVgaIndex].Priority)))) {
        SelectedVgaIndex = ListIndex;
      }
    }
    if (SelectedVgaIndex != 0xFF) {
      if (SaSetup != NULL) {
        FreePool (SaSetup);
      }
      return VgaDeviceList[SelectedVgaIndex].Handle;
    }
  }

  if (SaSetup != NULL) {
    FreePool (SaSetup);
  }
  return NULL;
}


EFI_HANDLE
ClassifyVgaHandleAndSelect (
  IN EFI_HANDLE                         *PciVgaHandleBuffer,
  IN UINTN                              PciVgaHandleCount
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Index;
  VGA_DEVICE_INFO                       *VgaDeviceList;
  UINTN                                 VgaHandleCount;
  BOOLEAN                               FoundFlag;
  EFI_HANDLE                            SelectedVgaHandle;
  UINT8                                 IndexCheckVgaId;
  PCI_TYPE00                            PciType;
  UINT16                                VendorId;
  UINT16                                DeviceId;
  UINT16                                SubsystemVendorID;
  UINT16                                SubsystemID;
  UINT8                                 CapabilityPtr;
  UINT8                                 CapabilityId;
  BOOLEAN                               FindVga;
  BOOLEAN                               FindIgfx;
  BOOLEAN                               FindPeg;
  BOOLEAN                               FindPci;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  UINTN                                 SegmentNumber;
  UINTN                                 BusNumber;
  UINTN                                 DeviceNumber;
  UINTN                                 FunctionNumber;

  FindIgfx = FALSE;
  FindPeg = FALSE;
  FindPci = FALSE;
  VendorId = 0;
  DeviceId = 0;
  SubsystemVendorID = 0;
  SubsystemID = 0;
  PciIo = NULL;
  SegmentNumber = 0;
  BusNumber = 0;
  DeviceNumber = 0;
  FunctionNumber = 0;

  SelectedVgaHandle = NULL;
  VgaHandleCount = PciVgaHandleCount;
  if (VgaHandleCount == 0) {
    return NULL;
  }

  if (VgaHandleCount == 1) {
    return PciVgaHandleBuffer[0];
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  (VgaHandleCount * sizeof (VGA_DEVICE_INFO)),
                  (VOID **) &VgaDeviceList
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < VgaHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    PciVgaHandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );

    VgaDeviceList[Index].Handle = PciVgaHandleBuffer[Index];
    FoundFlag = FALSE;

    if (!FindIgfx || !FindPeg || !FindPci) {
      FindVga        = FALSE;
      PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint32,
                  0,
                  sizeof(PciType) / sizeof(UINT32),
                  &PciType
                  );
          if ((PciType.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_VGA) || \
              (PciType.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_XGA) || \
              (PciType.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_3D) || \
              (PciType.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_OTHER)) {
            FindVga = TRUE;
            VendorId          = PciType.Hdr.VendorId;
            DeviceId          = PciType.Hdr.DeviceId;
            SubsystemVendorID = PciType.Device.SubsystemVendorID;
            SubsystemID       = PciType.Device.SubsystemID;
      }
      if (FindVga) {
        PciIo->GetLocation (PciIo, &SegmentNumber, &BusNumber, &DeviceNumber, &FunctionNumber);
        if (BusNumber <= 0 && !FindIgfx) {
          VgaDeviceList[Index].VgaType = Igfx;
          VgaDeviceList[Index].Priority = 0;
          FindIgfx = TRUE;
          continue;
        }
        CapabilityId  = 0xFF;
        CapabilityPtr = 0xFF;
        //
        //To check VGA is PCI or PCIe
        //
//        Address = EFI_PCI_ADDRESS (Bus, Device, Function, PCI_CAPBILITY_POINTER_OFFSET);
//        PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &CapabilityPtr);
        PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint8,
                  PCI_CAPBILITY_POINTER_OFFSET,
                  1,
                  &CapabilityPtr
                  );
        while (CapabilityId != 0x10 && CapabilityPtr != 0) {
//          Address = EFI_PCI_ADDRESS (Bus, Device, Function, CapabilityPtr);
//          PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &CapabilityId);
          PciIo->Pci.Read (
                    PciIo,
                    EfiPciIoWidthUint8,
                    CapabilityPtr,
                    1,
                    &CapabilityId
                    );
//          Address = EFI_PCI_ADDRESS (Bus, Device, Function, CapabilityPtr + 1);
//          PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &CapabilityPtr);
          PciIo->Pci.Read (
                    PciIo,
                    EfiPciIoWidthUint8,
                    CapabilityPtr + 1,
                    1,
                    &CapabilityPtr
                    );
        }
        //
        // Capability ID = 0x10, the device is PCIe
        //
        if (CapabilityId == 0x10) {
          for (IndexCheckVgaId = 0; IndexCheckVgaId < sizeof (CheckVgaId) / sizeof (CHECK_VGA_ID); IndexCheckVgaId++) {
            if (VendorId == CheckVgaId[IndexCheckVgaId].VendorId && \
                DeviceId == CheckVgaId[IndexCheckVgaId].DeviceId && \
                SubsystemVendorID == CheckVgaId[IndexCheckVgaId].SubsystemVendorID && \
                SubsystemID == CheckVgaId[IndexCheckVgaId].SubsystemID) {
              VgaDeviceList[Index].VgaType = Pci;
              VgaDeviceList[Index].Priority = 0;
              FindPci = TRUE;
              continue;
            }
          }
          if (!FindPeg) {
            VgaDeviceList[Index].VgaType = Peg;
            VgaDeviceList[Index].Priority = 0;
            FindPeg = TRUE;
            continue;
          }
        } else {
          if (!FindPci) {
            VgaDeviceList[Index].VgaType = Pci;
            VgaDeviceList[Index].Priority = 0;
            FindPci = TRUE;
            continue;
          }
        }
      }
    }

    //
    // VGA Information Default Setting
    //
    VgaDeviceList[Index].VgaType = Pci;
    VgaDeviceList[Index].Priority = 100;
  }

  SelectedVgaHandle = SelectVgaHandle (VgaDeviceList, VgaHandleCount);

  gBS->FreePool (VgaDeviceList);

  return SelectedVgaHandle;
}


EFI_STATUS
LockVgaControllerVgaDriverPolicy (
  IN EFI_HANDLE       SelectedVgaHandle,
  IN EFI_HANDLE       *VgaHandleBuffer,
  IN UINTN            VgaHandleCount
  )
{
  UINTN                          DriverBindingHandleCount;
  EFI_HANDLE                     *DriverBindingHandleBuffer;
  EFI_STATUS                     Status;
  UINTN                          Index;
  CHAR16                         *DriverName;
  EFI_COMPONENT_NAME2_PROTOCOL   *ComponentNameInterface;
  EFI_HANDLE                     LockGopImageHandleList[2];
  EFI_DEVICE_PATH_PROTOCOL       *SelectedVgaDevicePath;

  LockGopImageHandleList[0] = NULL;
  LockGopImageHandleList[1] = NULL;
  //
  // Get VgaDriverPolicy driver by Component name.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDriverBindingProtocolGuid,
                  NULL,
                  &DriverBindingHandleCount,
                  &DriverBindingHandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < DriverBindingHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    DriverBindingHandleBuffer[Index],
                    &gEfiComponentName2ProtocolGuid,
                    (VOID **)&ComponentNameInterface
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = ComponentNameInterface->GetDriverName (
                                       ComponentNameInterface,
                                       LANGUAGE_CODE_ENGLISH_RFC4646,
                                       &DriverName
                                       );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (StrCmp (DriverName, VGA_DRIVER_POLICY_NAME) == 0) {
      LockGopImageHandleList[0] = DriverBindingHandleBuffer[Index];
      break;
    }
  }

  FreePool (DriverBindingHandleBuffer);
  //
  // Get VgaDriverPolicy?
  //
  if (LockGopImageHandleList[0] == NULL) {
    return EFI_NOT_FOUND;
  }
  //
  // Create the volatile variable ACTIVE_VGA_VAR_NAME
  // to notice VgaPolicyDriver to lock the non-Active Vga.
  //
  Status = gBS->HandleProtocol (
                  SelectedVgaHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&SelectedVgaDevicePath
                  );

  BdsLibUpdateConsoleVariable (ACTIVE_VGA_VAR_NAME, SelectedVgaDevicePath, NULL);
  //
  // lock none-primary Vga device.
  //
  if (!EFI_ERROR(Status)) {
    //
    // Add the NULL handle to denote the terminal of driver list.
    //
    LockGopImageHandleList[1] = NULL;
    for (Index = 0; Index < VgaHandleCount; Index++) {
      if (VgaHandleBuffer[Index] != SelectedVgaHandle) {
        Status = gBS->ConnectController (VgaHandleBuffer[Index], LockGopImageHandleList, NULL, FALSE);
      }
    }
  }

  return Status;
}


VOID
EFIAPI
GopHotPlugCallback (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
  )
{
  EFI_HANDLE                             *HandleBuffer;
  UINTN                                  NumberOfHandles;
  UINTN                                  Index;
  EFI_DEVICE_PATH_PROTOCOL               *GopDevicePath;
  EFI_STATUS                             Status;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID*)&GopDevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    BdsLibUpdateConsoleVariable (L"ConOut", GopDevicePath, NULL);
  }

  BdsLibConnectConsoleVariable (L"ConOut");

  gBS->FreePool (HandleBuffer);
}


/**
 Find the platform active vga, and base on the policy to enable the vga as the console out device.
 The policy is driven by one setup variable "VBIOS".

 @param             none

 @retval            EFI_UNSUPPORTED     There is no active vga device
 @retval            EFI_STATUS          Return the status of BdsLibGetVariableAndSize ()
*/
EFI_STATUS
PlatformBdsForceActiveVga (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Index;
  EFI_HANDLE                            *VgaHandleBuffer;
  UINTN                                 VgaHandleCount;
  EFI_DEVICE_PATH_PROTOCOL              *ConOutDevPath;
  EFI_HANDLE                            *SelectedVgaHandle;
  VGA_HANDLES_INFO                      *VgaHandlesInfo;
  CHIPSET_CONFIGURATION                  *SystemConfiguration;
  EFI_EVENT                             ExitBootServicesEvent;
  EFI_EVENT                             LegacyBootEvent;
  EFI_EVENT                             GopHotPlugEvent;
  VOID                                  *Registration;

  SystemConfiguration = NULL;
  VgaHandleBuffer     = NULL;
  VgaHandleCount      = 0;
  ConOutDevPath       = NULL;

  SystemConfiguration = AllocateZeroPool (sizeof (CHIPSET_CONFIGURATION));
  if (SystemConfiguration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = GetChipsetSetupVariableDxe (SystemConfiguration, sizeof (CHIPSET_CONFIGURATION));
  if (EFI_ERROR (Status)) {
    FreePool (SystemConfiguration);
  ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GetAllVgaHandles (&VgaHandleBuffer, &VgaHandleCount);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  SelectedVgaHandle = ClassifyVgaHandleAndSelect (
                        VgaHandleBuffer,
                        VgaHandleCount
                        );
  if ((SystemConfiguration->UefiDualVgaControllers == DUAL_VGA_CONTROLLER_ENABLE) &&
      (BdsLibGetBootType ()                        == EFI_BOOT_TYPE)) {
    for (Index = 0; Index < VgaHandleCount; Index++) {
      PreparePciVgaDevicePath (VgaHandleBuffer[Index], &ConOutDevPath);
    }
  } else {
    PreparePciVgaDevicePath (SelectedVgaHandle, &ConOutDevPath);
  }

  //
  // Register Event for Monitor Hot-plug
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  GopHotPlugCallback,
                  NULL,
                  &GopHotPlugEvent
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiGraphicsOutputProtocolGuid,
                    GopHotPlugEvent,
                    &Registration
                    );
  }

  //
  // In Uefi Boot mode and the all the native GOP are connected failed.
  // We rollback the CSM and try to connect the UefiBiosVideo.
  //
  LegacyBiosDependency (NULL);

  if (VgaHandleCount <= 1) {
    goto Exit;
  }

  if (SystemConfiguration->UefiDualVgaControllers != DUAL_VGA_CONTROLLER_ENABLE) {
    LockVgaControllerVgaDriverPolicy (SelectedVgaHandle, VgaHandleBuffer, VgaHandleCount);
  }

  VgaHandlesInfo = AllocateZeroPool (VgaHandleCount * sizeof (VGA_HANDLES_INFO));
  if (VgaHandlesInfo != NULL) {
    VgaHandlesInfo->VgaHandleBuffer = AllocateZeroPool (VgaHandleCount * sizeof (EFI_HANDLE));
    if (VgaHandlesInfo->VgaHandleBuffer == NULL) {
      goto Exit;
    }
  } else {
    goto Exit;
  }

  VgaHandlesInfo->PrimaryVgaHandle = SelectedVgaHandle;
  VgaHandlesInfo->VgaHandleConut   = VgaHandleCount;
  CopyMem (VgaHandlesInfo->VgaHandleBuffer, VgaHandleBuffer, (VgaHandleCount * sizeof (EFI_HANDLE)));

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_NOTIFY,
                  SetVgaCommandRegEvent,
                  VgaHandlesInfo,
                  &ExitBootServicesEvent
                  );
  Status = EfiCreateEventLegacyBootEx (
             TPL_NOTIFY,
             SetVgaCommandRegEvent,
             VgaHandlesInfo,
             &LegacyBootEvent
             );

Exit:
  if (VgaHandleBuffer) {
    gBS->FreePool (VgaHandleBuffer);
  }
  if (SystemConfiguration != NULL) {
    FreePool (SystemConfiguration);
  }
  return Status;
}


/**
 Compare two device pathes

 @param[in]         DevicePath1         Input device pathes.
 @param[in]         DevicePath2         Input device pathes.

 @retval            TRUE                Same.
 @retval            FALSE               Different.
*/
BOOLEAN
CompareDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath1,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath2
  )
{
  UINTN Size1;
  UINTN Size2;

  Size1 = GetDevicePathSize (DevicePath1);
  Size2 = GetDevicePathSize (DevicePath2);

  if (Size1 != Size2) {
    return FALSE;
  }

  if (CompareMem (DevicePath1, DevicePath2, Size1)) {
    return FALSE;
  }

  return TRUE;
}


/**
 Connect the predefined platform default console device. Always try to find
 and enable the vga device if have.

 @param[in]         PlatformConsole     Predefined platform default console device array.

 @retval            EFI_SUCCESS         Success connect at least one ConIn and ConOut
                                        device, there must have one ConOut device is
                                        active vga device.
 @return            Return the status of BdsLibConnectAllDefaultConsoles ()
*/
EFI_STATUS
PlatformBdsConnectConsole (
  IN BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      Index;
  EFI_DEVICE_PATH_PROTOCOL                   *VarConout;
  EFI_DEVICE_PATH_PROTOCOL                   *VarConin;
  EFI_DEVICE_PATH_PROTOCOL                   *VarErrout;
  EFI_DEVICE_PATH_PROTOCOL                   *NewVarConout;
  EFI_DEVICE_PATH_PROTOCOL                   *NewVarConin;
  EFI_DEVICE_PATH_PROTOCOL                   *NewVarErrout;
  UINTN                                      DevicePathSize;
  CHIPSET_CONFIGURATION                      *SystemConfiguration;
  VOID                                       *CrService;
  EFI_DEVICE_PATH_PROTOCOL                   *UsbConsoleOuputDevPath = NULL;
  AMT_WRAPPER_PROTOCOL                       *AmtWrapper;

  SystemConfiguration = NULL;
  SystemConfiguration = AllocateZeroPool (sizeof (CHIPSET_CONFIGURATION));
  if (SystemConfiguration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = GetChipsetSetupVariableDxe (SystemConfiguration, sizeof (CHIPSET_CONFIGURATION));
  if (EFI_ERROR (Status)) {
    FreePool (SystemConfiguration);
  ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Index = 0;
  Status = EFI_SUCCESS;
  DevicePathSize = 0;

  VarConout = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );
  VarConin = BdsLibGetVariableAndSize (
               L"ConIn",
               &gEfiGlobalVariableGuid,
               &DevicePathSize
               );
  VarErrout = BdsLibGetVariableAndSize (
                L"ErrOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );
  if (VarConout == NULL || VarConin == NULL) {
    //
    // Have chance to connect the platform default console,
    // the platform default console is the minimue device group the platform should support
    //
    while (PlatformConsole[Index].DevicePath != NULL) {
      //
      // Update the console variable with the connect type
      //
      if ((PlatformConsole[Index].ConnectType & CONSOLE_IN) == CONSOLE_IN) {
        BdsLibUpdateConsoleVariable (L"ConIn"             , PlatformConsole[Index].DevicePath, NULL);
        BdsLibUpdateConsoleVariable (CON_IN_CANDIDATE_NAME, PlatformConsole[Index].DevicePath, NULL);
      }

      if ((PlatformConsole[Index].ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
        BdsLibUpdateConsoleVariable (L"ConOut", PlatformConsole[Index].DevicePath, NULL);
        BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME, PlatformConsole[Index].DevicePath, NULL);
      }

      if ((PlatformConsole[Index].ConnectType & STD_ERROR) == STD_ERROR) {
        BdsLibUpdateConsoleVariable (L"ErrOut", PlatformConsole[Index].DevicePath, NULL);
        BdsLibUpdateConsoleVariable (ERR_OUT_CANDIDATE_NAME, PlatformConsole[Index].DevicePath, NULL);
      }

      Index ++;
    }
  }
  if (VarConout) {
    BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME, VarConout, NULL);
  }

  if (VarConin) {
    BdsLibUpdateConsoleVariable (CON_IN_CANDIDATE_NAME, VarConin, NULL);
  }

  if (VarErrout) {
    BdsLibUpdateConsoleVariable (ERR_OUT_CANDIDATE_NAME, VarErrout, NULL);
  }

  //
  // If requested via ASF Intel OEM boot parameters, then we connect the SOL driver.
  // ASF Keyboard locking does not apply here
  //
  if (FeaturePcdGet (PcdAmtEnable)) {

    Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

    if ((!EFI_ERROR (Status)) && (AmtWrapper->IsSolEnabled () == TRUE)) {
      BdsLibUpdateConsoleVariable (CON_IN_CANDIDATE_NAME,   (EFI_DEVICE_PATH_PROTOCOL*) &gSerialOverLANDevicePath, NULL);
      BdsLibUpdateConsoleVariable (L"ConInNoLock",          (EFI_DEVICE_PATH_PROTOCOL*) &gSerialOverLANDevicePath, NULL);
      BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME,  (EFI_DEVICE_PATH_PROTOCOL*) &gSerialOverLANDevicePath, NULL);
    }
  }

  //
  // Make sure we have at least one active VGA, and have the right active VGA in console variable
  //
  Status = PlatformBdsForceActiveVga ();
  if (EFI_ERROR(Status)) {
    Status = gBS->LocateProtocol (&gConsoleRedirectionServiceProtocolGuid, NULL, &CrService);
    if (EFI_ERROR(Status)) {
      if (SystemConfiguration != NULL) {
        FreePool (SystemConfiguration);
      }
      return Status;
    }
  }

  if (VarConout) {
    gBS->FreePool (VarConout);
  }
  VarConout = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );
//if (FeaturePcdGet(PcdH2OConsoleRedirectionSupported)){
//
//  CRBdsHookProtocolStatus = gBS->LocateProtocol (&gCRBdsHookProtocolGuid, NULL, (VOID **)&CRBdsHookProtocol);
//  if (CRBdsHookProtocolStatus == EFI_SUCCESS) {
//      CRBdsHookProtocol->PlatformBDSConnectCRTerminal (
//                         Status,
//                         CON_IN_CANDIDATE_NAME,
//                         &gEfiGenericVariableGuid,
//                         CON_OUT_CANDIDATE_NAME,
//                         &gEfiGenericVariableGuid
//                       );
//  }
//
//}

  //
  // Update default device paths with candicate device path
  //
  NewVarConout = BdsLibGetVariableAndSize (
                   CON_OUT_CANDIDATE_NAME,
                   &gEfiGenericVariableGuid,
                   &DevicePathSize
                   );
  ASSERT(NewVarConout != NULL);
  if (!CompareDevicePath (NewVarConout, VarConout)) {
    gRT->SetVariable (
           L"ConOut",
           &gEfiGlobalVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           GetDevicePathSize (NewVarConout),
           NewVarConout
           );
  }
  gBS->FreePool (NewVarConout);
  if (VarConout) {
    gBS->FreePool (VarConout);
  }

  NewVarConin = BdsLibGetVariableAndSize (
                  CON_IN_CANDIDATE_NAME,
                  &gEfiGenericVariableGuid,
                  &DevicePathSize
                  );
  ASSERT(NewVarConin != NULL);
  if (!CompareDevicePath (NewVarConin,VarConin)) {
    gRT->SetVariable (
           L"ConIn",
           &gEfiGlobalVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           GetDevicePathSize (NewVarConin),
           NewVarConin
           );
  }
  gBS->FreePool (NewVarConin);
  if (VarConin) {
    gBS->FreePool (VarConin);
  }

  NewVarErrout = BdsLibGetVariableAndSize (
                   ERR_OUT_CANDIDATE_NAME,
                   &gEfiGenericVariableGuid,
                   &DevicePathSize
                   );
  if (NewVarErrout != NULL) {
    if (!CompareDevicePath (NewVarErrout,VarErrout)) {
      gRT->SetVariable (
             L"ErrOut",
             &gEfiGlobalVariableGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             GetDevicePathSize (NewVarErrout),
             NewVarErrout
             );
    }
    gBS->FreePool (NewVarErrout);
  }
  if (VarErrout != NULL) {
    gBS->FreePool (VarErrout);
  }
  BdsLibConnectI2cDevice ();
  if (!(SystemConfiguration->UsbHotKeySupport == 0 && BdsLibIsWin8FastBootActive ())) {
    BdsLibConnectUsbHID ();
    if (FoundUSBConsoleOutput (&UsbConsoleOuputDevPath)) {
      SetUsbConsoleOutToConOutVar (UsbConsoleOuputDevPath);
      FreePool (UsbConsoleOuputDevPath);
    }
  } else {
    BdsLibConnectUsbHIDNotifyRegister ();
  }
  //
  // Connect the all the default console with current console variable
  //
  Status = BdsLibConnectAllDefaultConsoles ();

  DEBUG ((DEBUG_INFO, "Dxe OemChipsetServices Call:OemSvcSetBacklightControl\n"));
  Status = OemSvcSetBacklightControl();
  DEBUG ((DEBUG_INFO, "Dxe OemChipsetServices OemSvcSetBacklightControl Status: %r\n", Status));

  if (SystemConfiguration != NULL) {
    FreePool (SystemConfiguration);
  }
  return Status;
}

/**
 Connect with predefined platform connect sequence,
 the OEM/IBV can customize with their own connect sequence.
*/
VOID
PlatformBdsConnectSequence (
  VOID
  )
{
  UINTN                     Index;

  Index = 0;

  //
  // Here we can get the customized platform connect sequence
  // Notes: we can connect with new variable which record the last time boots connect device path sequence
  //
  while (gPlatformConnectSequence[Index] != NULL) {
    //
    // Build the platform boot option
    //
    BdsLibConnectDevicePath (gPlatformConnectSequence[Index]);
    Index ++;
  }

  //
  // For the debug tip, just use the simple policy to connect all devices
  //
  BdsLibConnectAll ();
}

/**
 Load the predefined driver option, OEM/IBV can customize this
 to load their own drivers

 @param[in, out]    BdsDriverLists      The header of the driver option link list.
*/
VOID
PlatformBdsGetDriverOption (
  IN OUT LIST_ENTRY              *BdsDriverLists
  )
{
  UINTN                              Index;

  Index = 0;

  //
  // Here we can get the customized platform driver option
  //
  while (gPlatformDriverOption[Index] != NULL) {
    //
    // Build the platform boot option
    //
    BdsLibRegisterNewOption (BdsDriverLists, gPlatformDriverOption[Index], NULL, L"DriverOrder", NULL, 0);
    Index ++;
  }
}


/**
  Internal function to initalize H2O_BDS_CP_DISPLAY_BEFORE_DATA data.

  @param[in]  QuietBoot           Boolean value to indicate quiet boot is enabled or disabled.
  @param[out] CpData              A Pointer to H2O_BDS_CP_DISPLAY_BEFORE_DATA instance to initialized.

  @retval EFI_SUCCESS             Init H2O_BDS_CP_DISPLAY_BEFORE_DATA data successfully.
  @retval EFI_INVALID_PARAMETER   CpData is NULL.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory for H2O_BDS_CP_DISPLAY_BEFORE_DATA interface failed.
**/
STATIC
EFI_STATUS
InitBeforeDisplayData (
  IN  BOOLEAN                                QuietBoot,
  OUT H2O_BDS_CP_DISPLAY_BEFORE_DATA         **CpData
  )
{
  EFI_STATUS                           Status;
  VOID                                 *Interface;
  H2O_BDS_CP_DISPLAY_BEFORE_DATA       *BdsDisplayBeforeData;

  if (CpData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  BdsDisplayBeforeData = AllocateZeroPool (sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA));
  if (BdsDisplayBeforeData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (BdsDisplayBeforeData, sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA));
  BdsDisplayBeforeData->Size   = sizeof (H2O_BDS_CP_DISPLAY_BEFORE_DATA);
  BdsDisplayBeforeData->Status = H2O_BDS_TASK_NORMAL;
  if (QuietBoot) {
    BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_LOGO;
    Status = gBS->LocateProtocol (&gEfiBootLogoProtocolGuid, NULL, (VOID **) &Interface);
    if (!EFI_ERROR (Status)) {
      BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_BGRT;
    }
  } else {
    BdsDisplayBeforeData->Features |= H2O_BDS_CP_DISPLAY_INFO;
  }
  *CpData = BdsDisplayBeforeData;
  return EFI_SUCCESS;
}

/**
  Internal function to initalize H2O_BDS_CP_DISPLAY_BEFORE_DATA
  data and trigger gH2OBdsCpDisplayBeforeGuid checkpoint.

  @param[in] QuietBoot            Boolean value to indicate quiet boot is enabled or disabled.

  @retval EFI_SUCCESS             Trigger gH2OBdsCpDisplayBeforeGuid checkpoint successfully.
  @retval EFI_ALREADY_STARTED     The gH2OBdsCpDisplayBeforeGuid checkpoint has been triggered.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
  @return Other                   Other error occurred while triggering gH2OBdsCpDisplayBeforeGuid
                                  checkpoint.
**/
STATIC
EFI_STATUS
TriggerCpDisplayBefore (
  IN  BOOLEAN       QuietBoot
  )
{
  EFI_STATUS                           Status;
  H2O_BDS_CP_DISPLAY_BEFORE_DATA       *BdsDisplayBeforeData;
  EFI_HANDLE                           Handle;

  Status = gBS->LocateProtocol (
                  &gH2OBdsCpDisplayBeforeGuid,
                  NULL,
                  (VOID **) &BdsDisplayBeforeData
                  );
  ASSERT (Status != EFI_SUCCESS);
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }
  Status = InitBeforeDisplayData (QuietBoot, &BdsDisplayBeforeData);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  //
  // Install checpoint data to protocol for later usage.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  (EFI_GUID *) &gH2OBdsCpDisplayBeforeGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID *) BdsDisplayBeforeData
                  );
  DEBUG ((DEBUG_INFO, "Checkpoint Trigger: %g\n", gH2OBdsCpDisplayBeforeGuid));
  Status = H2OCpTrigger (&gH2OBdsCpDisplayBeforeGuid, BdsDisplayBeforeData);
  DEBUG ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsDisplayBeforeData->Status));
  return Status;
}

/**
 Perform the platform diagnostic, such like test memory. OEM/IBV also
 can customize this function to support specific platform diagnostic.

 @param[in]         MemoryTestLevel     The memory test intensive level
 @param[in]         QuietBoot           Indicate if need to enable the quiet boot
 @param[in]         BaseMemoryTest      A pointer to BdsMemoryTest()
*/
VOID
PlatformBdsDiagnostics (
  IN EXTENDMEM_COVERAGE_LEVEL    MemoryTestLevel,
  IN BOOLEAN                     QuietBoot,
  IN BASEM_MEMORY_TEST           BaseMemoryTest
  )
{
  EFI_STATUS                        Status;
  UINT8                             PrintLine;
  EFI_OEM_BADGING_SUPPORT_PROTOCOL  *Badging;
  UINT16                            FunctionKey;
  BOOLEAN                           KeyPressed;
  BOOLEAN                           EnableQuietBootState;
  EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;
  BOOLEAN                           EnableAmtSol;
  AMT_WRAPPER_PROTOCOL              *AmtWrapper;

  PrintLine = 0;
  EnableAmtSol = FALSE;

  //
  // PostCode = 0x1D, Display logo or system information
  //
  POST_CODE (BDS_DISPLAY_LOGO_SYSTEM_INFO);

  BdsLibOnStartOfBdsDiagnostics ();

  //
  // OemServices
  //
  DEBUG ((DEBUG_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
  Status = OemSvcDisplayLogo (
             &QuietBoot
             );
  DEBUG ((DEBUG_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

  Status = gBS->LocateProtocol (&gEfiConsoleControlProtocolGuid, NULL, (VOID **)&ConsoleControl);
  if (EFI_ERROR (Status)) {
    ConsoleControl = NULL;
  }

  //
  // Here we can decide if we need to show the diagnostics screen
  // Notes: this quiet boot code should be remove from the graphic lib
  //
  BdsLibGetQuietBootState (&EnableQuietBootState);
  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    TriggerCpDisplayBefore (EnableQuietBootState);
  }
  if (FeaturePcdGet (PcdAmtEnable)) {
    Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

    if ((!EFI_ERROR (Status)) && (AmtWrapper->IsSolEnabled ())) {
      EnableAmtSol = TRUE;
      QuietBoot = FALSE;
      EnableQuietBootState = FALSE;
    }
  }
  if (EnableQuietBootState) {
    EnableQuietBoot (&gEfiUgaSplashProtocolGuid);
  }

  BdsLibSetHotKeyDelayTime ();

  //
  // Perform system diagnostic
  //
  if (!QuietBoot) {
    gST->ConOut->ClearScreen (gST->ConOut);
  }

  BdsLibGetHotKey (&FunctionKey, &KeyPressed);
  Status = gBS->LocateProtocol (&gEfiOEMBadgingSupportProtocolGuid, NULL, (VOID **)&Badging);
  if (!EFI_ERROR (Status)) {
    if (KeyPressed) {
      if (ConsoleControl != NULL) {
        ShowOemString (Badging, TRUE, (UINT8) FunctionKey);
        BdsLibShowOemStringInTextMode (TRUE, (UINT8) FunctionKey);
      } else {
        if (QuietBoot) {
          ShowOemString (Badging, TRUE, (UINT8) FunctionKey);
        } else {
          BdsLibShowOemStringInTextMode (TRUE, (UINT8) FunctionKey);
        }
      }
    }
  }

  //
  // Perform system diagnostic
  //
  if (!QuietBoot) {
    gST->ConOut->ClearScreen (gST->ConOut);
    if (KeyPressed) {
      Status = BdsLibShowOemStringInTextMode (TRUE, (UINT8)FunctionKey);
    } else {
      Status = BdsLibShowOemStringInTextMode (FALSE, 0);
    }

    gST->ConOut->SetCursorPosition (gST->ConOut, 0, 0);
    Status = ShowSystemInfo (&PrintLine);
  }

  Status = BaseMemoryTest (MemoryTestLevel, (UINTN) (PrintLine + 2));
  if (EFI_ERROR (Status)) {
    DisableQuietBoot ();
  }

  return;
}


/**
 Perform the platform diagnostic, such like test memory. OEM/IBV also
 can customize this fuction to support specific platform diagnostic.

 @param[in]         LogoDisplay         Indicate if need to enable logo display
 @param[in]         HotKeyService       Indicate if need to enable hotkey service
 @param[in]         QuietBoot           Indicate if need to enable the quiet boot
 @param[in]         BaseMemoryTest      The memory test function pointer
*/
VOID
PlatformBdsDiagnosticsMini (
  IN BOOLEAN                     LogoDisplay,
  IN BOOLEAN                     HotKeyService,
  IN BOOLEAN                     QuietBoot,
  IN BASEM_MEMORY_TEST           BaseMemoryTest
  )
{
  EFI_STATUS                        Status;
  UINT8                             PrintLine;
  BOOLEAN                           EnableQuietBootState;
  EFI_OEM_BADGING_SUPPORT_PROTOCOL  *Badging;
  UINT16                            FunctionKey;
  BOOLEAN                           KeyPressed;
  EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;

  PrintLine = 0;

  if (LogoDisplay) {
    //
    // PostCode = 0x1D, Display logo or system information
    //
    POST_CODE (BDS_DISPLAY_LOGO_SYSTEM_INFO);

    BdsLibOnStartOfBdsDiagnostics ();
    //
    // OemServices
    //
    DEBUG ((DEBUG_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
    Status = OemSvcDisplayLogo (
               &LogoDisplay
               );
    DEBUG ((DEBUG_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

    //
    // Here we can decide if we need to show
    // the diagnostics screen
    // Notes: this quiet boot code should be remove
    // from the graphic lib
    //
    BdsLibGetQuietBootState (&EnableQuietBootState);
    if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
      TriggerCpDisplayBefore (EnableQuietBootState);
    }
    if (EnableQuietBootState) {
      EnableQuietBoot (&gEfiUgaSplashProtocolGuid);
    }
  }

  if (HotKeyService) {
    Status = gBS->LocateProtocol (&gEfiConsoleControlProtocolGuid, NULL, (VOID **)&ConsoleControl);
    if (EFI_ERROR (Status)) {
      ConsoleControl = NULL;
    }

    BdsLibGetHotKey (&FunctionKey, &KeyPressed);
    Status = gBS->LocateProtocol (&gEfiOEMBadgingSupportProtocolGuid, NULL, (VOID **)&Badging);
    if (!EFI_ERROR (Status)){
      if (KeyPressed) {
        if (ConsoleControl != NULL) {
          ShowOemString(Badging, TRUE, (UINT8) FunctionKey);
          BdsLibShowOemStringInTextMode (TRUE, (UINT8) FunctionKey);
        } else {
          if (QuietBoot) {
            ShowOemString(Badging, TRUE, (UINT8) FunctionKey);
          } else {
            BdsLibShowOemStringInTextMode (TRUE, (UINT8) FunctionKey);
          }
        }
      }
    }
  }

  Status = BaseMemoryTest (IGNORE, (UINTN)(PrintLine + 2));
  if (EFI_ERROR (Status)) {
    DisableQuietBoot ();
  }

  return;
}


/**
 If the bootable device is legacy OS, delete BootPrevious Variable.

 @param             Event               The event that triggered this notification function
 @param             ParentImageHandle   Pointer to the notification functions context

 @retval            EFI_STATUS          Success to delete BootPrevious Variable.
*/
EFI_STATUS
BdsLegacyBootEvent (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
{
  UINTN               BootPreviousSize;
  UINT16              *BootPrevious;

  BootPrevious = BdsLibGetVariableAndSize (
                   L"BootPrevious",
                   &gEfiGenericVariableGuid,
                   &BootPreviousSize
                   );
  if (BootPrevious != NULL) {
    //
    // OS has been changed from UEFI to Legacy
    //
    gBS->FreePool (BootPrevious);

    BootPrevious = NULL;
    gRT->SetVariable (
           L"BootPrevious",
           &gEfiGenericVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           0,
           BootPrevious
           );
  }

  gBS->CloseEvent (Event);

  return EFI_SUCCESS;
}

/**
 Remote assistance launch function.

 @param[in] This         Pointer to the current instance of this protocol.
 @param[in] Id           Unsigned integer associated with this specific hot key.
 @param[in] Options      Optional pointer to a null-terminated ASCII string that contains additional operating parameters
 @param[in] Context      Pointer to a structure that contains additional state information about the BDS

 @retval EFI_SUCCESS     Success to perform remote assistance launch.
*/
EFI_STATUS
RemoteAsstLaunch (
  IN H2O_BDS_HOT_KEY_PROTOCOL          *This,
  IN UINT32                            Id,
  IN CONST CHAR8                       *Options OPTIONAL,
  IN H2O_BDS_CONTEXT                   *Context
  )
{
  DisableQuietBoot ();
  gST->ConOut->EnableCursor (gST->ConOut, TRUE);
  BdsLibStartSetupUtility (TRUE);
  InvokeRemoteAsstHotKey ();

  return EFI_SUCCESS;
}

/**
 Set the hot key description of remote assistance.

 @param[in] H2OBdsServices    Pointer to H2O BDS serivce protocol
 @param[in] HotKeyHandle      Pointer to registered hot key handle
*/
VOID
RemoteAsstSetHotKeyDesc (
  IN H2O_BDS_SERVICES_PROTOCOL       *H2OBdsServices,
  IN EFI_HANDLE                      HotKeyHandle
  )
{
//  CHIPSET_CONFIGURATION              *SetupVariable;
  CHAR16                             *BeforeDescText;
  CHAR16                             *AfterDescText;
  UINT8                              BootDisplayMode;
  INT32                              X;
  UINT32                             XUnit;
  UINT32                             XOrigin;
  INT32                              Y;
  UINT32                             YUnit;
  UINT32                             YOrigin;
  EFI_STATUS                         Status;
  ME_SETUP                           *MeSetup;

  MeSetup = AllocateZeroPool (sizeof (ME_SETUP));
  if (MeSetup == NULL) {
    return;
  }
//  Status = GetChipsetSetupVariableDxe (SetupVariable, sizeof (CHIPSET_CONFIGURATION));
  Status = GetChipsetMeSetupVariableDxe (MeSetup, sizeof (ME_SETUP));
  if (EFI_ERROR (Status)) {
    FreePool (MeSetup);
    ASSERT_EFI_ERROR (Status);
    return;
  }


  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_ALL;
  BeforeDescText  = PlatformBdsGetStringById (STRING_TOKEN (STR_REMOTE_ASSISTANCE_BEFORE_STR));
  if (MeSetup->MeImageType == ME_IMAGE_CORPORATE_SKU_FW) {
    AfterDescText = PlatformBdsGetStringById (STRING_TOKEN (STR_REMOTE_ASSISTANCE_AFTER_STR));
  } else {
    AfterDescText = NULL;
  }
  H2OBdsServices->SetHotKeyDescText (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    BeforeDescText,
                    AfterDescText
                    );
  if (BeforeDescText != NULL) {
    FreePool (BeforeDescText);
  }
  if (AfterDescText != NULL) {
    FreePool (AfterDescText);
  }

  //
  // Graphics mode
  //
  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_QUIET;
  X       = 0;
  Y       = 1;
  XUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  YUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  XOrigin = H2O_BDS_DISPLAY_ORIGIN_X_CENTER;
  YOrigin = H2O_BDS_DISPLAY_ORIGIN_Y_TOP_PERCENT(70);
  H2OBdsServices->SetHotKeyDescPosn (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    X,
                    XUnit,
                    &XOrigin,
                    Y,
                    YUnit,
                    &YOrigin
                    );

  //
  // Text mode
  //
  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_NORMAL;
  X       = 0;
  Y       = -1;
  XUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  YUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  XOrigin = H2O_BDS_DISPLAY_ORIGIN_X_LEFT;
  YOrigin = H2O_BDS_DISPLAY_ORIGIN_Y_BOTTOM;
  H2OBdsServices->SetHotKeyDescPosn (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    X,
                    XUnit,
                    &XOrigin,
                    Y,
                    YUnit,
                    &YOrigin
                    );
  if (MeSetup != NULL) {
    FreePool (MeSetup);
  }
}

/**
 Register the hot key of remote assistance.

 @retval EFI_SUCCESS     Success to register the hot key of remote assistance.
 @retval EFI_NOT_FOUND   Fail to get the key string.
 @retval Other           Locate protocol failed or register hot key return failed.
*/
EFI_STATUS
RemoteAsstRegisterHotKey (
  VOID
  )
{
  EFI_STATUS                         Status;
  H2O_BDS_SERVICES_PROTOCOL          *H2OBdsServices;
  EFI_HANDLE                         HotKeyHandle;
  CHAR8                              *KeyAsciiStr;

  Status = gBS->LocateProtocol (&gH2OBdsServicesProtocolGuid, NULL, (VOID **) &H2OBdsServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  KeyAsciiStr = PlatformBdsGetAsciiStringById (STRING_TOKEN (STR_REMOTE_ASSISTANCE_KEY));
  if (KeyAsciiStr == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = H2OBdsServices->RegisterCallbackHotKey (
                             H2OBdsServices,
                             KeyAsciiStr,
                             RemoteAsstLaunch,
                             0,
                             NULL,
                             &HotKeyHandle
                             );
  FreePool (KeyAsciiStr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  RemoteAsstSetHotKeyDesc (H2OBdsServices, HotKeyHandle);

  return EFI_SUCCESS;
}

/**
 MEBX launch function.

 @param[in] This         Pointer to the current instance of this protocol.
 @param[in] Id           Unsigned integer associated with this specific hot key.
 @param[in] Options      Optional pointer to a null-terminated ASCII string that contains additional operating parameters
 @param[in] Context      Pointer to a structure that contains additional state information about the BDS

 @retval EFI_SUCCESS     Success to perform MEBX launch.
*/
EFI_STATUS
MEBXLaunch (
  IN H2O_BDS_HOT_KEY_PROTOCOL          *This,
  IN UINT32                            Id,
  IN CONST CHAR8                       *Options OPTIONAL,
  IN H2O_BDS_CONTEXT                   *Context
  )
{
  DisableQuietBoot ();
  gST->ConOut->EnableCursor (gST->ConOut, TRUE);
  BdsLibStartSetupUtility (TRUE);
  InvokeMebxHotKey ();

  return EFI_SUCCESS;
}

/**
 Set the hot key description of MEBX.

 @param[in] H2OBdsServices    Pointer to H2O BDS serivce protocol
 @param[in] HotKeyHandle      Pointer to registered hot key handle
*/
VOID
MEBXSetHotKeyDesc (
  IN H2O_BDS_SERVICES_PROTOCOL       *H2OBdsServices,
  IN EFI_HANDLE                      HotKeyHandle
  )
{
//  CHIPSET_CONFIGURATION              *SetupVariable;
  CHAR16                             *BeforeDescText;
  CHAR16                             *AfterDescText;
  UINT8                              BootDisplayMode;
  INT32                              X;
  UINT32                             XUnit;
  UINT32                             XOrigin;
  INT32                              Y;
  UINT32                             YUnit;
  UINT32                             YOrigin;
  ME_SETUP                           *MeSetup;
  EFI_STATUS                         Status;


  MeSetup = AllocateZeroPool (sizeof (ME_SETUP));
  if (MeSetup == NULL) {
    return;
  }

//  GetChipsetSetupVariableDxe (&SetupVariable);
  Status = GetChipsetMeSetupVariableDxe (MeSetup, sizeof (ME_SETUP));
  if (EFI_ERROR (Status)) {
    FreePool (MeSetup);
    ASSERT_EFI_ERROR (Status);
    return;
  }
  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_ALL;
  BeforeDescText  = PlatformBdsGetStringById (STRING_TOKEN (STR_MEBX_BEFORE_STR));
  if (MeSetup->MeImageType == ME_IMAGE_CORPORATE_SKU_FW) {
    AfterDescText = PlatformBdsGetStringById (STRING_TOKEN (STR_MEBX_AFTER_STR));
  } else {
    AfterDescText = NULL;
  }
  H2OBdsServices->SetHotKeyDescText (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    BeforeDescText,
                    AfterDescText
                    );
  if (BeforeDescText != NULL) {
    FreePool (BeforeDescText);
  }
  if (AfterDescText != NULL) {
    FreePool (AfterDescText);
  }

  //
  // Graphics mode
  //
  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_QUIET;
  X       = 0;
  Y       = 2;
  XUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  YUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  XOrigin = H2O_BDS_DISPLAY_ORIGIN_X_CENTER;
  YOrigin = H2O_BDS_DISPLAY_ORIGIN_Y_TOP_PERCENT(70);
  H2OBdsServices->SetHotKeyDescPosn (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    X,
                    XUnit,
                    &XOrigin,
                    Y,
                    YUnit,
                    &YOrigin
                    );

  //
  // Text mode
  //
  BootDisplayMode = H2O_BDS_BOOT_DISPLAY_MODE_NORMAL;
  X       = 0;
  Y       = -2;
  XUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  YUnit   = H2O_BDS_DISPLAY_UNIT_CHAR;
  XOrigin = H2O_BDS_DISPLAY_ORIGIN_X_LEFT;
  YOrigin = H2O_BDS_DISPLAY_ORIGIN_Y_BOTTOM;
  H2OBdsServices->SetHotKeyDescPosn (
                    H2OBdsServices,
                    HotKeyHandle,
                    BootDisplayMode,
                    X,
                    XUnit,
                    &XOrigin,
                    Y,
                    YUnit,
                    &YOrigin
                    );

  if (MeSetup != NULL) {
    FreePool (MeSetup);
  }
}

/**
 Register the hot key of MEBX.

 @retval EFI_SUCCESS     Success to register the hot key of MEBX.
 @retval EFI_NOT_FOUND   Fail to get the key string.
 @retval Other           Locate protocol failed or register hot key return failed.
*/
EFI_STATUS
MEBXRegisterHotKey (
  VOID
  )
{
  EFI_STATUS                         Status;
  H2O_BDS_SERVICES_PROTOCOL          *H2OBdsServices;
  EFI_HANDLE                         HotKeyHandle;
  CHAR8                              *KeyAsciiStr;

  Status = gBS->LocateProtocol (&gH2OBdsServicesProtocolGuid, NULL, (VOID **)&H2OBdsServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  KeyAsciiStr = PlatformBdsGetAsciiStringById (STRING_TOKEN (STR_MEBX_KEY));
  if (KeyAsciiStr == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = H2OBdsServices->RegisterCallbackHotKey (
                             H2OBdsServices,
                             KeyAsciiStr,
                             MEBXLaunch,
                             0,
                             NULL,
                             &HotKeyHandle
                             );
  FreePool (KeyAsciiStr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MEBXSetHotKeyDesc (H2OBdsServices, HotKeyHandle);

  return EFI_SUCCESS;
}

VOID
SignalAllDriversConnected (
  VOID
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;

  //
  // Inform other code that all drivers have been connected.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gBdsAllDriversConnectedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
 Change reset system type to specific to do PCH global reset.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
ResetSystemCallback (
  IN EFI_EVENT                         Event,
  IN H2O_CP_HANDLE                     Handle
  )
{
  EFI_STATUS                           Status;
  H2O_DXE_CP_RESET_SYSTEM_DATA         *ResetSystemData;
  STATIC PCH_RESET_DATA                ResetData;

  Status = H2OCpLookup (Handle, (VOID **) &ResetSystemData, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

  ResetSystemData->ResetType     = EfiResetPlatformSpecific;
  ResetSystemData->ResetStatus   = EFI_SUCCESS;
  ResetSystemData->ResetDataSize = sizeof (ResetData);
  ResetSystemData->ResetData     = &ResetData;
  ResetSystemData->Status        = H2O_CP_TASK_UPDATE;
}




/**
 This function is a sample to show how to request NVME driver to not
 bind with a specific NVME device

*/

VOID
IgnoreSpecficDevice(
  VOID
  )
{
  NVME_IGNORE_DEV           *IgnoreDevInfo;
  NVME_IGNORE_DEV           EmptyDevInfo;
  UINTN                     IgnoreTableSize;
  UINTN                     Count;


  //
  //  When a SATA controller is ignored, it should not enter start() to be initialized
  //
  ZeroMem (&EmptyDevInfo, sizeof (NVME_IGNORE_DEV));
  IgnoreDevInfo = NULL;
  IgnoreDevInfo = (NVME_IGNORE_DEV*) PcdGetPtr (PcdH2OSataIgnoredDeviceList);
  if (IgnoreDevInfo == NULL) {
    return;
  }

  IgnoreTableSize = PcdGetSize(PcdH2OSataIgnoredDeviceList);
  Count = IgnoreTableSize/sizeof(NVME_IGNORE_DEV);
  while ((IgnoreDevInfo->Bus != 0xFF) && (Count > 1)) {
    if (NVME_COMPARE_BUSDEVFUNC (IgnoreDevInfo, EmptyDevInfo) &&
      NVME_COMPARE_ID (IgnoreDevInfo, EmptyDevInfo)){
      IgnoreDevInfo->VendorId = 0x8086;
      IgnoreDevInfo->DeviceId = 0x0975;
    }
    IgnoreDevInfo++;
    Count--;
  }

}




/**
 The function will execute with as the platform policy, current policy
 is driven by boot mode. IBV/OEM can customize this code for their specific
 policy action.

 @param[in]         DriverOptionList    The header of the driver option link list
 @param[in]         BootOptionList      The header of the boot option link list
 @param[in]         ProcessCapsules     A pointer to ProcessCapsules()
 @param[in]         BaseMemoryTest      A pointer to BaseMemoryTest()
*/
VOID
EFIAPI
PlatformBdsPolicyBehavior (
  IN LIST_ENTRY                      *DriverOptionList,
  IN LIST_ENTRY                      *BootOptionList,
  IN PROCESS_CAPSULES                ProcessCapsules,
  IN BASEM_MEMORY_TEST               BaseMemoryTest
  )
{
  EFI_STATUS                         Status;
  UINT16                             Timeout;
  CHIPSET_CONFIGURATION              *SystemConfiguration;
  EXTENDMEM_COVERAGE_LEVEL           MemoryTestLevel;
  HOT_KEY_CONTEXT                    *HotKeyContext;
  UINT16                             FunctionKey;
  BOOLEAN                            EnterSetup;
  EFI_EVENT                          ReadyToBootEvent;
  EFI_DEVICE_PATH_PROTOCOL           *PlatformConnectLastBoot;
  UINT16                             *BootPrevious;
  EFI_HANDLE                         Handle;
  EFI_STATUS                         LocateDevicePathStatus;
  BOOLEAN                            HotKeyPressed;
  EFI_EVENT                          LegacyBootEvent;
  UINTN                              Size;
  EFI_BOOT_MODE                      BootMode;
//  EFI_PEI_HOB_POINTERS               Hob;
//  EFI_BOOT_MODE                      OriBootMode;
  VOID                               *Registration;
  EFI_DEVICE_PATH_PROTOCOL           *TargetHddDevicePath;
  UINTN                              VarSize;
  SA_SETUP                           *SaSetup;
  PCH_SETUP                          *PchSetup;
  UINTN                              SetupSize;
  AMT_WRAPPER_PROTOCOL               *AmtWrapper;

  TargetHddDevicePath = NULL;
  SystemConfiguration = NULL;
  PlatformConnectLastBoot = NULL;
  PchSetup = NULL;
  BootMode = GetBootModeHob ();
//  OriBootMode = GetBootModeHob ();;
  //
  // Install notifications for required protocols
  //
  EfiCreateProtocolNotifyEvent (
//    &gExitPmAuthProtocolGuid,
    &gEfiSmmLockEnablePointProtocolGuid,
    TPL_CALLBACK,
//    PlatformBdsExitPmAuthCallback,
    PlatformBdsTriggerEndOfDxeEvent,
    NULL,
    &Registration
    );

  EfiCreateProtocolNotifyEvent (
    &gEfiSmmLockEnablePointProtocolGuid,
    (TPL_CALLBACK - 1),
    PlatformBdsTriggerSmmReadyToLock,
    &BootMode,
    &Registration
    );

  SystemConfiguration = AllocateZeroPool (sizeof (CHIPSET_CONFIGURATION));
  if (SystemConfiguration == NULL) {
    goto FreePoolBeforeExitFunction;
  }
  Status = GetChipsetSetupVariableDxe (SystemConfiguration, sizeof (CHIPSET_CONFIGURATION));
  if (EFI_ERROR (Status)) {
    goto FreePoolBeforeExitFunction;
  }

  SetupSize = sizeof (SA_SETUP);
  SaSetup = AllocateZeroPool (SetupSize);
  if (SaSetup == NULL) {
    goto FreePoolBeforeExitFunction;
  }
  Status = gRT->GetVariable (
                SA_SETUP_VARIABLE_NAME,
                &gSaSetupVariableGuid,
                NULL,
                &SetupSize,
                SaSetup
                );
  if (EFI_ERROR (Status)) {
    FreePool (SaSetup);
    goto FreePoolBeforeExitFunction;
  }

  SetupSize = sizeof (PCH_SETUP);
  PchSetup = AllocateZeroPool (SetupSize);
  if (PchSetup == NULL) {
    return;
  }
  Status = gRT->GetVariable (
                PCH_SETUP_VARIABLE_NAME,
                &gPchSetupVariableGuid,
                NULL,
                &SetupSize,
                PchSetup
                );
  if (EFI_ERROR (Status)) {
    goto FreePoolBeforeExitFunction;
  }

//  Status = GetChipsetSetupVariableDxe(&SystemConfiguration);
//  ASSERT_EFI_ERROR (Status);

//  if (!DoesOsIndicateBootToFwUI () &&
//      !(BootMode == BOOT_ON_S4_RESUME && BdsLibGetBootType () != EFI_BOOT_TYPE)) {
  HotKeyContext = AllocateZeroPool (sizeof (HOT_KEY_CONTEXT));
  ASSERT (HotKeyContext != NULL);
  if (HotKeyContext == NULL) {
    goto FreePoolBeforeExitFunction;
  }
  HotKeyContext->EnableQuietBootPolicy = (BOOLEAN) SystemConfiguration->QuietBoot;
  HotKeyContext->CanShowString         = FALSE;

  //
  // OemServices
  //
  DEBUG ((EFI_D_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
  Status = OemSvcDisplayLogo (
             &HotKeyContext->EnableQuietBootPolicy
             );
  DEBUG ((EFI_D_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

  POST_CODE (BDS_INSTALL_HOTKEY);
  BdsLibInstallHotKeys (HotKeyContext);
//  } else {
//    Status = gBS->LocateProtocol (&gH2OBdsServicesProtocolGuid, NULL, (VOID **)&H2OBdsServices);
//    if (!EFI_ERROR (Status)) {
//      EnableHotKey = FALSE;
//      H2OBdsServices->EnableHotKeys (H2OBdsServices, &EnableHotKey);
//    }
//  }

  FunctionKey = NO_OPERATION;
  if (BootMode == BOOT_ASSUMING_NO_CONFIGURATION_CHANGES ||
      BootMode == BOOT_ON_S4_RESUME) {
    BootPrevious = BdsLibGetVariableAndSize (
                     L"BootPrevious",
                     &gEfiGenericVariableGuid,
                     &Size
                     );
    if (BootPrevious == NULL || Size == 0) {
      //
      // Cannot find BootPrevious variable, boot with full configuration.
      //
      BootMode = BOOT_WITH_FULL_CONFIGURATION;
    }
  } else {
    //
    // Clear the BootPrevious variable
    //
    BootPrevious = NULL;
    gRT->SetVariable (
           L"BootPrevious",
           &gEfiGenericVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           0,
           BootPrevious
           );
  }

  //
  // Create a Legacy Boot Event to delete variable, if the bootable device is legacy OS.
  //
  Status = EfiCreateEventLegacyBootEx (
             TPL_CALLBACK,
             BdsLegacyBootEvent,
             NULL,
             &LegacyBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  //
  // PostCode = 0x12, ASF Initial
  //
  POST_CODE (BDS_ASF_INIT);

  //
  // Get the ASF boot options that were discovered during ASF initialization.
  //
  if (FeaturePcdGet (PcdAmtEnable)) {
    Status = BdsAsfInitialization ();
    RemoteAsstRegisterHotKey ();
    MEBXRegisterHotKey ();
  }

  EnterSetup = FALSE;

  //
  // Init the time out value
  //
  if (FeaturePcdGet (PcdAmtEnable)) {
    Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

    if ((!EFI_ERROR (Status)) && (AmtWrapper->IsPauseBootEnabled () == TRUE)) {
      Timeout = 0xFFFF; // wait indefinetly at splash screen
    } else {
      Size = sizeof (Timeout);
      Status = gRT->GetVariable (L"Timeout", &gEfiGlobalVariableGuid, NULL, &Size, &Timeout);
    }
  } else {
    Size = sizeof (Timeout);
    Status = gRT->GetVariable (L"Timeout", &gEfiGlobalVariableGuid, NULL, &Size, &Timeout);
  }
  if (FeaturePcdGet (PcdAmtEnable)) {
    Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

    if (!EFI_ERROR (Status)) {
      if (AmtWrapper->IsEnterSetupEnabled () ||
          AmtWrapper->IsStorageRedirectionEnabled ()||
          AmtWrapper->IsKvmEnabled () ||
          AmtWrapper->IsSolEnabled () ||
          AsfGetSpecialCommand ()){
        //
        // clear TargetHddDevPath variable if any of ME remote features were enabled.
        //
        TargetHddDevicePath = BdsLibGetVariableAndSize (
                                L"TargetHddDevPath",
                                &gEfiGenericVariableGuid,
                                &VarSize
                                );
         if (TargetHddDevicePath != NULL) {
           gRT->SetVariable (
                  L"TargetHddDevPath",
                  &gEfiGenericVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  0,
                  (VOID *) NULL
                  );
         }
      }
    }
  }
  //
  // Load the driver option as the driver option list
  //
  PlatformBdsGetDriverOption (DriverOptionList);

  //
  // Clear the capsule variable as early as possible
  // which will avoid the next time boot after the capsule update will still into the capsule loop
  //
  gRT->SetVariable (
         L"CapsuleUpdateData",
         &gEfiCapsuleVendorGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         0,
         (VOID *) NULL
         );

  //
  // Set memory test level by platform requirement
  //
  MemoryTestLevel = (SystemConfiguration->QuickBoot == TRUE) ? IGNORE : EXTENSIVE;

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             ChipsetPrepareReadyToBootEvent,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Go the different platform policy with different boot mode
  // Notes: this part code can be changed with the table policy
  //
  switch (BootMode) {

  case BOOT_WITH_MINIMAL_CONFIGURATION:
    //
    // This is sample code for BOOT_WITH_MINIMAL_CONFIGURATION.
    //
    PlatformBdsConnectConsoleMini (gPlatformConsole);
    PlatformBdsDiagnosticsMini (TRUE, TRUE, SystemConfiguration->QuietBoot,BaseMemoryTest);
    PlatformBdsConnectDevicePath (gPlatformConnectSata[0], TRUE, TRUE);
    break;

  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
    //
    // Connect default console devices
    //
    Status = PlatformBdsConnectConsoleMini (gPlatformConsole);
    if (EFI_ERROR (Status)) {
      //
      // ConOut connect fail, force to boot with full configuration
      //
      goto FULL_CONFIGURATION;
    }

    PlatformBdsDiagnosticsMini (TRUE, TRUE, SystemConfiguration->QuietBoot,BaseMemoryTest);

    //
    // Update gPlatformConnectLastBoot via resotring saved device path
    // Its device path type should be other than BBS_DEVICE_PATH after transform
    //
    UpdateLastBootDevicePath (BootPrevious, &PlatformConnectLastBoot);

    //
    // Connect last boot device path
    //
    PlatformBdsConnectDevicePath (PlatformConnectLastBoot, TRUE, TRUE);
    LocateDevicePathStatus = gBS->LocateDevicePath (
                                    &gEfiSimpleFileSystemProtocolGuid,
                                    &PlatformConnectLastBoot,
                                    &Handle
                                    );
    if (!EFI_ERROR (LocateDevicePathStatus)) {
      BdsLibGetHotKey (&FunctionKey, &HotKeyPressed);
      if (!HotKeyPressed) {
        Status = PlatformBdsBootSelection (FunctionKey, 0);
        if (FeaturePcdGet (PcdAmtEnable)) {
          //
          // Parse ASF Boot options and execute any remote control boot
          // If we return, then there was no ASF boot option, or the option was not available on this platform
          //
		  Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

          if (!EFI_ERROR (Status)) {
            AmtWrapper->BdsBootViaAsf();
		  }
          if (IsUsbProvisionSupportEnabled()) {
            //
            // Skip connecting to USB controller also when booting with no configuration changes unless it is
            // Corporate SKU where USB Provisioning must be possible.
            //
            Status = PlatformBdsConnectDevicePath (gPlatformConnectXhciController[0], TRUE, TRUE);
          }
        }
        break;
      } else {
        //
        // Hot has been pressed, boot with full configuration
        //
        goto FULL_CONFIGURATION;
      }
    } else {
      goto FULL_CONFIGURATION;
    }

FULL_CONFIGURATION:
  case BOOT_ON_FLASH_UPDATE:
  case BOOT_IN_RECOVERY_MODE:
  case BOOT_ON_S4_RESUME:
  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
  default:
    Status = PlatformBdsConnectConsole (gPlatformConsole);

    PlatformBdsEnableSmmReadyToLock ();

    //
    // Add your customized condition here
    //
    IgnoreSpecficDevice();
    if (EFI_ERROR (Status)) {
      //
      // Here OEM/IBV can customize with defined action
      //
      PlatformBdsNoConsoleAction ();
    }


    if (HotKeyContext != NULL) {
      HotKeyContext->CanShowString = TRUE;
    }

//move after 3rd oprom loaded
//    PlatformBdsBootDisplayDevice (SystemConfiguration, SaSetup);
//
//    if (FeaturePcdGet(PcdAmtEnable)) {
//      BdsProcessSecureErase();
//    }
//
//    if (BootMode != BOOT_IN_RECOVERY_MODE) {
//      PlatformBdsDiagnostics (MemoryTestLevel, SystemConfiguration->QuietBoot, BaseMemoryTest);
//    }
    //
    // Perform some platform specific connect sequence
    //
    BdsLibConnectLegacyRoms ();

//    mIntelRapidStorageFastBootEnabled = FALSE;
//    if (IsIntelRapidStorageFastBootEnable (SystemConfiguration, PchSetup)) {
//      //
//      // For Rapid Storage Techanology fast boot requirement
//      // 1. Boot type must be EFI mode, SATA must operate in RAID
//      // 2. Boot Mode must be BOOT_WITH_MINIMAL_CONFIGURATION
//      // So change boot mode before start RAID UEFI driver
//      //
//      Hob.Raw = GetHobList ();
//      OriBootMode = Hob.HandoffInformationTable->BootMode;
//      Hob.HandoffInformationTable->BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;
//    }

    if (BdsLibIsWin8FastBootActive ()) {
      if (BdsLibGetBootType () == EFI_BOOT_TYPE && PchSetup->SataInterfaceMode == SATA_MODE_RAID) {
        PlatformBdsConnectDevicePath (gPlatformConnectSataController[0], TRUE, TRUE);
//
//        EfiRaidSpecialCheck ();
//
      } else {
        BdsLibConnectTargetDev ();
      }
    } else {
      PlatformBdsConnectSequence ();
    }

    // LocateAndInitializePcieSsd ();
    if (!(SystemConfiguration->QuietBoot)) {
      Status = ShowAllDevice ();
    }
    //
    // Dispatch the deferred 3rd party images.
    //
    Status = EfiBootManagerDispatchDeferredImages ();
    DEBUG ((DEBUG_INFO, "!! EfiBootManagerDispatchDeferredImages %r\n", Status));

    if (!EFI_ERROR (Status)) {
      //
      // After user authentication, the deferred drivers was loaded again.
      // Here, need to ensure the deferred drivers are connected.
      //
      Status = PlatformBdsConnectConsole (gPlatformConsole);
      if (EFI_ERROR (Status)) {
         PlatformBdsNoConsoleAction ();
      }
      PlatformBdsConnectSequence ();
    }

    PlatformBdsBootDisplayDevice (SystemConfiguration, SaSetup);

    if (FeaturePcdGet(PcdAmtEnable)) {
      BdsProcessSecureErase();
    }

    // show logo
    if (BootMode != BOOT_IN_RECOVERY_MODE) {
      PlatformBdsDiagnostics (MemoryTestLevel, SystemConfiguration->QuietBoot, BaseMemoryTest);
    }

    //
    // Here we have enough time to do the enumeration of boot device
    //
    BdsLibEnumerateAllBootOption (FALSE, BootOptionList);

//    if (mIntelRapidStorageFastBootEnabled) {
//      //
//      // After start UEFI driver succesfully, change boot mode back to original one.
//      //
//      Hob.Raw = GetHobList ();
//      Hob.HandoffInformationTable->BootMode = OriBootMode;
//    }
    //
    // If Resume From S4 BootMode is set become BOOT_ON_S4_RESUME
    //
    // move the check boot mode is whether BOOT_ON_S4_RESUME to PlatformBdsBootSelection ()
    // to make sure the memory mapping is the same between normal boot and S4
    //
    PlatformBdsBootSelection (FunctionKey, Timeout);
//    //
//    // Callback for CrSrvManager BDS function after Hotkey monitor.
//    //
//    CrSrvManager = NULL;
//    Status = gBS->LocateProtocol (&gCrSrvManagerProtocolGuid, NULL, (VOID **)&CrSrvManager);
//    if (Status == EFI_SUCCESS) {
//      CrSrvManager->CrSrvBdsCallback (CrSrvManager);
//    }

    SignalAllDriversConnected ();

    // For normal boot, the screen will be cleared by PlatformBdsBootSelection().
    // However, for recovery, PlatformBdsBootSelection returns directly and misses the screen clearing code.
    // As a result, clear the screen here to prevent Insyde logo to be shown in Shell in recovery boot.
    //
    if (BootMode == BOOT_IN_RECOVERY_MODE) {
      gST->ConOut->ClearScreen (gST->ConOut);
    }

    //
    // Parse ASF Boot options and execute any remote control boot
    // If we return, then there was no ASF boot option, or the option was not available on this platform
    //
    if (FeaturePcdGet (PcdAmtEnable)) {
      Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);

      if (!EFI_ERROR (Status)) {
        AmtWrapper->BdsBootViaAsf();
      }
      //
      // If local boot mode variable indicates fast boot or S4 resume, skip connecting to USB controller.
      //
      if (IsUsbProvisionSupportEnabled() && ((BootMode != BOOT_ON_S4_RESUME) || !(BdsLibIsWin8FastBootActive()))) {
        Status = PlatformBdsConnectDevicePath (gPlatformConnectXhciController[0], TRUE, TRUE);
      }
      AmtBdsBoot ();
    }

    break;
  }

FreePoolBeforeExitFunction:
  if (BootPrevious != NULL) {
    FreePool (BootPrevious);
  }
  if (TargetHddDevicePath != NULL) {
    FreePool (TargetHddDevicePath);
  }
  if (PlatformConnectLastBoot != NULL) {
    FreePool (PlatformConnectLastBoot);
  }
  if (HotKeyContext != NULL) {
    FreePool (HotKeyContext);
  }
  if (SystemConfiguration != NULL) {
    FreePool (SystemConfiguration);
  }
  if (SaSetup != NULL) {
    FreePool (SaSetup);
  }
  if (PchSetup != NULL) {
    FreePool (PchSetup);
  }
  return;
}


/**
  This function will connect console device base on the console
  device variable ConOut.

  @retval EFI_SUCCESS    At least one of the ConIn and ConOut device have
                         been connected success.
  @retval Other          Return the status of BdsLibConnectConsoleVariable ().

**/
EFI_STATUS
BdsConnectMiniConsoles (
  VOID
  )
{
  EFI_STATUS                Status;

  if (FeaturePcdGet (PcdH2OQ2LServiceSupported)) {
    //
    // PostCode = 0x17, Video device initial
    //
    POST_CODE (BDS_CONNECT_CONSOLE_OUT);
    Status = BdsLibConnectConsoleVariable (L"ConOut");
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //
    // PostCode = 0x16, Keyboard Controller, Keyboard and Moust initial
    //
    POST_CODE (BDS_CONNECT_CONSOLE_IN);
    Status = BdsLibConnectConsoleVariable (L"ConIn");
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    //
    // PostCode = 0x16, Keyboard Controller, Keyboard and Moust initial
    //
    POST_CODE (BDS_CONNECT_CONSOLE_IN);
    //
    // Because possibly the platform is legacy free, in such case,
    // ConIn devices (Serial Port and PS2 Keyboard ) does not exist,
    // so we need not check the status.
    //
    //
    // PostCode = 0x17, Video device initial
    //
    POST_CODE (BDS_CONNECT_CONSOLE_OUT);

    Status = BdsLibConnectConsoleVariable (L"ConOut");
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Special treat the err out device, becaues the null
  // err out var is legal.
  //
  return EFI_SUCCESS;
}


/**
  Connect the predefined platform default console device. Always try to find
  and enable the vga device if have.

  @param  PlatformConsole   Predfined platform default console device array.

  @retval EFI_STATUS    Success connect at least one ConIn and ConOut
                        device, there must have one ConOut device is
                        active vga device.
  @retval Other         Return the status of BdsLibConnectAllDefaultConsoles ()

**/
EFI_STATUS
PlatformBdsConnectConsoleMini (
  IN  BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  )
{
  EFI_STATUS                         Status;
  UINTN                              Index;
  EFI_DEVICE_PATH_PROTOCOL           *VarConout;
  EFI_DEVICE_PATH_PROTOCOL           *VarConin;
  UINTN                              DevicePathSize;
  CHIPSET_CONFIGURATION               *SystemConfiguration;
  EFI_DEVICE_PATH_PROTOCOL           *UsbConsoleOuputDevPath = NULL;
  EFI_STATUS                         OemSvcStatus;

  SystemConfiguration = NULL;
  SystemConfiguration = AllocateZeroPool (sizeof (CHIPSET_CONFIGURATION));
  if (SystemConfiguration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = GetChipsetSetupVariableDxe (SystemConfiguration, sizeof (CHIPSET_CONFIGURATION));
  if (EFI_ERROR (Status)) {
    FreePool (SystemConfiguration);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Connect RootBridge
  //
  Status = PlatformBdsConnectDevicePath (gPlatformRootBridges[0], FALSE, FALSE);
  if (EFI_ERROR (Status)) {
    if (SystemConfiguration != NULL) {
      FreePool (SystemConfiguration);
    }
    return Status;
  }

  VarConout = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );

  VarConin  = BdsLibGetVariableAndSize (
                L"ConIn",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );

  if (VarConout == NULL || VarConin == NULL) {
    //
    // Have chance to connect the platform default console,
    // the platform default console is the minimue device group
    // the platform should support
    //
    for (Index = 0; PlatformConsole[Index].DevicePath != NULL; ++Index) {
      //
      // Update the console variable with the connect type
      //
      if ((PlatformConsole[Index].ConnectType & CONSOLE_IN) == CONSOLE_IN) {
        BdsLibUpdateConsoleVariable (L"ConIn", PlatformConsole[Index].DevicePath, NULL);
      }
      if ((PlatformConsole[Index].ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
        BdsLibUpdateConsoleVariable (L"ConOut", PlatformConsole[Index].DevicePath, NULL);
      }
      if ((PlatformConsole[Index].ConnectType & STD_ERROR) == STD_ERROR) {
        BdsLibUpdateConsoleVariable (L"ErrOut", PlatformConsole[Index].DevicePath, NULL);
      }
    }
  }

  BdsLibConnectI2cDevice ();
  if (!(SystemConfiguration->UsbHotKeySupport == 0 && BdsLibIsWin8FastBootActive ())) {
    BdsLibConnectUsbHID ();
    if (FoundUSBConsoleOutput (&UsbConsoleOuputDevPath)) {
      SetUsbConsoleOutToConOutVar (UsbConsoleOuputDevPath);
      FreePool (UsbConsoleOuputDevPath);
    }
  } else {
    BdsLibConnectUsbHIDNotifyRegister ();
  }
  //
  // Connect the all the default console with current cosole variable
  //
  Status = BdsConnectMiniConsoles ();

  DEBUG ((DEBUG_INFO, "Base OemChipsetServices Call: OemSvcSetBacklightControl \n"));
  OemSvcStatus = OemSvcSetBacklightControl ();
  DEBUG ((DEBUG_INFO, "Base OemChipsetServices OemSvcSetBacklightControl Status: %r\n", OemSvcStatus));

  if (SystemConfiguration != NULL) {
    FreePool (SystemConfiguration);
  }
  return Status;
}


EFI_STATUS
PlatformBdsConnectDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL    *HandleDevicePath,
  IN  BOOLEAN                     ConnectChildHandle,
  IN  BOOLEAN                     DispatchPossibleChild
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle;

  BdsLibConnectDevicePath (HandleDevicePath);

  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &HandleDevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  do {
    gBS->ConnectController (Handle, NULL, NULL, ConnectChildHandle);

    //
    // Check to see if it's possible to dispatch an more DXE drivers
    //
    if (DispatchPossibleChild) {
      Status = gDS->Dispatch ();
    }
  } while (DispatchPossibleChild && !EFI_ERROR (Status));

  return EFI_SUCCESS;
}


/**
  This function is remained for IBV/OEM to do some platform action,
  if there no console device can be connected.

  @return EFI_SUCCESS      Direct return success now.

**/
EFI_STATUS
PlatformBdsNoConsoleAction (
  VOID
  )
{
  //
  // PostCode = 0x1C, Console device initial fail
  //
  POST_CODE (BDS_NO_CONSOLE_ACTION);

  return EFI_SUCCESS;
}


/**
  This function locks platform flash that is not allowed to be updated during normal boot path.
  The flash layout is platform specific.
**/
VOID
EFIAPI
PlatformBdsLockNonUpdatableFlash (
  VOID
  )
{
  return;
}


EFI_STATUS
PlatformBdsBootDisplayDevice (
  IN CHIPSET_CONFIGURATION        *SetupNVRam,
  IN SA_SETUP                     *SaSetupNVRam
  )
{
  EFI_LEGACY_BIOS_PROTOCOL      *LegacyBios;
  EFI_STATUS                    Status;
  BOOLEAN                       SkipOriginalCode;
  EFI_IA32_REGISTER_SET         Regs;

  if (SetupNVRam == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SkipOriginalCode = FALSE;

  Status = BdsLibDisplayDeviceReplace (&SkipOriginalCode);
  if (SkipOriginalCode || !EFI_ERROR (Status)) {
   return Status;
  }

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID **)&LegacyBios);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Dxe OemChipsetServices Call:OemSvcSetBootDisplayDevice \n"));
  Status = OemSvcSetBootDisplayDevice (SetupNVRam, SaSetupNVRam);
  DEBUG ((DEBUG_INFO, "Dxe OemChipsetServices OemSvcSetBootDisplayDevice  Status: %r\n", Status));
  if (Status != EFI_SUCCESS) {
    Status = EFI_SUCCESS;
    if (!IsPegDevExistence ()) {             // IGD
      //
      //  VBIOS INT 10h : 5F64h, 00h - Set Display Device
      //
      //  Calling Registers:
      //    AX  = 5F64h, Switch Display Device function
      //    BH  = 00h, Set Display Device sub-function
      //    BL  = Set Display Device Options:
      //         Bits 7 - 1    = Reserved
      //         Bit 0      = Force complete execution even if same display device
      //    CX = Display Combination to Set (1 = Enable display, 0 = Disable display):
      //         Bit 15 = Pipe B - LFP2
      //         Bit 14 = Pipe B - EFP2
      //         Bit 13 = Pipe B - EFP3
      //         Bit 12 = Pipe B - CRT2
      //         Bit 11 = Pipe B - LFP
      //         Bit 10 = Pipe B - EFP
      //         Bit 9  = Pipe B - TV
      //         Bit 8  = Pipe B - CRT
      //         Bit 7  = Pipe A - LFP2
      //         Bit 6  = Pipe A - EFP2
      //         Bit 5  = Pipe A - EFP3
      //         Bit 4  = Pipe A - CRT2
      //         Bit 3  = Pipe A - LFP
      //         Bit 2  = Pipe A - EFP
      //         Bit 1  = Pipe A - TV
      //         Bit 0  = Pipe A - CRT
      //  Return Registers:
      //    AX  = Return Status (function not supported if AL != 5Fh):
      //         = 005Fh, Function supported and successful
      //         = 015Fh, Function supported but failed
      //
      if (SaSetupNVRam->IgdBootType != 0) {      // Not VBT Default
        Regs.X.CX = (UINT16)SaSetupNVRam->IgdBootType;
        if (SaSetupNVRam->IgdBootType != 0 && (SaSetupNVRam->IgdBootType & SetupNVRam->IGDBootTypeSecondary) == 0) {
          Regs.X.CX |= (UINT16)SetupNVRam->IGDBootTypeSecondary << 8;
        }
        Regs.X.AX = 0x5F64;
        Regs.X.BX = 0x0001;                    // Force complete execution even if same display device
        Status = LegacyBios->Int86 (LegacyBios, 0x10, &Regs);
      }
    }
  }

  return EFI_SUCCESS;
}


/**
  Platform Bds Boot Selection

  @param  Selection     HotKey Selection

  @retval EFI_STATUS

**/
EFI_STATUS
PlatformBdsBootSelection (
  UINT16                                    Selection,
  UINT16                                    Timeout
  )
{
  BOOLEAN                            NoBootDevices;
  UINT8                              *VariablePtr;
  UINTN                              VariableSize;
  EFI_STATUS                         Status;
  EFI_BOOT_MODE                      BootMode;
  BOOLEAN                            KeyPressed;
  UINTN                              StringSetNum;
  EFI_OEM_BADGING_SUPPORT_PROTOCOL   *Badging;
  EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;
  CHIPSET_CONFIGURATION               *SystemConfiguration;
  AMT_WRAPPER_PROTOCOL               *AmtWrapper;
  H2O_CP_HANDLE                       CpHandle;

  NoBootDevices = FALSE;
  VariablePtr = NULL;
  VariableSize = 0;
  SystemConfiguration = NULL;
  CpHandle = NULL;

  //
  // It needs display dialog for user input Hdd password to unlock Hdd in S4 resume.
  //
  BdsLibGetBootMode (&BootMode);

  //
  // Disable Hot Key event and check Hot Key result
  //
  BdsLibStopHotKeyEvent ();

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    POST_CODE (BDS_RECOVERY_START_FLASH);
    //
    // Try to signal EndOfDxe event at here.
    //
    BdsLibOnEndOfBdsBootSelection ();
    if (FeaturePcdGet (PcdH2ODxeCpResetSystemSupported)) {
      Status = H2OCpRegisterHandler (
                 &gH2ODxeCpResetSystemGuid,
                 ResetSystemCallback,
                 H2O_CP_MEDIUM,
                 &CpHandle
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2ODxeCpResetSystemGuid, Status));
        return Status;
      }
      DEBUG ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2ODxeCpResetSystemGuid, Status));
    }
    RecoveryPopUp (DEFAULT_FLASH_DEVICE_TYPE);
    if (FeaturePcdGet (PcdH2ODxeCpResetSystemSupported)) {
      if (CpHandle != NULL) {
        Status = H2OCpUnregisterHandler (CpHandle);
      }
    }
    BdsLibStartSetupUtility (TRUE);
    return EFI_SUCCESS;
  }

  //
  // If there is no Boot option, go into SetupUtility & keep in SetupUtility
  //
  Status  = gRT->GetVariable (L"BootOrder", &gEfiGlobalVariableGuid, NULL, &VariableSize, VariablePtr);
  if (Status != EFI_BUFFER_TOO_SMALL) {
    NoBootDevices = TRUE;
  }

  BdsLibGetHotKey (&Selection, &KeyPressed);
  if (KeyPressed && Selection != FRONT_PAGE_HOT_KEY) {
    Status = gBS->LocateProtocol (&gEfiConsoleControlProtocolGuid, NULL, (VOID **)&ConsoleControl);
    if (EFI_ERROR (Status)) {
      ConsoleControl = NULL;
    }

  SystemConfiguration = AllocateZeroPool (sizeof (CHIPSET_CONFIGURATION));
  if (SystemConfiguration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = GetChipsetSetupVariableDxe (SystemConfiguration, sizeof (CHIPSET_CONFIGURATION));
  if (EFI_ERROR (Status)) {
    FreePool (SystemConfiguration);
  ASSERT_EFI_ERROR (Status);
    return Status;
  }

    Status = gBS->LocateProtocol (&gEfiOEMBadgingSupportProtocolGuid, NULL, (VOID **)&Badging);
    if (!EFI_ERROR (Status)) {
      //
      // BOOT_OS string always in the last one of array of OemBadgingString.
      //
      Badging->GetStringCount (Badging, &StringSetNum);
      if (ConsoleControl != NULL) {
        ShowOemString(Badging, TRUE, (UINT8) StringSetNum);
        BdsLibShowOemStringInTextMode (TRUE, (UINT8) StringSetNum);
      } else {
        if (SystemConfiguration->QuietBoot) {
          ShowOemString(Badging, TRUE, (UINT8) StringSetNum);
        } else {
          BdsLibShowOemStringInTextMode (TRUE, (UINT8) StringSetNum);
        }
      }
    }
  }

  gST->ConOut->EnableCursor (gST->ConOut, FALSE);

  //
  // Based on the key that was set, we can determine what to do
  //
  BdsLibOnEndOfBdsBootSelection ();

//  if ((IsHotKeyDetected () || DoesOsIndicateBootToFwUI ()) && mIntelRapidStorageFastBootEnabled) {
//    ReconnectSataDevice ();
//  }
  if (FeaturePcdGet(PcdAmtEnable)) {
    Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);
	if ((!EFI_ERROR (Status)) && (AmtWrapper->IsEnterSetupEnabled () == TRUE)) {
      if (FeaturePcdGet(PcdFrontPageSupported)) {
        PlatformBdsEnterFrontPage (0xffff, FALSE);
      } else {
        BdsLibStartSetupUtility (FALSE);
      }
    } else {
      Status = OemHotKeyCallback (
                 Selection,
                 Timeout,
                 BootMode,
                 NoBootDevices
                 );
    }
  } else {
    Status = OemHotKeyCallback (
               Selection,
               Timeout,
               BootMode,
               NoBootDevices
               );
  }

  if (SystemConfiguration != NULL) {
    FreePool (SystemConfiguration);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
PlatformBdsEnableSmmReadyToLock(
  VOID
  )
{
  EFI_STATUS                         Status;
  EFI_HANDLE                         Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiSmmLockEnablePointProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  DEBUG((DEBUG_INFO,"Installed gEfiSmmLockEnablePointProtocol Status:0x%08x.\n", Status));
  return Status;
}

VOID
EFIAPI
//PlatformBdsExitPmAuthCallback (
PlatformBdsTriggerEndOfDxeEvent (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                    Status;
//  VOID                          *ExitPmAuth;
  VOID                          *ProtocolPointer;
  EFI_EVENT                     EndOfDxeEvent;


  Status = gBS->LocateProtocol (&gEfiSmmLockEnablePointProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  if (EFI_ERROR (Status)) {
    return ;
  }
  gBS->CloseEvent (Event);
  //
  // Since PI1.2.1, we need signal EndOfDxe as ExitPmAuth
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BdsEmptyCallbackFuntion,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO,"!! To Signal EndOfDxe\n"));
  gBS->SignalEvent (EndOfDxeEvent);
  gBS->CloseEvent (EndOfDxeEvent);

  DEBUG((DEBUG_INFO,"All EndOfDxe callbacks have returned successfully\n"));
}


VOID
EFIAPI
PlatformBdsTriggerSmmReadyToLock (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                         Status;
  EFI_SMM_ACCESS2_PROTOCOL           *SmmAccess;
  EFI_ACPI_S3_SAVE_PROTOCOL          *AcpiS3Save;
//  EFI_HANDLE                         Handle;
  EFI_BOOT_MODE                      *BootMode;
  VOID                               *ProtocolPointer;
  UINTN                              Size;
  UINT64                             OsIndications;

  BootMode = (EFI_BOOT_MODE *) Context;

  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiSmmLockEnablePointProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }
  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "SmmReadyToLock triggered by gEfiSmmLockEnablePointProtocolGuid installed - Start.\n"));

  //
  // Check Boot mode and Recovery status
  //
  if ((*BootMode) == BOOT_ON_FLASH_UPDATE || (*BootMode) == BOOT_IN_RECOVERY_MODE ) {
    DEBUG((DEBUG_INFO," BootMode = %a\n", (*BootMode)==BOOT_ON_FLASH_UPDATE ? "Flash update":"Recovery"));
    return;
  }
  //
  // Check Capsule enable or not.
  //
  Size          = sizeof(UINT64);
  OsIndications = 0;
  Status = gRT->GetVariable (
                  L"OsIndications",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &Size,
                  &OsIndications
                  );

  if (!EFI_ERROR (Status) && (OsIndications & EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED)) {
    return;
  }

//  // Secure Flash should run after EndOfDxe
//  // Bios Lock need excute between EndOfDxe and SmmReadyToLock
//  TriggerCpReadyToBootBefore ();

  //
  // Install DxeSmmReadyToLock protocol prior to the processing of boot options
  //
  Status = gBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **) &SmmAccess);
  if (!EFI_ERROR (Status)) {
    //
    // Prepare S3 information, this MUST be done before DxeSmmReadyToLock
    //
    Status = gBS->LocateProtocol (&gEfiAcpiS3SaveProtocolGuid, NULL, (VOID **)&AcpiS3Save);
    if (!EFI_ERROR (Status)) {
      AcpiS3Save->S3Save (AcpiS3Save, NULL);
    }
    BdsLibTriggerDxeSmmReadyToLockEvent ();

  }
}



VOID
ChipsetPrepareReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{

//  EFI_STATUS                    Status;
//  EFI_ACPI_S3_SAVE_PROTOCOL     *AcpiS3Save;
//  EFI_HANDLE                    Handle;

  UINT8                         Port84Data;

//  //
//  // Prepare S3 information, this MUST be done before ExitPmAuth
//  //
//  Status = gBS->LocateProtocol (&gEfiAcpiS3SaveProtocolGuid, NULL, (VOID **)&AcpiS3Save);
//  if (!EFI_ERROR (Status)) {
//    AcpiS3Save->S3Save (AcpiS3Save, NULL);
//  }
//  Handle = NULL;
//  gBS->InstallProtocolInterface (
//         &Handle,
//         &gConOutDevStartedProtocolGuid,
//         EFI_NATIVE_INTERFACE,
//         NULL
//         );

  //
  // Intel Silicon View Technology (ISVT) IO Reading port 0x99 with AH = 0x30 Before INT 19h boot (Ready to boot)
  //
  Port84Data = IsvtCall (PcdGet8 (PcdIsvtCheckPoint3AhPreloadValue));
}


EFI_STATUS
EFIAPI
SetVgaCommandRegReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  PCI_DEV_DEF                           *VgaDevContext;
  UINTN                                 Index;
  UINT64                                Address;
  UINT8                                 Date8;

  VgaDevContext = (PCI_DEV_DEF *)Context;
  //
  // Index 0 is primary display VGA.
  //
  Index = 1;
  Date8 = 0x00;
  while ((VgaDevContext[Index].Seg != 0xFF) |
         (VgaDevContext[Index].Bus != 0xFF) |
         (VgaDevContext[Index].Dev != 0xFF) |
         (VgaDevContext[Index].Fun != 0xFF)) {
    if ((VgaDevContext[0].Seg == VgaDevContext[Index].Seg) &
        (VgaDevContext[0].Bus == VgaDevContext[Index].Bus) &
        (VgaDevContext[0].Dev == VgaDevContext[Index].Dev) &
        (VgaDevContext[0].Fun == VgaDevContext[Index].Fun)) {
      Index++;
      continue;
    }
    Address = PCI_SEGMENT_LIB_ADDRESS (
                VgaDevContext[Index].Seg,
                VgaDevContext[Index].Bus,
                VgaDevContext[Index].Dev,
                VgaDevContext[Index].Fun,
                PCICMD
                );
    Date8 = PciSegmentRead8 (Address);
    Date8 = Date8 & (~0x01);
    PciSegmentWrite8 (Address, Date8);
    DEBUG ((EFI_D_ERROR, "Set Pci command Reg(0x04) of (%02x/%02x/%02x) as 0.\n", (VgaDevContext[Index].Bus), (VgaDevContext[Index].Dev), (VgaDevContext[Index].Dev)));
    Index++;
  }

  gBS->FreePool (VgaDevContext);
  gBS->CloseEvent (Event);

  return EFI_SUCCESS;
}



/**
  Update gLastLegacyBootDevicePath if the last boot device is from legacy BBS table.
  Duplicate a new EFI device path if the last boot device is from native boot option.

  @param  BootPrevious
  @param  LastBootDevicePath    Point to device path of the last boot device for connection

  @retval EFI_SUCCESS

**/
EFI_STATUS
UpdateLastBootDevicePath (
  IN   UINT16                     *BootPrevious,
  OUT  EFI_DEVICE_PATH_PROTOCOL   **LastBootDevicePath
  )
{
  CHAR16                     Buffer[20];
  UINT8                      *Variable;
  UINTN                      VariableSize;
  UINT8                      *Ptr;
  EFI_DEVICE_PATH_PROTOCOL   *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL   *FullDevicePath;

  //
  // Check the last boot device exists.
  //
  if (BootPrevious == NULL) {
    return EFI_UNSUPPORTED;
  }

  FullDevicePath = NULL;
  //
  // Get the last boot device's information from boot####.
  //
  UnicodeSPrint (Buffer, sizeof (Buffer), L"Boot%04x", *BootPrevious);

  Variable = BdsLibGetVariableAndSize (
               Buffer,
               &gEfiGlobalVariableGuid,
               &VariableSize
               );

  //
  // If the device path is an EFI device path, duplicate device path directly.
  //
  Ptr = Variable;
  Ptr += sizeof (UINT32) + sizeof (UINT16);
  Ptr += StrSize ((CHAR16 *) Ptr);
  DevicePath = (EFI_DEVICE_PATH_PROTOCOL *) Ptr;

  if (DevicePath->Type == MEDIA_DEVICE_PATH && DevicePath->SubType == MEDIA_HARDDRIVE_DP) {
    Variable = BdsLibGetVariableAndSize (
                 L"BootPreviousData",
                 &gEfiGenericVariableGuid,
                 &VariableSize
                 );
    FullDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) Variable;
  }

  if (FullDevicePath == NULL) {
    *LastBootDevicePath = DuplicateDevicePath (DevicePath);
  } else {
    *LastBootDevicePath = DuplicateDevicePath (FullDevicePath);
  }

  return EFI_SUCCESS;
}


EFI_STATUS
AtEmptyEventFuntion (
  IN  EFI_EVENT        Event,
  IN  void             *ParentImageHandle
  )
{
  return EFI_SUCCESS;
}

///**
// Check if Intel Rapid Storage Technology Fast boot feature should be enabled.
// (1) SATA must be RAID mode and boot type is UEFI mode.
// (2) Windows 8 Fast boot feature must be enabled.
// (3) There is no Hotkey is detected during POST
// (4) System is not booting to UEFI configuration menu according to OS Indication variable
// If anyone of these condition does not meet, then disable fast boot feature.
//
//
// @retval TRUE  : Intel Rapid Storage Technology Fast boot feature should be enabled.
// @return FALSE : Intel Rapid Storage Technology Fast boot feature should not be enabled.
//
//**/
//BOOLEAN
//IsIntelRapidStorageFastBootEnable (
//  IN CHIPSET_CONFIGURATION         *SystemConfiguration,
//  IN PCH_SETUP                     *PchSetup
//)
//{
//  EFI_STATUS    Status;
//  UINT64        OsIndications;
//  UINT64        OsIndicationsSupported;
//  UINTN         VariableSize;
//
//  if ((BdsLibGetBootType () != EFI_BOOT_TYPE)) {
//
//    return FALSE;
//  }
//  if ((PchSetup->SataInterfaceMode != SATA_MODE_RAID)) {
//    return FALSE;
//  }
//  if ((SystemConfiguration->Win8FastBoot == 1)) {
//    return FALSE;
//  }
//  if (IsHotKeyDetected ()) {
//    return FALSE;
//  }
//
//  VariableSize = sizeof (OsIndicationsSupported);
//  Status = gRT->GetVariable (L"OsIndicationsSupported", &gEfiGlobalVariableGuid, NULL, &VariableSize, &OsIndicationsSupported);
//  if (EFI_ERROR (Status)) {
//    OsIndicationsSupported = 0;
//  }
//
//  VariableSize = sizeof (OsIndications);
//  Status = gRT->GetVariable (L"OsIndications", &gEfiGlobalVariableGuid, NULL, &VariableSize, &OsIndications);
//  if (!EFI_ERROR (Status)) {
//    if (OsIndications & OsIndicationsSupported & EFI_OS_INDICATIONS_BOOT_TO_FW_UI) {
//      return FALSE;
//    }
//  }
//
//  mIntelRapidStorageFastBootEnabled = TRUE;
//  return TRUE;
//}

/**
 Recoonect SATA controller
 (1) Get SATA controller handle
 (2) Disconnect SATA controller handle
 (3) Connect SATA controller handle again
 If anyone of these process failed, return error code.


 @return EFI_SUCCESS : Reconnect SATA controller successful.

**/
EFI_STATUS
ReconnectSataDevice ()
{
  EFI_STATUS          Status;
  UINTN               HandleCount;
  EFI_HANDLE          *HandleBuffer;
  UINTN               Index;
  EFI_PCI_IO_PROTOCOL *PciIo;
  PCI_TYPE00          PciConfigHeader00;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                  );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = PciIo->Pci.Read (
                      PciIo,
                      EfiPciIoWidthUint32,
                      0,
                      sizeof(PciConfigHeader00) / sizeof(UINT32),
                      &PciConfigHeader00
                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((PciConfigHeader00.Hdr.VendorId == V_PCH_INTEL_VENDOR_ID) &&
        (IS_PCH_SATA_DEVICE_ID(PciConfigHeader00.Hdr.DeviceId))) {
      Status = gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      break;
    }
  }

  if (Index != HandleCount && !EFI_ERROR (Status)) {
    BdsLibConnectAllDriversToAllControllers ();
  }
  return EFI_SUCCESS;
}

/**
  When system boot type is UEFI boot and SATA is RAID mode.
  After disable IRST acceleration feature in OS and reboot the system.
  System cannot find boot device at the 1st boot.
  It looks like UEFI RAID driver issue.

  This is a workaround to avoid boot device not found issue.

  (1) Check handle data base, find all handles with Simple File system protocol.
  (2) If this handle has the SATA controller device path, indicate boot device
      is found, mark FindBootableHandle flag to TRUE
  (3) In the end of this function, if FindBootableHandle is FALSE, disconnect
      SATA controller and reconnect all device again to ensure SATA device could
      be connected correctly.

  @param   none

  @retval  none

**/
// VOID
// EfiRaidSpecialCheck ()
// {
//   EFI_STATUS          Status;
//   UINTN               HandleCount;
//   EFI_HANDLE          *HandleBuffer;
//   UINTN               Index;
//   EFI_DEVICE_PATH_PROTOCOL *DevicePath;
//   BOOLEAN             FindBootableHandle;
//   PCI_DEVICE_PATH     *PciDevicePath;

//   FindBootableHandle = FALSE;

//   Status = gBS->LocateHandleBuffer (
//                   ByProtocol,
//                   &gEfiSimpleFileSystemProtocolGuid,
//                   NULL,
//                   &HandleCount,
//                   &HandleBuffer
//                 );
//   if (!EFI_ERROR (Status)) {
//     for (Index = 0; Index < HandleCount; Index++) {
//       Status = gBS->HandleProtocol (
//                       HandleBuffer[Index],
//                       &gEfiDevicePathProtocolGuid,
//                       (VOID **)&DevicePath
//                     );
//       if (!EFI_ERROR (Status)) {
//         while (!IsDevicePathEndType (DevicePath)) {
//           if (DevicePathType (DevicePath) == HARDWARE_DEVICE_PATH && DevicePathSubType (DevicePath) == HW_PCI_DP) {
//             PciDevicePath = (PCI_DEVICE_PATH*)DevicePath;
//             if (PciDevicePath->Device == PCI_DEVICE_NUMBER_PCH_SATA && PciDevicePath->Function == PCI_FUNCTION_NUMBER_PCH_SATA) {
//               FindBootableHandle = TRUE;
//               break;
//             }
//           }
//           DevicePath = NextDevicePathNode (DevicePath);
//         }
//       }
//       if (FindBootableHandle) {
//         break;
//       }
//     }
//   }

//   if (!FindBootableHandle) {
//     ReconnectSataDevice ();
//   }

//   return ;
// }

BOOLEAN
FoundUSBConsoleOutput (
  OUT EFI_DEVICE_PATH_PROTOCOL  **UsbConsoleOuputDevPath
  )
{
  EFI_STATUS                            Status;
  UINTN                                 NumberOfHandles;
  EFI_HANDLE                            *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL              *GopDevicePath;
  UINTN                                 Index;
  EFI_DEVICE_PATH_PROTOCOL              *DevPathNode;
  EFI_DEVICE_PATH_PROTOCOL              *NewDevPath;
  BOOLEAN                               FoundUsbConsole;

  //
  // init locals
  //
  NumberOfHandles = 0;
  HandleBuffer = NULL;
  GopDevicePath = NULL;
  Index = 0;
  DevPathNode = NULL;
  NewDevPath = NULL;
  FoundUsbConsole = FALSE;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiGraphicsOutputProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                );
  if (EFI_ERROR (Status)) {
    return FoundUsbConsole;
  }

  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                  &gEfiDevicePathProtocolGuid,
                  (VOID*)&GopDevicePath
                  );
    if (EFI_ERROR (Status)) {
      continue;
    }

    DevPathNode = GopDevicePath;
    while (!IsDevicePathEnd (DevPathNode)) {
      if (DevicePathType (DevPathNode) == DP_TYPE_MESSAGING &&
          DevicePathSubType (DevPathNode) == DP_SUBTYPE_USB
          ) {
        NewDevPath = AppendDevicePathInstance (*UsbConsoleOuputDevPath, GopDevicePath);
        if (*UsbConsoleOuputDevPath != NULL) {
          FreePool (*UsbConsoleOuputDevPath);
        }
        *UsbConsoleOuputDevPath = NewDevPath;
        FoundUsbConsole = TRUE;
        break;
      }
      DevPathNode = NextDevicePathNode (DevPathNode);
    }
  }
  FreePool (HandleBuffer);

  return FoundUsbConsole;
}

VOID
SetUsbConsoleOutToConOutVar (
  IN EFI_DEVICE_PATH_PROTOCOL  *UsbConsoleOuputDevPath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *VarConOut;
  UINTN                     DevPathSize;
  EFI_DEVICE_PATH_PROTOCOL  *NextDevPathInst;
  EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_STATUS                Status;
  UINTN                     Size;

  //
  //init locals
  //
  VarConOut = NULL;
  DevPathSize = 0;
  NextDevPathInst = NULL;
  TempDevicePath = NULL;
  RemainingDevicePath = UsbConsoleOuputDevPath;
  Size = 0;

  VarConOut = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevPathSize
                );

  do {
    NextDevPathInst = GetNextDevicePathInstance (&RemainingDevicePath, &Size);
    if (NextDevPathInst != NULL) {
      Status = BdsMatchDevicePaths(VarConOut, NextDevPathInst, NULL, FALSE);
      if (EFI_ERROR (Status)) {
        TempDevicePath = AppendDevicePathInstance (VarConOut, NextDevPathInst);
        if (VarConOut != NULL) {
          FreePool (VarConOut);
        }
        VarConOut = TempDevicePath;
      }
      FreePool (NextDevPathInst);
    }
  } while (RemainingDevicePath != NULL);

  gRT->SetVariable (
        L"ConOut",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
        GetDevicePathSize (VarConOut),
        VarConOut
        );
}

EFI_STATUS
BdsMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  * Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  * Single,
  IN  EFI_DEVICE_PATH_PROTOCOL  **NewDevicePath OPTIONAL,
  IN  BOOLEAN                   Delete
  )
/*++

Routine Description:
  Function compares a device path data structure to that of all the nodes of a
  second device path instance.

Arguments:
  Multi        - A pointer to a multi-instance device path data structure.

  Single       - A pointer to a single-instance device path data structure.

  NewDevicePath - If Delete is TRUE, this parameter must not be null, and it
                  points to the remaining device path data structure.
                  (remaining device path = Multi - Single.)

  Delete        - If TRUE, means removing Single from Multi.
                  If FALSE, the routine just check whether Single matches
                  with any instance in Multi.

Returns:

  The function returns EFI_SUCCESS if the Single is contained within Multi.
  Otherwise, EFI_NOT_FOUND is returned.

--*/
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath1;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath2;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInst;
  UINTN                     Size;

  //
  // The passed in DevicePath should not be NULL
  //
  if ((!Multi) || (!Single)) {
    return EFI_NOT_FOUND;
  }
  //
  // if performing Delete operation, the NewDevicePath must not be NULL.
  //
  TempDevicePath1 = NULL;

  DevicePath      = Multi;
  DevicePathInst  = GetNextDevicePathInstance (&DevicePath, &Size);

  //
  // search for the match of 'Single' in 'Multi'
  //
  while (DevicePathInst) {
    if (CompareMem (Single, DevicePathInst, Size) == 0) {
      if (!Delete) {
        gBS->FreePool (DevicePathInst);
        return EFI_SUCCESS;
      }
    } else {
      if (Delete) {
        TempDevicePath2 = AppendDevicePathInstance (
                            TempDevicePath1,
                            DevicePathInst
                            );
        gBS->FreePool (TempDevicePath1);
        TempDevicePath1 = TempDevicePath2;
      }
    }

    gBS->FreePool (DevicePathInst);
    DevicePathInst = GetNextDevicePathInstance (&DevicePath, &Size);
  }

  if (Delete) {
    *NewDevicePath = TempDevicePath1;
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}
/**
  Launch the front page.

  Deprecated. Use the LaunchLoadOption() function with well-known device path.
**/
VOID
PlatformBdsEnterFrontPage (
  IN UINT16                 TimeoutDefault,
  IN BOOLEAN                ConnectAllHappened
  )
{
  LaunchBootOptionByDevicePath ((EFI_DEVICE_PATH_PROTOCOL *) &gH2OFrontPageDevicePath);
}

VOID
BdsProcessSecureErase(
  VOID
)
{
  EFI_STATUS                              Status;
  EFI_HANDLE                              *Handles;
  UINTN                                   Index;
  UINTN                                   Count;
  AMT_READY_TO_BOOT_PROTOCOL              *AmtReadyToBoot;
  UINTN                                   HandleNum;
  EFI_HANDLE                              *AtaPassThruHandles;

  if (PcdGetBool (PcdSkipHddPasswordPrompt)) {

    BdsLibConnectAll ();
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiAtaPassThruProtocolGuid,
                    NULL,
                    &HandleNum,
                    &AtaPassThruHandles
                    );
    if (!EFI_ERROR (Status)) {
      if (HandleNum != 0) {
        Status = gBS->LocateHandleBuffer (
                        ByProtocol,
                        &gAmtReadyToBootProtocolGuid,
                        NULL,
                        &Count,
                        &Handles
                        );
        if (!EFI_ERROR (Status)) {
          //
          // Disable Hot Key event to avoid hot key event get key before the SecureErase
          // function,cause to SecureErase function cannot get key
          //
          DEBUG ( ( EFI_D_ERROR | EFI_D_INFO, "Stop Hot key event before Secure Erase\n") );
          BdsLibStopHotKeyEvent ();
          for (Index = 0; Index < Count; Index++) {
            Status = gBS->HandleProtocol (Handles[Index], &gAmtReadyToBootProtocolGuid, (VOID **) &AmtReadyToBoot);
            ASSERT_EFI_ERROR (Status);
            AmtReadyToBoot->Signal ();
          }
        }
      }
    }
  }
}

VOID
AmtBdsBoot (
  VOID
  )
{
  EFI_STATUS               Status;
  AMT_WRAPPER_PROTOCOL     *AmtWrapper;
  UINTN                    EventIndex;

  Status = gBS->LocateProtocol (&gAmtWrapperProtocolGuid, NULL, (VOID **) &AmtWrapper);
  if (!EFI_ERROR (Status) && AmtWrapper->IsPauseBootEnabled ()) {
    //
    // Wait until user presses any key when PauseBoot is requested
    //
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
    return;
  }
}

