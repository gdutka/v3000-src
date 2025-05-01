/** @file

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "AhciInit.h"

PEI_AHCI_CONTROLLER_PPI mAhciControllerPpi = {
  GetAhciMemBase 
};

EFI_PEI_PPI_DESCRIPTOR mAhciPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiAhciControllerPpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mAhciNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  AhciEndOfPeiPpiNotifyCallback
};

/**
 Get memory mapped base address of AHCI

 @param [in]   PeiServices              Pointer to the PEI Services Table.
 @param [in]   This                     Pointer to PEI_AHCI_CONTROLLER_PPI
 @param [in]   AhciControllerId         AHCI controller ID
 @param [out]  BaseAddress              The result AHCI memory base address

 @retval EFI_INVALID_PARAMETER          Invalid AhciControllerId is given
 @retval EFI_SUCCESS                    ACHI memory base address is successfully retrieved

**/
EFI_STATUS
GetAhciMemBase (
  IN     EFI_PEI_SERVICES           **PeiServices,
  IN     PEI_AHCI_CONTROLLER_PPI    *This,
  IN     UINT8                      AhciControllerId,
     OUT EFI_PHYSICAL_ADDRESS       *BaseAddress
  )
{
  PEI_AHCI_DEVICE         *PeiAhciDev;

  PeiAhciDev = PEI_AHCI_DEVICE_FROM_THIS (This);

  if (AhciControllerId >= PeiAhciDev->TotalControllers) {
    return EFI_INVALID_PARAMETER;
  }

  *BaseAddress = PeiAhciDev->BaseAddress;

  return EFI_SUCCESS;
}

/**
 Register notify ppi to reset the AHCI.


 @param[in]   PeiServices               Pointer to the PEI Services Table.
 @param[in]   NotifyDescriptor          Pointer to the notify descriptor

 @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
AhciEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  PEI_AHCI_DEVICE             *PeiAhciDev;

  PeiAhciDev = PEI_AHCI_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);

  //
  // Clear ABAR
  //
  PciWrite32 (PeiAhciDev->PciAddress | AHCI_ABAR_OFFSET, 0);

  //
  // Disable AHCI
  //
  PciAnd16 (
    PeiAhciDev->PciAddress | PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  return EFI_SUCCESS;
}

/**
  Init AHCI controller.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BaseAddress              MMIO base address
 
  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
InitAhciController (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN       PCI_RESOURCE_DATA        *PciRes
  )
{
  EFI_STATUS              Status;
  PEI_AHCI_DEVICE         *PeiAhciDev;
  UINT8                   SubClass;
  UINT8                   BaseClass;
  UINT32                  Bus;
  UINT32                  Device;
  UINT32                  Function;

  Bus      = PciRes->Bus;
  Device   = PciRes->Device;
  Function = PciRes->Function;

  SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1));
  BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2));

  if ((SubClass != PCI_CLASS_MASS_STORAGE_SATADPA) || (BaseClass != PCI_CLASS_MASS_STORAGE)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Program BARs and enable AHCI controller
  //
  Status = ProgramBar (PciRes);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiAhciDev = (PEI_AHCI_DEVICE *)AllocateZeroPool (sizeof (PEI_AHCI_DEVICE));
  ASSERT (PeiAhciDev != NULL);
  if (PeiAhciDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PeiAhciDev->Signature            = PEI_AHCI_SIGNATURE;
  PeiAhciDev->ControllerPpi        = mAhciControllerPpi;
  PeiAhciDev->PpiList              = mAhciPpiList;
  PeiAhciDev->PpiList.Ppi          = &PeiAhciDev->ControllerPpi;
  PeiAhciDev->NotifyList           = mAhciNotifyList;
  PeiAhciDev->TotalControllers     = 1;
  PeiAhciDev->BaseAddress          = PciRes->PciBar[5]; // ABAR
  PeiAhciDev->PciAddress           = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

  //
  // Install notification in order to reset the AHCI
  //
  Status = PeiServicesNotifyPpi (&PeiAhciDev->NotifyList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Install AHCI Controller PPI
  //
  Status = PeiServicesInstallPpi (&PeiAhciDev->PpiList);
  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "InstallPpi :%r\n",Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "End InitAhciController !\n"));
  return EFI_SUCCESS;
}
