/** @file
  Project dependent initial code for Insyde Debugger.

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

#include <Library/SecOemSvcChipsetLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/PlatformHookLib.h>
#include <Library/XhciEralyInitLib.h>

#define XHCI_BUS 0xF

/**
  Project dependent initial code for Insyde Debugger.

  @param  Base on OEM design.

  @retval EFI_UNSUPPORTED    Returns unsupported by default.
  @retval EFI_SUCCESS        The service is customized in the project.
  @retval EFI_MEDIA_CHANGED  The value of IN OUT parameter is changed.
  @retval Others             Depends on customization.
**/
EFI_STATUS
OemSvcInitializeInsydeDebugger (
  VOID
  )
{
  UINT32    Value32;
  UINT32    *Ptr;

  //
  // Enable FCH MMIO
  //
  Value32 = 0xFED80304;
  Ptr = (UINT32 *)(UINTN)Value32;
  *Ptr = (*Ptr | BIT1);

  //
  // Initialize Socket0 xHCI controller
  //
  XhciControllerInit(0);

  Value32 = PCI_LIB_ADDRESS (0, 0x18, 0, 0x2F0);
  PciWrite32(Value32, 0x0000F000);
  Value32 = PCI_LIB_ADDRESS (0, 0x18, 0, 0x2F4);
  PciWrite32(Value32, 0x0000FDFF);
  Value32 = PCI_LIB_ADDRESS (0, 0x18, 0, 0x2F8);
  PciWrite32(Value32, 0x00000043);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x18);
  PciWrite8(Value32, 0x00);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x19);
  PciWrite8(Value32, XHCI_BUS);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x1A);
  PciWrite8(Value32, XHCI_BUS);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x20);
  PciWrite32 (Value32, 0xFDFFFC00);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x24);
  PciWrite32 (Value32, 0x0001FFF1);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x28);
  PciWrite32 (Value32, 0xFFFFFFFF);
  Value32 = PCI_LIB_ADDRESS (0, 0x08, 1, 0x4);
  PciWrite8 (Value32, 0x6);
  Value32 = PCI_LIB_ADDRESS (XHCI_BUS, 0x0, 4, 0x10);
  PciWrite32 (Value32, 0xFC000000);
  Value32 = PCI_LIB_ADDRESS (XHCI_BUS, 0x0, 4, 0x4);
  PciWrite8 (Value32, 0x6);

  return EFI_SUCCESS;
}
