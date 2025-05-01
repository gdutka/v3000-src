/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_ESPI_CMD_DXE_H_
#define _FCH_ESPI_CMD_DXE_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/FchBaseLib.h>
#include <Library/FchEspiCmdLib.h>
#include <Protocol/FchEspiCmdProtocol.h>

VOID
EFIAPI
DxeInBandRst (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase
  );

UINT32
EFIAPI
DxeGetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 );

VOID
EFIAPI
DxeSetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
);

VOID
EFIAPI
DxeMemRead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
DxeMemWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
DxeIORead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);

VOID
EFIAPI
DxeIOWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);

EFI_STATUS
EFIAPI
AmdFchEspiCmdDxeInit (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _FCH_ESPI_CMD_DXE_H_

