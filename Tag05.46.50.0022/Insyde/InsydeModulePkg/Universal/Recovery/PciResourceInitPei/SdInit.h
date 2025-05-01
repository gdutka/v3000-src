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
#ifndef _SD_INIT_H
#define _SD_INIT_H

#include "PciResourceInitPei.h"

#include <Ppi/SdController.h>
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>

#define SD_MMC_HC_PEI_SLOT_OFFSET  0x40
#define SOFT_RESET_REGISTER        0x2F
#define SD_MMC_HC_MAX_SLOT         6

#define PEI_SDHC_SIGNATURE SIGNATURE_32 ('S', 'D', 'H', 'C')

typedef struct _PEI_SDHC_DEVICE {
  UINT32                        Signature;
  UINT32                        TotalControllers;
  UINT32                        BaseAddress;
  UINT32                        PciAddress;
  EFI_PEI_PPI_DESCRIPTOR        PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR     NotifyList;
  PEI_SD_CONTROLLER_PPI         ControllerPpi;
} PEI_SDHC_DEVICE;

#define PEI_SDHC_DEVICE_FROM_THIS(a) CR(a, PEI_SDHC_DEVICE, ControllerPpi, PEI_SDHC_SIGNATURE)
#define PEI_SDHC_DEVICE_FROM_NOTIFY_DESC(a) CR(a, PEI_SDHC_DEVICE, NotifyList, PEI_SDHC_SIGNATURE)

#define PCILIB_ADDRESS_TO_PCICFG_ADDRESS(A) \
  ((((A) << 4) & 0xff000000) | (((A) >> 4) & 0x00000700) | (((A) << 1) & 0x001f0000) | (LShiftU64((A) & 0xfff, 32)))

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
  );  

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
  );                                

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
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

#endif
