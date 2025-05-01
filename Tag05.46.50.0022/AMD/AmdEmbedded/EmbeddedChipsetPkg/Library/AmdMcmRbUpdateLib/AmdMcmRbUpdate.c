/** @file
  
;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/FabricResourceManagerServicesProtocol.h>
#include "AmdMcmRbUpdate.h"

UINT32
SmnRegisterRead (
  IN UINTN         IohcBusNum,
  IN UINTN         SmnIndexValue
  )
{
  UINT32        PciRegAddr;
  UINT32        PciRegValue;

  PciRegAddr = PCI_LIB_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_INDEX2);
  PciWrite32(PciRegAddr, (UINT32)SmnIndexValue);

  PciRegAddr = PCI_LIB_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_DATA2);
  PciRegValue = PciRead32(PciRegAddr);

  return PciRegValue;
}

UINT32
SmnRegisterWrite (
  IN UINTN         IohcBusNum,
  IN UINTN         SmnIndexValue,
  IN UINTN         SmnDataValue
  )
{
  UINT32        PciRegAddr;
  UINT32        PciRegValue;
  UINT64        PciRegAddrForS3;

  PciRegAddr = PCI_LIB_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_INDEX2);
  PciWrite32(PciRegAddr, (UINT32)SmnIndexValue);
  PciRegAddrForS3 = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_INDEX2);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      PciRegAddrForS3,
      1,
      &SmnIndexValue
      );
  PciRegAddr = PCI_LIB_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_DATA2);
  PciRegValue = PciWrite32(PciRegAddr, (UINT32)SmnDataValue);
  PciRegAddrForS3 = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (IohcBusNum, 0, 0, IOHC_NB_SMN_DATA2);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      PciRegAddrForS3,
      1,
      &SmnDataValue
      );

  return PciRegValue;
}

EFI_STATUS
SetGnbIoApicId (
  IN UINT32           SocketIndex,
  IN UINT32           RbNum,
  IN UINT64           ApicMmioBarAddr
)
{
  UINT32        IoApicIndex;
  UINT32        ApicId;
  UINT32        Value32;

  IoApicIndex = 0;
  IoApicIndex = (SocketIndex << 2) + RbNum;

  ApicId = 0;

  switch (IoApicIndex) {
  case 0:
    ApicId = PcdGet8(PcdCfgGnbIoapicId);
    //PcdSet8S (PcdCfgGnbIoapicId, (UINT8)ApicId);
    break;

  /*case 1:
    ApicId = PcdGet8(PcdGnbIoApic1ID);
    break;

  case 2:
    ApicId = PcdGet8(PcdGnbIoApic2ID);
    break;

  case 3:
    ApicId = PcdGet8(PcdGnbIoApic3ID);
    break;

  case 4:
    ApicId = PcdGet8(PcdGnbIoApic4ID);
    break;

  case 5:
    ApicId = PcdGet8(PcdGnbIoApic5ID);
    break;

  case 6:
    ApicId = PcdGet8(PcdGnbIoApic6ID);
    break;

  case 7:
    ApicId = PcdGet8(PcdGnbIoApic7ID);
    break;*/

  default:
    return EFI_UNSUPPORTED;
  }

  MmioWrite32 (ApicMmioBarAddr, 0);
  Value32 = MmioRead32 ((ApicMmioBarAddr + 0x10));
  Value32 = ((Value32 & 0x0000FFFF) | (ApicId <<24));
  MmioWrite32 ((ApicMmioBarAddr + 0x10), Value32);

  return EFI_SUCCESS;
}

EFI_STATUS
SetFchIoApicId (
  VOID
)
{
  UINT32         Value32;
  UINT32         ApicId;

  ApicId = PcdGet8(PcdCfgFchIoapicId);
  MmioWrite32 (0xFEC00000, 0);
  Value32 = MmioRead32 (0xFEC00010);
  Value32 = ((Value32 & 0x0000FFFF) | (ApicId <<24));
  MmioWrite32 (0xFEC00010, Value32);
  //PcdSet8S (PcdCfgFchIoapicId, (UINT8)ApicId);

  return EFI_SUCCESS;
}

/*UINTN
GetVgaEnRootBridge (
  VOID
)
{
  EFI_STATUS                            Status;
  UINTN                                 PrimaryVgaRbId;
  UINTN                                 Size;

  Size = sizeof (PrimaryVgaRbId);
  Status = gRT->GetVariable (
                  L"PrimaryVgaRbId",
                  &gEfiGenericVariableGuid,
                  NULL,
                  &Size,
                  &PrimaryVgaRbId
                  );
  if (Status != EFI_SUCCESS) {
    PrimaryVgaRbId = PcdGet8(PcdDefaultVgaEnRb);
  }

  return PrimaryVgaRbId;
}*/

EFI_STATUS
GetSystemInfoFromFabric (
  OUT UINTN           *NumberOfSockets,
  OUT UINTN           *NumberOfDies,
  OUT UINTN           *NumberOfRootBridges
  )
{
  EFI_STATUS                             Status;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;

  Status = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **)&FabricTopology);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  FabricTopology->GetSystemInfo (FabricTopology, NumberOfSockets, NumberOfDies, NumberOfRootBridges, NULL, NULL);

  return EFI_SUCCESS;
}

EFI_STATUS
UpdateNonPciRegisters (
  VOID
  )
{
  EFI_STATUS                             Status;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;
  FABRIC_RESOURCE_MANAGER_PROTOCOL       *FabricResource;
  UINTN                                  NumberOfSockets;
  UINTN                                  NumberOfDies;
  UINTN                                  NumberOfRootBridges;
  UINTN                                  DiePerSocketCount;
  UINTN                                  RootBridgePerDieCount;
  UINT32                                 SmnRegIndex;
  UINT32                                 SmnRegValue;
  UINTN                                  PciBusBase;
  UINTN                                  Index;
  UINTN                                  SocketIndex;
  UINTN                                  RbCount;
  FABRIC_TARGET                          FabricTarget;
  FABRIC_MMIO_ATTRIBUTE                  MmioAttribute;
  EFI_PHYSICAL_ADDRESS                   BaseAddress;
  UINT64                                 Length;
  UINT64                                 ResAlignment;
  UINTN                                  IommuPciBus;
  //UINTN                                  VgaEnRbId;

  Status = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid, NULL, (VOID **)&FabricTopology);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gAmdFabricResourceManagerServicesProtocolGuid, NULL, (VOID**)&FabricResource);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Status = FabricTopology->GetSystemInfo (FabricTopology, &NumberOfSockets, &NumberOfDies, &NumberOfRootBridges, NULL, NULL);
  FabricTopology->GetProcessorInfo (FabricTopology, 0, &DiePerSocketCount, NULL);
  FabricTopology->GetDieInfo (FabricTopology, 0, 0, &RootBridgePerDieCount, NULL, NULL);

  //  The non-PCI MMIO will following the Maps
  //  +------------------------------+  <-- Top of root bridge MMIO
  //  |  Debug/UMC Registers         |
  //  +------------------------------+
  //  |  SMN MMIO Control Registers  |
  //  +------------------------------+
  //  |  SMN MMIO Aperture           |
  //  +------------------------------+
  //  |  MP1 Public Mailbox          |
  //  +------------------------------+
  //  |  MP0 Public Mailbox          |
  //  +------------------------------+
  //  |  IOMMU MMIO                  |
  //  +------------------------------+
  //  |  GNB IOAPIC MMIO             |
  //  +------------------------------+  <-- MmioBaseAddr
  //
  SocketIndex = 0;
  RbCount = 0;
  IommuPciBus = 0;
  for (Index = 0; Index < NumberOfRootBridges; Index++) {
    if (RbCount == RootBridgePerDieCount) {
      RbCount = 0;
      SocketIndex++;
    }
    BaseAddress = 0;
    Length = 0x800000;
    FabricTarget.PciBusNum = 0;
    FabricTarget.SocketNum = (UINT16)(SocketIndex);
    FabricTarget.RbNum = (UINT16)(RbCount);
    FabricTarget.TgtType = 1;
    MmioAttribute.ReadEnable = 1;
    MmioAttribute.WriteEnable = 1;
    MmioAttribute.NonPosted = 0;
    MmioAttribute.CpuDis = 0;
    MmioAttribute.MmioType = NON_PCI_DEVICE_BELOW_4G;
    ResAlignment = ALIGN_1M;
    Status = FabricResource->FabricAllocateMmio (FabricResource, &BaseAddress, &Length, ResAlignment, FabricTarget, &MmioAttribute);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    FabricTopology->GetRootBridgeInfo (FabricTopology, SocketIndex, 0, 0, NULL, &PciBusBase, NULL, NULL, NULL, NULL);
    // IOAPIC MMIO
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_IOAPIC_BASE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20))+ SMN_REG_IOAPIC_BASE_HI;
      SmnRegValue = (BaseAddress >> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x APIC base Hi Register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_IOAPIC_BASE_LO;
      SmnRegValue = (BaseAddress & 0xFFFFFF00);
      SmnRegValue |= 0x3; //bit 0: enable, bit 3: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo Register %x Value %x\n", SmnRegIndex, SmnRegValue));
      SetGnbIoApicId ((UINT32)SocketIndex, (UINT32)(RbCount), BaseAddress);
    }
    // IOMMU MMIO
    SmnRegIndex = (SMN_IOMMU_BASE + ((UINT32)(RbCount) << 20)) + PCI_REG_IOMMU_BASE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOMMU_BASE + ((UINT32)(RbCount) << 20)) + PCI_REG_IOMMU_BASE_HI;
      SmnRegValue = ((BaseAddress + 0x100000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x IOMMU MMIO base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOMMU_BASE + ((UINT32)(RbCount) << 20)) + PCI_REG_IOMMU_BASE_LO;
      SmnRegValue = ((BaseAddress + 0x100000) & 0xFFF80000);
      SmnRegValue |= 0x1; // bit 0: enable
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    // MP0 PSP MMIO
    // IOHC0 PSP MMIO was assign in PEI and address was locked.
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP0_MAILBOX_BASE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP0_MAILBOX_BASE_HI;
      SmnRegValue = ((BaseAddress + 0x200000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x PSP MMIO base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP0_MAILBOX_BASE_LO;
      SmnRegValue = ((BaseAddress + 0x200000) & 0xFFF00000);
      SmnRegValue |= 0x101; // bit 0: enable, bit 8: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    // MP1 SMU MMIO
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP1_MAILBOX_BASE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP1_MAILBOX_BASE_HI;
      SmnRegValue = ((BaseAddress + 0x300000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x SMU MMIO base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_MP1_MAILBOX_BASE_LO;
      SmnRegValue = ((BaseAddress + 0x300000) & 0xFFFFFF00);
      SmnRegValue |= 0x3; // bit 0: enable, bit 1: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    // SMN MMIO Aperture
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_APERTURE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_APERTURE_HI;
      SmnRegValue = ((BaseAddress + 0x400000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x SMN Aperture MMIO base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_APERTURE_LO;
      SmnRegValue = ((BaseAddress + 0x400000) & 0xFFF00000);
      SmnRegValue |= 0x3; // bit 0: enable, bit 1: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    // SMN MMIO Control Registers
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_CTRL_REG_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_CTRL_REG_HI;
      SmnRegValue = ((BaseAddress + 0x500000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x SMN Ctrl MMIO base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_SMN_MMIO_CTRL_REG_LO;
      SmnRegValue = ((BaseAddress + 0x500000) & 0xFFFFF000);
      SmnRegValue |= 0x3; // bit 0: enable, bit 1: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    // Debug Registers
    SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_DEBUG_REG_BASE_LO;
    SmnRegValue = SmnRegisterRead (PciBusBase, SmnRegIndex);
    if ((SmnRegValue & BIT0) == 0) {
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_DEBUG_REG_BASE_HI;
      SmnRegValue = ((BaseAddress + 0x600000)>> 32);
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, "Iohc bus 0x%x Debug Reg base Hi register %x Value %x", PciBusBase, SmnRegIndex, SmnRegValue));
      SmnRegIndex = (SMN_IOHCMISC_BASE + ((UINT32)(RbCount) << 20)) + SMN_REG_DEBUG_REG_BASE_LO;
      SmnRegValue = ((BaseAddress + 0x600000) & 0xFFF00000);
      SmnRegValue |= 0x3; // bit 0: enable, bit 1: lock
      SmnRegisterWrite (PciBusBase, SmnRegIndex, SmnRegValue);
      DEBUG ((DEBUG_INFO, " base Lo register %x Value %x\n", SmnRegIndex, SmnRegValue));
    }
    RbCount++;
  }

  SetFchIoApicId ();

  //VgaEnRbId = GetVgaEnRootBridge();
  FabricTarget.PciBusNum = 0;
  FabricTarget.SocketNum = 0;
  FabricTarget.RbNum = 0;
  FabricTarget.TgtType = 1;
  FabricResource->FabricEnableVgaMmio (FabricResource, FabricTarget);

  return EFI_SUCCESS;
}
