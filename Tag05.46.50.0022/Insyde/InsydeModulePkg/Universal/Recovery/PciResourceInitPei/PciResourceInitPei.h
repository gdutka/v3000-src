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
#ifndef _PEI_PCI_RESOURCE_INIT_H
#define _PEI_PCI_RESOURCE_INIT_H

#include <PiPei.h>
#include <Guid/H2OCp.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DeviceInfo2Lib.h>
#include <IndustryStandard/Pci.h>
#include <Ppi/EndOfPeiPhase.h>

#include <Protocol/DevicePath.h>

#define IS_ACPI_DP(a)       ((((EFI_DEVICE_PATH_PROTOCOL *)a)->Type == ACPI_DEVICE_PATH) && (((EFI_DEVICE_PATH_PROTOCOL *)a)->SubType == ACPI_DP))
#define IS_PCI_DP(a)        ((((EFI_DEVICE_PATH_PROTOCOL *)a)->Type == HARDWARE_DEVICE_PATH) && (((EFI_DEVICE_PATH_PROTOCOL *)a)->SubType == HW_PCI_DP))

#define FIRST_DOWNSTREAM_NUM                       0x08
#define PCI_BRIDGE_BASE_ADDRESS_REGISTER_OFFSET    0x20
#define PCI_BRIDGE_PREFETCHABLE_BASE_ADDRESS_REGISTER_OFFSET    0x24
#define PCI_BRIDGE_PREFETCHABLE_BASE_LIMIT_REGISTER_OFFSET      0x26
#define PCI_BRIDGE_PREFETCHABLE_UPPER32BIT_BASE_ADDRESS_REGISTER_OFFSET    0x28
#define PCI_BRIDGE_PREFETCHABLE_UPPER32BIT_BASE_LIMIT_REGISTER_OFFSET    0x2C

#define PCI_INFO_NODE_FROM_LINK(a)   CR (a, PCI_INFO_NODE, Link, PCI_INFO_NODE_SIGNATURE)
#define PCI_INFO_NODE_SIGNATURE      SIGNATURE_32 ('P', 'D', 'I', 'L')

typedef struct {
  UINT32                     NodeUid; // ACPI UID
  UINT8                      Bus;
  UINT8                      Device;
  UINT8                      Function;
  UINT32                     PciBar[PCI_MAX_BAR];
} PCI_RESOURCE_DATA;

typedef struct _PCI_INFO_NODE PCI_INFO_NODE;

typedef PCI_INFO_NODE *PCI_INFO_NODE_PTR;

struct _PCI_INFO_NODE {
  UINT32                     Signature;
  LIST_ENTRY                 Link;
  PCI_RESOURCE_DATA          PciRes;
  PCI_INFO_NODE              *Parent;
  //
  // P2P Bridge
  //
  BOOLEAN                    IsP2pBridge;
  UINT8                      SecondaryBus;
  UINT8                      SubordinateBus;
  UINT32                     MemBase;
  UINT32                     MemLimit;
};

typedef
EFI_STATUS
(* CONTROLLER_INIT_ROUTINE) (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PCI_RESOURCE_DATA         *PciRes
  );

/**
  Program BAR register for PCI device.

  @param [in]   Bus                      PCI Bus number
  @param [in]   Device                   PCI Device number
  @param [in]   Function                 PCI Function number
  @param [in]   BarIndex                 Index of BAR
  @param [in]   BaseAddress              Bass address
  @param [out]  MmioSize                 MMIO size
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
ProgramBar (
  IN      PCI_RESOURCE_DATA         *PciRes
  );

/**
  Register notify ppi to reset the bridges.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
BridgeEndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

#endif
