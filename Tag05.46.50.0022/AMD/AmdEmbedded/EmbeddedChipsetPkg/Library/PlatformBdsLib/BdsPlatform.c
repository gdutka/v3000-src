/** @file
 This file include all platform action which can be customized by IBV/OEM.

;******************************************************************************
;* Copyright (c) 2015 - 2021, Insyde Software Corp. All Rights Reserved.
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
#include <OemBootDisplayDevice.h>
#include <BootDevicesDisplay.h>
#include <String.h>
#include <Guid/H2OBdsCheckPoint.h>
#include <Protocol/AcpiS3Save.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmAccess2.h>
#include <Library/H2OCpLib.h>

#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>

#include <Library/KernelConfigLib.h>
#include <Library/ChipsetConfigLib.h>
#include <Library/PciSegmentLib.h>
#include <SecureFlash.h>

extern PLATFORM_ISA_SERIAL_OVER_LAN_DEVICE_PATH   gSerialOverLANDevicePath;
extern PLATFORM_ONBOARD_VGA_DEVICE_PATH           gOnChipIgfxDevicePath;
extern EFI_GUID                                   gUefiRaidOpromReadyGuid;

#define DP_TYPE_MESSAGING   0x03
#define DP_SUBTYPE_USB      0x05

EFI_MP_SERVICES_PROTOCOL  *MpService;

typedef struct {
  UINT8  CpData;
} H2O_CP_VERIFY_PKG_CHECK_DATA;

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

VOID
EFIAPI
PlatformBdsTriggerEndOfDxeEvent (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  );

//
// BDS Platform Functions
//
/**
  Platform Bds init. Include the platform firmware vendor, revision
  and so crc check.

**/
//[-start-150909-IB12970000-add]//
VOID
DisableiGpuDecode (
  VOID
  )
{
  UINT8  Reg2;
  UINT64  Address;

  Reg2 = 0;
  Address = 0;

  Address = PCI_SEGMENT_LIB_ADDRESS (0, 0, 0x18, 0, 0x80);
  Reg2 = PciSegmentRead8 (Address);
  Reg2 &= (~BIT0);
  PciSegmentWrite8 (Address, Reg2);
  return;

}
//[-end-150909-IB12970000-add]//
VOID
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
    return;
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
    return;
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

  return;
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
}

VOID
BdsEmptyCallbackFuntion (
  IN  EFI_EVENT        Event,
  IN  void             *ParentImageHandle
  )
{
  return;
}


/**
  Connect RootBridge

  @retval EFI_SUCCESS   Connect RootBridge successfully.
  @retval EFI_STATUS    Connect RootBridge fail.

**/
EFI_STATUS
ConnectRootBridge (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                RootHandle;
  EFI_HANDLE                Handle;

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

  //
  // At this point, all the PCI_IO protocols are installed completely.
  // We install a AllPciIoPrtclsInstlFinishedProtocol to signal this point.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAllPciIoPrtclsInstlFinishedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

VOID
EnterExternalVgaCheckCp (
  IN EFI_EVENT      Event,
  IN H2O_CP_HANDLE  Handle
  )
{
  EFI_STATUS                    Status;
  H2O_CP_VERIFY_PKG_CHECK_DATA  *VerifyPkgCheckData;
  UINTN                         DataSize;
  UINT8                         Data;
  UINT32                        Attributes;

  Status             = EFI_SUCCESS;
  VerifyPkgCheckData = NULL;
  DataSize           = 1;
  Data               = 0;
  Attributes         = 0x00;

  Status = H2OCpLookup (
             Handle,
             (VOID **) &VerifyPkgCheckData,
             &gH2OBdsCpEnterExternalVgaCheckGuid
             );
  if (!EFI_ERROR (Status)) {
    Status = gRT->GetVariable (
                    L"ExternalVgaCheckExist",
                    &gH2OEnterExternalVgaCheckVariableGuid,
                    &Attributes,
                    &DataSize,
                    &Data
                    );

    VerifyPkgCheckData->CpData = Data;
  }
}

/**
  To check the VGA device is internal or external.

  @param[in] Handle               The handle which exist PCI IO Protocol..

  @retval TRUE                    The VGA device is external VGA.
  @retval FALSE                   The VGA device is internal VGA.
**/
BOOLEAN
ExternalVgaCheck (
  IN EFI_HANDLE           Handle
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *VgaDevicePath;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID*)&VgaDevicePath
                  );
  if (CompareMem(
        VgaDevicePath,
        (EFI_DEVICE_PATH_PROTOCOL*)&gOnChipIgfxDevicePath,
        GetDevicePathSize (VgaDevicePath) - END_DEVICE_PATH_LENGTH ) == 0) {
          return FALSE;
        }

  return TRUE;
}

EFI_STATUS
GetAllVgaHandles (
  OUT EFI_HANDLE            **VgaHandleBuffer,
  OUT UINTN                 *VgaHandleCount
  )
{
  EFI_STATUS           Status;
  EFI_HANDLE           *HandleBuffer;
  UINTN                HandleCount;
  UINTN                HandleIndex;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE00           PciType00;
  EFI_HANDLE           *TempVgaHandleBuffer;
  UINTN                BufferIndex;

  Status              = EFI_SUCCESS;
  HandleBuffer        = NULL;
  HandleCount         = 0;
  PciIo               = NULL;
  ZeroMem (&PciType00, sizeof (PCI_TYPE00));
  TempVgaHandleBuffer = NULL;
  BufferIndex         = 0;
  //
  // Start PciBus.
  //
  ConnectRootBridge ();
  if (FeaturePcdGet (PcdHybridGraphicsSupported)) {
    EFI_HANDLE                          HgHandle;
    //
    // Install Hybrid Graphics Protocol to trigger
    // Hybrid Graphics DXE driver registered callback function.
    //
    HgHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &HgHandle,
                    &gH2OHybridGraphicsEventProtocolGuid,
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

  TempVgaHandleBuffer = AllocatePool (HandleCount * sizeof (EFI_HANDLE));
  if (TempVgaHandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[HandleIndex],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    if (!EFI_ERROR (Status)) {
      //
      // Check for all VGA device
      //
      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0,
                            sizeof (PciType00) / sizeof (UINT32),
                            &PciType00
                            );
      if (EFI_ERROR (Status)) {
        continue;
      }

      if (!IS_PCI_DISPLAY (&PciType00)) {
        continue;
      }

      TempVgaHandleBuffer[BufferIndex] = HandleBuffer[HandleIndex];
      BufferIndex++;
    }
  }

  if (BufferIndex == 0) {
    return EFI_NOT_FOUND;
  }

  *VgaHandleBuffer = AllocateCopyPool (BufferIndex * sizeof (EFI_HANDLE), TempVgaHandleBuffer);
  if (*VgaHandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *VgaHandleCount = BufferIndex;

  gBS->FreePool (HandleBuffer);
  gBS->FreePool (TempVgaHandleBuffer);

  return Status;
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

  @param  DeviceHandle    Handle of PCIIO protocol.

  @retval EFI_SUCCESS     PCI VGA is added to ConOut.
  @retval EFI_STATUS      No PCI VGA device is added.

**/
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
  POSSIBLE_VGA_TYPE                     CheckSequence[] = {Igfx, Peg, Pcie, Pci};
  AMD_PBS_SETUP_OPTION                  AmdPbsConfiguration;

  SelectedVgaIndex = 0xFF;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return NULL;
  }

  //
  // Check Vga Order
  //
  switch (AmdPbsConfiguration.SpecialVgaFeature) {

  case 0: // Disable
    switch (AmdPbsConfiguration.PrimaryVideoAdaptor) {

    case 0: // AUTO
    case 1: // Igd
      CheckSequence[0] = Igfx;
      CheckSequence[1] = Peg;
      CheckSequence[2] = Pcie;
      CheckSequence[3] = Pci;
      break;

    case 2:
    case 3: // Peg
      CheckSequence[0] = Peg;
      CheckSequence[1] = Pcie;
      CheckSequence[2] = Pci;
      CheckSequence[3] = Igfx;
      break;
    }
    break;

  case 3: // Px
  case 4: // Hg
    CheckSequence[0] = Igfx;
    CheckSequence[1] = Peg;
    CheckSequence[2] = Pcie;
    CheckSequence[3] = Pci;
    break;

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
      return VgaDeviceList[SelectedVgaIndex].Handle;
    }
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
  PCI_TYPE00                            PciType00;
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

  FindIgfx          = FALSE;
  FindPeg           = FALSE;
  FindPci           = FALSE;
  VendorId          = 0;
  DeviceId          = 0;
  SubsystemVendorID = 0;
  SubsystemID       = 0;
  PciIo             = NULL;

  SelectedVgaHandle = NULL;
  VgaHandleCount    = PciVgaHandleCount;
  if (VgaHandleCount == 0) {
    return NULL;
  }

  if (VgaHandleCount == 1) {
    return PciVgaHandleBuffer[0];
  }

  VgaDeviceList = AllocatePool (VgaHandleCount * sizeof (VGA_DEVICE_INFO));
  if (VgaDeviceList == NULL) {
    return NULL;
  }

  for (Index = 0; Index < VgaHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    PciVgaHandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );

    VgaDeviceList[Index].Handle = PciVgaHandleBuffer[Index];
    FoundFlag = FALSE;

    if (!FindIgfx || !FindPeg || !FindPci) {
      FindVga = FALSE;
      PciIo->Pci.Read (
                   PciIo,
                   EfiPciIoWidthUint32,
                   0,
                   sizeof (PciType00) / sizeof (UINT32),
                   &PciType00
                   );
      if ((PciType00.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType00.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_VGA) || \
          (PciType00.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType00.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_XGA) || \
          (PciType00.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType00.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_3D)  || \
          (PciType00.Hdr.ClassCode[2] == PCI_CLASS_DISPLAY && PciType00.Hdr.ClassCode[1] == PCI_CLASS_DISPLAY_OTHER)) {
        FindVga = TRUE;
        VendorId          = PciType00.Hdr.VendorId;
        DeviceId          = PciType00.Hdr.DeviceId;
        SubsystemVendorID = PciType00.Device.SubsystemVendorID;
        SubsystemID       = PciType00.Device.SubsystemID;
      }
      if (FindVga) {
        if (!ExternalVgaCheck (VgaDeviceList[Index].Handle)) {
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
SetDisplayEnginePrimayDisplay (
  IN EFI_HANDLE                        PrimayVgaHandle,
  IN EFI_DEVICE_PATH_PROTOCOL          *GopDevicePath
  )
{
  EFI_HANDLE                             GopHandle;
  EFI_DEVICE_PATH_PROTOCOL               *VgaDevicePath;
  EFI_DEVICE_PATH_PROTOCOL               *TempGopDevicePath;
  EFI_STATUS                             Status;
  VOID                                   *Instance;
  UINTN                                  HandleCount;
  UINTN                                  HandleIndex;
  EFI_HANDLE                             *VgaHandleBuffer;
  UINTN                                  VgaHandleCount;
  EFI_PCI_IO_PROTOCOL                    *PciIo;
  PCI_TYPE00                             Pci;

  if (PrimayVgaHandle == NULL) {
    //
    // avoid GetAllVgaHandles ConnectRootBridge twice
    // Get VGA handle list by itself
    //
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiPciIoProtocolGuid,
                    NULL,
                    &HandleCount,
                    &VgaHandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      return ;
    }

    VgaHandleCount = 0;
    for (HandleIndex = 0; HandleIndex < HandleCount; HandleIndex++) {
      Status = gBS->HandleProtocol (
                      VgaHandleBuffer[HandleIndex],
                      &gEfiPciIoProtocolGuid,
                      (VOID **)&PciIo
                      );
      if (EFI_ERROR (Status)) {
        continue;
      }

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

      if (!IS_PCI_VGA (&Pci) && !IS_PCI_OLD_VGA (&Pci)) {
        continue;
      }

      VgaHandleBuffer[VgaHandleCount++] = VgaHandleBuffer[HandleIndex];
    }

    if (VgaHandleCount == 0) {
      FreePool (VgaHandleBuffer);
      return ;
    }

    PrimayVgaHandle = ClassifyVgaHandleAndSelect (
                          VgaHandleBuffer,
                          VgaHandleCount
                          );
    FreePool (VgaHandleBuffer);
    if (PrimayVgaHandle == NULL) {
      return ;
    }
  }

  Status = gBS->HandleProtocol (
                  PrimayVgaHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID*) &VgaDevicePath
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return ;
  }

  TempGopDevicePath = GopDevicePath;
  Status = gBS->LocateDevicePath (
                  &gEfiDevicePathProtocolGuid,
                  &TempGopDevicePath,
                  &GopHandle
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  Status = gBS->HandleProtocol (
                  GopHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID*)&TempGopDevicePath
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  if (CompareMem (
        VgaDevicePath,
        TempGopDevicePath,
        GetDevicePathSize (VgaDevicePath) - END_DEVICE_PATH_LENGTH
        ) != 0) {
    return ;
  }

  Status = gBS->HandleProtocol (
                  GopHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID*)&Instance
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  Status = gBS->HandleProtocol (
                  GopHandle,
                  &gH2ODisplayTypePrimaryDisplayGuid,
                  (VOID*)&Instance
                  );
  if (!EFI_ERROR (Status)) {
    return ;
  }

  //
  // Install gH2ODisplayTypePrimaryDisplayGuid for primary display
  //
  Status = gBS->InstallProtocolInterface (
                  &GopHandle,
                  &gH2ODisplayTypePrimaryDisplayGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}


/**
  Find the platform active vga, and base on the policy to enable the vga as the console out device.
  The policy is driven by one setup variable "VBIOS".

  @retval EFI_UNSUPPORTED   There is no active vga device
  @retval EFI_STATUS        Return the status of BdsLibGetVariableAndSize ()

**/
EFI_STATUS
PlatformBdsForceActiveVga (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *VgaHandleBuffer;
  UINTN                                 VgaHandleCount;
  EFI_DEVICE_PATH_PROTOCOL              *ConOutDevPath;
  EFI_HANDLE                            SelectedVgaHandle;
  VGA_HANDLES_INFO                      *VgaHandlesInfo;
  EFI_DEVICE_PATH_PROTOCOL              *VarConout;
  UINTN                                 DevicePathSize;
  AMD_PBS_SETUP_OPTION                  AmdPbsConfiguration;
  H2O_CP_HANDLE                         CpHandle;
  EFI_EVENT                             ExitBootServicesEvent;
  EFI_EVENT                             LegacyBootEvent;

  VgaHandleBuffer     = NULL;
  VgaHandleCount      = 0;
  ConOutDevPath       = NULL;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    goto Exit;
  }

  Status = GetAllVgaHandles (&VgaHandleBuffer, &VgaHandleCount);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  if (H2OGetBootType () != EFI_BOOT_TYPE) {
    if ((VgaHandleCount > 1) && (AmdPbsConfiguration.PrimaryVideoAdaptor == 2)) {
      DisableiGpuDecode ();
    }
  }

  Status = H2OCpRegisterHandler (
             &gH2OBdsCpEnterExternalVgaCheckGuid,
             EnterExternalVgaCheckCp,
             H2O_CP_MEDIUM,
             &CpHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG (((EFI_D_ERROR | EFI_D_INFO), "!!! Checkpoint Register ERROR(%r) !!!\n", Status));
    return Status;
  }

  SelectedVgaHandle = ClassifyVgaHandleAndSelect (
                        VgaHandleBuffer,
                        VgaHandleCount
                        );

  if (H2OGetBootType () == EFI_BOOT_TYPE) {
    Status = PreparePciVgaDevicePath (SelectedVgaHandle, &ConOutDevPath);
    if (!EFI_ERROR (Status)) {
      SetDisplayEnginePrimayDisplay (SelectedVgaHandle, ConOutDevPath);
    }

    LockVgaControllerVgaDriverPolicy (SelectedVgaHandle, VgaHandleBuffer, VgaHandleCount);

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

  } else {
  VarConout = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );

    Status = gBS->HandleProtocol (
                    SelectedVgaHandle,
                    &gEfiDevicePathProtocolGuid,
                    (VOID*) &ConOutDevPath
                    );
    if (CompareMem (
          ConOutDevPath,
          VarConout,
          GetDevicePathSize (ConOutDevPath) - END_DEVICE_PATH_LENGTH)) {
          Status = BdsLibUpdateConsoleVariable (
                     CON_OUT_CANDIDATE_NAME,
                     ConOutDevPath,
                     VarConout
                     );
      gRT->SetVariable (
             L"ConOut",
             &gEfiGlobalVariableGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             GetDevicePathSize (ConOutDevPath),
             ConOutDevPath
             );
    }
    Status = PreparePciVgaDevicePath (SelectedVgaHandle, &ConOutDevPath);
    if (!EFI_ERROR (Status)) {
      SetDisplayEnginePrimayDisplay (SelectedVgaHandle, ConOutDevPath);
    }
  }

Exit:
  if (VgaHandleBuffer) {
    gBS->FreePool (VgaHandleBuffer);
  }

  return Status;
}


/**
  Compare two device pathes

  @param  DevicePath1   Input device pathes.
  @param  DevicePath2   Input device pathes.

  @retval TRUE          Same.
  @retval FALSE         Different.

**/
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

  @param PlatformConsole          Predefined platform default console device array.

  @retval EFI_SUCCESS             Success connect at least one ConIn and ConOut
                                  device, there must have one ConOut device is
                                  active vga device.
  @return Return the status of BdsLibConnectAllDefaultConsoles ()

**/
EFI_STATUS
PlatformBdsConnectConsole (
  IN BDS_CONSOLE_CONNECT_ENTRY   *PlatformConsole
  )
{
  EFI_STATUS                         Status;
  UINTN                              Index;
  EFI_DEVICE_PATH_PROTOCOL           *VarConout;
  EFI_DEVICE_PATH_PROTOCOL           *VarConin;
  EFI_DEVICE_PATH_PROTOCOL           *VarErrout;
  EFI_DEVICE_PATH_PROTOCOL           *NewVarConout;
  EFI_DEVICE_PATH_PROTOCOL           *NewVarConin;
  EFI_DEVICE_PATH_PROTOCOL           *NewVarErrout;
  UINTN                              DevicePathSize;
  KERNEL_CONFIGURATION               KernelSetup;
  VOID                               *CrService;
  EFI_DEVICE_PATH_PROTOCOL           *UsbConsoleOuputDevPath = NULL;

  Index = 0;
  Status = EFI_SUCCESS;
  DevicePathSize = 0;

  Status = GetKernelConfiguration(&KernelSetup);
  if (EFI_ERROR (Status)) {
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
  if (VarConout != NULL) {
    BdsLibUpdateConsoleVariable (CON_OUT_CANDIDATE_NAME, VarConout, NULL);
    gBS->FreePool (VarConout);
  }

  if (VarConin != NULL) {
    BdsLibUpdateConsoleVariable (CON_IN_CANDIDATE_NAME, VarConin, NULL);
    gBS->FreePool (VarConin);
  }

  if (VarErrout != NULL) {
    BdsLibUpdateConsoleVariable (ERR_OUT_CANDIDATE_NAME, VarErrout, NULL);
    gBS->FreePool (VarErrout);
  }

  //
  // DASH_SUPPORT
  //
#ifdef DASH_SUPPORT
  BdsLibUpdateConsoleVariable (L"ConInDev", (EFI_DEVICE_PATH_PROTOCOL*)&gSerialOverLANDevicePath, NULL);
  BdsLibUpdateConsoleVariable (L"ConOutDev", (EFI_DEVICE_PATH_PROTOCOL*)&gSerialOverLANDevicePath, NULL);
  Status = PlatformBdsConnectDevicePath ((EFI_DEVICE_PATH_PROTOCOL*)&gSerialOverLANDevicePath, FALSE, FALSE);
#endif

  //
  // Make sure we have at least one active VGA, and have the right active VGA in console variable
  //
  Status = PlatformBdsForceActiveVga ();
  if (EFI_ERROR(Status)) {
    Status = gBS->LocateProtocol (&gConsoleRedirectionServiceProtocolGuid, NULL, &CrService);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  //
  // Update default device paths with candicate device path
  //
  VarConout = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );
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
  if (NewVarConout != NULL) {
    gBS->FreePool (NewVarConout);
  }
  if (VarConout != NULL) {
    gBS->FreePool (VarConout);
  }

  DisplayUserSelectionDefault();

  VarConin = BdsLibGetVariableAndSize (
               L"ConIn",
               &gEfiGlobalVariableGuid,
               &DevicePathSize
               );
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
  if (NewVarConin != NULL) {
    gBS->FreePool (NewVarConin);
  }
  if (VarConin != NULL) {
    gBS->FreePool (VarConin);
  }

  VarErrout = BdsLibGetVariableAndSize (
                L"ErrOut",
                &gEfiGlobalVariableGuid,
                &DevicePathSize
                );
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
  if (!(KernelSetup.UsbHotKeySupport == 0 && BdsLibIsWin8FastBootActive ())) {
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

  return Status;
}

/**
  Connect with predefined platform connect sequence,
  the OEM/IBV can customize with their own connect sequence.
**/
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

  @param BdsDriverLists  - The header of the driver option link list.

**/
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
    BdsLibRegisterNewOption (NULL, gPlatformDriverOption[Index], NULL, L"DriverOrder", NULL, 0);
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
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", gH2OBdsCpDisplayBeforeGuid));
  Status = H2OCpTrigger (&gH2OBdsCpDisplayBeforeGuid, BdsDisplayBeforeData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsDisplayBeforeData->Status));
  return Status;
}

// EFI_STATUS
// CoreLogBistStatus (
//   VOID
//   )
// {
//   EFI_STATUS              Status;
//   EFI_HOB_GUID_TYPE       *GuidHob;
//   UINT32                  *DataInHob;
//   UINTN                   NumberOfData;
//   UINTN                   CpuNumber;
//
//   Status = MpService->WhoAmI (MpService, &CpuNumber);
//
//   GuidHob = GetFirstGuidHob (&gEfiHtBistHobGuid);
//   if (GuidHob == NULL) {
//     return EFI_SUCCESS;
//   }
//
//   DataInHob    = GET_GUID_HOB_DATA (GuidHob);
//   NumberOfData = GET_GUID_HOB_DATA_SIZE (GuidHob) / sizeof (UINT32);
//   if (CpuNumber >= NumberOfData) {
//     return EFI_SUCCESS;
//   }
//
//   DataInHob[CpuNumber] = AsmReadMsr32 (0xC0010060);
//   DataInHob[CpuNumber] &= (UINT32)0x61FFFFFF;       // Mask Reserved bits
//   return EFI_SUCCESS;
//}

// /**
//   Show Processor Infomation, ie processor ID, status Flag,
//   and physical location.
//
//   @param  PrintLine         Display line number of system information
//
//   @retval EFI_SUCCESS       Success to show system information
//
// **/
// EFI_STATUS
// ShowProcessorInfo (
//   IN OUT UINT8                        *PrintLine
//   )
// {
//   EFI_STATUS              Status;
//   UINTN                   BspNumber;
//   UINTN                   Index;
//   UINTN                   NumberOfEnabledCPUs;
//   UINTN                   NumberOfProcessors;
//   EFI_HOB_GUID_TYPE       *GuidHob;
//   UINT32                  *DataInHob;
//   UINTN                   NumberOfData;
//   BOOLEAN                 BISTERROR;
//   CHAR16                  TempStr[20];
//
//   Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpService);
//   if (EFI_ERROR (Status)) {
//     return Status;
//   }
//
//   Status = MpService->GetNumberOfProcessors (
//                         MpService,
//                         &NumberOfProcessors,
//                         &NumberOfEnabledCPUs
//                         );
//   if (EFI_ERROR (Status)) {
//     return Status;
//   }
//
//   Status = MpService->WhoAmI (MpService, &BspNumber);
//
//   for (Index = 0; Index < NumberOfEnabledCPUs; Index++) {
//     if (Index != BspNumber) {
//       Status = MpService->StartupThisAP (
//                 MpService,
//                 (EFI_AP_PROCEDURE) CoreLogBistStatus,
//                 Index,
//                 NULL,
//                 0,
//                 NULL,
//                 NULL
//                 );
//     } else {
//       Status = CoreLogBistStatus ();
//     }
//     if (EFI_ERROR (Status)) {
//       return Status;
//     }
//   }
//
//   GuidHob = GetFirstGuidHob (&gEfiHtBistHobGuid);
//   if (GuidHob == NULL) {
//     return EFI_SUCCESS;
//   }
//
//   DataInHob    = GET_GUID_HOB_DATA (GuidHob);
//   NumberOfData = GET_GUID_HOB_DATA_SIZE (GuidHob) / sizeof (UINT32);
//   BISTERROR    = FALSE;
//
//   for (Index = 0; Index < NumberOfData; Index++, DataInHob++) {
//     if (*DataInHob != 0) {
//       UnicodeValueToString (TempStr, PREFIX_ZERO | RADIX_HEX, Index, 1);
//       BdsLibOutputStrings (gST->ConOut, L"\n\r", L"BIST ERROR CPU ", TempStr, L" = ", NULL);
//       UnicodeValueToString (TempStr, PREFIX_ZERO | RADIX_HEX, *DataInHob, 16);
//       BdsLibOutputStrings (gST->ConOut, TempStr, NULL);
//       BISTERROR = TRUE;
//     }
//   }
//
//   if (BISTERROR) {
//     gST->ConOut->SetAttribute (gST->ConOut, EFI_LIGHTRED);
//     BdsLibOutputStrings (gST->ConOut, L"\n\r  BIST ERROR! System Halted!", NULL);
//     IoWrite16 (PcdGet16(PcdH2ODebugPort), 0xB188); //SIMPLE_SLT_ERROR_BIST
// //    CpuDeadLoop ();
//     gBS->Stall (1 * 5000000);
//   }
//
//   return EFI_SUCCESS;
//}

/**
  Perform the platform diagnostic, such like test memory. OEM/IBV also
  can customize this function to support specific platform diagnostic.

  @param MemoryTestLevel  The memory test intensive level
  @param QuietBoot        Indicate if need to enable the quiet boot
  @param BaseMemoryTest   A pointer to BdsMemoryTest()

**/
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

  PrintLine = 0;

  //
  // PostCode = 0x1D, Display logo or system information
  //
  POST_CODE (BDS_DISPLAY_LOGO_SYSTEM_INFO);

  BdsLibOnStartOfBdsDiagnostics ();

  //
  // OemServices
  //
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
  Status = OemSvcDisplayLogo (
             &QuietBoot
             );
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

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
    //Status = ShowProcessorInfo (&PrintLine);
  }

  Status = BaseMemoryTest (MemoryTestLevel, (UINTN) (PrintLine + 2));
  if (EFI_ERROR (Status)) {
    DisableQuietBoot ();
  }

  return;
}

VOID
ReadyToBootEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                 Status;
  EFI_ACPI_S3_SAVE_PROTOCOL  *AcpiS3Save;

  Status = gBS->LocateProtocol (&gEfiAcpiS3SaveProtocolGuid, NULL, (VOID **)&AcpiS3Save);
  if (!EFI_ERROR (Status)) {
    AcpiS3Save->S3Save (AcpiS3Save, NULL);
  }

}


/**
  Internal function to initialize  _H2O_BDS_CP_END_OF_DXE_BEFORE_PROTOCOL data and trigger gH2OBdsCpEndOfDxeBeforeGuid
  checkpoint.

  @retval EFI_SUCCESS             Trigger gH2OBdsCpEndOfDxeBeforeGuid checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
  @return Other                   Other error occurred while triggering gH2OBdsCpEndOfDxeBeforeGuid
                                  checkpoint.
**/
STATIC
EFI_STATUS
TriggerCpEndOfDxeBefore (
  VOID
  )
{
  H2O_BDS_CP_END_OF_DXE_BEFORE_DATA       BdsEndOfDxeBeforeData;
  EFI_STATUS                              Status;

  BdsEndOfDxeBeforeData.Size   = sizeof (H2O_BDS_CP_END_OF_DXE_BEFORE_DATA);
  BdsEndOfDxeBeforeData.Status = H2O_CP_TASK_NORMAL;
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpEndOfDxeBeforeGuid));
  Status = H2OCpTrigger (&gH2OBdsCpEndOfDxeBeforeGuid, &BdsEndOfDxeBeforeData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsEndOfDxeBeforeData.Status));
  return Status;
}

/**
  Internal function to initialize H2O_BDS_CP_END_OF_DXE_AFTER_DATA data and trigger
  gH2OBdsCpEndOfDxeAfterGuid checkpoint.

  @retval EFI_SUCCESS             Trigger gH2OBdsCpEndOfDxeAfterGuid checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
  @return Other                   Other error occurred while triggering gH2OBdsCpEndOfDxeAfterGuid
                                  checkpoint.
**/
STATIC
EFI_STATUS
TriggerCpEndOfDxeAfter (
  VOID
  )
{

  H2O_BDS_CP_END_OF_DXE_AFTER_DATA   BdsEndOfDxeAfterData;
  EFI_STATUS                         Status;

  BdsEndOfDxeAfterData.Size   = sizeof (H2O_BDS_CP_END_OF_DXE_AFTER_DATA);
  BdsEndOfDxeAfterData.Status = H2O_CP_TASK_NORMAL;
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpEndOfDxeAfterGuid));
  Status = H2OCpTrigger (&gH2OBdsCpEndOfDxeAfterGuid, &BdsEndOfDxeAfterData);
  DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsEndOfDxeAfterData.Status));
  return Status;
}

/**
 The function will execute with as the platform policy, current policy
 is driven by boot mode. IBV/OEM can customize this code for their specific
 policy action.

 @param[in]         DriverOptionList    The header of the driver option link list. Deprecated
 @param[in]         BootOptionList      The header of the boot option link list. Deprecated
 @param[in]         ProcessCapsules     A pointer to ProcessCapsules()
 @param[in]         BaseMemoryTest      A pointer to BaseMemoryTest()
*/
VOID
EFIAPI
PlatformBdsPolicyBehavior (
  IN LIST_ENTRY                      *DriverOptionList OPTIONAL,
  IN LIST_ENTRY                      *BootOptionList OPTIONAL,
  IN PROCESS_CAPSULES                ProcessCapsules,
  IN BASEM_MEMORY_TEST               BaseMemoryTest
  )
{
  EFI_STATUS                         Status;
  UINT16                             Timeout;
  KERNEL_CONFIGURATION               KernelSetup;
  CHIPSET_CONFIGURATION              ChipsetSetup;
  EXTENDMEM_COVERAGE_LEVEL           MemoryTestLevel;
  HOT_KEY_CONTEXT                    *HotKeyContext;
  UINT16                             FunctionKey = 0;
  UINTN                              Size;
  EFI_BOOT_MODE                      BootMode;
  EFI_EVENT                          ReadyToBootEvt;
  VOID                               *Registration;
  H2O_BDS_SERVICES_PROTOCOL          *H2OBdsServices;
  BOOLEAN                            EnableHotKey;
  EFI_HANDLE                         Handle;

  DEBUG_CODE (
    if (DriverOptionList != NULL) {
      DEBUG ((DEBUG_INFO, "PlatformBdsPolicyBehavior(): DriverOptionList is deprecated. Please upgrade caller code to input NULL.\n"));
    }
    if (BootOptionList != NULL) {
      DEBUG ((DEBUG_INFO, "PlatformBdsPolicyBehavior(): BootOptionList is deprecated. Please upgrade caller code to input NULL.\n"));
    }
  );

  Handle = NULL;
  Status = EFI_NOT_FOUND;

  //
  // Install notifications for required protocols
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiSmmLockEnablePointProtocolGuid,
    (TPL_CALLBACK - 1),
    PlatformBdsTriggerEndOfDxeEvent,
    NULL,
    &Registration
    );


  Status = GetKernelConfiguration (&KernelSetup);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = GetChipsetConfiguration (&ChipsetSetup);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Get current Boot Mode
  //
  BootMode = GetBootModeHob ();

  HotKeyContext = NULL;
  if (!DoesOsIndicateBootToFwUI () &&
      !(BootMode == BOOT_ON_S4_RESUME && H2OGetBootType () != EFI_BOOT_TYPE)) {
    HotKeyContext                        = AllocateZeroPool (sizeof (HOT_KEY_CONTEXT));
    ASSERT (HotKeyContext != NULL);
    if (HotKeyContext == NULL) {
      return;
    }
    HotKeyContext->EnableQuietBootPolicy = (BOOLEAN) KernelSetup.QuietBoot;
    HotKeyContext->CanShowString         = FALSE;

    //
    // OemServices
    //
    DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcDisplayLogo \n"));
    Status = OemSvcDisplayLogo (
               &HotKeyContext->EnableQuietBootPolicy
               );
    DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcDisplayLogo Status: %r\n", Status));

    POST_CODE (BDS_INSTALL_HOTKEY);
    BdsLibInstallHotKeys (HotKeyContext);
  } else {
    Status = gBS->LocateProtocol (&gH2OBdsServicesProtocolGuid, NULL, (VOID **)&H2OBdsServices);
    if (!EFI_ERROR (Status)) {
      EnableHotKey = FALSE;
      H2OBdsServices->EnableHotKeys (H2OBdsServices, &EnableHotKey);
    }
  }

  //
  // Init the time out value
  //
  Size = sizeof (Timeout);
  Status = gRT->GetVariable (L"Timeout", &gEfiGlobalVariableGuid, NULL, &Size, &Timeout);

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
  MemoryTestLevel = (KernelSetup.QuickBoot == TRUE) ? IGNORE : EXTENSIVE;

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             ReadyToBootEvent,
             NULL,
             &ReadyToBootEvt
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Go the different platform policy with different boot mode
  // Notes: this part code can be changed with the table policy
  //
  switch (BootMode) {

  case BOOT_WITH_MINIMAL_CONFIGURATION:
  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
    //
    // Connect default console devices
    //
    Status = PlatformBdsConnectConsoleMini (gPlatformConsole);
    if (HotKeyContext != NULL) {
      HotKeyContext->CanShowString = TRUE;
    }
    
    PlatformBdsDiagnostics (IGNORE, TRUE, BaseMemoryTest);

    //
    // Update gPlatformConnectLastBoot via resotring saved device path
    // Its device path type should be other than BBS_DEVICE_PATH after transform
    //

    //
    // Connect last boot device path
    //
    PlatformBdsConnectDevicePath (gPlatformConnectSata[0], TRUE, TRUE);

    //
    // Here we have enough time to do the enumeration of boot device
    //
    if (H2OGetBootType () == EFI_BOOT_TYPE && KernelSetup.NewPositionPolicy == IN_AUTO) {
      BdsLibEnumerateAllBootOption (FALSE, BootOptionList);
    } else {
      BdsLibEnumerateAllBootOption (TRUE, BootOptionList);
    }
    PlatformBdsBootSelection (FunctionKey, 0);
    break;

  case BOOT_ON_FLASH_UPDATE:
    //
    // Boot with the specific configuration
    //
    PlatformBdsConnectConsole (gPlatformConsole);
    if (HotKeyContext != NULL) {
      HotKeyContext->CanShowString = TRUE;
    }
    PlatformBdsDiagnostics (EXTENSIVE, FALSE, BaseMemoryTest);
    BdsLibConnectAll ();
    Status = ShowAllDevice ();
    ProcessCapsules (BOOT_ON_FLASH_UPDATE);
    break;

  case BOOT_IN_RECOVERY_MODE:
    //
    // In recovery mode, just connect platform console and show up the front page
    //
    PlatformBdsConnectConsole (gPlatformConsole);
    if (HotKeyContext != NULL) {
      HotKeyContext->CanShowString = TRUE;
    }
    PlatformBdsDiagnostics (EXTENSIVE, FALSE, BaseMemoryTest);
    BdsLibConnectAll ();
    Status = ShowAllDevice ();

    //
    // Recovery don't support HotKey, goto Boot Manager
    //
    BdsLibEnumerateAllBootOption (TRUE, BootOptionList);
    PlatformBdsBootSelection(FunctionKey, Timeout);

    if (FeaturePcdGet(PcdUseFastCrisisRecovery)) {
      //
      // PostCode = 0x35, Fast recovery start flash
      //
      POST_CODE (BDS_RECOVERY_START_FLASH);
      //
      // Try to signal EndOfDxe event at here.
      //
      BdsLibOnEndOfBdsBootSelection ();
      if (FeaturePcdGet (PcdH2OBdsReoveryLongRunSupport)) {
        RecoveryFlash (DEFAULT_FLASH_DEVICE_TYPE);
      } else{
        RecoveryPopUp (DEFAULT_FLASH_DEVICE_TYPE);
      }
    }
    BdsLibStartSetupUtility (TRUE);
    break;

  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
  case BOOT_ON_S4_RESUME:
  default:

    if (PcdGet8 (PcdNvmeRaid)) {
      Status = gBS->InstallProtocolInterface (
                      &Handle,
                      &gUefiRaidOpromReadyGuid,
                      EFI_NATIVE_INTERFACE,
                      NULL
                      );
      Status = gDS->Dispatch ();
    }

    //
    // Connect platform console
    //
    Status = PlatformBdsConnectConsole (gPlatformConsole);
    if (EFI_ERROR (Status)) {
      //
      // Here OEM/IBV can customize with defined action
      //
      PlatformBdsNoConsoleAction ();
    }
    Status = PlatformBdsEnableSmmReadyToLock ();

    if (HotKeyContext != NULL) {
      HotKeyContext->CanShowString = TRUE;
    }

    PlatformBdsBootDisplayDevice ((CHIPSET_CONFIGURATION *) &ChipsetSetup);
    PlatformBdsDiagnostics (MemoryTestLevel, KernelSetup.QuietBoot, BaseMemoryTest);

    //
    // Perform some platform specific connect sequence
    //
    BdsLibConnectLegacyRoms ();

    if (BdsLibIsWin8FastBootActive ()) {
      BdsLibConnectTargetDev ();
    } else {
      PlatformBdsConnectSequence ();
    }

    if (!(KernelSetup.QuietBoot)) {
      Status = ShowAllDevice();
    }

    //
    // Here we have enough time to do the enumeration of boot device
    //
    BdsLibEnumerateAllBootOption (FALSE, BootOptionList);

    //
    // If Resume From S4 BootMode is set become BOOT_ON_S4_RESUME
    //
    // move the check boot mode is whether BOOT_ON_S4_RESUME to PlatformBdsBootSelection ()
    // to make sure the memory mapping is the same between normal boot and S4
    //
    PlatformBdsBootSelection (FunctionKey, Timeout);


    break;
  }

  return;
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
  KERNEL_CONFIGURATION               KernelSetup;
  EFI_HANDLE                         Handle;
  EFI_DEVICE_PATH_PROTOCOL           *UsbConsoleOuputDevPath = NULL;

  Status = GetKernelConfiguration (&KernelSetup);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Connect RootBridge
  //
  POST_CODE (BDS_PCI_ENUMERATION_START);
  Status = PlatformBdsConnectDevicePath (gPlatformRootBridges[0], FALSE, FALSE);
  POST_CODE (BDS_PCI_ENUMERATION_END);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // At this point, all the PCI_IO protocols are installed completely.
  // We install a AllPciIoPrtclsInstlFinishedProtocol to signal this point.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAllPciIoPrtclsInstlFinishedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

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
        BdsLibUpdateConsoleVariable (L"ConOut", PlatformConsole[Index].DevicePath, NULL);
      }
    }
  }

  BdsLibConnectI2cDevice ();

  if (!(KernelSetup.UsbHotKeySupport == 0 && BdsLibIsWin8FastBootActive ())) {
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
  Status = BdsLibConnectAllDefaultConsoles ();

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
  IN CHIPSET_CONFIGURATION      *SetupNVRam
  )
{
  EFI_LEGACY_BIOS_PROTOCOL      *LegacyBios;
  EFI_STATUS                    Status;
  BOOLEAN                       SkipOriginalCode;

  if (SetupNVRam == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SkipOriginalCode = FALSE;

  Status = BdsLibDisplayDeviceReplace (&SkipOriginalCode);
  if (SkipOriginalCode || !EFI_ERROR (Status)) {
   return Status;
  }

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, (VOID**)&LegacyBios);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = OemBootDisplayDeviceCallBack (SetupNVRam, LegacyBios);

  return Status;
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
  EFI_CONSOLE_CONTROL_PROTOCOL       *ConsoleControl;
  KERNEL_CONFIGURATION               KernelSetup;

  NoBootDevices = FALSE;
  VariablePtr = NULL;
  VariableSize = 0;

  //
  // It needs display dialog for user input Hdd password to unlock Hdd in S4 resume.
  //
  BdsLibGetBootMode (&BootMode);

  //
  // Disable Hot Key event and check Hot Key result
  //
  BdsLibStopHotKeyEvent ();

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
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

    Status = GetKernelConfiguration (&KernelSetup);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->LocateProtocol (&gEfiOEMBadgingSupportProtocolGuid, NULL, (VOID**)&Badging);
    if (!EFI_ERROR (Status)) {
      //
      // BOOT_OS string always in the last one of array of OemBadgingString.
      //
      Badging->GetStringCount (Badging, &StringSetNum);
      if (ConsoleControl != NULL) {
        ShowOemString(Badging, TRUE, (UINT8) StringSetNum);
        BdsLibShowOemStringInTextMode (TRUE, (UINT8) StringSetNum);
      } else {
        if (KernelSetup.QuietBoot) {
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

  Status = OemHotKeyCallback (
             Selection,
             Timeout,
             BootMode,
             NoBootDevices
             );

  return EFI_SUCCESS;
}

VOID
EFIAPI
PlatformBdsTriggerSmmReadyToLock (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                    Status;
  EFI_SMM_ACCESS2_PROTOCOL      *SmmAccess;
  EFI_ACPI_S3_SAVE_PROTOCOL     *AcpiS3Save;
//  EFI_HANDLE                    Handle;
  VOID                          *ProtocolPointer;
  UINTN                         Size;
  UINT64                        OsIndications;
  IMAGE_INFO                    ImageInfo;
  EFI_BOOT_MODE                 BootMode;

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

  BootMode = GetBootModeHob ();
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
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

  if (!EFI_ERROR (Status) &&
      (OsIndications & (EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED))) {
    return;
  }

  Size = sizeof (IMAGE_INFO);
  Status = gRT->GetVariable (
                  SECURE_FLASH_INFORMATION_NAME,
                  &gSecureFlashInfoGuid,
                  NULL,
                  &Size,
                  &ImageInfo
                  );
  if (!EFI_ERROR (Status) && (ImageInfo.FlashMode)) {
    return;
  }

  // Secure Flash should run after EndOfDxe
  // Bios Lock need excute between EndOfDxe and SmmReadyToLock
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
//    DEBUG((DEBUG_INFO,"To Signal DxeSmmReadyToLockProtocol\n"));
//    Handle = NULL;
//    Status = gBS->InstallProtocolInterface (
//                    &Handle,
//                    &gEfiDxeSmmReadyToLockProtocolGuid,
//                    EFI_NATIVE_INTERFACE,
//                    NULL
//                    );
//    DEBUG ((DEBUG_INFO, "SmmReadyToLock triggered by gEfiSmmLockEnablePointProtocolGuid installed - Done.\n"));
//    ASSERT_EFI_ERROR (Status);
  }
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
  VOID                          *Registration;

  Status = gBS->LocateProtocol (&gEfiSmmLockEnablePointProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_ERROR (Status)) {
    return ;
  }
  gBS->CloseEvent (Event);

  //
  // EndOfDxe is signaled here if PcdEndOfDxeEventSupported is FALSE.
  // Otherwise, EndOfDxe is signaled at the entry of BDS.
  //
  if (!PcdGetBool (PcdEndOfDxeEventSupported)) {
    if (FeaturePcdGet (PcdH2OBdsCpEndOfDxeBeforeSupported)) {
      TriggerCpEndOfDxeBefore ();
    }

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

    gBS->SignalEvent (EndOfDxeEvent);
    gBS->CloseEvent (EndOfDxeEvent);

    if (FeaturePcdGet (PcdH2OBdsCpEndOfDxeAfterSupported)) {
      TriggerCpEndOfDxeAfter ();
    }
  }

  //
  // Signal SmmReadyToLock after EndOfDxe.
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiSmmLockEnablePointProtocolGuid,
    (TPL_CALLBACK - 1),
    PlatformBdsTriggerSmmReadyToLock,
    NULL,
    &Registration
    );

}


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

/**
  Create first User disaplay selection variable when User disaplay selection variable doesn't exist.

  @retval EFI_SUCCESS        Save disaplay selection variable successfully
  @retval EFI_UNSUPPORTED    Save disaplay selection variable fail or the variable has existed.
**/
EFI_STATUS
DisplayUserSelectionDefault (
  VOID
  )
{
  EFI_STATUS                               Status;
  EFI_DEVICE_PATH_PROTOCOL                 *ConOutVar;
  UINTN                                    ConOutSize;
  UINT32                                   DisplayDeviceAdr;

  DisplayDeviceAdr = 0;
  ConOutVar = NULL;
  ConOutSize = 0;

  //
  // UserVgaSelection variable is only saved in EFI BOOT.
  //
  if (H2OGetBootType () != EFI_BOOT_TYPE) {
    return EFI_UNSUPPORTED;
  }

  ConOutVar = BdsLibGetVariableAndSize (
                L"UserVgaSelection",
                &gEfiGenericVariableGuid,
                &ConOutSize
                );

  if (ConOutVar != NULL) {
    gBS->FreePool (ConOutVar);
    return EFI_UNSUPPORTED;
  }

  ConOutVar = NULL;
  ConOutVar = BdsLibGetVariableAndSize (
                L"ConOut",
                &gEfiGlobalVariableGuid,
                &ConOutSize
                );

  if (ConOutVar != NULL) {

    //
    // Create an UserVgaSelection variable by current Conout Variable when UserVgaSelection doesn't exist.
    //
    Status = gRT->SetVariable (
                    L"UserVgaSelection",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    ConOutSize,
                    ConOutVar
                    );

    gBS->FreePool (ConOutVar);
    return Status;
  }

  return EFI_UNSUPPORTED;
}


