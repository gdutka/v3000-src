/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _BHT_HOST_H_
#define _BHT_HOST_H_

#include <PiPei.h>

#include <Library/PeiServicesLib.h>

#include <Ppi/PciCfg2.h>
#include <Ppi/Stall.h>

#define VENDOR_ID_REGISTER         0x00
#define DEVICE_ID_REGISTER         0x02

/**
  Configure SDHC and install PEI_SD_CONTROLLER_PPI.

  @param [in]   PeiServices              Pointer to the PEI Services Table
  @param [in]   Address                  Target SDHC PCI Cfg Adreess
  @param [in]   BaseAddress              Target SDHC Base Address
  @param [in]   BehindBridge             Whether enabling bridge is required
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EnableBht (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     Address
  );

BOOLEAN
BhtHostPciSupport (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     Address
  );

#if(0)
#define DbgMsg Print
#else
#define DbgMsg DbgNull
#endif

#define PCI_DEV_ID_RJ       0x8320
#define PCI_DEV_ID_SDS0     0x8420
#define PCI_DEV_ID_SDS1     0x8421
#define PCI_DEV_ID_FJ2      0x8520
#define PCI_DEV_ID_SB0      0x8620
#define PCI_DEV_ID_SB1      0x8621

// O2/BHT add BAR1 for PCIR mapping registers
// These registers is defined by O2/BHT, but we may follow name definition rule.
#define BHT_PCIRMappingVa   (0x200)  /* PCI CFG Space Register Mapping Value Register */
#define BHT_PCIRMappingCtl  (0x204)  /* PCI CFG Space Register Mapping Control Register */
#define BHT_PCIRMappingEn   (0x208)  /* PCI CFG Space Register Mapping Enable Register */
#define BHT_GPIOCTL         (0x210)  /* GPIO control register*/

#endif
