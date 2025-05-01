/** @file

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "SdInit.h"

PEI_SD_CONTROLLER_PPI mSdControllerPpi = {
  GetSdhcController
};

EFI_PEI_PPI_DESCRIPTOR mSdhcPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiSdControllerPpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mSdhcNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  SdhcEndOfPeiPpiNotifyCallback
};

/**
  Retrieve SDHC controller information.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   This                     Pointer to PEI_SD_CONTROLLER_PPI
  @param [in]   SdControllerId           SDHC Controller ID
  @param [out]  ControllerType           Result SDHC controller type
  @param [out]  PciCfgAddress            Result SDHC base address
 
  @retval EFI_SUCCESS                    SDHC controller information is retrieved successfully
  @retval EFI_INVALID_PARAMETER          Invalid SdControllerId is given
  @retval Others                         Operation failed

**/
EFI_STATUS
EFIAPI
GetSdhcController (
  IN     EFI_PEI_SERVICES           **PeiServices,
  IN     PEI_SD_CONTROLLER_PPI      *This,
  IN     UINT8                      SdControllerId,
     OUT UINTN                      *ControllerType,
     OUT UINT64                     *PciCfgAddress
  )
{
  PEI_SDHC_DEVICE         *PeiSdhcDev;

  PeiSdhcDev = PEI_SDHC_DEVICE_FROM_THIS (This);

  if (SdControllerId >= PeiSdhcDev->TotalControllers) {
    return EFI_INVALID_PARAMETER;
  }

  *ControllerType = PEI_SDHC_CONTROLLER;
  //
  // Convert PciLibAddress to PciCfgAddress
  //
  *PciCfgAddress = PCILIB_ADDRESS_TO_PCICFG_ADDRESS (PeiSdhcDev->PciAddress);

  return EFI_SUCCESS;
}

/**
  Register notify ppi to reset the SDHC.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
SdhcEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  PEI_SDHC_DEVICE             *PeiSdhcDev;

  PeiSdhcDev = PEI_SDHC_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);

  //
  // Reset the host
  //
  MmioOr8 (PeiSdhcDev->BaseAddress + SOFT_RESET_REGISTER, BIT0);

  MicroSecondDelay (1);

  //
  // Disable SDHC
  //
  PciAnd16 (
    PeiSdhcDev->PciAddress | PCI_COMMAND_OFFSET,
    (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE)
    );

  return EFI_SUCCESS;
}

/**
  Init SD controller.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BaseAddress              MMIO base address
 
  @retval EFI_SUCCESS         Operation completed successfully.
  @retval Others              Operation was unsuccessful.

**/
EFI_STATUS
InitSdController (
  IN CONST EFI_PEI_SERVICES        **PeiServices,
  IN       PCI_RESOURCE_DATA       *PciRes
  )
{
  EFI_STATUS                   Status;
  PEI_SDHC_DEVICE              *PeiSdhcDev;
  UINT8                        SlotInfo;
  UINT8                        SlotNum;
  UINT8                        FirstBar;
  UINT8                        Index;
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

  if ((SubClass != PCI_SUBCLASS_SD_HOST_CONTROLLER) || (BaseClass != PCI_CLASS_SYSTEM_PERIPHERAL)) {
    return EFI_UNSUPPORTED;
  }

  Status = ProgramBar (PciRes);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the SD/MMC Pci host controller's Slot Info.
  //
  SlotInfo = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, SD_MMC_HC_PEI_SLOT_OFFSET));
  SlotNum  = ((SlotInfo & 0x70) >> 4) + 1;
  FirstBar = SlotInfo & 0x07;
  
  if (SlotNum > SD_MMC_HC_MAX_SLOT) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < SlotNum; Index++) {
    PeiSdhcDev = (PEI_SDHC_DEVICE *)AllocateZeroPool (sizeof (PEI_SDHC_DEVICE));
    ASSERT (PeiSdhcDev != NULL);
    if (PeiSdhcDev == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    PeiSdhcDev->Signature            = PEI_SDHC_SIGNATURE;
    PeiSdhcDev->ControllerPpi        = mSdControllerPpi;
    PeiSdhcDev->PpiList              = mSdhcPpiList;
    PeiSdhcDev->PpiList.Ppi          = &PeiSdhcDev->ControllerPpi;
    PeiSdhcDev->NotifyList           = mSdhcNotifyList;
    PeiSdhcDev->TotalControllers     = 1;
    PeiSdhcDev->BaseAddress          = PciRes->PciBar[Index];
    PeiSdhcDev->PciAddress           = PCI_LIB_ADDRESS (Bus, Device, Function, 0);

    //
    // Install notification in order to reset the SDHC
    //
    Status = PeiServicesNotifyPpi (&PeiSdhcDev->NotifyList);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Install SD Controller PPI
    //
    Status = PeiServicesInstallPpi (&PeiSdhcDev->PpiList);
    DEBUG ((DEBUG_ERROR | DEBUG_INFO, "InstallPpi :%r\n",Status));
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  DEBUG ((DEBUG_ERROR | DEBUG_INFO, "End InitSdController !\n"));
  return EFI_SUCCESS;
}
