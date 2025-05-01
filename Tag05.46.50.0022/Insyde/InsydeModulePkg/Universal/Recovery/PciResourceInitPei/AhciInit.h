/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _AHCI_INIT_H
#define _AHCI_INIT_H

#include "PciResourceInitPei.h"

#include <Ppi/AhciController.h>
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>

#define AHCI_ABAR_OFFSET                   0x24

#define PEI_AHCI_SIGNATURE                 SIGNATURE_32 ('P', 'A', 'H', 'C')

typedef struct _PEI_AHCI_DEVICE {
  UINT32                        Signature;
  UINT32                        TotalControllers;
  UINT32                        BaseAddress;
  UINT32                        PciAddress;
  EFI_PEI_PPI_DESCRIPTOR        PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR     NotifyList;
  PEI_AHCI_CONTROLLER_PPI       ControllerPpi;
} PEI_AHCI_DEVICE;

#define PEI_AHCI_DEVICE_FROM_THIS(a)        CR(a, PEI_AHCI_DEVICE, ControllerPpi, PEI_AHCI_SIGNATURE)
#define PEI_AHCI_DEVICE_FROM_NOTIFY_DESC(a) CR(a, PEI_AHCI_DEVICE, NotifyList, PEI_AHCI_SIGNATURE)

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
  );

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
  );

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
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

#endif
