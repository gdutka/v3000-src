/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _FCH_DXE_LIB_H_
#define _FCH_DXE_LIB_H_

#include <PiDxe.h>
#include <Library/DebugLib.h>

#include <Protocol/PciRootBridgeIo.h>


VOID
LibFchIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  );


VOID
LibFchIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  );


VOID
LibFchMemRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
     OUT   VOID                                     *Value
  );


VOID
LibFchMemWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  );


VOID
LibFchIoRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  );


VOID
LibFchMemRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  );


VOID
LibFchIndirectIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
     OUT   VOID                                     *Value
  );


VOID
LibFchIndirectIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
  IN       VOID                                     *Value
  );


VOID
LibFchGetAcpiMmioBase (
  OUT   UINT32        *AcpiMmioBase
  );


VOID
LibFchGetAcpiPmBase (
  OUT  UINT16         *AcpiPmBase
  );


/*----------------------------------------------------------------------------------------*/
/**
 * LibFchGetAcpiPmBaseV2
 *
 * Use Acpi Mmio to get PmBase.
 *
 * @param IN  UINT16 *AcpiPmBase
 * @param OUT UINT16 *AcpiPmBase
 *
 * return VOID
 */
VOID
LibFchGetAcpiPmBaseV2 (
  OUT  UINT16         *AcpiPmBase
  );


VOID
LibFchPciRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  );


VOID
LibFchPciWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  );

/*----------------------------------------------------------------------------------------*/
/**
 * FchGpioConfigIomux
 *
 *
 *
 * @param IN UINT8 Socket
 * @param IN UINT8 Die
 * @param IN UINT8 GpioNum
 *
 * return FALSE: fail, TRUE success;
 */

BOOLEAN
FchGpioConfigIomux (
  IN UINT32 Socket,
  IN UINT32 Die,
  IN UINT32 GpioNum
  );

#endif


