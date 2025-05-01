/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "FchEspiCmdDxe.h"

#define FILECODE FCH_COMMON_FCHESPICMDDXE_FCHESPICMDDXE_FILECODE

extern EFI_BOOT_SERVICES *gBS;

/**
  * DxeInBandRst - eSPI In Band Reset
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  *
  */
VOID
EFIAPI
DxeInBandRst (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase
  )
{
  FchEspiCmd_InBandRst(EspiBase);
}

/**
  *  DxeGetConfiguration - eSPI Get Configuration
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
DxeGetConfiguration (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  UINT32                      RegAddr
 )
{
  return (FchEspiCmd_GetConfiguration(EspiBase, RegAddr));
}

/**
  * DxeSetConfiguration - eSPI Set Configuration
  *
  * @param This                   Pointer to an EFI_ESPI_CMD_PROTOCOL structure.
  * @param[in]  EspiBase          Espi MMIO base
  * @param[in]  RegAddr           Slave register address
  * @param[in]  Value             Slave register value
  *
  */
VOID
EFIAPI
DxeSetConfiguration (
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
DxeMemRead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
)
{
  FchEspiCmd_PeripheralMemRead(EspiBase, MemData);
}

VOID
EFIAPI
DxeMemWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_MEM_RW               *MemData
)
{
  FchEspiCmd_PeripheralMemWrite(EspiBase, MemData);
}
VOID
EFIAPI
DxeIORead (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
)
{
  FchEspiCmd_PeripheralIORead(EspiBase, Message_IO);
}
VOID
EFIAPI
DxeIOWrite (
  IN CONST EFI_ESPI_CMD_PROTOCOL  *This,
  IN  UINT32                      EspiBase,
  IN  PERIPH_IO_RW                *Message_IO
)
{
  FchEspiCmd_PeripheralIOWrite(EspiBase, Message_IO);
}

EFI_ESPI_CMD_PROTOCOL DxeFchEspiCmdProt = {
  DxeInBandRst,
  DxeGetConfiguration,
  DxeSetConfiguration,
  DxeMemRead,
  DxeMemWrite,
  DxeIORead,
  DxeIOWrite  

};

/**
  *  @brief Entry point of the AMD FCH eSPI Command DXE driver
  *  @details Install supported protocol
  *  @param[in] ImageHandle - EFI Image Handle for the DXE driver
  *  @param[in] SystemTable - pointer to the EFI system table
  *  @returns EFI_STATUS
  *  @retval EFI_SUCCESS : Module initialized successfully
  *          EFI_ERROR   : Initialization failed (see error for more details)
  */
EFI_STATUS
EFIAPI
AmdFchEspiCmdDxeInit (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HANDLE            Handle;

  DEBUG ((EFI_D_INFO, "FchEspiCmdDxe entry point: AmdFchEspiCmdDxeInit.\n"));


  // Install Protocol
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdFchEspiCmdProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &DxeFchEspiCmdProt
                  );

    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "[Warning] InstallProtocolInterface failed (Status: 0x%x).\n", Status));
    }

  return (Status);
}
