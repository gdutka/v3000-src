/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef RAS_PCI_IO_PROT_EARLY_PROTOCOL_H_
#define RAS_PCI_IO_PROT_EARLY_PROTOCOL_H_

#include <AmdCpmRas.h>

extern EFI_GUID gRasPciIoProtEarlyProtocolGuid;

typedef
EFI_STATUS
(EFIAPI *BLD_PCIE_MAP_AND_BOOT_ERR_DET)(
    VOID
  );

typedef struct _RAS_PCI_IO_PROT_EARLY_PROTOCOL {
  BLD_PCIE_MAP_AND_BOOT_ERR_DET    BldPcieMapAndBootErrDet;
} RAS_PCI_IO_PROT_EARLY_PROTOCOL;

#endif // RAS_PCI_IO_PROT_EARLY_PROTOCOL_H_
