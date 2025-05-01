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
#ifndef _UFS_INIT_H
#define _UFS_INIT_H

#include "PciResourceInitPei.h"

#include <Ppi/UfsController.h>
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>

#define PEI_UFS_SIGNATURE                  SIGNATURE_32 ('p', 'u', 'f', 's')

typedef struct _PEI_UFS_DEVICE {
  UINT32                        Signature;
  UINT32                        TotalControllers;
  UINT32                        BaseAddress;
  UINT32                        PciAddress;
  EFI_PEI_PPI_DESCRIPTOR        PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR     NotifyList;
  PEI_UFS_CONTROLLER_PPI        ControllerPpi;
} PEI_UFS_DEVICE;

#define PEI_UFS_DEVICE_FROM_THIS(a)        CR(a, PEI_UFS_DEVICE, ControllerPpi, PEI_UFS_SIGNATURE)
#define PEI_UFS_DEVICE_FROM_NOTIFY_DESC(a) CR(a, PEI_UFS_DEVICE, NotifyList, PEI_UFS_SIGNATURE)

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
  );

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
  );

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
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

#endif
