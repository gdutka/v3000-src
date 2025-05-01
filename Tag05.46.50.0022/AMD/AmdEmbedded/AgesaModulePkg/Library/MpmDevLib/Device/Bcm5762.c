/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/MpmDevLib.h>
#include <Protocol/PciIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciLib.h>

#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#define FILECODE LIBRARY_MPMDEVLIB_DEVICE_BCM5762_FILECODE


#define BCM_VID               0x14E4
#define BCM5762_DID           0x1687
#define BCM5762_RID           0x10
#define BCM_5762_DEVID        0x168714E4ul
#define BCM5762_BAR0_OFFSET   0x10
#define BCM5762_BAR2_OFFSET   0x18
#define KVM_BCM_STATUS_REG_OFFSET  0x8

/**
 * @brief Check if device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @return BOOLEAN  TRUE KVM feature supported
 */
BOOLEAN
Brcm5762IsSupported (
  IN     EFI_HANDLE         *KvmDevHandle
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINT32               VendorDevice;
  UINT8                ClassRevID;

  Status = gBS->HandleProtocol (
      KvmDevHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );
  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      0,
      1,
      &VendorDevice
      );
  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint8,
      8,
      1,
      &ClassRevID
      );
  IDS_HDT_CONSOLE_PSP_TRACE ("VendorDevice ID = 0x%x, ClassRevID = 0x%x\n", VendorDevice, ClassRevID);
  if ((VendorDevice == ((BCM5762_DID << 16) | BCM_VID)) && (ClassRevID == BCM5762_RID)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Reture TRUE\n");
    return TRUE;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Reture FALSE\n");
    return FALSE;
  }
}
/**
 * @brief Get the Brcm5762 Mmio Base object
 *
 * @param KvmDevHandle
 * @param NetCardMmioBase
 * @return EFI_STATUS
 */
EFI_STATUS
GetBrcm5762MmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioBase
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINTN                SegmentNumber;
  UINTN                BusNumber;
  UINTN                DeviceNumber;
  UINTN                FunctionNumber;
  Status = gBS->HandleProtocol (
      KvmDevHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //Aquired NetCard Info
  Status = PciIo->GetLocation(
      PciIo,
      &SegmentNumber,
      &BusNumber,
      &DeviceNumber,
      &FunctionNumber
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Brcm5762 Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}

/**
 * @brief Get the Brcm5762 Mmio Size object
 *
 * @param KvmDevHandle
 * @param NetCardMmioSize
 * @return EFI_STATUS
 */
EFI_STATUS
GetBrcm5762MmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

/**
 * @brief Get the Brcm5762 Base Addr object
 *
 * @param KvmDevHandle
 * @param KvmBaseAddr
 * @return EFI_STATUS
 */
EFI_STATUS
GetBrcm5762BaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  )
{
  EFI_STATUS           Status;
  Status = GetPciBarBase (KvmDevHandle, 0x18, KvmBaseAddr);
  return Status;
}
/**
 * @brief  Read the NIC card KVM related register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadBrcm5762KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT16   Command;
  UINT16   Val;
  GetBrcm5762BaseAddr (KvmDevHandle, &KvmBaseAddr);
  GetNicCommand (KvmDevHandle, &Command);
  Val = 6;
  SetNicCommand (KvmDevHandle, &Val);
  LibAmdMemRead(AccessWidth32, KvmBaseAddr + KVM_BCM_STATUS_REG_OFFSET, value, NULL);
  SetNicCommand (KvmDevHandle, &Command);
  return EFI_SUCCESS;
}

/**
 * @brief Write the NIC card KVM related register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteBrcm5762KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  GetBrcm5762BaseAddr (KvmDevHandle, &KvmBaseAddr);
  LibAmdMemWrite(AccessWidth32, KvmBaseAddr + KVM_BCM_STATUS_REG_OFFSET, value, NULL);
  return EFI_SUCCESS;
}

/**
 * @brief Read KVM REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadBrcm5762KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.BrcmField.KvmRequest;
  return EFI_SUCCESS;
}

/**
 * @brief Write the NIC card KVM REQUEST register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteBrcm5762KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.BrcmField.KvmRequest = *value;
  WriteBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

/**
 * @brief Read the NIC card KVM SESSION register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadBrcm5762KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.BrcmField.KvmSessionProgress;
  return EFI_SUCCESS;
}

/**
 * @brief Write the NIC card KVM SESSION register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteBrcm5762KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.BrcmField.KvmSessionProgress = *value;
  WriteBrcm5762KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

/**
 * @brief Get the Brcm5762 Net Card Bar Base object
 *
 * @param KvmDevHandle
 * @param NetCardInfo
 * @return EFI_STATUS
 */
EFI_STATUS
GetBrcm5762NetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  )
{
  EFI_STATUS           Status;
  UINT64               PciBarBase;
  Status = GetPciBarBase (KvmDevHandle, BCM5762_BAR0_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar0BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar0BaseHi = PciBarBase >> 32;
  Status = GetPciBarBase (KvmDevHandle, BCM5762_BAR2_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar2BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar2BaseHi = PciBarBase >> 32;
  return Status;
}

/**
 * @brief Read TCR REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadBrcm5762TcrRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  *value = TCR_NOT_REQUESTED;

  return EFI_UNSUPPORTED;
}

/**
 * @brief KVM internal device protocol, abstart all required interface for a device
 *
 */
MPM_DEV_PROTOCOL mBrcm5762Protocol = {
  MPM_NIC_WIRED,
  Brcm5762IsSupported,
  GetBrcm5762MmioBase,
  GetBrcm5762MmioSize,
  ReadBrcm5762KvmRequest,
  WriteBrcm5762KvmRequest,
  ReadBrcm5762KvmSessionProgress,
  WriteBrcm5762KvmSessionProgress,
  GetBrcm5762NetCardBarBase,
  ReadBrcm5762TcrRequest
};
