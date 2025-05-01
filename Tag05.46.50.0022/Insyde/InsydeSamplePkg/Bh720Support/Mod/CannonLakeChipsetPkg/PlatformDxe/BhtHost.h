/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
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

#include <Uefi.h>
#include <IndustryStandard/Pci.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/PciIo.h>
#include <Protocol/SdMmcOverride.h>

#include <Guid/DebugMask.h>

/**

  Override function for SDHCI capability bits

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in,out]  SdMmcHcSlotCapability The SDHCI capability structure.
  @param[in,out]  BaseClkFreq           The base clock frequency value that
                                        optionally can be updated.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER SdMmcHcSlotCapability is NULL

**/
EFI_STATUS
EFIAPI
BhtHostOverrideCapability (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN OUT  VOID                            *SdMmcHcSlotCapability,
  IN OUT  UINT32                          *BaseClkFreq
  );

/**

  Override function for SDHCI controller operations

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in]      PhaseType             The type of operation and whether the
                                        hook is invoked right before (pre) or
                                        right after (post)
  @param[in,out]  PhaseData             The pointer to a phase-specific data.
  
  @retval EFI_SUCCESS                   The override function completed successfully.
  @retval EFI_NOT_FOUND                 The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER         PhaseType is invalid
  

**/
EFI_STATUS
EFIAPI
BhtHostOverrideNotifyPhase (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE         PhaseType,
  IN OUT  VOID                           *PhaseData
  );

BOOLEAN
BhtHostPciSupport(EFI_PCI_IO_PROTOCOL *PciIo);
UINT32
PciBhtRead32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset);
void
PciBhtWrite32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value);
void 
PciBhtOr32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value);
void 
PciBhtAnd32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value);
extern void 
DbgNull(IN CONST CHAR16 * fmt, ...);
UINT32 bht_readl(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset);
void bht_writel(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value);

typedef struct {
  UINT32   TimeoutFreq:6;     // bit 0:5
  UINT32   Reserved:1;        // bit 6
  UINT32   TimeoutUnit:1;     // bit 7
  UINT32   BaseClkFreq:8;     // bit 8:15
  UINT32   MaxBlkLen:2;       // bit 16:17
  UINT32   BusWidth8:1;       // bit 18
  UINT32   Adma2:1;           // bit 19
  UINT32   Reserved2:1;       // bit 20
  UINT32   HighSpeed:1;       // bit 21
  UINT32   Sdma:1;            // bit 22
  UINT32   SuspRes:1;         // bit 23
  UINT32   Voltage33:1;       // bit 24
  UINT32   Voltage30:1;       // bit 25
  UINT32   Voltage18:1;       // bit 26
  UINT32   Reserved3:1;       // bit 27
  UINT32   SysBus64:1;        // bit 28
  UINT32   AsyncInt:1;        // bit 29
  UINT32   SlotType:2;        // bit 30:31
  UINT32   Sdr50:1;           // bit 32
  UINT32   Sdr104:1;          // bit 33
  UINT32   Ddr50:1;           // bit 34
  UINT32   Reserved4:1;       // bit 35
  UINT32   DriverTypeA:1;     // bit 36
  UINT32   DriverTypeC:1;     // bit 37
  UINT32   DriverTypeD:1;     // bit 38
  UINT32   DriverType4:1;     // bit 39
  UINT32   TimerCount:4;      // bit 40:43
  UINT32   Reserved5:1;       // bit 44
  UINT32   TuningSDR50:1;     // bit 45
  UINT32   RetuningMod:2;     // bit 46:47
  UINT32   ClkMultiplier:8;   // bit 48:55
  UINT32   Reserved6:7;       // bit 56:62
  UINT32   Hs400:1;           // bit 63
} SD_MMC_HC_SLOT_CAP;

#if(0)
#define DbgMsg	Print
#else
#define DbgMsg	DbgNull
#endif


#define PCI_DEV_ID_RJ		  0x8320
#define PCI_DEV_ID_SDS0		0x8420
#define PCI_DEV_ID_SDS1		0x8421
#define PCI_DEV_ID_FJ2		0x8520
#define PCI_DEV_ID_SB0		0x8620
#define PCI_DEV_ID_SB1		0x8621


// O2/BHT add BAR1 for PCIR mapping registers
// These registers is defined by O2/BHT, but we may follow name definition rule.
#define	BHT_PCIRMappingVal	        (0x200)		/* PCI CFG Space Register Mapping Value Register */
#define	BHT_PCIRMappingCtl	        (0x204)		/* PCI CFG Space Register Mapping Control Register */
#define	BHT_PCIRMappingEn		  	    (0x208)		/* PCI CFG Space Register Mapping Enable Register */
#define	BHT_GPIOCTL			  		      (0x210)		/* GPIO control register*/

#define SD_MMC_HC_GENERIC_TIMEOUT     1 * 1000 * 1000

#endif
