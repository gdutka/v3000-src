/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#include "PiDxe.h"
#include "PiSmm.h"

#ifndef _FCH_SMM_LIB_H_
#define _FCH_SMM_LIB_H_

VOID
LibFchSmmMemRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmMemWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmIoRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT16                      Address,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmIoWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
  IN       VOID                        *Value
  );

VOID
LibFchSmmPciRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT32                      PciAddress,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioRead8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioRead8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioRead16 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioRead16V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioWrite8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibFchSmmPmioReadWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
  IN       UINT8                       mask,
  IN       UINT8                       Value
  );

VOID
LibFchSmmPmioReadWrite8V2 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
  IN       UINT8                       mask,
  IN       UINT8                       Value
  );

#endif


