/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _NVME_INIT_H
#define _NVME_INIT_H

#include "PciResourceInitPei.h"

#include <IndustryStandard/Nvme.h>
#include <Ppi/NvmeController.h>
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>

#define PEI_NVME_SIGNATURE              SIGNATURE_32 ('N', 'V', 'M', 'C')

typedef struct _PEI_NVME_DEVICE {
  UINT32                        Signature;
  UINT32                        TotalControllers;
  UINT32                        BaseAddress;
  UINT32                        PciAddress;
  EFI_PEI_PPI_DESCRIPTOR        PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR     NotifyList;
  PEI_NVME_CONTROLLER_PPI       ControllerPpi;
} PEI_NVME_DEVICE;

#define PEI_NVME_DEVICE_FROM_THIS(a)        CR(a, PEI_NVME_DEVICE, ControllerPpi, PEI_NVME_SIGNATURE)
#define PEI_NVME_DEVICE_FROM_NOTIFY_DESC(a) CR(a, PEI_NVME_DEVICE, NotifyList, PEI_NVME_SIGNATURE)

/**
  Retrieve NVMe controller information.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   This                     Pointer to PEI_NVME_CONTROLLER_PPI
  @param [in]   NvmeControllerId         NVMe Controller ID
  @param [out]  BaseAddress              Result NVMe base address
 
  @retval EFI_SUCCESS                    NVMe controller information is retrieved successfully
  @retval EFI_INVALID_PARAMETER          Invalid NvmeControllerId is given
  @retval Others                         Operation failed

**/
EFI_STATUS
EFIAPI
GetNvmeController (
  IN     EFI_PEI_SERVICES              **PeiServices,
  IN     PEI_NVME_CONTROLLER_PPI       *This,
  IN     UINT8                         NvmeControllerId,
     OUT EFI_PHYSICAL_ADDRESS          *BaseAddress
  );

/**
  Register notify ppi to reset the NVMe.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
NvmeEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );

/**
  Init NVME controller.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BaseAddress              MMIO base address
 
  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
InitNvmeController (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

#endif
