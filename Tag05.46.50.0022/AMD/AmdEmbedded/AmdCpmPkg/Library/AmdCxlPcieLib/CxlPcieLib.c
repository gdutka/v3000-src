/** @file

Cxl PCIE helper library.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

#include <IndustryStandard/Pci.h>

#include <Protocol/PciIo.h>

#include <Register/AmdCxlPcieReg.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/AmdCxlPcieLib.h>


STATIC
BOOLEAN
PciCapabilitySupport (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  UINT8  PciStatus;

  PciStatus = 0;
  PciIo->Pci.Read (
           PciIo,
           EfiPciIoWidthUint8,
           PCI_PRIMARY_STATUS_OFFSET,
           1,
           &PciStatus
           );

  return (PciStatus & EFI_PCI_STATUS_CAPABILITY) ? TRUE : FALSE;
}

STATIC
EFI_STATUS
GetCapabilityHeader (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8                Offset,
  OUT PCI_CAP_HEADER       *CapHdr
)
{
  EFI_STATUS  Status;

  // Get Capability Header
  CapHdr->AsUint16 = 0;
  Status = PciIo->Pci.Read (
    PciIo,
    EfiPciIoWidthUint16,
    Offset,
    sizeof (PCI_CAP_HEADER) / sizeof (UINT16),
    (VOID *)CapHdr
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  CapHdr->AsBits.NextCapPtr &= (UINT16)(UINT8)~0x03;

  // Certain PCI devices may incorrectly have capability pointing to itself,
  // so treat that as the end of the list.
  if (Offset == (UINT8)CapHdr->AsBits.NextCapPtr) {
    CapHdr->AsBits.NextCapPtr = 0;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
LocatePciCapabilityOffset (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8                CapId,
  OUT UINT8                *Offset,
  OUT UINT8                *NextCapPtr OPTIONAL
  )
{
  PCI_CAP_HEADER  CapHeader;
  UINT8           CapPtr;
  EFI_STATUS      Status;

  //
  // PCI Capability supported?
  //
  if (!PciCapabilitySupport (PciIo)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Get starting offset
  //
  if (*Offset >= 0x40) {
    CapPtr = *Offset;
  }
  else {
    CapPtr = 0;
    Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint8,
      PCI_CAPBILITY_POINTER_OFFSET,
      1,
      &CapPtr
      );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  }
  CapPtr &= (UINT8)~0x03;

  while (CapPtr >= 0x40) {
    //
    // Get Capability header
    //
    Status = GetCapabilityHeader (PciIo, CapPtr, &CapHeader);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    //
    // Found matching ID?
    //
    if (CapId == (UINT8)CapHeader.AsBits.CapId) {
      *Offset = CapPtr;
      if (NextCapPtr != NULL) {
        *NextCapPtr = (UINT8)CapHeader.AsBits.NextCapPtr;
      }
      return EFI_SUCCESS;
    }

    //
    // Let's try the next capability
    //
    CapPtr = (UINT8)CapHeader.AsBits.NextCapPtr;
  }

  return EFI_NOT_FOUND;
}

STATIC
BOOLEAN
PciExpressCapabilitySupport (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
  UINT8  Offset;

  //
  // Detect if PCI Express Device
  //
  Offset = 0;
  LocatePciCapabilityOffset (
     PciIo,
     EFI_PCI_CAPABILITY_ID_PCIEXP,
     &Offset,
     NULL
     );

  return (Offset != 0) ? TRUE : FALSE;
}

EFI_STATUS
EFIAPI
CxlPcieGetExtCapabilityHeader (
  IN  EFI_PCI_IO_PROTOCOL       *PciIo,
  IN  UINT16                    Offset,
  OUT PCIE_EXTENDED_CAP_HEADER  *ExtCapHdr
  )
{
  EFI_STATUS  Status;

  //
  // Get ExtCapability Header
  //
  ExtCapHdr->AsUint32 = 0;
  Status = PciIo->Pci.Read (
    PciIo,
    EfiPciIoWidthUint32,
    Offset,
    sizeof (PCIE_EXTENDED_CAP_HEADER) / sizeof (UINT32),
    (VOID *)ExtCapHdr
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ExtCapHdr->AsBits.NextCapPtr &= (UINT32)(UINT16)~0x03;

  //
  // Certain PCIe devices may incorrectly have capability pointing to itself,
  // so treat that as the end of the list.
  //
  if (Offset == (UINT16)ExtCapHdr->AsBits.NextCapPtr) {
    ExtCapHdr->AsBits.NextCapPtr = 0;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
LocatePciExpressCapabilityOffset (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT16                CapId,
  OUT UINT16               *Offset,
  OUT UINT16               *NextCapPtr OPTIONAL
  )
{
  PCIE_EXTENDED_CAP_HEADER  CapHeader;
  UINT16                    CapPtr;
  EFI_STATUS                Status;

  //
  // Get starting offset
  //
  if (*Offset >= EFI_PCIE_CAPABILITY_BASE_OFFSET) {
    CapPtr = *Offset & (UINT16)~0x03;
  }
  else {
    CapPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;
  }

  do {
    //
    // Get ExtCapability header
    //
    Status = CxlPcieGetExtCapabilityHeader (PciIo, CapPtr, &CapHeader);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    //
    // Found matching ID?
    //
    if (CapId == (UINT16)CapHeader.AsBits.CapId) {
      *Offset = CapPtr;
      if (NextCapPtr != NULL) {
        *NextCapPtr = (UINT16)CapHeader.AsBits.NextCapPtr;
      }
      DEBUG ((DEBUG_INFO, "%a: Offset = 0x%03X, ExtCapId = 0x%04X, NextCapPtr = 0x%03X\n",
        __FUNCTION__, CapPtr, CapHeader.AsBits.CapId, CapHeader.AsBits.NextCapPtr));
      return EFI_SUCCESS;
    }

    //
    // Let's try the next capability
    //
    CapPtr = (UINT16)CapHeader.AsBits.NextCapPtr;
  } while (CapPtr != 0);

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
CxlPcieLocateCxlDvsecCapability (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  OUT UINT16               *CxlDvsecOffset,
  OUT UINT16               *CxlDvsecVendorId
  )
{
  PCIE_DVSEC_HEADER1  PcieHdr1;
  PCIE_DVSEC_HEADER2  PcieHdr2;
  UINT16              Offset;
  UINT16              NextCapPtr;
  UINTN               PciSegment;
  UINTN               PciBus;
  UINTN               PciDevice;
  UINTN               PciFunction;
  EFI_STATUS          Status;

  *CxlDvsecVendorId = INVALID_VENDOR_ID;

  //
  // PCIe Capability supported?
  //
  if (!PciExpressCapabilitySupport (PciIo)) {
    return EFI_UNSUPPORTED;
  }

  Offset = EFI_PCIE_CAPABILITY_BASE_OFFSET;
  do {
    //
    // Locate DVSEC Extended Capability
    //
    NextCapPtr = 0;
    Status = LocatePciExpressCapabilityOffset (
      PciIo,
      DVSEC_EXTENDED_CAP_ID,
      &Offset,
      &NextCapPtr
      );
    if (EFI_ERROR (Status)) {
      goto ON_EXIT;
    }

    // Get PCI Location
    Status = PciIo->GetLocation (
      PciIo,
      &PciSegment,
      &PciBus,
      &PciDevice,
      &PciFunction
      );
    if (EFI_ERROR (Status)) {
      goto ON_EXIT;
    }
    DEBUG ((DEBUG_INFO, "%a: Segment = %d, Bus = 0x%02X, Device = 0x%02X, Function = 0x%X\n",
      __FUNCTION__, PciSegment, PciBus, PciDevice, PciFunction));
    DEBUG ((DEBUG_INFO, "DVSEC ExtCap: Offset = 0x%03X\n", Offset));

    //
    // Get DVSEC Header1
    //
    Status = PciIo->Pci.Read (
      PciIo,
      EfiPciIoWidthUint32,
      Offset + DVSEC_HEADER1_OFFSET,
      sizeof (PCIE_DVSEC_HEADER1) / sizeof (UINT32),
      (VOID *)&PcieHdr1
      );
    if (EFI_ERROR (Status)) {
      goto ON_EXIT;
    }
    DEBUG ((DEBUG_INFO, " DVSEC: VendorId = 0x%04X, Revision = %d, Length = 0x%03X\n",
      PcieHdr1.AsBits.VendorId, PcieHdr1.AsBits.Revision, PcieHdr1.AsBits.Length));

    //
    // Found matching DVSEC VendorId?
    //
    if ((CXL_1_0_VENDOR_ID == (UINT16)PcieHdr1.AsBits.VendorId) || (CXL_VENDOR_ID == (UINT16)PcieHdr1.AsBits.VendorId)) {
      //
      // Get DVSEC Header2
      //
      *CxlDvsecVendorId = (UINT16)PcieHdr1.AsBits.VendorId;
      *CxlDvsecOffset = Offset;
      Status = PciIo->Pci.Read (
        PciIo,
        EfiPciIoWidthUint32,
        Offset + DVSEC_HEADER2_OFFSET,
        sizeof (PCIE_DVSEC_HEADER2) / sizeof (UINT32),
        (VOID *)&PcieHdr2
        );
      if (EFI_ERROR (Status)) {
        goto ON_EXIT;
      }
      DEBUG ((DEBUG_INFO, " CXL Component ID = 0x%04X\n", PcieHdr2.AsBits.DvsecId));
      break;
    }

    Offset = NextCapPtr;
  } while (NextCapPtr);

ON_EXIT:
  if (*CxlDvsecVendorId == INVALID_VENDOR_ID) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
