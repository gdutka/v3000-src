/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _FCH_PEI_LIB_H_
#define _FCH_PEI_LIB_H_

//#include "FchPlatform.h"

#define FCH_PCICFG_ADDRESS(Src32)  (                \
      (UINT64) ((UINT32) (Src32 & 0xff0000ff) +     \
               ((UINT32) (Src32 & 0xf80000) >> 3) + \
               ((UINT32) (Src32 & 0x70000) >> 8)) & 0x00000000ffffffff)

UINT8
LibFchMemRead8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address
  );

VOID
LibFchMemWrite8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             *Value
  );

VOID
LibFchMemRMW8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT64            Address,
  IN       UINT8             AndMask,
  IN       UINT8             OrMask
  );

UINT8
LibFchPmIoRead8 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT8
LibFchPmIoRead8V2 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT16
LibFchPmIoRead16 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT16
LibFchPmIoRead16V2 (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN       UINT8             RegIndex
  );

UINT32
LibFchGetAcpiMmio (
  IN CONST EFI_PEI_SERVICES  **PeiServices
  );

VOID
LibFchPciCfgRead (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH Width,
  IN       UINT64                    Address,
  IN OUT   VOID                      *Value
  );

VOID
LibFchPciCfgWrite (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH  Width,
  IN       UINT64                     Address,
  IN OUT   VOID                       *Value
  );

VOID
LibFchPciCfgModify (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       EFI_PEI_PCI_CFG_PPI_WIDTH  Width,
  IN       UINT64                     Address,
  IN       UINTN                      SetBits,
  IN       UINTN                      ClearBits
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


