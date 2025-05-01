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

#define FILECODE LIBRARY_MPMDEVLIB_DEVICE_QCAHASTINGS_FILECODE

#define QCA_VID               0x17CB
#define QCA6390_DID           0x1101
#define QCA6851_DID           0x1103
#define QCA_6390_DEVID        0x110117CBul
#define QCA_6851_DEVID        0x110317CBul
#define QCAHastings_BAR0_OFFSET   0x10
#define QCAHastings_BAR2_OFFSET   0x18

/**
 * @brief Check if device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @return BOOLEAN  TRUE KVM feature supported
 */
BOOLEAN
QcaHastingsIsSupported (
  IN     EFI_HANDLE         *KvmDevHandle
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINT32               VendorDevice;
  BOOLEAN              Supported;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tQcaHastingsIsSupported:\n");
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

  ((VendorDevice == QCA_6390_DEVID) || (VendorDevice == QCA_6851_DEVID)) ? (Supported = TRUE) : (Supported = FALSE);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t Return %x\n", Supported);
  return Supported;
}
/**
 * @brief Get the QcaHastings Mmio Base object
 *
 * @param KvmDevHandle
 * @param NetCardMmioBase
 * @return EFI_STATUS
 */
EFI_STATUS
GetQcaHastingsMmioBase (
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
  IDS_HDT_CONSOLE_PSP_TRACE ("QcaHastings Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}

/**
 * @brief Get the QcaHastings Mmio Size object
 *
 * @param KvmDevHandle
 * @param NetCardMmioSize
 * @return EFI_STATUS
 */
EFI_STATUS
GetQcaHastingsMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

/**
 * @brief Get the QcaHastings Base Addr object
 *
 * @param KvmDevHandle
 * @param KvmBaseAddr
 * @return EFI_STATUS
 */
EFI_STATUS
GetQcaHastingsBaseAddr (
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
ReadQcaHastingsKvmReg (
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
WriteQcaHastingsKvmReg (
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
ReadQcaHastingsKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  EFI_STATUS Status;
  MPM_PRVIATE_HEAP_DATA_V1 *Data;

  IDS_HDT_CONSOLE_PSP_TRACE ("ReadQcaHastingsKvmRequest\n");
  *value = KVM_NOT_REQUESTED;
  //In wireless case, check pending bit + VIDDID
  Status = LocateMpmPrivateData (&Data);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Can't Locate MpmData\n");
    return Status;
  }
  if (Data->WirelessKvmPending) {
    if ((Data->KvmDidVid == QCA_6390_DEVID) || (Data->KvmDidVid == QCA_6851_DEVID)) {
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
WriteQcaHastingsKvmRequest (
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
ReadQcaHastingsKvmSessionProgress (
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
WriteQcaHastingsKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  return EFI_UNSUPPORTED;
}

/**
 * @brief Get the QcaHastings Net Card Bar Base object
 *
 * @param KvmDevHandle
 * @param NetCardInfo
 * @return EFI_STATUS
 */
EFI_STATUS
GetQcaHastingsNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  )
{
  EFI_STATUS           Status;
  UINT64               PciBarBase;
  Status = GetPciBarBase (KvmDevHandle, QCAHastings_BAR0_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar0BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar0BaseHi = PciBarBase >> 32;
  Status = GetPciBarBase (KvmDevHandle, QCAHastings_BAR2_OFFSET, &PciBarBase);
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
ReadQcaHastingsTcrRequest (
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
    if ((Data->TcrDidVid == QCA_6390_DEVID) || (Data->TcrDidVid == QCA_6851_DEVID)){
      *value = TCR_REQUESTED;
    }
  }
  return EFI_SUCCESS;
}

/**
 * @brief KVM internal device protocol, abstart all required interface for a device
 *
 */
MPM_DEV_PROTOCOL mQcaHastingsProtocol = {
  MPM_NIC_WIRELESS,
  QcaHastingsIsSupported,
  GetQcaHastingsMmioBase,
  GetQcaHastingsMmioSize,
  ReadQcaHastingsKvmRequest,
  WriteQcaHastingsKvmRequest,
  ReadQcaHastingsKvmSessionProgress,
  WriteQcaHastingsKvmSessionProgress,
  GetQcaHastingsNetCardBarBase,
  ReadQcaHastingsTcrRequest
};
