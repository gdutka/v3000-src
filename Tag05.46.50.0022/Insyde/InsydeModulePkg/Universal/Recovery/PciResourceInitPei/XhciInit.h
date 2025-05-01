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
#ifndef _XHCI_INIT_H
#define _XHCI_INIT_H

#include "PciResourceInitPei.h"

#include <Ppi/UsbController.h>
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Guid/H2OPeiStorage.h>

#define XHCI_XHCC1_OFFSET               0x40
#define XHCI_ACCTRL_REGISTER            BIT31

//
// The MdePkg/Include/Ppi/UsbController.h does not define PEI_XHCI_CONTROLLER
// It is defined here and is to be removed if it is defined in UsbController.h
//
#define PEI_XHCI_CONTROLLER             0x04

#define PEI_XHCI_SIGNATURE              SIGNATURE_32 ('X', 'H', 'C', 'I')

typedef struct _PEI_XHCI_DEVICE {
  UINT32                        Signature;
  UINT32                        TotalControllers;
  UINT32                        BaseAddress;
  UINT32                        PciAddress;
  EFI_PEI_PPI_DESCRIPTOR        PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR     NotifyList;
  PEI_USB_CONTROLLER_PPI        ControllerPpi;
} PEI_XHCI_DEVICE;

#define PEI_XHCI_DEVICE_FROM_THIS(a)        CR(a, PEI_XHCI_DEVICE, ControllerPpi, PEI_XHCI_SIGNATURE)
#define PEI_XHCI_DEVICE_FROM_NOTIFY_DESC(a) CR(a, PEI_XHCI_DEVICE, NotifyList, PEI_XHCI_SIGNATURE)

/**
  Decide whether to install usb stack.

  @param None.

  @retval TRUE         Currently support USB on PEI phase.
  @retval FALSE        Currently not support USB on PEI phase.

**/
BOOLEAN
CheckUsbPolicy (
  VOID
  );

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
  );

/**
  Register notify ppi to reset the XHCI.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
XhciEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES               **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR      *NotifyDescriptor,
  IN VOID                           *Ppi
  );

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
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

#endif
