/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "FchEspiCmdSmm.h"

#define FILECODE FCH_COMMON_FCHESPICMDSMM_FCHESPICMDSMM_FILECODE

/**
  * SmmInBandRst - eSPI In Band Reset
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  *
  */
VOID
EFIAPI
SmmInBandRst (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase
  )
{
  FchEspiCmd_InBandRst(EspiBase);
}

/**
  *  SmmGetConfiguration - eSPI Get Configuration
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  *
  * @retval    Register Value
  *
  */
UINT32
EFIAPI
SmmGetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 )
{
  return (FchEspiCmd_GetConfiguration(EspiBase, RegAddr));
}

/**
  * SmmSetConfiguration - eSPI Set Configuration
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  * @param[in]  Value             Slave register value
  *
  */
VOID
EFIAPI
SmmSetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr,
  IN  UINT32                      Value
)
{
  FchEspiCmd_SetConfiguration(EspiBase, RegAddr, Value);
}

VOID
EFIAPI
SmmMemRead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW                *MemData
)
{
  FchEspiCmd_PeripheralMemRead(EspiBase, MemData);
}

VOID
EFIAPI
SmmMemWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
)
{
  FchEspiCmd_PeripheralMemWrite(EspiBase, MemData);
}
VOID
EFIAPI
SmmIORead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
)
{
  FchEspiCmd_PeripheralIORead(EspiBase, Message_IO);
}
VOID
EFIAPI
SmmIOWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
)
{
  FchEspiCmd_PeripheralIOWrite(EspiBase, Message_IO);
}

EFI_ESPI_CMD_PROTOCOL SmmFchEspiCmdProt = {
  SmmInBandRst,
  SmmGetConfiguration,
  SmmSetConfiguration,
  SmmMemRead,
  SmmMemWrite,
  SmmIORead,
  SmmIOWrite 
};

/**
  *  @brief Entry point of the AMD FCH eSPI Command SMM driver
  *  @details Install supported protocol
  *  @param[in] ImageHandle - EFI Image Handle for the SMM driver
  *  @param[in] SystemTable - pointer to the EFI system table
  *  @returns EFI_STATUS
  *  @retval EFI_SUCCESS : Module initialized successfully
  *          EFI_ERROR   : Initialization failed (see error for more details)
  */
EFI_STATUS
EFIAPI
AmdFchEspiCmdSmmInit (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            Handle;

  DEBUG ((EFI_D_ERROR, "FchEspiCmdSmm entry point: AmdFchEspiCmdSmmInit.\n"));


  // Install Protocol
  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                  &Handle,
                  &gAmdFchEspiCmdSmmProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &SmmFchEspiCmdProt
                  );

    if (Status != EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "[Warning] InstallProtocolInterface failed (Status: 0x%x).\n", Status));
    }
  return (Status);
}

