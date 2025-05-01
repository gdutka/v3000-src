/** @file

Cxl PCIE helper library.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

#ifndef __AMD_CXL_PCIE_LIB_H__
#define __AMD_CXL_PCIE_LIB_H__

#include <Protocol/PciIo.h>
#include <Register/AmdCxlPcieReg.h>

EFI_STATUS
EFIAPI
CxlPcieGetExtCapabilityHeader (
  IN  EFI_PCI_IO_PROTOCOL       *PciIo,
  IN  UINT16                    Offset,
  OUT PCIE_EXTENDED_CAP_HEADER  *ExtCapHdr
);

EFI_STATUS
EFIAPI
CxlPcieLocateCxlDvsecCapability (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  OUT UINT16               *CxlDvsecOffset,
  OUT UINT16               *CxlDvsecVendorId
);

#endif
