/** @file

;******************************************************************************
;* Copyright (c) 2014 - 2017, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _PEI_PCH_SDHC_H
#define _PEI_PCH_SDHC_H

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <IndustryStandard/Pci.h>
//
// Driver Produced PPI Prototypes
//
#include <Ppi/SdController.h>

//
// Driver Consumed PPI Prototypes
//
#include <Ppi/PciCfg2.h>
#include <Ppi/EndOfPeiPhase.h>

#define COMMAND_REGISTER                0x04
#define CLASSC_REGISTER                 0x08
#define BASE_ADDRESS_REGISTER_0         0x10
#define BASE_ADDRESS_REGISTER_1         0x14
#define SOFT_RESET_REGISTER             0x2F

#define SDC_CLASSC                      0x08050100

#define ICH_NUMBER_OF_PCIE_BRIDGES      6
#define ICH_PCIE_BRIDGE_ADDRESS         PcdGet32 (PcdIchPcieBridgeAddress)

#define ROOT_BRIDGE_BUS_REGISTER        0x18
#define ROOT_BRIDGE_ADDRESS_REGISTER    0x20

#define EMMC_HC_ADDRESS                 0x001E0400  // onborad eMMC HC
#define EMMC_HC_BASE_ADDRESS            0xB1050000  // onborad eMMC HC

#define SDCARD_HC_ADDRESS               0x001E0600  // onborad SD card HC
#define SDCARD_HC_BASE_ADDRESS          0xB1051000  // onborad SD card HC

#define ADDON_SDHC_ADDRESS              0x08000100
#define ADDON_SDHC_BASE_ADDRESS         0xC4000000

#define PEI_ICH_SDHC_SIGNATURE          SIGNATURE_32 ('S', 'D', 'H', 'C')

typedef struct {
  UINTN                      Signature;
  PEI_SD_CONTROLLER_PPI      SdControllerPpi;
  EFI_PEI_PPI_DESCRIPTOR     PpiList;
  EFI_PEI_NOTIFY_DESCRIPTOR  NotifyList;
  EFI_PEI_PCI_CFG2_PPI       *PciCfgPpi;
  UINTN                      TotalSdControllers;
  UINTN                      MemBase;
  UINTN                      RootBridge;
  UINTN                      PciCfgAddress;
} ICH_SDHC_DEVICE;

#define ICH_SDHC_DEVICE_FROM_THIS(a) \
  CR(a, ICH_SDHC_DEVICE, SdControllerPpi, PEI_ICH_SDHC_SIGNATURE)

#define ICH_SDHC_DEVICE_FROM_NOTIFY_DESC(a) CR(a, ICH_SDHC_DEVICE, NotifyList, PEI_ICH_SDHC_SIGNATURE)

#endif
