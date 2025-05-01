/** @file
  Provide OEM to install the PCI Option ROM table and Non-PCI Option ROM table.

;******************************************************************************
;* Copyright (c) 2012 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>

#include <Library/DxeOemSvcKernelLib.h>
#include <Protocol/PciRootBridgeIo.h>
#include <KernelSetupConfig.h>
#include <Library/H2OLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Protocol/PciIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciExpressLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Pci.h>

#define PCI_CLASS_MASS_STORAGE_NVM                0x08  // mass storage sub-class non-volatile memory.
#define PCI_IF_NVMHCI                             0x02  // mass storage programming interface NVMHCI.
#define NVME_CLASS_CODE                           (UINT32) ( (PCI_CLASS_MASS_STORAGE<<24) | \
                                                   (PCI_CLASS_MASS_STORAGE_NVM<<16) | \
                                                   (PCI_IF_NVMHCI<<8) )  //0x01080200
#define AMD_RAID_CONFIG_NAME L"AMD_RAID"

EFI_GUID gAmdRaidConfigVarGuid = \
  { 0xfe26a894, 0xd199, 0x47d4, { 0x8a, 0xfa, 0x7, 0xe, 0x3d, 0x54, 0xba, 0x86 } };

/*++

Todo:
  Define the relateaed data.

//=======================================
//         Pci Option Rom Table
//=======================================
PCI_OPTION_ROM_TABLE      PciOptionRomTable[] = {
  {
    NULL_ROM_FILE_GUID,
    0,
    0,
    0,
    0,
    0xffff,
    0xffff
  }
};


//=======================================
//        Non Pci Option Rom Table
//=======================================
//
// System Rom table
//
SYSTEM_ROM_TABLE    SystemRomTable[] = {
  {
    //
    // CSM16 binary
    //
    SYSTEM_ROM_FILE_GUID,
    TRUE,
    SYSTEM_ROM
  },
  {
    NULL_ROM_FILE_GUID,
    FALSE
    MAX_NUM
  }
};
.
.
.
--*/

/**
  To handle a special case, more than 1 PCI devices which has the same VID/DID 
  exist on the system but not all of them has PCI option ROM to be loaded. 
  So provide an OEM service, let project owner to decide if this device has 
  PCI Option ROM to be loaded or not. if SkipGetPciRom is TRUE, 
  means there is no option ROM for this device, otherwise, 
  option ROM described on PCI Option ROM table will be loaded for this device. 
  This OEM service will only be invoked if the VID/DID of PCI device could be found in PCI Option ROM Table.	

  @param[in]  Segment               Segment number.
  @param[in]  Bus                   Bus number.
  @param[in]  Device                Device number.
  @param[in]  Function              Function number.
  @param[in]  VendorId              Device vendor ID.
  @param[in]  DeviceId              Device ID.
  @param[out] *SkipGetPciRom        If SkipGetPciRom == TRUE means that there is no 
			            ROM in this device.
  
  @retval     EFI_UNSUPPORTED       Returns unsupported by default.
  @retval     EFI_SUCCESS           The service is customized in the project.
  @retval     EFI_MEDIA_CHANGED     The value of IN OUT parameter is changed. 
  @retval     Others                Based on OEM design.
**/
EFI_STATUS
OemSvcSkipLoadPciOptionRom (
  IN  UINTN                                 Segment,
  IN  UINTN                                 Bus,
  IN  UINTN                                 Device,
  IN  UINTN                                 Function,
  IN  UINT16                                VendorId,
  IN  UINT16                                DeviceId,
  OUT BOOLEAN                               *SkipGetPciRom
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/
//
//  Here is a sample to skip option ROM and UEFI driver for ASM1061 PCIE to SATA card
//  in current mechanism, nomatter it is in FV or on the card, all will be skipped
//
//  if (VendorId == 0x1b21 && DeviceId == 0x0612) {
//    *SkipGetPciRom = TRUE;
//    return EFI_MEDIA_CHANGED;
//  }
//
  EFI_STATUS                        Status;
  UINT8                             SecBus;
  UINT8                             SubBus;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *PciIoRootBridge;
  UINT64                            Address;
  UINT16                            TestVendorId;
  UINT16                            TestClassCode;
  UINT32                            ClassCode;
  UINTN                             VariableSize;
  UINTN                             AmdRaid;
  UINT32                            DidVid;

  DEBUG ((EFI_D_INFO, " OemSvcSkipLoadPciOptionRom-Start\n"));
  SecBus = 0;
  SubBus = 0;
  TestVendorId = 0;
  TestClassCode = 0;
  PciIoRootBridge = NULL;
  Address = 0;
  VariableSize = sizeof(UINTN);
  AmdRaid = 0;

  Status = gRT->GetVariable (
                  AMD_RAID_CONFIG_NAME,
                  &gAmdRaidConfigVarGuid,
                  NULL,
                  &VariableSize,
                  &AmdRaid
                  );

  DEBUG ((DEBUG_INFO, " Get AMD_RAID Variable fail! %r\n", Status));
  DEBUG ((DEBUG_INFO, "PcdNvmeRaid %d\n", PcdGet8 (PcdNvmeRaid)));

  //
  // Check BIT2 of AMD_RAID Variable.
  //
  AmdRaid = AmdRaid & 0x4;
  if (PcdGet8 (PcdNvmeRaid) && (AmdRaid == 0x4)) {
    //
    // Now further check the PCI header: Base class (offset 0x0B) and Sub Class (offset 0x0A).
    // This controller should be a Nvm Express controller.
    //
    DidVid = PciExpressRead32(PCI_EXPRESS_LIB_ADDRESS(Bus, Device, Function, 0x00));
    ClassCode = PciExpressRead32(PCI_EXPRESS_LIB_ADDRESS(Bus, Device, Function, 0x08));
    if ((ClassCode & 0xFFFFFF00) == NVME_CLASS_CODE) {
      *SkipGetPciRom = TRUE;
      DEBUG((DEBUG_INFO, " Skip NVME Option ROM! DidVid = 0x%x\n", DidVid));
      return EFI_MEDIA_CHANGED;
    }
  }

  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &PciIoRootBridge
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

//  if ((VendorId == 0x144D) && (DeviceId == 0xA802)) {
//    *SkipGetPciRom = TRUE;
//    return EFI_MEDIA_CHANGED;
//  }

  if ((VendorId == 0x10DF) && (DeviceId == 0x0720)) { //Emulex 10G NIC
    *SkipGetPciRom = TRUE;
    return EFI_MEDIA_CHANGED;
  }

  if ((VendorId == 0x10DF) && (DeviceId == 0xF400)) { //Emulex Gen4 NIC
    *SkipGetPciRom = TRUE;
    return EFI_MEDIA_CHANGED;
  }

  if (H2OGetBootType () == EFI_BOOT_TYPE) {
    Address = EFI_PCI_ADDRESS (0, 1, 1, 0x19);
    PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &SecBus);
    Address = EFI_PCI_ADDRESS (0, 1, 1, 0x1A);
    PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &SubBus);
    if ((SecBus != 0) && (SecBus != 0xFF) && (SubBus != 0) && (SubBus != 0xFF)) {
      Address = EFI_PCI_ADDRESS (SecBus, 0, 0, 0);
      PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint16, Address, 1, &TestVendorId);
      Address = EFI_PCI_ADDRESS (SecBus, 0, 0, 0x0A);
      PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint16, Address, 1, &TestClassCode);
      if ((TestVendorId == 0x8086) && (TestClassCode == 0x0604)) {
        if ((Bus >= SecBus) && (Bus <= SubBus)) {
          *SkipGetPciRom = TRUE;
          if ((VendorId == 0x144D) && (DeviceId == 0xA802)) {
            return EFI_UNSUPPORTED;
          }
          return EFI_MEDIA_CHANGED;
        }
      }
    }

    Address = EFI_PCI_ADDRESS (0, 1, 2, 0x19);
    PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &SecBus);
    Address = EFI_PCI_ADDRESS (0, 1, 2, 0x1A);
    PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint8, Address, 1, &SubBus);
    if ((SecBus != 0) && (SecBus != 0xFF) && (SubBus != 0) && (SubBus != 0xFF)) {
      Address = EFI_PCI_ADDRESS (SecBus, 0, 0, 0);
      PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint16, Address, 1, &TestVendorId);
      Address = EFI_PCI_ADDRESS (SecBus, 0, 0, 0x0A);
      PciIoRootBridge->Pci.Read (PciIoRootBridge, EfiPciIoWidthUint16, Address, 1, &TestClassCode);
      if ((TestVendorId == 0x8086) && (TestClassCode == 0x0604)) {
        if ((Bus >= SecBus) && (Bus <= SubBus)) {
          *SkipGetPciRom = TRUE;
          if ((VendorId == 0x144D) && (DeviceId == 0xA802)) {
            return EFI_UNSUPPORTED;
          }
          return EFI_MEDIA_CHANGED;
        }
      }
    }
  }

  return EFI_UNSUPPORTED;
}
