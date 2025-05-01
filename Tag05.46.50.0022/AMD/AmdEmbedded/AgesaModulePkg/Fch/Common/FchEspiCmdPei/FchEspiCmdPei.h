/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_ESPI_CMD_PEI_H_
#define _FCH_ESPI_CMD_PEI_H_

#include <Pi/PiPeiCis.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FchBaseLib.h>
#include <Library/DebugLib.h>
#include <Library/FchEspiCmdLib.h>
#include <Ppi/FchEspiCmdPpi.h>

extern EFI_GUID gFchResetDataHobGuid;

VOID
EFIAPI
PeiInBandRst (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase
  );

UINT32
EFIAPI
PeiGetConfiguration (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
  );

VOID
EFIAPI
PeiSetConfiguration (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
  );

VOID
EFIAPI
PeiEspiMemRead (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
PeiEspiMemWrite (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
PeiEspiIORead (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);

VOID
EFIAPI
PeiEspiIOWrite (
  IN CONST EFI_PEI_ESPI_CMD_PPI  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);

EFI_STATUS
EFIAPI
AmdFchEspiCmdPeiInit (
  IN       EFI_PEI_FILE_HANDLE FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

#endif // _FCH_ESPI_CMD_PEI_H_

