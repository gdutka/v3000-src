/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_ESPI_CMD_SMM_H_
#define _FCH_ESPI_CMD_SMM_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/FchBaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/FchEspiCmdLib.h>
#include <Protocol/FchEspiCmdProtocol.h>

VOID
EFIAPI
SmmInBandRst (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase
  );

UINT32
EFIAPI
SmmGetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 );

VOID
EFIAPI
SmmSetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
);

VOID
EFIAPI
SmmMemRead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
SmmMemWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
);

VOID
EFIAPI
SmmIORead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);

VOID
EFIAPI
SmmIOWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
);


EFI_STATUS
EFIAPI
AmdFchEspiCmdSmmInit (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _FCH_ESPI_CMD_SMM_H_

