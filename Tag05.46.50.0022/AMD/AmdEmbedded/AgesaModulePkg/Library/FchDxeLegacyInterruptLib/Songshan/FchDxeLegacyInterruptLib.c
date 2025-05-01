/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Interrupt Legacy Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   DXE Legacy Interrupt Library
 * @e \$Revision: 309090 $  @e \$Date$
 *
 */

#define FCH_IOMAP_REGC00    0xC00
#define FCH_IOMAP_REGC01    0xC01

#define FCH_PCI_BUS         0
#define FCH_PCI_DEV         20
#define FCH_PCI_FUNC        4

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/FchDxeLib.h>

// Consumed Protocols
//#include <Protocol/PciRootBridgeIo.h>

//
// Module specific definitions
//
#define     MAX_NUMBER_PIRQS     8
#define FILECODE LIBRARY_FCHDXELEGACYINTERRUPTLIB_FILECODE


// Get the number of PIRQs supported.
EFI_STATUS
AmdGetNumberPirqs (
     OUT   UINT8                         *NumberPirqs
  )
{
  if (NULL == NumberPirqs) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberPirqs = MAX_NUMBER_PIRQS;
  return EFI_SUCCESS;
}


// Get the PCI bus#, device# and function# that associated with this protocol.
EFI_STATUS
AmdGetLocation (
     OUT   UINT8                         *Bus,
     OUT   UINT8                         *Device,
     OUT   UINT8                         *Function
  )
{
  if ((NULL == Bus) || (NULL == Device) || (NULL == Function)) {
    return EFI_INVALID_PARAMETER;
  }

  *Bus      = FCH_PCI_BUS;
  *Device   = FCH_PCI_DEV;
  *Function = FCH_PCI_FUNC;

  return EFI_SUCCESS;
}


// Read the given PIRQ register and return the IRQ value that is assigned to it.
EFI_STATUS
AmdReadPirq (
  IN       UINT8                         PirqNumber,
     OUT   UINT8                         *PirqData
  )
{
  if (NULL == PirqData) {
    return EFI_INVALID_PARAMETER;
  }
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGC00, PirqNumber, PirqData);

  return EFI_SUCCESS;
}


// Write an IRQ value to the specified PIRQ register.
EFI_STATUS
AmdWritePirq (
  IN       UINT8                         PirqNumber,
  IN       UINT8                         PirqData
  )
{
  //if ((PirqNumber >= MAX_NUMBER_PIRQS) || (PirqData > 15)) {
  //  return EFI_INVALID_PARAMETER;
  //}
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGC00, PirqNumber, &PirqData);

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Name: AmdInstallLegacyInterrupt
 *
 * This DXE driver produces the legacy interrupt protocol as part of the EfiCompatibility
 * component in CSM. This protocol will manage the programming of PCI IRQ assignment.
 *
 * @param[in]    ImageHandle     Pointer to the image handle
 * @param[in]    SystemTable     Pointer to the EFI system table
 *
 * @retval       EFI_SUCCESS     Driver initialized successfully
 * @retval       EFI_ERROR       Driver initialization failed
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
FchDxeLegacyInterruptLibConstructor(
  IN       EFI_HANDLE              ImageHandle,
  IN       EFI_SYSTEM_TABLE        *SystemTable
  )
{
  UINT8                            i;
  UINT8                            bValue;

  DEBUG(( DEBUG_INFO, __FUNCTION__" - enter..\n" ));
  //
  // Initialize PCI IRQ routing registers for INTA#-INTH#
  //
  for (i = 0; i < 8; i++) {
    bValue = i | BIT7;                                 // Select IRQ routing to APIC
    LibFchIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGC00, &bValue);
    bValue = i | BIT4;
    LibFchIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGC01, &bValue);
  }

  return (EFI_SUCCESS);
}



