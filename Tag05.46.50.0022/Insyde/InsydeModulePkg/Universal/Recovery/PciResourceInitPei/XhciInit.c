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
#include "XhciInit.h"

PEI_USB_CONTROLLER_PPI mUsbControllerPpi = {
  GetXhciController 
};

EFI_PEI_PPI_DESCRIPTOR mXhciPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiUsbControllerPpiGuid,
  NULL
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mXhciNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  XhciEndOfPeiPpiNotifyCallback
};

EFI_STATUS
EnableXhciController (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN PEI_XHCI_DEVICE          *PeiPchXhciDev,
  IN UINT8                    UsbControllerId
  );

/**
 Decide whether to install usb stack.

 @param None.

 @retval TRUE         Currently support USB on PEI phase.
 @retval FALSE        Currently not support USB on PEI phase.

**/
BOOLEAN
CheckUsbPolicy (
  VOID
  )
{
  PEI_STORAGE_HOB_DATA              *StorageData;
  EFI_PEI_HOB_POINTERS              GuidHob;

  GuidHob.Raw = GetHobList ();
  while ((GuidHob.Raw = GetNextGuidHob ( &gH2OPeiStorageHobGuid, GuidHob.Raw)) != NULL) {
    StorageData = (PEI_STORAGE_HOB_DATA *)GET_GUID_HOB_DATA (GuidHob.Guid);
    if (StorageData->Bits.UsbEnable == 1) {
      DEBUG ((DEBUG_INFO, "[%a]Support USB stack on PEI phase\n", __FUNCTION__));
      return TRUE;
    }
    GuidHob.Raw = GET_NEXT_HOB (GuidHob);
  };

  DEBUG ((DEBUG_INFO, "[%a]Can't support USB stack on PEI phase\n", __FUNCTION__));
  return FALSE;
}

/**
 Retrieve XHCI controller information

 @param [in]   PeiServices              Pointer to the PEI Services Table.
 @param [in]   This                     Pointer to PEI_AHCI_CONTROLLER_PPI
 @param [in]   UsbControllerId          USB Controller ID
 @param [out]   ControllerType          Result USB controller type
 @param [out]   BaseAddress             Result XHCI base address

 @retval EFI_INVALID_PARAMETER          Invalid AhciControllerId is given
 @retval EFI_SUCCESS                    XHCI controller information is retrieved successfully

**/
EFI_STATUS
GetXhciController (
  IN     EFI_PEI_SERVICES               **PeiServices,
  IN     PEI_USB_CONTROLLER_PPI         *This,
  IN     UINT8                          UsbControllerId,
     OUT UINTN                          *ControllerType,
     OUT UINTN                          *BaseAddress
  )
{
  PEI_XHCI_DEVICE         *PeiXhciDev;

  PeiXhciDev = PEI_XHCI_DEVICE_FROM_THIS (This);

  if (UsbControllerId >= PeiXhciDev->TotalControllers) {
    return EFI_INVALID_PARAMETER;
  }

  *ControllerType = PEI_XHCI_CONTROLLER;
  *BaseAddress = PeiXhciDev->BaseAddress;

  return EFI_SUCCESS;
}

/**
 Register notify ppi to reset the XHCI.


 @param[in]   PeiServices               Pointer to the PEI Services Table.
 @param[in]   NotifyDescriptor          Pointer to the notify descriptor

 @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
XhciEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  )
{
  PEI_XHCI_DEVICE             *PeiXhciDev;
  UINT8                       CapLength;
  UINT32                      OpRegister;

  PeiXhciDev = PEI_XHCI_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);

  CapLength = MmioRead8 (PeiXhciDev->BaseAddress + 0);
  OpRegister = PeiXhciDev->BaseAddress + CapLength;

  //
  // Halt HC
  //
  MmioAnd32 (OpRegister, (UINT32)(~BIT0));

  MicroSecondDelay (10 * 1000);

  //
  // HC Reset
  //
  MmioOr32 (OpRegister, BIT1);

  //
  // Disable XHCI
  //
  PciAnd16 (
    PeiXhciDev->PciAddress | PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  return EFI_SUCCESS;
}

/**
  Init XHCI controller.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BaseAddress              MMIO base address
 
  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
InitXhciController (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN       PCI_RESOURCE_DATA        *PciRes
  )
{
  EFI_STATUS                Status;
  PEI_XHCI_DEVICE           *PeiXhciDev;
  UINT8                     ProgIf;
  UINT8                     SubClass;
  UINT8                     BaseClass;
  UINT32                    Bus;
  UINT32                    Device;
  UINT32                    Function;

  Bus      = PciRes->Bus;
  Device   = PciRes->Device;
  Function = PciRes->Function;

  ProgIf    = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET));
  SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1));
  BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2));

  if ((ProgIf != PCI_IF_XHCI) || (SubClass != PCI_CLASS_SERIAL_USB) || (BaseClass != PCI_CLASS_SERIAL)) {
    return EFI_UNSUPPORTED;
  }

  if (!CheckUsbPolicy ()) {
    return EFI_UNSUPPORTED;
  }

  //
  // Program BAR and enable XHCI controller
  //
  Status = ProgramBar (PciRes);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PeiXhciDev = (PEI_XHCI_DEVICE *)AllocateZeroPool (sizeof (PEI_XHCI_DEVICE));
  ASSERT (PeiXhciDev != NULL);
  if (PeiXhciDev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PeiXhciDev->Signature            = PEI_XHCI_SIGNATURE;
  PeiXhciDev->ControllerPpi        = mUsbControllerPpi;
  PeiXhciDev->PpiList              = mXhciPpiList;
  PeiXhciDev->PpiList.Ppi          = &PeiXhciDev->ControllerPpi;
  PeiXhciDev->NotifyList           = mXhciNotifyList;
  PeiXhciDev->TotalControllers     = 1;
  PeiXhciDev->BaseAddress          = PciRes->PciBar[0];
  PeiXhciDev->PciAddress           = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

  //
  // Set Access Control (ACCTRL) to locked state
  //
  PciOr32 (PeiXhciDev->PciAddress | XHCI_XHCC1_OFFSET, XHCI_ACCTRL_REGISTER);

  //
  // Install notification in order to reset XHCI
  //
  Status = PeiServicesNotifyPpi (&PeiXhciDev->NotifyList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Install USB Controller PPI
  //
  Status = PeiServicesInstallPpi (&PeiXhciDev->PpiList);
  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "InstallPpi :%r\n",Status));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "End InitXhciController !\n"));
  return EFI_SUCCESS;
}
