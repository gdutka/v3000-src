/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/MpmFuncLib.h>
#include <Library/MpmDevLib.h>
#include <Protocol/PciIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciLib.h>

#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>

#define FILECODE LIBRARY_MPMDEVLIB_MPMDEVINTLIB_FILECODE

MPM_DEV_PROTOCOL * mMpmDevProtocols [] = {
  &mRtk8168F_Protocol,
  &mBrcm5762Protocol,
  &mRtk8168E_Protocol,
  &mAquantiaProtocol,
  &mAquantiaProtocol,
  &mQcaHastingsProtocol,
  &mMediatekProtocol
};


/**
 * @brief Get the Pci Bar Base value
 *
 * @param PciHandle PciIoProtocol handle
 * @param Offset    Offset of BAR register in PCI CFG space
 * @param PciBarBase point to PCI BAR value
 * @return EFI_STATUS
 */
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
  if ((BarLo & BIT0) != 0) {
    return EFI_INVALID_PARAMETER;
  }
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

/**
 * @brief Get the Nic Command PCI register
 *
 * @param KvmDevHandle
 * @param Command
 * @return EFI_STATUS
 */
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

/**
 * @brief Set the Nic Command PCI register
 *
 * @param KvmDevHandle
 * @param Command
 * @return EFI_STATUS
 */
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

UINT32
GetMpmDevProtocolNum (
  )
{
  return sizeof (mMpmDevProtocols ) / sizeof (MPM_DEV_PROTOCOL *);
}
