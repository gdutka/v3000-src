/** @file

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "NvmeInit.h"

PEI_NVME_CONTROLLER_PPI mNvmeControllerPpi = {
  GetNvmeController
};

EFI_PEI_PPI_DESCRIPTOR mNvmePpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiNvmeControllerPpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mNvmeNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  NvmeEndOfPeiPpiNotifyCallback
};

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
GetNvmeController (
  IN     EFI_PEI_SERVICES              **PeiServices,
  IN     PEI_NVME_CONTROLLER_PPI       *This,
  IN     UINT8                         NvmeControllerId,
     OUT EFI_PHYSICAL_ADDRESS          *BaseAddress
  )
{
  PEI_NVME_DEVICE         *PeiNvmeDev;

  PeiNvmeDev = PEI_NVME_DEVICE_FROM_THIS (This);

  if (NvmeControllerId >= PeiNvmeDev->TotalControllers) {
    return EFI_INVALID_PARAMETER;
  }
  *BaseAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)PeiNvmeDev->BaseAddress;
  return EFI_SUCCESS;
}

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
  )
{
  PEI_NVME_DEVICE             *PeiNvmeDev;
  
  PeiNvmeDev = PEI_NVME_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);

  //
  // Disable CC_ENABLE
  //
  MmioAnd32 (PeiNvmeDev->BaseAddress + NVME_CC_OFFSET, (UINT32)(~BIT0));

  MicroSecondDelay (500 * 1000);

  //
  // Disable NVME
  //
  PciAnd16 (
    PeiNvmeDev->PciAddress | PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  return EFI_SUCCESS;
}


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
  IN CONST EFI_PEI_SERVICES        **PeiServices,
  IN       PCI_RESOURCE_DATA       *PciRes
  )
{
  EFI_STATUS                   Status;
  PEI_NVME_DEVICE              *PeiNvmeDev;
  UINT8                        SubClass;
  UINT8                        BaseClass;
  UINT32                       Bus;
  UINT32                       Device;
  UINT32                       Function;

  Bus      = PciRes->Bus;
  Device   = PciRes->Device;
  Function = PciRes->Function;

  SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1));
  BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2));

  if ((SubClass != PCI_CLASS_MASS_STORAGE_SOLID_STATE) || (BaseClass != PCI_CLASS_MASS_STORAGE)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Program BAR and enable NVME controller
  //
  Status = ProgramBar (PciRes);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiNvmeDev = (PEI_NVME_DEVICE *)AllocateZeroPool (sizeof (PEI_NVME_DEVICE));
  ASSERT (PeiNvmeDev != NULL);
  if (PeiNvmeDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PeiNvmeDev->Signature            = PEI_NVME_SIGNATURE;
  PeiNvmeDev->ControllerPpi        = mNvmeControllerPpi;
  PeiNvmeDev->PpiList              = mNvmePpiList;
  PeiNvmeDev->PpiList.Ppi          = &PeiNvmeDev->ControllerPpi;
  PeiNvmeDev->NotifyList           = mNvmeNotifyList;
  PeiNvmeDev->TotalControllers     = 1;
  PeiNvmeDev->BaseAddress          = PciRes->PciBar[0];
  PeiNvmeDev->PciAddress           = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

  //
  // Install notification in order to reset the NVME
  //
  Status = PeiServicesNotifyPpi (&PeiNvmeDev->NotifyList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Install NVME Controller PPI
  //
  Status = PeiServicesInstallPpi (&PeiNvmeDev->PpiList);
  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "InstallPpi :%r\n",Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "End InitNvmeController !\n"));
  return EFI_SUCCESS;
}

