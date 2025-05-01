/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "AmdPspKvm.h"
#include <AmdPspDirectory.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/FchSpiAccessLib.h>

#define FILECODE PSP_AMDPSPKVM_AMDPSPKVM_FILECODE

STATIC BOOLEAN mAmdPspKvmDriverStarted = FALSE;
BOOLEAN mKvmCapableDevDetected = FALSE;

STATIC KVM_DEV_PROTOCOL mRtkProtocol_F = {
  GetRtkMmioBase,
  GetRtkMmioSize,
  ReadRtkKvmRequest_F,
  WriteRtkKvmRequest_F,
  ReadRtkKvmSessionProgress_F,
  WriteRtkKvmSessionProgress_F,
  GetRtkNetCardBarBase
};

STATIC KVM_DEV_PROTOCOL mRtkProtocol = {
  GetRtkMmioBase,
  GetRtkMmioSize,
  ReadRtkKvmRequest,
  WriteRtkKvmRequest,
  ReadRtkKvmSessionProgress,
  WriteRtkKvmSessionProgress,
  GetRtkNetCardBarBase
};

STATIC KVM_DEV_PROTOCOL mBrcmProtocol = {
  GetBrcmMmioBase,
  GetBrcmMmioSize,
  ReadBrcmKvmRequest,
  WriteBrcmKvmRequest,
  ReadBrcmKvmSessionProgress,
  WriteBrcmKvmSessionProgress,
  GetBrcmNetCardBarBase
};

STATIC KVM_DEV_PROTOCOL mAquantiaProtocol = {
  GetAquantiaMmioBase,
  GetAquantiaMmioSize,
  ReadAquantiaKvmRequest,
  WriteAquantiaKvmRequest,
  ReadAquantiaKvmSessionProgress,
  WriteAquantiaKvmSessionProgress,
  GetAquantiaNetCardBarBase
};

KVM_DEV_INFO mKvmDevInfo[] = {
  {KVM_UNSUPPORTED, KVM_UNSUPPORTED, KVM_UNSUPPORTED, NULL},
  {RTK_VID,         RTK8168_DID,     RTK8168_F_RID,   &mRtkProtocol_F},
  {BCM_VID,         BCM5762_DID,     BCM5762_RID,     &mBrcmProtocol},
  {RTK_VID,         RTK8168_DID,     RTK8168_E_RID,   &mRtkProtocol},
  {AQUANTIA_VID,    AQUANTIA_10G_DID,AQUANTIA_10G_RID, &mAquantiaProtocol},
  {AQUANTIA_VID,    AQUANTIA_5G_DID, AQUANTIA_5G_RID,  &mAquantiaProtocol}
};

EFI_COMPONENT_NAME_PROTOCOL  AmdPspKvmComponentName = {
  AmdPspKvmComponentNameGetDriverName,
  AmdPspKvmComponentNameGetControllerName,
  "eng"
};

EFI_COMPONENT_NAME2_PROTOCOL AmdPspKvmComponentName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) AmdPspKvmComponentNameGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) AmdPspKvmComponentNameGetControllerName,
  "en"
};

EFI_UNICODE_STRING_TABLE mAmdPspKvmDriverNameTable[] = {
  {
    "eng;en",
    L"PSP KVM Virtual KB/MS Driver"
  },
  {
    NULL,
    NULL
  }
};

EFI_DRIVER_BINDING_PROTOCOL gAmdPspKvmDriver = {
  AmdPspKvmDriverSupported,
  AmdPspKvmDriverStart,
  AmdPspKvmDriverStop,
  0xa,
  NULL,
  NULL
};

STATIC AMD_PSP_KVM_SERVICE_PROTOCOL mAmdPspKvmServiceProtocol = {
  AMD_PSP_KVM_SERVICE_PROTOCOL_VERSION_1,
  AmdPspKvmGetRequest,
  AmdPspKvmSetRequest,
  AmdPspKvmGetSessionProgress,
  AmdPspKvmSetSessionProgress,
  AmdPspKvmInitiateKvm,
  AmdPspKvmTerminateKvm,
  AmdPspKvmRetrieveSmbiosTable
};

STATIC AMD_PSP_KVM_DEV mAmdPspKvmDev;
UINT64 mNicCoherencyBufferAddr = 0;
EFI_PCI_IO_PROTOCOL  *mKvmDevPciIo = NULL;
UINT64 mKvmDmaHpaAddr = 0;
UINT64 mKvmNicCoherentBufferHpaAddr = 0;
UINT64 mKvmDmaGpaAddr = 0;
UINT64 mKvmNicCoherentBufferGpaAddr = 0;

EFI_STATUS
GetPciBarBase (
  IN     EFI_HANDLE         *PciHandle,
  IN     UINT32             Offset,
  IN OUT UINT64             *PciBarBase
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINT32               BarLo;
  UINT32               BarHi;

  Status = gBS->HandleProtocol (
      PciHandle,
      &gEfiPciIoProtocolGuid,
      &PciIo
      );

  //Get Base Addr of Bar0
  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      Offset,
      1,
      &BarLo
      );
  //Bar should be memory space
  ASSERT ((BarLo & BIT0) == 0);
  BarHi = 0;
  //Check if it is 32bits bar or 64bits bar (b10)
  if ((BarLo & (BIT1 + BIT2)) == 0x4) {
    Status = PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint32,
        Offset + 4,
        1,
        &BarHi
        );
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }
  *PciBarBase = ((UINT64) BarHi << 32) + (BarLo & 0xFFFFFFF0);
  return EFI_SUCCESS;
}

EFI_STATUS
GetAquantiaMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
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

EFI_STATUS
GetAquantiaMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

EFI_STATUS
GetBrcmNetCardBarBase (
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

EFI_STATUS
GetBrcmMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
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
  IDS_HDT_CONSOLE_PSP_TRACE ("Brcm Dev: Bus - 0x%x; Device - 0x%x; Function - 0x%x\n", BusNumber, DeviceNumber, FunctionNumber);
  *NetCardMmioBase = PCI_LIB_ADDRESS (BusNumber, DeviceNumber, FunctionNumber, 0x0);
  return EFI_SUCCESS;
}

EFI_STATUS
GetBrcmMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

EFI_STATUS
GetBrcmBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  )
{
  EFI_STATUS           Status;
  Status = GetPciBarBase (KvmDevHandle, 0x18, KvmBaseAddr);
  return Status;
}

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

EFI_STATUS
GetNicCommand (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT16             *Command
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
    return Status;
  }

  Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint16,
      0x4,
      1,
      Command
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
SetNicCommand (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT16             *Command
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
    return Status;
  }

  Status = PciIo->Pci.Write (
      PciIo,
      EfiPciIoWidthUint16,
      0x4,
      1,
      Command
      );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}

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

EFI_STATUS
ReadBrcmKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT16   Command;
  UINT16   Val;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
  GetNicCommand (KvmDevHandle, &Command);
  Val = 6;
  SetNicCommand (KvmDevHandle, &Val);
  LibAmdMemRead(AccessWidth32, KvmBaseAddr + KVM_BCM_STATUS_REG_OFFSET, value, NULL);
  SetNicCommand (KvmDevHandle, &Command);
  return EFI_SUCCESS;
}

EFI_STATUS
WriteBrcmKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
  LibAmdMemWrite(AccessWidth32, KvmBaseAddr + KVM_BCM_STATUS_REG_OFFSET, value, NULL);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadBrcmKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.BrcmField.KvmRequest;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteBrcmKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.BrcmField.KvmRequest = *value;
  WriteBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadBrcmKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.BrcmField.KvmSessionProgress;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteBrcmKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.BrcmField.KvmSessionProgress = *value;
  WriteBrcmKvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

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

EFI_STATUS
GetRtkMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
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

EFI_STATUS
GetRtkMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  )
{
  *NetCardMmioSize = 0x1000;
  return EFI_SUCCESS;
}

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

EFI_STATUS
ReadRtkKvmReg_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  UINT16   Command;
  UINT16   Val;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
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

EFI_STATUS
WriteRtkKvmReg_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
  RegVal = RTK_F_REG_WRITE_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtkKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  UINT16   Command;
  UINT16   Val;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
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

EFI_STATUS
WriteRtkKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  )
{
  UINT64   KvmBaseAddr;
  UINT32   RegVal;
  GetBrcmBaseAddr (KvmDevHandle, &KvmBaseAddr);
  RegVal = RTK_REG_WRITE_BASE_INDEX + KVM_RTK_STATUS_REG_OFFSET;
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, value, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_DATA_REG_OFFSET, *value);
  LibAmdMemWrite (AccessWidth32, KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, &RegVal, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tWrite Mem: %x[31:0] = %x\n", KvmBaseAddr + RTK_REG_INDEX_REG_OFFSET, RegVal);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtkKvmRequest_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmRequest;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtkKvmRequest_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmRequest = (UINT8) (*value & 0xFF);
  WriteRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtkKvmSessionProgress_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmSessionProgress;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtkKvmSessionProgress_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmSessionProgress = (UINT8) (*value & 0xFF);
  WriteRtkKvmReg_F (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtkKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmRequest;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtkKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmRequest = (UINT8) (*value & 0xFF);
  WriteRtkKvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

EFI_STATUS
ReadRtkKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg (KvmDevHandle, &RegVal.Value);
  *value = RegVal.RtkField.KvmSessionProgress;
  return EFI_SUCCESS;
}

EFI_STATUS
WriteRtkKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  )
{
  KVM_STATUS_REG   RegVal;
  ReadRtkKvmReg (KvmDevHandle, &RegVal.Value);
  RegVal.RtkField.KvmSessionProgress = (UINT8) (*value & 0xFF);
  WriteRtkKvmReg (KvmDevHandle, &RegVal.Value);
  return EFI_SUCCESS;
}

UINT32
IsSupportedDev (
  IN     UINT32    VendorDeviceID,
  IN     UINT32    ClassRevID
  )
{
  UINT32      KvmDevNum;
  KVM_DEVID   SupportedDevID;
  UINT32      SupportedClassRevId;

  IDS_HDT_CONSOLE_PSP_TRACE ("Scanning supported device list\n");
  // Check VendorDeviceID
  for (KvmDevNum = sizeof (mKvmDevInfo) / sizeof (KVM_DEV_INFO) - 1; KvmDevNum > 0; KvmDevNum--) {
    // Joint VID,DID into SupportedDevID
    SupportedDevID.Field.DeviceID = mKvmDevInfo[KvmDevNum].DeviceID & 0xFFFF;
    SupportedDevID.Field.VendorID = mKvmDevInfo[KvmDevNum].VendorID & 0xFFFF;
    SupportedClassRevId = mKvmDevInfo[KvmDevNum].ClassRevID & 0xFF;
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmDevNum: %d, VendorDeviceID: 0x%x, RevID: 0x%x\n", KvmDevNum, SupportedDevID.VendorDeviceID,SupportedClassRevId);
    if ((SupportedDevID.VendorDeviceID == VendorDeviceID) &&(SupportedClassRevId == ClassRevID)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Supported device matched - KvmDevNum: %d\n", KvmDevNum);
      break;
    }
  }
  return KvmDevNum;
}

EFI_STATUS
AmdPspKvmRetrieveSmbiosTable (
  IN    UINT8                    KvmEnabled,
  OUT   SMBIOS_TABLE_TYPEA0    **SmbiosTableTypeA0
  )
{
  EFI_STATUS                        Status;
  UINT8                             Size;
  SMBIOS_TABLE_TYPEA0              *SmbiosTable;
  UINT32                            KvmRequest;

  Status = EFI_SUCCESS;
  Size = sizeof(SMBIOS_TABLE_TYPEA0);
  gBS->AllocatePool (EfiBootServicesData, Size, &SmbiosTable);
  gBS->SetMem (SmbiosTable, Size, 0);
  SmbiosTable->Hdr.Handle                           = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosTable->Hdr.Type                             = KVM_SMBIOS_TYPE_KVM_INFORMATION;
  SmbiosTable->Hdr.Length                           = Size;
  SmbiosTable->KvmInformation.Field.KvmEnabled      = KvmEnabled;
  SmbiosTable->KvmInformation.Field.Protocol        = KVM_SMBIOS_PROTOCOL_VNC;
  SmbiosTable->KvmInformation.Field.Connectivity    = KVM_SMBIOS_CONNECTIVITY_SHARED;
  SmbiosTable->KvmInformation.Field.ConcurrentCount = KVM_SMBIOS_CONCURRENTCOUNT_SINGLE;

  Status = mAmdPspKvmDev.KvmDevProtocol->ReadKvmRequest (mAmdPspKvmDev.KvmDevHandle, &KvmRequest);
  SmbiosTable->KvmInformation.Field.KvmRequested = (UINT8)KvmRequest;

  *SmbiosTableTypeA0 = SmbiosTable;

  return Status;
}

///Get value of KvmRequest through PCI address
EFI_STATUS
AmdPspKvmGetRequestPciAddr (
  IN     PCI_ADDR    PciAddr,
  IN     UINT32                *KvmRequest
  )
{
  EFI_STATUS Status;

  //Hardcode the value to force KvmRequest
  Status = EFI_SUCCESS;
  *KvmRequest = KVM_REQUESTED;
  return Status;
}

/**
 * @brief Get value of KvmRequest and init KvmRequest,SessionProgress to 0
 *
 * @param KvmRequest Point to the address to hold KvmRequest
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
EFI_STATUS
AmdPspKvmGetRequest (
  IN     UINT32                *KvmRequest
  )
{
  UINT32           KvmRequestVal;
  UINT32           KvmSessionProgressVal;
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetRequest >>\n");
  status = mAmdPspKvmDev.KvmDevProtocol->ReadKvmRequest (mAmdPspKvmDev.KvmDevHandle, &KvmRequestVal);
  if (KvmRequestVal == 0x1) {
    *KvmRequest = KVM_REQUESTED;
  } else {
    *KvmRequest = KVM_NOT_REQUESTED;
  }
  status = mAmdPspKvmDev.KvmDevProtocol->ReadKvmSessionProgress (mAmdPspKvmDev.KvmDevHandle, &KvmSessionProgressVal);
  if (EFI_ERROR(status)) {
    return status;
  }
  if (KvmSessionProgressVal == 0x1) {
    KvmSessionProgressVal = 0x0;
    status = mAmdPspKvmDev.KvmDevProtocol->WriteKvmSessionProgress (mAmdPspKvmDev.KvmDevHandle, &KvmSessionProgressVal);
    if (EFI_ERROR(status)) {
      return status;
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetRequest <<\n");
  return EFI_SUCCESS;
}

///Get value of KvmRequest without init
EFI_STATUS
AmdPspKvmGetRequestWithoutInit (
  IN     UINT32                *KvmRequest
  )
{
  UINT32           KvmRequestVal;
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetRequestWithoutInit >>\n");
  status = mAmdPspKvmDev.KvmDevProtocol->ReadKvmRequest (mAmdPspKvmDev.KvmDevHandle, &KvmRequestVal);
  if (EFI_ERROR(status)) {
    return status;
  }
  if (KvmRequestVal == 0x1) {
    *KvmRequest = KVM_REQUESTED;
  } else {
    *KvmRequest = KVM_NOT_REQUESTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetRequestWithoutInit <<\n");
  return EFI_SUCCESS;
}

/**
 * @brief Set value of KvmRequest
 *
 * @details SBIOS send this request to set KVM  request
 *
 * @param KvmRequest Point to the address to hold KvmRequest
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
EFI_STATUS
AmdPspKvmSetRequest (
  IN     UINT32                *KvmRequest
  )
{
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmSetRequest >>\n");
  status = mAmdPspKvmDev.KvmDevProtocol->WriteKvmRequest (mAmdPspKvmDev.KvmDevHandle, KvmRequest);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmSetRequest <<\n");
  return status;
}

/**
 * @brief Get value of KvmSessionProgress
 *
 * @param KvmSessionProgress Point to the address to hold KvmSessionProgress
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
EFI_STATUS
AmdPspKvmGetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  )
{
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetSessionProgress >>\n");
  status = mAmdPspKvmDev.KvmDevProtocol->ReadKvmSessionProgress (mAmdPspKvmDev.KvmDevHandle, KvmSessionProgress);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmGetSessionProgress <<\n");
  return status;
}

/**
 * @brief Set value of KvmSessionProgress
 *
 * @param KvmSessionProgress Point to the address to hold KvmSessionProgress
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
EFI_STATUS
AmdPspKvmSetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  )
{
  EFI_STATUS       status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmSetSessionProgress >>\n");
  status = mAmdPspKvmDev.KvmDevProtocol->WriteKvmSessionProgress (mAmdPspKvmDev.KvmDevHandle, KvmSessionProgress);
  IDS_HDT_CONSOLE_PSP_TRACE ("\tAmdPspKvmSetSessionProgress <<\n");
  return EFI_SUCCESS;
}

// Initiate Kvm params and Send Message to PSP
EFI_STATUS
AmdPspKvmInitiateKvm (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *ModeInfo,
  IN KVM_IP_INFO                       *KvmIpInfo
  )
{
  MBOX_KVM                                     KvmMboxMemBuffer;
  KVM_OS_PARAM                                 KvmOsParams;
  EFI_STATUS                                   Status;

  if (!mKvmCapableDevDetected) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&KvmOsParams, sizeof (KVM_OS_PARAM));
  KvmOsParams.NetCardMmioBaseLo = (UINT32) (mAmdPspKvmDev.NetCardMmioBase & 0xFFFFFFFF);
  KvmOsParams.NetCardMmioBaseHi = (UINT32) (mAmdPspKvmDev.NetCardMmioBase >> 32);
  KvmOsParams.NetCardMmioSize = mAmdPspKvmDev.NetCardMmioSize;
  KvmOsParams.FrameBufferBaseLo = (UINT32) (ModeInfo->FrameBufferBase & 0xFFFFFFFF);
  KvmOsParams.FrameBufferBaseHi = (UINT32) (ModeInfo->FrameBufferBase >> 32);
  KvmOsParams.KvmKbBufferAddrLo = (UINT32) ((UINT64) mKbRingBuffer & 0xFFFFFFFF);
  KvmOsParams.KvmKbBufferAddrHi = (UINT32) ((UINT64) mKbRingBuffer >> 32);
  KvmOsParams.KvmKbBufferSize = KVM_KEYBOARD_BUFFER_SIZE;
  KvmOsParams.KvmMsBufferAddrLo = (UINT32) ((UINT64) mMRingBuffer & 0xFFFFFFFF);
  KvmOsParams.KvmMsBufferAddrHi = (UINT32) ((UINT64) mMRingBuffer >> 32);
  KvmOsParams.KvmMsBufferSize = KVM_MOUSE_BUFFER_SIZE;
  KvmOsParams.KvmNicCoherencyBufferAddrLo = (UINT32) ((UINT64) mNicCoherencyBufferAddr & 0xFFFFFFFF);
  KvmOsParams.KvmNicCoherencyBufferAddrHi = (UINT32) ((UINT64) mNicCoherencyBufferAddr >> 32);
  KvmOsParams.KvmNicCoherencyBufferSize = KVM_NIC_COHERENCY_SIZE;
  KvmOsParams.KvmMouseProtocolSelection = PcdGet8(PcdAmdPspKvmMouseProtocol);
  CopyMem (&KvmOsParams.GModeInfo, ModeInfo->Info, sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  CopyMem (&KvmOsParams.KvmIpInfo, KvmIpInfo, sizeof (KVM_IP_INFO));
  KvmOsParams.FrameBufferSize = (UINT32) ((KvmOsParams.GModeInfo.HorizontalResolution * KvmOsParams.GModeInfo.VerticalResolution * 4) & 0xFFFFFFFF);
  KvmOsParams.NetCardBar0BaseLo = mAmdPspKvmDev.NetCardInfo.NetCardBar0BaseLo;
  KvmOsParams.NetCardBar0BaseHi = mAmdPspKvmDev.NetCardInfo.NetCardBar0BaseHi;
  KvmOsParams.NetCardBar2BaseLo = mAmdPspKvmDev.NetCardInfo.NetCardBar2BaseLo;
  KvmOsParams.NetCardBar2BaseHi = mAmdPspKvmDev.NetCardInfo.NetCardBar2BaseHi;
  KvmOsParams.NetCardDidVid = mAmdPspKvmDev.NetCardInfo.NetCardDidVid;
  KvmOsParams.NetCardClassRevID = mAmdPspKvmDev.NetCardInfo.NetCardClassRevID;
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmOsParams\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardMmioBaseLo=0x%x\n", KvmOsParams.NetCardMmioBaseLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardMmioBaseHi=0x%x\n", KvmOsParams.NetCardMmioBaseHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardMmioSize=0x%x\n", KvmOsParams.NetCardMmioSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("FrameBufferBaseLo=0x%x\n", KvmOsParams.FrameBufferBaseLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("FrameBufferBaseHi=0x%x\n", KvmOsParams.FrameBufferBaseHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("FrameBufferSize=0x%x\n", KvmOsParams.FrameBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbBufferAddrLo=0x%x\n", KvmOsParams.KvmKbBufferAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbBufferAddrHi=0x%x\n", KvmOsParams.KvmKbBufferAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbBufferSize=0x%x\n", KvmOsParams.KvmKbBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmMsBufferAddrLo=0x%x\n", KvmOsParams.KvmMsBufferAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmMsBufferAddrHi=0x%x\n", KvmOsParams.KvmMsBufferAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmMsBufferSize=0x%x\n", KvmOsParams.KvmMsBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherencyBufferAddrLo=0x%x\n", KvmOsParams.KvmNicCoherencyBufferAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherencyBufferAddrHi=0x%x\n", KvmOsParams.KvmNicCoherencyBufferAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherencyBufferSize=0x%x\n", KvmOsParams.KvmNicCoherencyBufferSize);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmMouseProtocolSelection=0x%x\n", KvmOsParams.KvmMouseProtocolSelection);
  IDS_HDT_CONSOLE_PSP_TRACE ("GModeInfo.HorizontalResolution=%d\n GModeInfo.VerticalResolution=%d GModeInfo.PixelFormat=0x%x\n", KvmOsParams.GModeInfo.HorizontalResolution, KvmOsParams.GModeInfo.VerticalResolution, KvmOsParams.GModeInfo.PixelFormat);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetworkType=%d\n", KvmOsParams.KvmIpInfo.NetworkType);
  IDS_HDT_CONSOLE_PSP_TRACE ("Ipv4Addr=%a\n", KvmOsParams.KvmIpInfo.Ipv4Addr);
  IDS_HDT_CONSOLE_PSP_TRACE ("Ipv4SubMask=%a\n", KvmOsParams.KvmIpInfo.Ipv4SubMask);
  IDS_HDT_CONSOLE_PSP_TRACE ("Ipv4DefGateway=%a\n", KvmOsParams.KvmIpInfo.Ipv4DefGateway);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardBar0BaseLo = %x\n", KvmOsParams.NetCardBar0BaseLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardBar0BaseHi = %x\n", KvmOsParams.NetCardBar0BaseHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardBar2BaseLo = %x\n", KvmOsParams.NetCardBar2BaseLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardBar2BaseHi = %x\n", KvmOsParams.NetCardBar2BaseHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardDidVid = %x\n", KvmOsParams.NetCardDidVid);
  IDS_HDT_CONSOLE_PSP_TRACE ("NetCardClassRevID = %x\n", KvmOsParams.NetCardClassRevID);

  KvmOsParams.DMArEnabled = PcdGetBool(PcdIvInfoDmaReMap);
  KvmOsParams.KvmDmaHpaAddrLo = (UINT32) (mKvmDmaHpaAddr & 0xFFFFFFFF);
  KvmOsParams.KvmDmaHpaAddrHi = (UINT32) (mKvmDmaHpaAddr >> 32);
  KvmOsParams.KvmNicCoherentBufferHpaAddrLo = (UINT32) (mKvmNicCoherentBufferHpaAddr & 0xFFFFFFFF);
  KvmOsParams.KvmNicCoherentBufferHpaAddrHi = (UINT32) (mKvmNicCoherentBufferHpaAddr >> 32);

  KvmOsParams.KvmDmaGpaAddrLo = (UINT32) (mKvmDmaGpaAddr & 0xFFFFFFFF);
  KvmOsParams.KvmDmaGpaAddrHi = (UINT32) (mKvmDmaGpaAddr >> 32);
  KvmOsParams.KvmNicCoherentBufferGpaAddrLo = (UINT32) (mKvmNicCoherentBufferGpaAddr & 0xFFFFFFFF);
  KvmOsParams.KvmNicCoherentBufferGpaAddrHi = (UINT32) (mKvmNicCoherentBufferGpaAddr >> 32);

  IDS_HDT_CONSOLE_PSP_TRACE ("DMArEnabled = %x\n", KvmOsParams.DMArEnabled);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmDmaHpaAddrLo = %x\n", KvmOsParams.KvmDmaHpaAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmDmaHpaAddrHi = %x\n", KvmOsParams.KvmDmaHpaAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherentBufferHpaAddrLo = %x\n", KvmOsParams.KvmNicCoherentBufferHpaAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherentBufferHpaAddrHi = %x\n", KvmOsParams.KvmNicCoherentBufferHpaAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmDmaGpaAddrLo = %x\n", KvmOsParams.KvmDmaGpaAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmDmaGpaAddrHi = %x\n", KvmOsParams.KvmDmaGpaAddrHi);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherentBufferGpaAddrLo = %x\n", KvmOsParams.KvmNicCoherentBufferGpaAddrLo);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmNicCoherentBufferGpaAddrHi = %x\n", KvmOsParams.KvmNicCoherentBufferGpaAddrHi);

  CopyMem((VOID *) mAmdPspKvmDev.KvmDmaAddr, &KvmOsParams, sizeof(KVM_OS_PARAM));
  if (!FchSpiRomReadEx ((UINT32)mAmdPspKvmDev.KvmEntryAddress,
                      (VOID *) (mAmdPspKvmDev.KvmDmaAddr + KVM_ENGINE_BINARY_OFFSET),
                      mAmdPspKvmDev.KvmEntrySize)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("KVM Engine read fail @ %x\n", mAmdPspKvmDev.KvmEntryAddress);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("KvmEntryAddress = %x\n", (UINT32)mAmdPspKvmDev.KvmEntryAddress);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmBufferAddress = %x\n", (UINT8 *)mAmdPspKvmDev.KvmDmaAddr + KVM_ENGINE_BINARY_OFFSET);
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmEntrySize = %x\n", mAmdPspKvmDev.KvmEntrySize);

  ZeroMem (&KvmMboxMemBuffer, sizeof (MBOX_KVM));
  KvmMboxMemBuffer.KvmDmaAddrLo = (UINT32) (mAmdPspKvmDev.KvmDmaAddr & 0xFFFFFFFF);
  KvmMboxMemBuffer.KvmDmaAddrHi = (UINT32) (mAmdPspKvmDev.KvmDmaAddr >> 32);
  KvmMboxMemBuffer.KvmDmaSize = KVM_DMA_SIZE;
  mKvmMouseDev.AbsoluteMode.AbsoluteMaxX = KvmOsParams.GModeInfo.HorizontalResolution;
  mKvmMouseDev.AbsoluteMode.AbsoluteMaxY = KvmOsParams.GModeInfo.VerticalResolution;

  PcdSetBoolS (PcdAmdPspKvmMbox, 1);
  Status = PspMboxBiosCmdKvmInfo (&KvmMboxMemBuffer);
  return Status;
}
/**
 * @brief Terminate KVM explicitly
 *
 * @return EFI_STATUS
 */
EFI_STATUS
AmdPspKvmTerminateKvm (
  VOID
  )
{
  UINT32 KvmSessionProgress;
  UINT32 LoopTimes;

  KvmSessionProgress = 0;
  AmdPspKvmSetSessionProgress(&KvmSessionProgress);
  mKbRingBuffer->TerminateKVMSession = 1;

  LoopTimes = 500;
  while (LoopTimes > 0) {
    if (mKbRingBuffer->TerminateKVMSession == 0) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay(10000);
    LoopTimes--;
  }
  return EFI_SUCCESS;
}

BOOLEAN
AmdPspKvmCheckSsp (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x3)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
AmdPspKvmCheckMts (
  )
{
  CPUID_DATA CpuId;
  UINT8 Family;
  UINT8 ExtFamily;
  UINT8 Model;
  UINT8 ExtModel;
  UINT8 Stepping;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  Family    = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF;   // bit 11:8
  ExtFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  Model     = (UINT8) (CpuId.EAX_Reg >> 4) & 0xF;   // bit 7:4
  ExtModel  = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF;  // bit 19:16
  Stepping  = (UINT8) (CpuId.EAX_Reg >> 0) & 0xF;   // bit 3:0

  if ((Family == 0xF) && (ExtFamily == 0x8) && (ExtModel == 0x7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

EFI_STATUS
EFIAPI
AmdPspKvmDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                            Status;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  PCI_DEVICE_INDEPENDENT_REGION         DeviceHdr;
  BOOLEAN                               KbMsDev;

  // IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspKvmDriverSupported\n");

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

  if (Status == EFI_ALREADY_STARTED) {
    // IDS_HDT_CONSOLE_PSP_TRACE ("Already Started\n");
    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    // IDS_HDT_CONSOLE_PSP_TRACE ("OpenProtocol fail %x\n", Status);
    return EFI_UNSUPPORTED;
  }

  PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint8,
                  0,
                  sizeof (DeviceHdr),
                  &DeviceHdr
                  );

  // IDS_HDT_CONSOLE_PSP_TRACE ("KVM VID %x DID %x\n", DeviceHdr.VendorId, DeviceHdr.DeviceId);

  //Check if PSP Device ID & KVM capable device detected
  if (((AMD_RV_PSP_VID == DeviceHdr.VendorId) && (AMD_RV_PSP_DID == DeviceHdr.DeviceId)) ||
      ((AMD_PR_PSP_VID == DeviceHdr.VendorId) && (AMD_PR_PSP_DID == DeviceHdr.DeviceId)) ||
      ((AMD_SSP_PSP_VID == DeviceHdr.VendorId) && (AMD_SSP_PSP_DID == DeviceHdr.DeviceId))){
    KbMsDev = TRUE;
  } else {
    KbMsDev = FALSE;
  }
  if ((KbMsDev == TRUE) &&
      (mKvmCapableDevDetected == TRUE) &&
      !mAmdPspKvmDriverStarted){
    IDS_HDT_CONSOLE_PSP_TRACE ("Dev for KVM KB/MS found.\n");
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_UNSUPPORTED;
  }

  gBS->CloseProtocol (
         Controller,
         &gEfiPciIoProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );

  return Status;
}



EFI_STATUS
EFIAPI
AmdPspKvmDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS         Status;
  AMD_PSP_KVM_DEV    *KvmDev;
  UINT32             KvmMouseProtocolSelection;

  KvmDev = &mAmdPspKvmDev;
  KvmDev->Signature = AMD_PSP_KVM_DEV_SIGNATURE;

  Status = KvmKbdControllerDriverStart(This, Controller, RemainingDevicePath);
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbdDriver Install Error!\n");
    return Status;
  }

  KvmMouseProtocolSelection = PcdGet8(PcdAmdPspKvmMouseProtocol);
  if (KvmMouseProtocolSelection == KVM_ABSOLUTE_MOUSE_PROTOCOL) {
    Status = KvmAbsoluteMouseDriverStart(This, Controller, RemainingDevicePath);
    IDS_HDT_CONSOLE_PSP_TRACE ("Install KvmAbsoluteMouseDriver\n");
  }
  else if (KvmMouseProtocolSelection == KVM_SIMPLE_MOUSE_PROTOCOL) {
    Status = KvmSimpleMouseDriverStart(This, Controller, RemainingDevicePath);
    IDS_HDT_CONSOLE_PSP_TRACE ("Install KvmSimpleMouseDriver\n");
  }
  else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unknown Mouse Protocol Selection: 0x%x\n", KvmMouseProtocolSelection);
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmMouseDriver Install Error!\n");
    return Status;
  }

  KvmDev->ControllerNameTable = NULL;
  AddUnicodeString2 (
    "eng",
    AmdPspKvmComponentName.SupportedLanguages,
    &KvmDev->ControllerNameTable,
    L"PSP KVM Virtual KB/MS Device",
    TRUE
    );

  AddUnicodeString2 (
    "en",
    AmdPspKvmComponentName2.SupportedLanguages,
    &KvmDev->ControllerNameTable,
    L"PSP KVM Virtual KB/MS Device",
    FALSE
    );

  mAmdPspKvmDriverStarted = TRUE;
  return Status;
}

EFI_STATUS
EFIAPI
AmdPspKvmDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )
{
  EFI_STATUS Status;
  UINT32     KvmMouseProtocolSelection;
  Status = KvmKbdControllerDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmKbdDriver Stop Error!\n");
    return Status;
  }
  KvmMouseProtocolSelection = PcdGet8(PcdAmdPspKvmMouseProtocol);
  if (KvmMouseProtocolSelection == KVM_ABSOLUTE_MOUSE_PROTOCOL) {
    Status = KvmAbsoluteMouseDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("Stop KvmAbsoluteMouseDriver\n");
  }
  else if (KvmMouseProtocolSelection == KVM_SIMPLE_MOUSE_PROTOCOL) {
    Status = KvmSimpleMouseDriverStop(This, Controller, NumberOfChildren, ChildHandleBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("Stop KvmSimpleMouseDriver\n");
  }
  else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unknown Mouse Protocol Selection: 0x%x\n", KvmMouseProtocolSelection);
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR(Status)){
    IDS_HDT_CONSOLE_PSP_TRACE ("KvmMouseDriver Stop Error!\n");
    return Status;
  }
  return Status;
}

EFI_STATUS
KvmDeviceDetect (
  VOID
  )
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_HANDLE           *PciIoHandles;
  UINTN                NoPciIoHandles;
  UINT32               VendorDevice;
  UINT32               ClassRevID;
  UINTN                Index;
  UINT32               KvmDevNum;
  EFI_STATUS           Status;

  PciIoHandles = NULL;
  PciIo = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("KvmPciScan\n");
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("%x PciIoHandles for Scan\n", NoPciIoHandles);

  for (Index = 0; Index < NoPciIoHandles; Index++) {
    Status = gBS->HandleProtocol (
        PciIoHandles[Index],
        &gEfiPciIoProtocolGuid,
        &PciIo
        );
    if (EFI_ERROR(Status)) continue;

    Status = PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint32,
        0,
        1,
        &VendorDevice
        );
    Status = PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint32,
        8,
        1,
        &ClassRevID
        );
    ClassRevID = ClassRevID & 0xFF;
    if (Status==EFI_SUCCESS) {
      IDS_HDT_CONSOLE_PSP_TRACE ("VendorDevice ID: %x\n", VendorDevice);
      IDS_HDT_CONSOLE_PSP_TRACE ("ClassRevID ID: %x\n", ClassRevID);
      KvmDevNum = IsSupportedDev(VendorDevice, ClassRevID);
      if (KvmDevNum > 0) {
        IDS_HDT_CONSOLE_PSP_TRACE ("KVM Dev @ VendorDeviceID:%x\n", VendorDevice);
        IDS_HDT_CONSOLE_PSP_TRACE ("KVM Dev @ ClassRevID: %x\n", ClassRevID);
        mAmdPspKvmDev.KvmDevProtocol = mKvmDevInfo[KvmDevNum].KvmDevProtocol;
        mAmdPspKvmDev.KvmDevHandle = PciIoHandles[Index];
        mAmdPspKvmDev.NetCardInfo.NetCardDidVid = VendorDevice;
        mAmdPspKvmDev.NetCardInfo.NetCardClassRevID = ClassRevID;
        mAmdPspKvmDev.KvmDevProtocol->GetNetCardBarBase(mAmdPspKvmDev.KvmDevHandle, &mAmdPspKvmDev.NetCardInfo);
        mKvmCapableDevDetected = TRUE;
        mKvmDevPciIo = PciIo;
        break;
      }
    }
  }

  if (mKvmCapableDevDetected) {
    Status = mAmdPspKvmDev.KvmDevProtocol->GetMmioBase(mAmdPspKvmDev.KvmDevHandle, &mAmdPspKvmDev.NetCardMmioBase);
    IDS_HDT_CONSOLE_PSP_TRACE ("Netcard mmioBase %lx\n", mAmdPspKvmDev.NetCardMmioBase);
    Status = mAmdPspKvmDev.KvmDevProtocol->GetMmioSize(mAmdPspKvmDev.KvmDevHandle, &mAmdPspKvmDev.NetCardMmioSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("Netcard mmioSize %x\n", mAmdPspKvmDev.NetCardMmioSize);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
InitializeKvmDriver(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS              Status;
  VOID                    *TempBuffer;
  VOID                    *Interface;
  EFI_HANDLE              Handle;
  BOOLEAN                 KvmEngineExist;
  UINT32                  KvmRequest;
  UINT32                  HSTIState;
  UINTN                   BufferSize;
  VOID                    *Mapping;
  EFI_PHYSICAL_ADDRESS    MappedAddr;

  KvmRequest = KVM_NOT_REQUESTED;
  HSTIState = 0;
  //If PSP feature turn off, exit the driver
  if ((CheckPspDevicePresentV2 () == FALSE) ||
      (PcdGetBool (PcdAmdPspEnable) == FALSE) ||
      (FeaturePcdGet (PcdAmdPspKvmEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  if (SocFamilyIdentificationCheck (F17_RN_RAW_ID) || SocFamilyIdentificationCheck (F19_CZN_RAW_ID)) {
    //Check KVM capability
    PspMboxBiosQueryHSTIState (&HSTIState);
    if ((HSTIState & PSP_RYZEN_PRO_SKU_FLAG) == 0) {
      return EFI_SUCCESS;
    }
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("InitializeKvmDriver >>\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("KVM PCI SCAN\n");
  Status = gBS->LocateProtocol (
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate PciIO protocol Fail\n");
    return Status;
  }
  //Scan PCI to find KVM Dev
  KvmDeviceDetect ();
  Status = AmdPspKvmGetRequestWithoutInit (&KvmRequest);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Read KvmRequest value Fail!\n");
    return Status;
  }
  //Search for the Entry address and size of KVM Engine binary
  KvmEngineExist = PSPEntryInfoV2(KVM_ENGINE_BINARY, &mAmdPspKvmDev.KvmEntryAddress, &mAmdPspKvmDev.KvmEntrySize);
  if (mKvmCapableDevDetected && KvmEngineExist) {
    if (KvmRequest == KVM_REQUESTED){
      IDS_HDT_CONSOLE_PSP_TRACE ("Alocate Buffer for DMA, KB, MS\n");
      //Alocate Buffer for DMA, KB, MS
      TempBuffer = AllocateAlignedReservedPages (EFI_SIZE_TO_PAGES (KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE + KVM_NIC_COHERENCY_SIZE), KVM_BUFFER_ALIGNMENT);
      ZeroMem (TempBuffer, (KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE));
      mAmdPspKvmDev.KvmDmaAddr =  (UINT64) (UINTN) (TempBuffer);
      mKbRingBuffer = (KB_RING_BUFFER *) (UINTN) (mAmdPspKvmDev.KvmDmaAddr + KVM_DMA_SIZE);
      mMRingBuffer = (KVM_MOUSE_BUFFER *) (UINTN) (mAmdPspKvmDev.KvmDmaAddr + KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE);
      mNicCoherencyBufferAddr = (UINT64) (mAmdPspKvmDev.KvmDmaAddr + KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE);

      if (PcdGetBool(PcdIvInfoDmaReMap)) {
          if (mKvmDevPciIo != NULL) {
              BufferSize = EFI_SIZE_TO_PAGES (KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE + KVM_NIC_COHERENCY_SIZE)*EFI_PAGE_SIZE;
              mKvmDevPciIo->Map(mKvmDevPciIo,
                              EfiPciIoOperationBusMasterCommonBuffer,
                              (VOID *)TempBuffer,
                              &BufferSize,
                              &MappedAddr,
                              &Mapping
                              );
              mKvmDmaHpaAddr = mAmdPspKvmDev.KvmDmaAddr;
              mKvmNicCoherentBufferHpaAddr = (UINT64) (mAmdPspKvmDev.KvmDmaAddr + KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE);
              mKvmDmaGpaAddr = (UINT64) MappedAddr;
              mKvmNicCoherentBufferGpaAddr = (UINT64) ((UINT64) MappedAddr + KVM_DMA_SIZE + KVM_KEYBOARD_BUFFER_SIZE + KVM_MOUSE_BUFFER_SIZE);
              IDS_HDT_CONSOLE_PSP_TRACE (" GPA:MappedAddr = 0x%08x%08x, BufferSize = 0x%x\n", (UINT32)(mKvmDmaGpaAddr >> 32), (UINT32)mKvmDmaGpaAddr, BufferSize);
          }
      }

      Status = EfiLibInstallDriverBindingComponentName2 (
                 ImageHandle,
                 SystemTable,
                 &gAmdPspKvmDriver,
                 ImageHandle,
                 &AmdPspKvmComponentName,
                 &AmdPspKvmComponentName2
                 );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Locate Kvm Driver Fail\n");
        return Status;
      }
    }
    //Publish KVM service
    IDS_HDT_CONSOLE_PSP_TRACE ("Install PSP KVM protocol\n");
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gAmdPspKvmServiceProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mAmdPspKvmServiceProtocol
                    );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Install PSP KVM protocol Fail\n");
      return Status;
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("KVM Dev Detect Fail\n");
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("InitializeKvmDriver <<\n");
  return Status;
}


EFI_STATUS
EFIAPI
AmdPspKvmComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mAmdPspKvmDriverNameTable,
           DriverName,
           (BOOLEAN)(This == &AmdPspKvmComponentName)
           );
}

EFI_STATUS
EFIAPI
AmdPspKvmComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  )
{
  EFI_STATUS                                  Status;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL              *ConIn;
  KVM_KEYBOARD_CONSOLE_IN_DEV                 *ConsoleIn;
  AMD_PSP_KVM_DEV                             *KvmDev;
  EFI_PCI_IO_PROTOCOL                         *PciIo;

  //
  // This is a device driver, so ChildHandle must be NULL.
  //
  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }
  //
  // Check Controller's handle
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  gAmdPspKvmDriver.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (!EFI_ERROR (Status)) {
    gBS->CloseProtocol (
           ControllerHandle,
           &gEfiPciIoProtocolGuid,
           gAmdPspKvmDriver.DriverBindingHandle,
           ControllerHandle
           );

    return EFI_UNSUPPORTED;
  }

  if (Status != EFI_ALREADY_STARTED) {
    return EFI_UNSUPPORTED;
  }
  //
  // Get the device context
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSimpleTextInProtocolGuid,
                  (VOID **) &ConIn,
                  gAmdPspKvmDriver.DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ConsoleIn = KVM_KEYBOARD_CONSOLE_IN_DEV_FROM_THIS (ConIn);
  KvmDev = AMD_PSP_KVM_DEV_FROM_THIS(ConsoleIn);

  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           KvmDev->ControllerNameTable,
           ControllerName,
           (BOOLEAN)(This == &AmdPspKvmComponentName)
           );
}

