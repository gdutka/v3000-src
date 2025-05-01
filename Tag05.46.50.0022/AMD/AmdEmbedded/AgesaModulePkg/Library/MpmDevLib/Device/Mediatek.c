/******************************************************************************
  Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
*****************************************************************************/

#include <Library/MpmDevLib.h>
#include <Protocol/PciIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciLib.h>

#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>

#define FILECODE LIBRARY_MPMDEVLIB_DEVICE_MEDIATEK_FILECODE

#define MED_VID               0x14C3
#define MED_RZ616_DID         0x0616
#define MEDIATKE_RZ616_DIDVID 0x061614C3ul
#define MEDIATEK_BAR0_OFFSET   0x10
#define MEDIATEK_BAR2_OFFSET   0x18

/**
 * @brief Check if device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @return BOOLEAN  TRUE KVM feature supported
 */
BOOLEAN
MediatekIsSupported (
  IN     EFI_HANDLE         *KvmDevHandle
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINT32               VendorDevice;
  BOOLEAN              Supported;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tMediatekIsSupported:\n");
  Status = gBS->HandleProtocol (
      KvmDevHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );
  if (EFI_ERROR(Status)) {
    return FALSE;
  }
  Supported = FALSE;
  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      0,
      1,
      &VendorDevice
      );
  IDS_HDT_CONSOLE_PSP_TRACE ("\tVendorDevice %x\n", VendorDevice);

  (VendorDevice == MEDIATKE_RZ616_DIDVID) ? (Supported = TRUE) : (Supported = FALSE);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t Return %x\n", Supported);
  return Supported;
}
/**
 * @brief Get the Mediatek Mmio Base object
 *
 * @param KvmDevHandle
 * @param NetCardMmioBase
 * @return EFI_STATUS
 */
EFI_STATUS
GetMediatekMmioBase (
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
  //Acquired NetCard Info
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
  IDS_HDT_CONSOLE_PSP_TRACE ("Mediatek Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}

/**
 * @brief Get the Mediatek Mmio Size object
 *
 * @param KvmDevHandle
 * @param NetCardMmioSize
 * @return EFI_STATUS
 */
EFI_STATUS
GetMediatekMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

/**
 * @brief Get the Mediatek Base Addr object
 *
 * @param KvmDevHandle
 * @param KvmBaseAddr
 * @return EFI_STATUS
 */
EFI_STATUS
GetMediatekBaseAddr (
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
ReadMediatekKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Write the NIC card KVM related register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteMediatekKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Read KVM REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadMediatekKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("ReadMediatekKvmRequest\n");
  *value = KVM_NOT_REQUESTED;
  //In wireless case, check pending bit + VIDDID
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  if (Data->WirelessKvmPending) {
    if (Data->KvmDidVid == MEDIATKE_RZ616_DIDVID) {
      *value = KVM_REQUESTED;
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Return:%x\n", *value);
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
WriteMediatekKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Read the NIC card KVM SESSION register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadMediatekKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Write the NIC card KVM SESSION register, access through PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteMediatekKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Get the Mediatek Net Card Bar Base object
 *
 * @param KvmDevHandle
 * @param NetCardInfo
 * @return EFI_STATUS
 */
EFI_STATUS
GetMediatekNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  )
{
  EFI_STATUS           Status;
  UINT64               PciBarBase;
  Status = GetPciBarBase (KvmDevHandle, MEDIATEK_BAR0_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar0BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar0BaseHi = PciBarBase >> 32;
  Status = GetPciBarBase (KvmDevHandle, MEDIATEK_BAR2_OFFSET, &PciBarBase);
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
ReadMediatekTcrRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;
  *value = TCR_NOT_REQUESTED;
  //In wireless case, check pending bit + VIDDID
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  if (Data->WirelessTxrPending) {
    if (Data->TcrDidVid == MEDIATKE_RZ616_DIDVID){
      *value = TCR_REQUESTED;
    }
  }
  return EFI_SUCCESS;
}

/**
 * @brief KVM internal device protocol, abstart all required interface for a device
 *
 */
MPM_DEV_PROTOCOL mMediatekProtocol = {
  MPM_NIC_WIRELESS,
  MediatekIsSupported,
  GetMediatekMmioBase,
  GetMediatekMmioSize,
  ReadMediatekKvmRequest,
  WriteMediatekKvmRequest,
  ReadMediatekKvmSessionProgress,
  WriteMediatekKvmSessionProgress,
  GetMediatekNetCardBarBase,
  ReadMediatekTcrRequest
};
