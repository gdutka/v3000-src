/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include <Library/UefiBootServicesTableLib.h>

#include <Library/UefiRuntimeServicesTableLib.h>

#include <Library/FchDxeLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>

#include "FchRegistersCommon.h"

#define FILECODE LIBRARY_FCHDXELIB_FCHDXELIB_FILECODE


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  I/O Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  )
{
  switch (EfiWidth) {
  case EfiPciWidthUint8:
    *(UINT8 *) Value = IoRead8 (Address);
    break;
  case EfiPciWidthUint16:
    *(UINT16 *) Value = IoRead16 (Address);
    break;
  case EfiPciWidthUint32:
    *(UINT32 *) Value = IoRead32 (Address);
    break;
  default:
    ASSERT (FALSE);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  I/O Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  )
{
  switch (EfiWidth) {
  case EfiPciWidthUint8:
    IoWrite8 (Address, *(UINT8 *) Value);
    break;
  case EfiPciWidthUint16:
    IoWrite16 (Address, *(UINT16 *) Value);
    break;
  case EfiPciWidthUint32:
    IoWrite32 (Address, *(UINT32 *) Value);
    break;
  default:
    ASSERT (FALSE);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Memory Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Memory address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
     OUT   VOID                                     *Value
  )
{
  switch ( EfiWidth ) {
  case EfiPciWidthUint8:
    *((UINT8*)Value) = *((volatile UINT8*) ((UINTN)Address));
    break;

  case EfiPciWidthUint16:
    *((UINT16*)Value) = *((volatile UINT16*) ((UINTN)Address));
    break;

  case EfiPciWidthUint32:
    *((UINT32*)Value) = *((volatile UINT32*) ((UINTN)Address));
    break;

  default:
    ASSERT (FALSE);
    break;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Memory Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Memory address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  switch ( EfiWidth ) {
  case EfiPciWidthUint8 :
    *((volatile UINT8*) ((UINTN)Address)) = *((UINT8*)Value);
    break;

  case EfiPciWidthUint16:
    *((volatile UINT16*) ((UINTN)Address)) = *((UINT16*)Value);
    break;

  case EfiPciWidthUint32:
    *((volatile UINT32*) ((UINTN)Address)) = *((UINT32*)Value);
    break;

  default:
    ASSERT (FALSE);
    break;
  }
}


UINT32
ModifyDataByWidth (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN OUT   VOID                                     *Data,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32   TempValue;

  switch (EfiWidth) {
  case EfiPciWidthUint8:
    TempValue = ((UINT32)*(UINT8 *)Data & (UINT32)*(UINT8 *) AndMask) | (UINT32)*(UINT8 *) OrMask;
    break;
  case EfiPciWidthUint16:
    TempValue = ((UINT32)*(UINT16 *)Data & (UINT32)*(UINT16 *) AndMask) | (UINT32)*(UINT16 *) OrMask;
    break;
  case EfiPciWidthUint32:
    TempValue = (*(UINT32 *)Data & *(UINT32 *) AndMask) | *(UINT32 *) OrMask;
    break;
  default:
    ASSERT (FALSE);
    TempValue = 0;
  }
  return TempValue;
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  I/O Read-Modify-Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       *AndMask     Pointer to And Mask
 * @param[in]       *OrMask      Pointer to Or Mask
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32     FchDxeLibData32;

  LibFchIoRead (EfiWidth, Address, &FchDxeLibData32);
  FchDxeLibData32 = ModifyDataByWidth (EfiWidth, &FchDxeLibData32, AndMask, OrMask);
  LibFchIoWrite (EfiWidth, Address, &FchDxeLibData32);
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Memory Read-Modify-Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       *AndMask     Pointer to And Mask
 * @param[in]       *OrMask      Pointer to Or Mask
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32     FchDxeLibData32;

  LibFchMemRead (EfiWidth, Address, &FchDxeLibData32);
  FchDxeLibData32 = ModifyDataByWidth (EfiWidth, &FchDxeLibData32, AndMask, OrMask);
  LibFchMemWrite (EfiWidth, Address, &FchDxeLibData32);
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Indirect I/O Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       IoBase       IO BASE address
 * @param[in]       IndexAddress Register Index
 * @param[out]      *Value       Pointer to Data Buffer
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIndirectIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
     OUT   VOID                                     *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;

  ByteCount = (UINT8) EfiWidth + 1;
  for (i = 0; i < ByteCount; i++, IndexAddress++) {
    LibFchIoWrite (EfiPciWidthUint8, IoBase, &IndexAddress);
    LibFchIoRead (EfiPciWidthUint8, IoBase + 1, (UINT8 *)Value + i);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Indirect I/O Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       IoBase       IO BASE address
 * @param[in]       IndexAddress Register Index
 * @param[in]       *Value       Pointer to Data Buffer
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIndirectIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
  IN       VOID                                     *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;

  ByteCount = (UINT8) EfiWidth + 1;
  for (i = 0; i < ByteCount; i++, IndexAddress++) {
    LibFchIoWrite (EfiPciWidthUint8, IoBase, &IndexAddress);
    LibFchIoWrite (EfiPciWidthUint8, IoBase + 1, (UINT8 *)Value + i);
  }
}


VOID
LibFchGetAcpiMmioBase (
  OUT   UINT32         *AcpiMmioBase
  )
{
  UINT32    Value32;

  Value32 = 0xFED80000;
  *AcpiMmioBase = Value32;
}


VOID
LibFchGetAcpiPmBase (
  OUT   UINT16         *AcpiPmBase
  )
{
  LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD6, FCH_PMIOA_REG60, AcpiPmBase);
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  LibFchGetAcpiPmBaseV2
 *
 * Use Acpi Mmio to get PmBase.
 *
 * @param[out]      *AcpiPmBase  Pointer to Data Buffer
 *
 * return VOID
 */
VOID
LibFchGetAcpiPmBaseV2 (
  OUT   UINT16         *AcpiPmBase
  )
{
  UINT32  AcpiMmioBase;

  AcpiMmioBase    = 0;

  // Get Acpi Mmio Base
  LibFchGetAcpiMmioBase (&AcpiMmioBase);
  LibFchMemRead (
    EfiPciWidthUint16,
    AcpiMmioBase + PMIO_BASE + FCH_PMIOA_REG60,
    AcpiPmBase
    );
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  PCI Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Address
 * @param[out]      *Value       Pointer to Data Buffer
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchPciRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  UINTN  PciAddress;

  PciAddress = ((Address >> 4) & ~0xFFF) + (Address & 0xFFF);

  switch (EfiWidth) {
  case EfiPciWidthUint8:
    *((UINT8 *) Value) = PciRead8 (PciAddress);
    break;
  case EfiPciWidthUint16:
    *((UINT16 *) Value) = PciRead16 (PciAddress);
    break;
  case EfiPciWidthUint32:
    *((UINT32 *) Value) = PciRead32 (PciAddress);
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  PCI Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Address
 * @param[in]       *Value       Pointer to Data Buffer
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchPciWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  UINTN  PciAddress;

  PciAddress = ((Address >> 4) & ~0xFFF) + (Address & 0xFFF);

  switch (EfiWidth) {
  case EfiPciWidthUint8:
    PciWrite8 (PciAddress, *((UINT8 *) Value));
    break;
  case EfiPciWidthUint16:
    PciWrite16 (PciAddress, *((UINT16 *) Value));
    break;
  case EfiPciWidthUint32:
    PciWrite32 (PciAddress, *((UINT32 *) Value));
    break;
  default:
    ASSERT (FALSE);
    break;
  }
}







