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
#include <Library/TimerLib.h>

#define FILECODE LIBRARY_MPMDEVLIB_DEVICE_RTK8168_FILECODE

#define RTK_VID               0x10EC
#define RTK8168_DID           0x8168
#define RTK8168_E_RID         0x0E
#define RTK8168_F_RID         0x1A
#define RTK8168_G_RID         0x1C

#define RTK8168_BAR2_OFFSET   0x18
#define KVM_RTK_STATUS_REG_OFFSET  0x0C


#define RTK_F_REG_WRITE_BASE_INDEX 0xBC021000
#define RTK_F_REG_READ_BASE_INDEX  0x3C021000
#define RTK_REG_WRITE_BASE_INDEX   0x80421100
#define RTK_REG_READ_BASE_INDEX    0x00421100
#define RTK_REG_INDEX_REG_OFFSET   0x74
#define RTK_REG_DATA_REG_OFFSET    0x70

/**
 * @brief Get KVM Device DID and RevID
 *
 * @param KvmDevHandle PCI device handle
 * @param DIDVID, ClassRevID
 * @return DIDVID, ClassRevID
 */
VOID
GetKvmDeviceDidRevId (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *VendorDeviceId,
  IN OUT UINT8              *ClassRevId
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;

  Status = gBS->HandleProtocol (
      KvmDevHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      0,
      1,
      VendorDeviceId
      );
  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint8,
      8,
      1,
      ClassRevId
      );
  IDS_HDT_CONSOLE_PSP_TRACE ("RTK VendorDevice ID = 0x%x, ClassRevId = 0x%x\n", *VendorDeviceId, *ClassRevId);
}

/**
 * @brief Check if RTK8168F device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @param PcieDidVid Pcie Device Id Vendor ID
 * @return BOOLEAN  TRUE KVM feature supported
 */
BOOLEAN
Rtk8168FIsSupported (
  IN     EFI_HANDLE         *KvmDevHandle
  )
{
  UINT32    DidVid;
  UINT8     ClassRevId;

  GetKvmDeviceDidRevId (KvmDevHandle, &DidVid, &ClassRevId);
  if ((DidVid == ((RTK8168_DID << 16) | RTK_VID)) && (ClassRevId == RTK8168_F_RID)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("RTK8168_F is detected, return TRUE\n");
    return TRUE;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Return FALSE\n");
    return FALSE;
  }
}

/**
 * @brief Check if RTK8168E device support KVM feature
 *
 * @param KvmDevHandle PCI device handle
 * @param PcieDidVid Pcie Device Id Vendor ID
 * @return BOOLEAN  TRUE KVM feature supported
 */
BOOLEAN
Rtk8168EIsSupported (
  IN     EFI_HANDLE         *KvmDevHandle
  )
{
  UINT32    DidVid;
  UINT8     ClassRevId;

  GetKvmDeviceDidRevId (KvmDevHandle, &DidVid, &ClassRevId);
  if ((DidVid == ((RTK8168_DID << 16) | RTK_VID)) && ((ClassRevId ==RTK8168_E_RID) || (ClassRevId ==RTK8168_G_RID))) {
    IDS_HDT_CONSOLE_PSP_TRACE ("RTK8168_E or RTK8168_G is detected, return TRUE\n");
    return TRUE;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Return FALSE\n");
    return FALSE;
  }
}

/**
 * @brief Get the Rtk Net Card Bar Base object
 *
 * @param KvmDevHandle
 * @param NetCardInfo
 * @return EFI_STATUS
 */
EFI_STATUS
GetRtkNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  )
{
  EFI_STATUS           Status;
  UINT64               PciBarBase;
  NetCardInfo->NetCardBar0BaseLo = 0;
  NetCardInfo->NetCardBar0BaseHi = 0;
  Status = GetPciBarBase (KvmDevHandle, RTK8168_BAR2_OFFSET, &PciBarBase);
  if (Status != EFI_SUCCESS)
    return Status;
  NetCardInfo->NetCardBar2BaseLo = PciBarBase & 0xFFFFFFFF;
  NetCardInfo->NetCardBar2BaseHi = PciBarBase >> 32;
  return Status;
}

/**
 * @brief Get the Rtk Mmio Base object
 *
 * @param KvmDevHandle
 * @param NetCardMmioBase
 * @return EFI_STATUS
 */
EFI_STATUS
GetRtkMmioBase (
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
  IDS_HDT_CONSOLE_PSP_TRACE ("RTK Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}

/**
 * @brief Get the Rtk Mmio Size object
 *
 * @param KvmDevHandle
 * @param NetCardMmioSize
 * @return EFI_STATUS
 */
EFI_STATUS
GetRtkMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

/**
 * @brief Get the Rtk Base Addr object
 *
 * @param KvmDevHandle
 * @param KvmBaseAddr
 * @return EFI_STATUS
 */
EFI_STATUS
GetRtkBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINT32               BarLo;
  UINT32               BarHi;
  Status = gBS->HandleProtocol (
      KvmDevHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //Get Base Addr of Bar2
  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      0x18,
      1,
      &BarLo
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //Bar should be memory space
  ASSERT ((BarLo & BIT0) == 0);
  if ((BarLo & BIT0) !=0) {
    return EFI_INVALID_PARAMETER;
  }
  BarHi = 0;
  //Check if it is 32bits bar or 64bits bar (b10)
  if ((BarLo & (BIT1 + BIT2)) == 0x4) {
    Status = PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint32,
        0x1C,
        1,
        &BarHi
        );
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  *KvmBaseAddr = ((UINT64) BarHi << 32) + (BarLo & 0xFFFFFFF0);
  return EFI_SUCCESS;
}
/**
 * @brief Read the NIC card KVM related register, access through index/data pair of PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadRtk8168F_KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  UINT16   Command;
  UINT16   Val;
  EFI_STATUS Status;

  Status = GetPciBarBase (KvmDevHandle, 0x18, &KvmBaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  GetNicCommand (KvmDevHandle, &Command);
  Val = 6;
  SetNicCommand (KvmDevHandle, &Val);
  RegVal = RTK_F_REG_READ_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  MicroSecondDelay (2000);  //Delay 2ms
  LibAmdMemRead (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tRead Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  SetNicCommand (KvmDevHandle, &Command);
  return EFI_SUCCESS;
}

/**
 * @brief Write the NIC card KVM related register, access through index/data pair of PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteRtk8168F_KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  EFI_STATUS Status;

  Status = GetPciBarBase (KvmDevHandle, 0x18, &KvmBaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  RegVal = RTK_F_REG_WRITE_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  return EFI_SUCCESS;
}

/**
 * @brief Read the NIC card KVM related register, access through index/data pair of PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadRtk8168E_KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  UINT16   Command;
  UINT16   Val;
  EFI_STATUS Status;

  Status = GetPciBarBase (KvmDevHandle, 0x18, &KvmBaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  GetNicCommand (KvmDevHandle, &Command);
  Val = 6;
  SetNicCommand (KvmDevHandle, &Val);
  RegVal = RTK_REG_READ_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  MicroSecondDelay (2000);  //Delay 2ms
  LibAmdMemRead (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tRead Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  SetNicCommand (KvmDevHandle, &Command);
  return EFI_SUCCESS;
}

/**
 * @brief Write the NIC card KVM related register, access through index/data pair of PCI BAR MMIO
 *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
WriteRtk8168E_KvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  EFI_STATUS Status;

  Status = GetPciBarBase (KvmDevHandle, 0x18, &KvmBaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  RegVal = RTK_REG_WRITE_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtk8168F_KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmRequest;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtk8168F_KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmRequest = (UINT8) (*value & 0xFF);
  WriteRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtk8168F_KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmSessionProgress;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtk8168F_KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmSessionProgress = (UINT8) (*value & 0xFF);
  WriteRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtk8168E_KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168E_KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmRequest;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtk8168E_KvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168E_KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmRequest = (UINT8) (*value & 0xFF);
  WriteRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtk8168E_KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168E_KvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmSessionProgress;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtk8168E_KvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtk8168E_KvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmSessionProgress = (UINT8) (*value & 0xFF);
  WriteRtk8168F_KvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

/**
 * @brief Read TCR REQUEST status, wired lan access through PCI BAR MMIO, or check the private data structure for wireless *
 * @param KvmDevHandle
 * @param value
 * @return EFI_STATUS
 */
EFI_STATUS
ReadRtk8168TcrRequest (
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
MPM_DEV_PROTOCOL mRtk8168F_Protocol = {
  MPM_NIC_WIRED,
  Rtk8168FIsSupported,
  GetRtkMmioBase,
  GetRtkMmioSize,
  ReadRtk8168F_KvmRequest,
  WriteRtk8168F_KvmRequest,
  ReadRtk8168F_KvmSessionProgress,
  WriteRtk8168F_KvmSessionProgress,
  GetRtkNetCardBarBase,
  ReadRtk8168TcrRequest
};

MPM_DEV_PROTOCOL mRtk8168E_Protocol = {
  MPM_NIC_WIRED,
  Rtk8168EIsSupported,
  GetRtkMmioBase,
  GetRtkMmioSize,
  ReadRtk8168E_KvmRequest,
  WriteRtk8168E_KvmRequest,
  ReadRtk8168E_KvmSessionProgress,
  WriteRtk8168E_KvmSessionProgress,
  GetRtkNetCardBarBase,
  ReadRtk8168TcrRequest
};
