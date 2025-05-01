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

#define AQUANTIA_VID            0x1D6A
#define AQUANTIA_10G_DID        0x07B1
#define AQUANTIA_5G_DID         0x08B1
#define AQUANTIA_5G_RID       0x02
#define AQUANTIA_10G_RID      0x02
#define KVM_AQUANTIA_STATUS_REG_OFFSET   0x380
#define AQUANTIA_BAR0_OFFSET    0x10
#define AQUANTIA_BAR2_OFFSET    0x18

#define FILECODE LIBRARY_MPMDEVLIB_DEVICE_AQUANTIA_G_FILECODE

/**
 * @brief Check if device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @return BOOLEAN  True KVM feature supported
 */
BOOLEAN
AquantiaIsSupported (
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
  IDS_HDT_CONSOLE_PSP_TRACE ("\tVendorDevice 0%x, ClassRevID 0x%x\n", VendorDevice, ClassRevID);
  if (((VendorDevice == ((AQUANTIA_10G_DID << 16) | AQUANTIA_VID)) && (ClassRevID ==AQUANTIA_10G_RID)) ||
      ((VendorDevice == ((AQUANTIA_5G_DID << 16) | AQUANTIA_VID)) && (ClassRevID ==AQUANTIA_5G_RID))) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Reture TRUE\n");
    return TRUE;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Reture FLASE\n");
    return FALSE;
  }
}
/**
 * @brief Get the Aquantia Mmio Base object
 *
 * @param KvmDevHandle
 * @param NetCardMmioBase
 * @return EFI_STATUS
 */
EFI_STATUS
GetAquantiaMmioBase (
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
  IDS_HDT_CONSOLE_PSP_TRACE ("Aquantia Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}
/**
 * @brief Get the Aquantia Mmio Size object
 *
 * @param KvmDevHandle
 * @param NetCardMmioSize
 * @return EFI_STATUS
 */
EFI_STATUS
GetAquantiaMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}
/**
 * @brief Get the Aquantia Net Card Bar Base object
 *
 * @param KvmDevHandle
 * @param NetCardInfo
 * @return EFI_STATUS
 */
EFI_STATUS
GetAquantiaNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  )
{
  EFI_STATUS           Status;
  UINT64               PciBarBase;
  Status = GetPciBarBase (KvmDevHandle, AQUANTIA_BAR0_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar0BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar0BaseHi = PciBarBase >> 32;
  Status = GetPciBarBase (KvmDevHandle, AQUANTIA_BAR2_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar2BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar2BaseHi = PciBarBase >> 32;
  return Status;
}

/**
 * @brief Get the Aquantia Base Addr object
 *
 * @param KvmDevHandle
 * @param KvmBaseAddr
 * @return EFI_STATUS
 */
EFI_STATUS
GetAquantiaBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  )
{
  EFI_STATUS           Status;
  Status = GetPciBarBase (KvmDevHandle, 0x10, KvmBaseAddr);
  return Status;
}

/**
 * @brief Read the NIC card KVM related register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadAquantiaKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT16   Command;
  UINT16   Val;
  GetAquantiaBaseAddr (KvmDevHandle, &KvmBaseAddr);
  GetNicCommand (KvmDevHandle, &Command);
  Val = 6;
  SetNicCommand (KvmDevHandle, &Val);
  LibAmdMemRead(AccessWidth32, KvmBaseAddr + KVM_AQUANTIA_STATUS_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tRead Mem: %x[31:0] = %x\n", KvmBaseAddr + KVM_AQUANTIA_STATUS_REG_OFFSET, *value);
  SetNicCommand (KvmDevHandle, &Command);

  return EFI_SUCCESS;
}

/**
 * @brief  Write the NIC card KVM related register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteAquantiaKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  GetAquantiaBaseAddr (KvmDevHandle, &KvmBaseAddr);
  LibAmdMemWrite(AccessWidth32, KvmBaseAddr + KVM_AQUANTIA_STATUS_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + KVM_AQUANTIA_STATUS_REG_OFFSET, *value);

  return EFI_SUCCESS;
}

/**
 * @brief Read KVM REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadAquantiaKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadAquantiaKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.AquantiaField.KvmRequest;

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
WriteAquantiaKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadAquantiaKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.AquantiaField.KvmRequest = *value;
  WriteAquantiaKvmReg (KvmDevHandle, &RegVal.Value);

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
ReadAquantiaKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadAquantiaKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.AquantiaField.KvmSessionProgress;

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
WriteAquantiaKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadAquantiaKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.AquantiaField.KvmSessionProgress = *value;
  WriteAquantiaKvmReg (KvmDevHandle, &RegVal.Value);

  return EFI_SUCCESS;
}

/**
 * @brief Read TCR REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadAquantiaTcrRequest (
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
MPM_DEV_PROTOCOL mAquantiaProtocol = {
  MPM_NIC_WIRED,
  AquantiaIsSupported,
  GetAquantiaMmioBase,
  GetAquantiaMmioSize,
  ReadAquantiaKvmRequest,
  WriteAquantiaKvmRequest,
  ReadAquantiaKvmSessionProgress,
  WriteAquantiaKvmSessionProgress,
  GetAquantiaNetCardBarBase,
  ReadAquantiaTcrRequest
};
