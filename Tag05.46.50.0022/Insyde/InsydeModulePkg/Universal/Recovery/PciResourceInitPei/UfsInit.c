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
#include "UfsInit.h"

PEI_UFS_CONTROLLER_PPI mUfsControllerPpi = {
  GetUfsController 
};

EFI_PEI_PPI_DESCRIPTOR mUfsPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiUfsControllerPpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mUfsNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  UfsEndOfPeiPpiNotifyCallback
};

/**
 Retrieve UFS controller information

 @param [in]   PeiServices              Pointer to the PEI Services Table.
 @param [in]   This                     Pointer to PEI_UFS_CONTROLLER_PPI
 @param [in]   UfsControllerId          UFS Controller ID
 @param [out]  ControllerType           Result UFS controller type
 @param [out]  BaseAddress              Result UFS base address

 @retval EFI_INVALID_PARAMETER          Invalid parameter input
 @retval EFI_SUCCESS                    UFS controller information retrieved successfully

**/
EFI_STATUS
GetUfsController (
  IN     EFI_PEI_SERVICES               **PeiServices,
  IN     PEI_UFS_CONTROLLER_PPI         *This,
  IN     UINT8                          UfsControllerId,
     OUT UINT32                         *ControllerType,
     OUT EFI_PHYSICAL_ADDRESS           *BaseAddress
  )
{
  PEI_UFS_DEVICE                     *PeiUfsDev;

  PeiUfsDev = PEI_UFS_DEVICE_FROM_THIS (This);

  if (UfsControllerId >= PeiUfsDev->TotalControllers) {
    return EFI_INVALID_PARAMETER;
  }
  
  *ControllerType = PEI_UFS_CONTROLLER;
  *BaseAddress  = PeiUfsDev->BaseAddress;

  return EFI_SUCCESS;
}

/**
  Register notify ppi to reset the UFS controller.
 
  @param[in]   PeiServices               Pointer to the PEI Services Table
  @param[in]   NotifyDescriptor          Pointer to the notify descriptor

  @retval EFI_SUCCESS                    Reset UFS controller successfully

**/
EFI_STATUS
EFIAPI
UfsEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  PEI_UFS_DEVICE              *PeiUfsDev;

  PeiUfsDev = PEI_UFS_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);

  //
  // Disable UFS
  //
  PciAnd16 (
    PeiUfsDev->PciAddress | PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  return EFI_SUCCESS;
}

/**
  Init UFS controller.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BaseAddress              MMIO base address
 
  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
InitUfsController (
  IN CONST EFI_PEI_SERVICES        **PeiServices,
  IN       PCI_RESOURCE_DATA       *PciRes
  )
{
  EFI_STATUS              Status;
  PEI_UFS_DEVICE          *PeiUfsDev;
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

  if ((SubClass != 0x09) || (BaseClass != PCI_CLASS_MEMORY_CONTROLLER)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Program BAR and enable UFS controller
  //
  Status = ProgramBar (PciRes);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiUfsDev = (PEI_UFS_DEVICE *)AllocateZeroPool (sizeof (PEI_UFS_DEVICE));
  ASSERT (PeiUfsDev != NULL);
  if (PeiUfsDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PeiUfsDev->Signature            = PEI_UFS_SIGNATURE;
  PeiUfsDev->ControllerPpi        = mUfsControllerPpi;
  PeiUfsDev->PpiList              = mUfsPpiList;
  PeiUfsDev->PpiList.Ppi          = &PeiUfsDev->ControllerPpi;
  PeiUfsDev->NotifyList           = mUfsNotifyList;
  PeiUfsDev->TotalControllers     = 1;
  PeiUfsDev->BaseAddress          = PciRes->PciBar[0];
  PeiUfsDev->PciAddress           = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

  //
  // Install notification in order to reset the UFS
  //
  Status = PeiServicesNotifyPpi (&PeiUfsDev->NotifyList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Install UFS Controller PPI
  //
  Status = PeiServicesInstallPpi (&PeiUfsDev->PpiList);
  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "InstallPpi :%r\n",Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "End InitUfsController !\n"));
  return EFI_SUCCESS;
}

